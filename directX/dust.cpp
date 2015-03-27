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
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_size	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_parent	= NULL;
	m_AI		= NULL;
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
	m_AI->Update();
}

//=============================================================================
// 描画
//=============================================================================
void CDust::Draw(void)
{
	CSceneX::Draw();
}