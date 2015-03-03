//*****************************************************************************
//
// カメラ処理 [camera.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "camera.h"
#include "debugproc.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// アスペクト比
#define RADIAN(a)	((D3DX_PI / 180.0f) * a)						// ラジアン計算

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera(void)
{
	m_at		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_distance	= DISTANCE_3D(m_pos.x, m_pos.y, m_pos.z);

	m_atDest		= m_at;
	m_posDest		= m_pos;
	m_rotDest		= m_rot;
	m_distanceDest	= m_distance;

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecF = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecR = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

//=============================================================================
// 生成
//=============================================================================
CCamera* CCamera::Create(D3DXVECTOR3 at, D3DXVECTOR3 pos)
{
	CCamera* pointer = new CCamera;
	pointer->Init(at, pos);
	return pointer;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(D3DXVECTOR3 at, D3DXVECTOR3 pos)
{
	//----------------------------
	// 位置と距離
	//----------------------------
	m_pos		= pos;
	m_at		= at;
	m_distance	= DISTANCE_3D(pos.x, pos.y, pos.z);

	//----------------------------
	// 回転
	//----------------------------
	m_rot.y = atan2(m_pos.x, -m_pos.z);
	m_rot.x = atan2(m_pos.y, (float)hypot(m_pos.z, m_pos.x));

	if(m_rot.y < -D3DX_PI || m_rot.y > D3DX_PI)
	{
		m_rot.y = m_rot.y + (-SIGN_F(m_rot.y) * D3DX_PI * 2.0f);
	}

	if(m_rot.x < -D3DX_PI || m_rot.x > D3DX_PI)
	{
		m_rot.x = m_rot.x + (-SIGN_F(m_rot.x) * D3DX_PI * 2.0f);
	}

	//----------------------------
	// 目標値
	//----------------------------
	m_posDest		= m_pos;
	m_atDest		= m_at;
	m_rotDest		= m_rot;
	m_distanceDest	= m_distance;

	// 初期化成功
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CCamera::SetCamera(LPDIRECT3DDEVICE9 device)
{
	D3DXVECTOR3 eye = m_pos + m_at;

	//----------------------------
	// ビューマトリックス
	//----------------------------
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_view);
	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_view, &eye, &m_at, &m_vecU);
	// ビューマトリックスの設定
	device->SetTransform(D3DTS_VIEW, &m_view);

	//----------------------------
	// プロジェクションマトリックス
	//----------------------------
	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_projection);
	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_projection,
								RADIAN(45),	// 視野角
								ASPECT,		// アスペクト比
								10.0f,		// near値
								10000.0f);	// far値
	// プロジェクションマトリックスの設定
	device->SetTransform(D3DTS_PROJECTION, &m_projection);

#ifdef _DEBUG
	// カメラ情報表示
	CDebugproc::PrintDebugProc("***カメラ情報**********\nPos x:%f y:%f z:%f\nAt  x:%f y:%f z:%f\nRot x:%f y:%f z:%f\n",
								m_pos.x, m_pos.y, m_pos.z,
								m_at.x,  m_at.y,  m_at.z,
								m_rot.x, m_rot.y, m_rot.z);
#endif
}

//=============================================================================
// 視点再設定
//=============================================================================
void CCamera::ResetPos(void)
{
	m_pos.x = m_at.x + cosf(m_rot.y) * cosf(m_rot.x) * m_distance;
	m_pos.z = m_at.z + sinf(m_rot.y) * cosf(m_rot.x) * m_distance;
	m_pos.y = m_at.y + sinf(m_rot.x) * m_distance;
}

//=============================================================================
// 回転角再設定
//=============================================================================
void CCamera::ResetRot(void)
{
	m_rot.y = atan2(m_pos.x, -m_pos.z);
	m_rot.x = atan2(m_pos.y, (float)hypot(m_pos.z, m_pos.x));

	if(m_rot.y < -D3DX_PI || m_rot.y > D3DX_PI)	// 符号替え
	{
		m_rot.y = m_rot.y + (-SIGN_F(m_rot.y) * D3DX_PI * 2.0f);
	}

	if(m_rot.x < -D3DX_PI || m_rot.x > D3DX_PI)	// 符号替え
	{
		m_rot.x = m_rot.x + (-SIGN_F(m_rot.x) * D3DX_PI * 2.0f);
	}
}

//=============================================================================
// 目的視点再設定
//=============================================================================
void CCamera::ResetPosDest(void)
{
	m_posDest.x = m_atDest.x + cosf(m_rotDest.y - D3DX_PI * 0.5f) * cosf(m_rotDest.x) * m_distance;
	m_posDest.z = m_atDest.z + sinf(m_rotDest.y - D3DX_PI * 0.5f) * cosf(m_rotDest.x) * m_distance;
	m_posDest.y = m_atDest.y + sinf(m_rotDest.x) * m_distance;
}

//=============================================================================
// 目的回転角再設定
//=============================================================================
void CCamera::ResetRotDest(void)
{
	m_rotDest.x = -(atan2f(m_posDest.y - m_atDest.y, m_posDest.z - m_atDest.z) - D3DX_PI);
	m_rotDest.y = -(atan2f(m_posDest.x - m_atDest.x, m_posDest.z - m_atDest.z) - D3DX_PI);

	if(m_rotDest.y < -D3DX_PI || m_rotDest.y > D3DX_PI)	// 符号替え
	{
		m_rotDest.y = m_rotDest.y + (-SIGN_F(m_rotDest.y) * D3DX_PI * 2.0f);
	}
}

//=============================================================================
// 距離再設定
//=============================================================================
void CCamera::ResetDistance(void)
{
	m_distance = DISTANCE_3D(m_pos.x, m_pos.y, m_pos.z);
}
