//*****************************************************************************
//
// CTitleクラス [title.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "title.h"
#include "manager.h"
#include "fade.h"

#include "game.h"

#include "inputKeyboard.h"

#include "scene2D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 静的変数
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// 初期化
//=============================================================================
HRESULT CTitle::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// デバイス取得
	//----------------------------
	m_device = device;

	//----------------------------
	// カメラ
	//----------------------------

	//----------------------------
	// ライト
	//----------------------------
	// 生成
	// 主電源オン

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
void CTitle::Uninit(void)
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
	// ライト
}

//=============================================================================
// 更新
//=============================================================================
void CTitle::Update(void)
{
	if(m_fade->GetState() == CFade::FADESTATE_NONE)
	{
		//----------------------------
		// ライト
		//----------------------------

		//----------------------------
		// 入力
		//----------------------------
		if(m_keyboard->GetTrigger(DIK_RETURN))
		{
			m_fade->Start(CFade::FADESTATE_OUT, 1, 1.0f, 1.0f, 1.0f, 0.0f);
		}
	}

	//----------------------------
	// 画面遷移
	//----------------------------
	if(m_fade->GetState() == CFade::FADESTATE_OUTEND)
	{
		CManager::SetNextPhase((CPhase*)new CGame);
	}
}

//=============================================================================
// 描画
//=============================================================================
void CTitle::Draw(void)
{
	//----------------------------
	// カメラ設定
	//----------------------------

	// ビルボードのマトリックス設定
}

//=============================================================================
// オブジェクト初期化
//=============================================================================
void CTitle::InitObject(LPDIRECT3DDEVICE9 device)
{
	CScene2D* scene = CScene2D::Create(device, CImport::TEX_TITLE, CScene2D::POINT_CENTER);
	scene->SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	scene->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
}