//*****************************************************************************
//
// CDustクラス [dust.cpp]
// Author : YUJI MOMOI
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "dust.h"
#include "dustAI.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// コンストラクタ
//=============================================================================
CDust::CDust(int priority, OBJTYPE objType) : CSceneX(priority, objType)
{
	m_xFile.mesh	= NULL;
	m_xFile.buffMat = NULL;

	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDest = m_pos;
	m_posOld = m_pos;
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_size	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_parent	= NULL;
	m_AI		= NULL;

	m_FrontVector = D3DXVECTOR3(0.0f,0.0f,-1.0f);
	m_RightVector = D3DXVECTOR3(-1.0f,0.0f,0.0f);
	m_RotPower = D3DXVECTOR3(0,0,0);
}

//=============================================================================
// 生成
//=============================================================================
CDust* CDust::Create(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile)
{
	CDust* pointer = new CDust;
	pointer->Init(device, xFile);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CDust::Init(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile)
{
	//----------------------------
	// デバイス取得
	//----------------------------
	m_device = device;

	//----------------------------
	// モデル取得
	//----------------------------
	m_xFile = CImport::GetXFile(xFile);

	//----------------------------
	// AI生成
	//----------------------------
	m_AI = CDustAI::Create(this);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CDust::Uninit(void)
{
	// 開放
	CScene::Delete();

	// AI解放
	SAFE_DELETE(m_AI);
}

//=============================================================================
// 更新
//=============================================================================
void CDust::Update(void)
{
	// AI更新
	m_AI->Update();

	// 移動
	m_posOld = m_pos;
	D3DXVECTOR3 front = m_FrontVector;
	D3DXVECTOR3 right = m_RightVector;
	D3DXVECTOR3 vecTarget = m_posDest - m_pos;

	D3DXVec3Normalize(&vecTarget,&vecTarget);
	float dot = D3DXVec3Dot(&right,&vecTarget);
	m_RotPower.y += dot * DUST_ROTATION_SPEED;

	float frontDot = D3DXVec3Dot(&front,&vecTarget);
	if(frontDot < -0.95f)
	{
		m_RotPower.y += DUST_ROTATION_SPEED;
	}

	if(frontDot > -0.2f)
	{
		m_pos += front * DUST_MOVE_SPEED;
	}

	front.x = sinf(-m_RotPower.y);
	front.z = cosf(m_RotPower.y - D3DX_PI);

	right.x = cosf(m_RotPower.y - D3DX_PI);
	right.z = sinf(m_RotPower.y);

	m_FrontVector = front;
	m_RightVector = right;
}

//=============================================================================
// 描画
//=============================================================================
void CDust::Draw(void)
{
	CSceneX::Draw();
}