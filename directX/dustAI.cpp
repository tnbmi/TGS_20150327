//*****************************************************************************
//
// CDustAIAIクラス [dustAI.cpp]
// Author : YUJI MOMOI
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "dustAI.h"
#include "dust.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// コンストラクタ
//=============================================================================
CDustAI::CDustAI()
{
}

//=============================================================================
// 生成
//=============================================================================
CDustAI* CDustAI::Create(CDust* pDust)
{
	CDustAI* pointer = new CDustAI;
	pointer->Init(pDust);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CDustAI::Init(CDust* pDust)
{

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CDustAI::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CDustAI::Update(void)
{
}