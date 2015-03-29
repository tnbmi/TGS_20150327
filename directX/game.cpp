//*****************************************************************************
//
// CGameクラス [game.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "game.h"
#include "manager.h"
#include "fade.h"

#include "result.h"

#include "inputKeyboard.h"

#include "camera.h"
#include "light.h"

#include "sceneBillboard.h"
#include "meshDome.h"
#include "meshField.h"

#include "player.h"
#include "dustManager.h"
#include "billManager.h"
#include "dust.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// カメラ
#define CAMERA_POS			D3DXVECTOR3(0.0f, 50.0f, -150.0f)
#define CAMERA_DEBUG_POS	D3DXVECTOR3(0.0f, 100.0f, -700.0f)

// ライト
#define LIGHT_MAX	(3)
const D3DXVECTOR3 LIGHT_VEC[LIGHT_MAX] =
{
	D3DXVECTOR3(1.2f, -2.0f, 3.0f),
	D3DXVECTOR3(1.0f, -1.5f, -1.5f),
	D3DXVECTOR3(-4.0f, 1.0f, -1.0f)
};
const D3DCOLORVALUE LIGHT_DIFFUSE[LIGHT_MAX] =
{
	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
	D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f),
	D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)
};

// ドーム
#define DOME_BLOCK_SIZE	D3DXVECTOR3(10.0f, 10.0f, 0.0f)
#define DOME_BLOCK_NUM	D3DXVECTOR3(18.0f, 16.0f, 0.0f)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 静的変数
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CCamera*		CGame::m_camera	= NULL;
CPlayer*		CGame::m_player	= NULL;
CDustManager*	CGame::m_dustManager = NULL;
CBillManager*	CGame::m_billManager = NULL;

//=============================================================================
// 初期化
//=============================================================================
HRESULT CGame::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// デバイス取得
	//----------------------------
	m_device = device;

	//----------------------------
	// カメラ
	//----------------------------
	m_camera = CCamera::Create(D3DXVECTOR3(0.0f, 20.0f, 0.0f), CAMERA_POS);

	m_dcFlg = false;
	m_debugCamera = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERA_DEBUG_POS);

	//----------------------------
	// ライト
	//----------------------------
	// 生成
	for(int cnt = 0; cnt < LIGHT_MAX; ++cnt)
	{
		m_light[cnt] = CLight::Create(device,
									  cnt,
									  D3DLIGHT_DIRECTIONAL,
									  LIGHT_VEC[cnt],
									  LIGHT_DIFFUSE[cnt],
									  D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f),
									  D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// 主電源オン
	device->SetRenderState(D3DRS_LIGHTING, TRUE);

	//----------------------------
	// オブジェクト
	//----------------------------
	InitObject(device);

	//----------------------------
	// フェード
	//----------------------------
	m_fade = CFade::Create(device);
	m_fade->Start(CFade::FADESTATE_IN, 1, 1.0f, 1.0f, 1.0f, 1.0f);

	//----------------------------
	// 初期化成功
	//----------------------------
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CGame::Uninit(void)
{
	//----------------------------
	// 敵情報
	//----------------------------
	SAFE_DELETE(m_dustManager);

	SAFE_DELETE(m_billManager);

	//----------------------------
	// オブジェクト
	//----------------------------
	// シーン
	CScene::ReleaseAll();

	//----------------------------
	// ビュー
	//----------------------------
	// カメラ
	SAFE_END(m_camera);
	SAFE_END(m_debugCamera);

	// ライト
	for(int cnt = 0; cnt < LIGHT_MAX; ++cnt)
	{
		SAFE_END(m_light[cnt]);
	}
}

//=============================================================================
// 更新
//=============================================================================
void CGame::Update(void)
{
	if(m_fade->GetState() == CFade::FADESTATE_NONE)
	{
		//----------------------------
		// デバッグコマンド
		//----------------------------
		Debug();

		//----------------------------
		// ライト
		//----------------------------
		for(int cnt = 0; cnt < LIGHT_MAX; ++cnt)
		{
			m_light[cnt]->Update();
		}

		//----------------------------
		// 更新内容
		//----------------------------
		// カメラ
		UpdateCamera();

		m_billManager->Update();

		// 敵との当たり判定
		/*CDust* dust = NULL;
		for(int cnt = 0;cnt < ENEMY_MAX; ++cnt)
		{
			dust = m_dustManager->GetDust(cnt);

			if(dust != nullptr)
			{
				if(m_player->HitMist(dust->GetPos()))
				{
					dust->SetDamege();
					break;
				}
			}
		}*/
	}

	//----------------------------
	// 画面遷移
	//----------------------------
	if(m_fade->GetState() == CFade::FADESTATE_OUTEND)
	{
		CManager::SetNextPhase((CPhase*)new CResult);
	}
}

//=============================================================================
// 描画
//=============================================================================
void CGame::Draw(void)
{
	// カメラ設定
	if(m_dcFlg)
	{
		m_debugCamera->SetCamera(m_device);
		CSceneBillboard::SetMtxView(m_debugCamera->GetMtxView());
	}
	else
	{
		m_camera->SetCamera(m_device);
		CSceneBillboard::SetMtxView(m_camera->GetMtxView());
	}
}

//=============================================================================
// デバック内容
//=============================================================================
void CGame::Debug(void)
{
	//----------------------------
	// 画面遷移
	//----------------------------
	if(m_keyboard->GetTrigger(DIK_RETURN))
	{
		m_fade->Start(CFade::FADESTATE_OUT, 1, 1.0f, 1.0f, 1.0f, 0.0f);
	}

	//----------------------------
	// カメラ切り替え
	//----------------------------
	if(m_keyboard->GetTrigger(DIK_F2))
	{
		if(m_dcFlg)
		{
			m_dcFlg = false;
		}
		else
		{
			m_dcFlg = true;
		}
	}

	if(m_keyboard->GetTrigger(DIK_B))
	{
		CDust* dust = NULL;
		for(int cnt = 0;cnt < ENEMY_MAX; ++cnt)
		{
			dust = m_dustManager->GetDust(cnt);
			if(dust != NULL)
			{
				dust->SetDamege();
				break;
			}
		}
	}
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
void CGame::InitObject(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// フィールド
	//----------------------------
	// 空
	CMeshDome* sky = CMeshDome::Create(device, CImport::TEX_DOME, DOME_BLOCK_SIZE, DOME_BLOCK_NUM);
	sky->SetSize(4000.0f, 4000.0f, 4000.0f);

	// 地面
	D3DXVECTOR3 size = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
	D3DXVECTOR3 num  = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	float*		heightVtx = nullptr;

	m_field = CMeshField::Create(device,
								 CImport::TEX_FIELD,
								 size,
								 num,
								 heightVtx,
								 CScene::MESHTEX_PATCH);

	//----------------------------
	// キャラクター
	//----------------------------
	m_player = CPlayer::Create(device);
	m_player->SetKeyboard(m_keyboard);

	// 壁
	CSceneX::Create(device,CImport::X_WALL);

	// 敵
	m_dustManager = CDustManager::Create(device);

	m_billManager = new CBillManager();
	m_billManager ->Init(device);
}

//=============================================================================
// カメラ更新
//=============================================================================
void CGame::UpdateCamera(void)
{
	D3DXVECTOR3 targetPos	= m_player->GetPos();
	D3DXVECTOR3 targetVecF	= m_player->GetVecF();
	D3DXVECTOR3 targetSpeed	= m_player->GetSpeed();
	D3DXVECTOR3 targetSize	= m_player->GetSize();

	D3DXVECTOR3 at			= m_camera->GetAt();
	D3DXVECTOR3 pos			= m_camera->GetPos();
	D3DXVECTOR3 rot			= m_camera->GetRot();
	float		distance	= m_camera->GetDistance();

	D3DXVECTOR3 atDest		= m_camera->GetAtDest();
	D3DXVECTOR3 posDest		= m_camera->GetPosDest();

	//----------------------------
	// 入力処理
	//----------------------------
	// 左旋回
	if(m_keyboard->GetPress(DIK_L) == true)
	{
		rot.y += 0.01f;
	}
	// 右旋回
	if(m_keyboard->GetPress(DIK_J) == true)
	{
		rot.y += -0.01f;
	}

	// 上旋回
	if(m_keyboard->GetPress(DIK_I) == true
	&& rot.x < D3DX_PI * 0.2f)
	{
		rot.x += 0.01f;
	}
	// 下旋回
	if(m_keyboard->GetPress(DIK_K) == true
	&& rot.x > -D3DX_PI * 0.05f)
	{
		rot.x += -0.01f;
	}

	// ズームイン
	if(m_keyboard->GetPress(DIK_U) == true
	&& distance > 25.0f)
	{
		distance -= 1.0f;
		m_camera->SetDistance(distance);
	}
	// ズームアウト
	if(m_keyboard->GetPress(DIK_O) == true
	&& distance < 80.0f)
	{
		distance += 1.0f;
		m_camera->SetDistance(distance);
	}

	//----------------------------
	// 回転
	//----------------------------
	// 符号替え
	if(rot.y < -D3DX_PI || rot.y > D3DX_PI)
	{
		rot.y = rot.y + (-SIGN_F(rot.y) * D3DX_PI * 2.0f);
	}

	if(rot.x < -D3DX_PI || rot.x > D3DX_PI)
	{
		rot.x = rot.x + (-SIGN_F(rot.x) * D3DX_PI * 2.0f);
	}

	//----------------------------
	// 注視点
	//----------------------------
	atDest.x = targetPos.x + targetVecF.x * 10.0f;
	atDest.z = targetPos.z + targetVecF.z * 10.0f;
	atDest.y = targetPos.y + targetSize.y;
	m_camera->SetAtDest(atDest);

	at += (atDest - at) * 0.5f;
	m_camera->SetAt(at);

	//----------------------------
	// 視点
	//----------------------------
	pos.x = cosf(rot.y - D3DX_PI * 0.5f) * cosf(rot.x) * distance;
	pos.z = sinf(rot.y - D3DX_PI * 0.5f) * cosf(rot.x) * distance;
	pos.y = sinf(rot.x) * distance;

	posDest.x = -targetVecF.x * cosf(rot.x) * distance;
	posDest.z = -targetVecF.z * cosf(rot.x) * distance;
	m_camera->SetPosDest(posDest);

	pos += (posDest - pos) * 0.05f;

	m_camera->SetPos(pos);
}
