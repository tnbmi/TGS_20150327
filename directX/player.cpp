//*****************************************************************************
//
// CPlayerクラス [player.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "player.h"

#include "game.h"
#include "debugproc.h"
#include "fade.h"

#include "inputKeyboard.h"
#include "camera.h"
#include "manager.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SIGN(n)			((n) / abs(n))	// 符号取得
#define SIGN_F(n)		((n) / fabs(n))	// 符号取得(float)

#define PLAER_SPEED		(0.05f)
#define PLAER_SPEEDMAX	(1.0f)

#define STATE_MAX	(100)

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(int priority, OBJTYPE objType) : CSceneX(priority, objType)
{
	m_rot  = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_size = D3DXVECTOR3(5.0f, 15.0f, 5.0f);

	m_vecF = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecR = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_speed = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// 独自のリソース設定
	//----------------------------

	//----------------------------
	// 親クラス初期化
	//----------------------------
	HRESULT_FUNC(CSceneX::Init(device, CImport::X_PLAYER));

	//----------------------------
	// 初期化成功
	//----------------------------
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CPlayer::Uninit(void)
{
	//----------------------------
	// 独自の開放
	//----------------------------

	//----------------------------
	// 親クラス終了
	//----------------------------
	CSceneX::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CPlayer::Update(void)
{
	if(CPhase::GetFade()->GetState() == CFade::FADESTATE_NONE)
	{
		D3DXVECTOR3 cameraRot	 = CGame::GetCamera()->GetRot();
		D3DXMATRIX	workMtx;
		bool rotFlg = false;

		//----------------------------
		// 前情報処理
		//----------------------------
		// 前更新
		m_posOld = m_pos;
		m_rotOld = m_rot;

		//----------------------------
		// 移動入力
		//----------------------------
		// 奥移動
		if(m_keyboard->GetPress(DIK_W))
		{
			m_speed.x -= sinf(cameraRot.y) * PLAER_SPEED;
			m_speed.z += cosf(cameraRot.y) * PLAER_SPEED;

			rotFlg = true;
		}
		// 手前移動
		if(m_keyboard->GetPress(DIK_S))
		{
			m_speed.x += sinf(cameraRot.y) * PLAER_SPEED;
			m_speed.z -= cosf(cameraRot.y) * PLAER_SPEED;

			rotFlg = true;
		}
		// 左移動
		if(m_keyboard->GetPress(DIK_A))
		{
			m_speed.x -= cosf(cameraRot.y) * PLAER_SPEED;
			m_speed.z -= sinf(cameraRot.y) * PLAER_SPEED;

			rotFlg = true;
		}
		// 右移動
		if(m_keyboard->GetPress(DIK_D))
		{
			m_speed.x += cosf(cameraRot.y) * PLAER_SPEED;
			m_speed.z += sinf(cameraRot.y) * PLAER_SPEED;

			rotFlg = true;
		}

		// 上限速度
		if(m_speed.x > PLAER_SPEEDMAX)
		{
			m_speed.x = PLAER_SPEEDMAX;
		}
		else if(m_speed.x < -PLAER_SPEEDMAX)
		{
			m_speed.x = -PLAER_SPEEDMAX;
		}

		if(m_speed.z > PLAER_SPEEDMAX)
		{
			m_speed.z = PLAER_SPEEDMAX;
		}
		else if(m_speed.z < -PLAER_SPEEDMAX)
		{
			m_speed.z = -PLAER_SPEEDMAX;
		}

		// 重力
		m_speed.x += (0 - m_speed.x) * 0.07f;
		m_speed.z += (0 - m_speed.z) * 0.07f;
		//m_speed.y += -0.07f;

		//----------------------------
		// 移動
		//----------------------------
		m_pos += m_speed;

		//----------------------------
		// 回転
		//----------------------------
		if(rotFlg)
		{
			float diffRot;		// 回転目的までの差分
			float excessRot;	// 回転超過分

			// 横軸が移動している
			if(m_pos.x != m_posOld.x || m_pos.z != m_posOld.z)
			{
				D3DXVECTOR3 Def;			// 基準
				D3DXVECTOR3 End;			// 目的(終点)
				float		fVec1, fVec2;	// ベクトル絶対値
				float		fInner;			// 内積
	
				// ベクトル成分
				// 基準(z軸の奥)
				Def.x = 0;
				Def.z = D3DX_PI;
				// 目的(移動前と後の差)
				End.x = m_speed.x;
				End.z = -m_speed.z;

				// ベクトル絶対値
				// 基準
				fVec1 = sqrt(Def.x*Def.x + Def.z*Def.z);
				// 目的
				fVec2 = sqrt(End.x*End.x + End.z*End.z);

				// 内積
				fInner = (Def.x*End.x + Def.z*End.z);

				// 目的の角度
				m_rotDest.y = acos(fInner / (fVec1 * fVec2));

				if(End.x != 0)	// x軸が移動している
				{
					m_rotDest.y *= -SIGN(End.x);
				}
			}

			// 目的の向きまでの差分
			diffRot =	m_rotDest.y - m_rot.y;
			if(diffRot < -D3DX_PI || diffRot > D3DX_PI)	// 符号替え
			{
				excessRot	= diffRot - (SIGN_F(diffRot) * D3DX_PI);

				diffRot	= (SIGN_F(diffRot) * D3DX_PI) - excessRot;
				diffRot	= (diffRot * (-1)) + (SIGN_F(diffRot) * 0.01f);
			}

			// 向きを更新
			m_rot.y +=	diffRot * 0.1f;
			if(m_rot.y < -D3DX_PI || m_rot.y > D3DX_PI)	// 符号替え
			{
				m_rot.y = (m_rot.y * (-1)) + (SIGN_F(m_rot.y) * 0.1f);
			}

			// ベクトル変換
			D3DXMatrixRotationAxis(&workMtx, &m_vecU, 0.0f);
			D3DXVECTOR3 workRot = m_rot - m_rotOld;

			D3DXMatrixRotationAxis(&workMtx, &m_vecU, workRot.y);
			D3DXVec3TransformCoord(&m_vecF, &m_vecF, &workMtx);
			D3DXVec3TransformCoord(&m_vecR, &m_vecR, &workMtx);
			D3DXVec3Normalize(&m_vecU, &m_vecU);
			D3DXVec3Normalize(&m_vecF, &m_vecF);
			D3DXVec3Normalize(&m_vecR, &m_vecR);
		}

	#ifdef _DEBUG
		// プレイヤー情報表示
		CDebugproc::PrintDebugProc("***プレイヤー情報******\n");
		CDebugproc::PrintDebugProc("Pos  x:%f y:%f z:%f\n", m_pos.x, m_pos.y, m_pos.z);
		CDebugproc::PrintDebugProc("Speed x:%f y:%f z:%f\n", m_speed.x, m_speed.y, m_speed.z);
		CDebugproc::PrintDebugProc("Vec U:%f F:%f R:%f\n", m_vecU, m_vecF, m_vecR);
	#endif

		//----------------------------
		// 親クラス更新
		//----------------------------
		CSceneX::Update();
	}
}

//=============================================================================
// 描画
//=============================================================================
void CPlayer::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// 生成
//=============================================================================
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 device)
{
	CPlayer* scene = new CPlayer;
	scene->Init(device);

	return scene;
}
//=============================================================================
// 回転
//=============================================================================
void CPlayer::SetRot(D3DXVECTOR3 rot)
{
	D3DXVECTOR3 oldRot = m_rot;
	D3DXMATRIX	workMtx;
	m_rot = rot;

	if(m_rot.y < -D3DX_PI || m_rot.y > D3DX_PI)	// 符号替え
	{
		m_rot.y = m_rot.y + (-SIGN_F(m_rot.y) * D3DX_PI * 2.0f);
	}

	if(m_rot.x < -D3DX_PI || m_rot.x > D3DX_PI)	// 符号替え
	{
		m_rot.x = m_rot.x + (-SIGN_F(m_rot.x) * D3DX_PI * 2.0f);
	}

	// ベクトル変換
	D3DXMatrixRotationAxis(&workMtx, &m_vecU, 0.0f);
	D3DXVECTOR3 workRot = m_rot - oldRot;

	D3DXMatrixRotationAxis(&workMtx, &m_vecU, workRot.y);
	D3DXVec3TransformCoord(&m_vecF, &m_vecF, &workMtx);
	D3DXVec3TransformCoord(&m_vecR, &m_vecR, &workMtx);
	D3DXVec3Normalize(&m_vecF, &m_vecF);
	D3DXVec3Normalize(&m_vecR, &m_vecR);
}
