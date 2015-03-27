//*****************************************************************************
//
// CSceneXクラス [sceneX.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "sceneX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneX::CSceneX(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_xFile.mesh	= NULL;
	m_xFile.buffMat = NULL;

	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_size	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_parent = NULL;
}

//=============================================================================
// 生成
//=============================================================================
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile)
{
	CSceneX* pointer = new CSceneX;
	pointer->Init(device, xFile);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CSceneX::Init(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile)
{
	//----------------------------
	// デバイス取得
	//----------------------------
	m_device = device;

	//----------------------------
	// モデル取得
	//----------------------------
	m_xFile = CImport::GetXFile(xFile);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CSceneX::Uninit(void)
{
	// 開放
	CScene::Delete();
}

//=============================================================================
// 更新
//=============================================================================
void CSceneX::Update(void)
{
}

//=============================================================================
// 描画
//=============================================================================
void CSceneX::Draw(void)
{
	//----------------------------
	// ワールドマトリックス
	//----------------------------
	D3DXMATRIX	scl, rot, translate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_world);
	// スケールの反映
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&m_world, &m_world, &scl);
	// 回転の反映
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_world, &m_world, &rot);
	// 位置の反映
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_world, &m_world, &translate);

	//親が存在する場合は親マトリクスを合成
	if(m_parent)
	{
		D3DXMatrixMultiply(&m_world, &m_world, &m_parent->m_world);
	}

	// ワールドマトリックスの設定
	m_device->SetTransform(D3DTS_WORLD, &m_world);

	//----------------------------
	// モデルの描画
	//----------------------------
	D3DMATERIAL9 matDef;

	m_device->GetMaterial(&matDef);	// 現在のマテリアルを取得

	for(unsigned int cntMat = 0; cntMat < m_xFile.numMat; cntMat++)
	{
		m_device->SetMaterial(&m_xFile.mat[cntMat].MatD3D);
		m_device->SetTexture(0, m_xFile.tex[cntMat]);
		m_xFile.mesh->DrawSubset(cntMat);
	}

	m_device->SetMaterial(&matDef);	// マテリアルを元に戻す
}