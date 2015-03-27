//*****************************************************************************
//
// CMistクラス [mist.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "mist.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define LIFE_TIME	(60)
#define GRAVITY		(0.1f)
#define POLIGON_SIZE (3.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
CMist::CMist(int priority, OBJTYPE objType) : CSceneBillboard(priority, objType)
{
	m_size = D3DXVECTOR3(POLIGON_SIZE, POLIGON_SIZE, 0.0f);
	m_speed = D3DXVECTOR3(5.0f, 0.0f, 5.0f);

	m_pos.x = (float)(rand() % 6) - 3 * 0.1f;
	m_pos.y = (float)(rand() % 6) - 3 * 0.1f;
	m_pos.z = (float)(rand() % 6) - 3 * 0.1f;
}

//=============================================================================
// 生成
//=============================================================================
CMist* CMist::Create(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 vec)
{
	CMist* pointer = new CMist;
	pointer->Init(device, pos, vec);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMist::Init(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 vec)
{
	//----------------------------
	// 独自のリソース設定
	//----------------------------
	m_time = LIFE_TIME;
	m_pos += pos;
	m_vec = vec;

	//----------------------------
	// 親クラス初期化
	//----------------------------
	HRESULT_FUNC(CSceneBillboard::Init(device, CImport::TEX_MIST))

	//----------------------------
	// 初期化成功
	//----------------------------
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CMist::Uninit(void)
{
	//----------------------------
	// 親クラス終了
	//----------------------------
	CSceneBillboard::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CMist::Update(void)
{
	//----------------------------
	// 移動
	//----------------------------
	m_pos.x	+= m_vec.x * m_speed.x;
	m_pos.z	+= m_vec.z * m_speed.z;
	m_pos.y += m_speed.y;
	
	m_speed.y += -0.1f;

	//----------------------------
	// カウントダウン
	//----------------------------
	m_time--;
	if(m_time < 0 || m_pos.y < 0.0f)
	{
		Delete();
	}

	//----------------------------
	// 親クラス更新
	//----------------------------
	CSceneBillboard::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CMist::Draw(void)
{
	// 加算レンダーステート
	m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// 通常合成
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// αソースカラーの指定
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// αデスティネーションカラーの指定

	m_device->SetRenderState(D3DRS_ZENABLE, FALSE);

	// 描画
	CSceneBillboard::Draw();

	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// デフォルトレンダーステート
	m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 通常合成
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定
}