//*****************************************************************************
//
// CFadeクラス [fade.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "fade.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int FADE_CNG[CFade::FADESTATE_MAX] =
{
	0,
	1,
	0,
	-1,
	0
};

const CFade::FADESTATE FADE_NEXT[CFade::FADESTATE_MAX] =
{
	CFade::FADESTATE_NONE,
	CFade::FADESTATE_OUTEND,
	CFade::FADESTATE_NONE,
	CFade::FADESTATE_INEND,
	CFade::FADESTATE_NONE
};

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade(int priority, OBJTYPE objType) : CScene2D(priority, objType)
{
	// メンバ初期化
	m_state = FADESTATE_NONE;
	m_time	= 0;
	m_alpha = 0.0f;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_alpha);
	m_flg	= false;
}

//=============================================================================
// 生成
//=============================================================================
CFade* CFade::Create(LPDIRECT3DDEVICE9 device)
{
	CFade* pointer = new CFade;
	pointer->Init(device);

	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CFade::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// 独自のリソース設定
	//----------------------------

	//----------------------------
	// 親クラス初期化
	//----------------------------
	HRESULT_FUNC(CScene2D::Init(device, CImport::TEX_FADE, CScene2D::POINT_CENTER))
	SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));

	//----------------------------
	// 初期化成功
	//----------------------------
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CFade::Uninit(void)
{
	//----------------------------
	// 独自の開放
	//----------------------------

	//----------------------------
	// 親クラス終了
	//----------------------------
	CScene2D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CFade::Update(void)
{
	//----------------------------
	// フェード処理
	//----------------------------
	if(m_flg && m_time >= 0)
	{
		// アルファ値変更
		m_alpha += m_cngAlpha * FADE_CNG[m_state];
		if(m_alpha < 0.0f)
		{
			m_alpha = 0.0f;
		}
		else if(m_alpha > 1.0f)
		{
			m_alpha = 1.0f;
		}

		m_color = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_alpha);
		CScene2D::SetColor(m_color);

		// カウントダウン
		m_time--;
		if(m_time < 0)
		{
			// モード更新
			m_state = FADE_NEXT[m_state];
			m_flg = false;
		}
	}
	else if(m_state == FADESTATE_INEND)
	{
		m_state = FADESTATE_NONE;
	}

	//----------------------------
	// 親クラス更新
	//----------------------------
	CScene2D::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CFade::Draw(void)
{
	if(m_state != FADESTATE_NONE)
	{
		// αテストオフ
		m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// 描画
		CScene2D::Draw();

		// αテストオン
		m_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	}
}

//=============================================================================
// 開始
//=============================================================================
void CFade::Start(FADESTATE state, int time, float r, float g, float b, float a)
{
	m_state		= state;
	m_time		= time * 60;
	m_alpha		= a;
	m_color		= D3DXCOLOR(r, g, b, a);
	m_cngAlpha	= 1.0f / (float)m_time;
	CScene2D::SetColor(m_color);

	if(state == FADESTATE_IN || state == FADESTATE_OUT)
	{
		m_flg = true;
	}
}