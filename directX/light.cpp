//*****************************************************************************
//
// ライト処理 [light.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "light.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight(void)
{
	// メンバ初期化
	m_index	 = 0;
	m_vecDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// 生成
//=============================================================================
CLight* CLight::Create(LPDIRECT3DDEVICE9 device, int index, D3DLIGHTTYPE type, D3DXVECTOR3 vecDir, D3DCOLORVALUE diffuse, D3DCOLORVALUE specular, D3DCOLORVALUE ambient)
{
	CLight* light = new CLight;

	light->Init(device, index, type, vecDir, diffuse, ambient, specular);

	return light;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLight::Init(LPDIRECT3DDEVICE9 device, int index, D3DLIGHTTYPE type, D3DXVECTOR3 vecDir, D3DCOLORVALUE diffuse, D3DCOLORVALUE specular, D3DCOLORVALUE ambient)
{
	//----------------------------
	// メンバー設定
	//----------------------------
	m_index	 = index;
	m_vecDir = vecDir;

	//----------------------------
	// メインライトの初期化
	//----------------------------
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));					// 初期化(クリア)

	m_light.Type	 = type;
	m_light.Diffuse	 = diffuse;
	m_light.Ambient	 = ambient;
	m_light.Specular = specular;

	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, &m_vecDir);	// 正規化

	device->SetLight(m_index, &m_light);	// 値を設定
	device->LightEnable(m_index, TRUE);		// 有効にする

	//----------------------------
	// 初期化成功
	//----------------------------
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CLight::Update(void)
{
}
