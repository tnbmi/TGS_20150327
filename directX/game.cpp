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
#include "meshField.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// カメラ
#define CAMERA_POS	D3DXVECTOR3(0.0f, 500.0f, -1000.0f)

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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 静的変数
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CCamera*	CGame::m_camera	= NULL;

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
	m_camera = new CCamera();
	HRESULT_FUNC(m_camera->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERA_POS));

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
	// オブジェクト
	//----------------------------
	// シーン
	CScene::ReleaseAll();

	//----------------------------
	// ビュー
	//----------------------------
	// カメラ
	SAFE_END(m_camera);

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
	m_camera->SetCamera(m_device);
	CSceneBillboard::SetMtxView(m_camera->GetMtxView());
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
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
void CGame::InitObject(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// フィールド
	//----------------------------
	D3DXVECTOR3 size = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	D3DXVECTOR3 num  = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
	float*		heightVtx = nullptr;

	m_field = CMeshField::Create(device,
								 CImport::TEX_FIELD,
								 size,
								 num,
								 heightVtx,
								 CScene::MESHTEX_PATCH);
}

//=============================================================================
// カメラ更新
//=============================================================================
void CGame::UpdateCamera(void)
{
	D3DXVECTOR3 targetPos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 targetVecF	= D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 targetSpeed	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 targetSize	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);

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

	// プレイヤーが移動中
	if(abs(targetSpeed.x) >= 1.0f || abs(targetSpeed.z) >= 1.0f)
	{
		D3DXVECTOR2 vec[2];

		// カメラベクトル
		vec[0].x = cosf(rot.y - D3DX_PI * 0.5f);
		vec[0].y = sinf(rot.y - D3DX_PI * 0.5f);

		// プレイヤー後ろベクトル
		vec[1].x = -targetVecF.x;
		vec[1].y = -targetVecF.z;

		// 内積
		float dot = D3DXVec2Dot(&vec[0], &vec[1]);

		// 鋭角ならば回りこむ
		if(dot > 0.0f)
		{
			posDest.x = -targetVecF.x * cosf(rot.x) * distance;
			posDest.z = -targetVecF.z * cosf(rot.x) * distance;
			m_camera->SetPosDest(posDest);
		}

		pos += (posDest - pos) * 0.03f;
	}

	m_camera->SetPos(pos);
}
