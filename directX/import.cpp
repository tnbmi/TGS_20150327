//*****************************************************************************
//
// CImportクラス [import.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// テクスチャ
const char* TEX_PATH[] =
{
	NULL,
	"./data/TEXTURE/fade000.png",
	"./data/TEXTURE/concrete.jpg",
};

// Xファイル
const char* X_PATH[] =
{
	NULL,
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 静的変数
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LPDIRECT3DTEXTURE9	CImport::m_tex[TEX_MAX];
XFILE				CImport::m_xFile[X_MAX];

//=============================================================================
// コンストラクタ
//=============================================================================
CImport::CImport(void)
{
	// テクスチャ
	for(int cnt = 0; cnt < TEX_MAX; ++cnt)
	{
		m_tex[cnt] = nullptr;
	}

	// Xファイル
	for(int cnt = 0; cnt < TEX_MAX; ++cnt)
	{
		m_xFile[cnt] = XFILE();
	}
}

//=============================================================================
// 生成
//=============================================================================
CImport* CImport::Create(LPDIRECT3DDEVICE9 device)
{
	CImport* pointer = new CImport;
	pointer->Init(device);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CImport::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// テクスチャ
	//----------------------------
	for(int cnt = 0; cnt < TEX_MAX; ++cnt)
	{
		D3DXCreateTextureFromFile(device, TEX_PATH[cnt], &m_tex[cnt]);
	}

	//----------------------------
	// Xファイル
	//----------------------------
	for(int cnt = 0; cnt < X_MAX; ++cnt)
	{
		// モデル読み込み
		if(FAILED(D3DXLoadMeshFromX(X_PATH[cnt],
									D3DXMESH_SYSTEMMEM,
									device,
									NULL,
									&m_xFile[cnt].buffMat,
									NULL,
									&m_xFile[cnt].numMat,
									&m_xFile[cnt].mesh)))
		{
			continue;
		}

		// マテリアルの設定
		m_xFile[cnt].mat = (D3DXMATERIAL *)m_xFile[cnt].buffMat->GetBufferPointer();	// マテリアルの先頭アドレス取得
		m_xFile[cnt].tex = new LPDIRECT3DTEXTURE9[m_xFile[cnt].numMat];

		for(unsigned int cntMat = 0; cntMat < m_xFile[cnt].numMat; cntMat++)
		{
			//テクスチャをロード
			m_xFile[cnt].tex[cntMat] = NULL;

			if(m_xFile[cnt].mat[cntMat].pTextureFilename != NULL)
			{
				HRESULT hr = D3DXCreateTextureFromFile(device, m_xFile[cnt].mat[cntMat].pTextureFilename, &m_xFile[cnt].tex[cntMat]);
				if(FAILED(hr))
				{
					m_xFile[cnt].tex[cntMat] = NULL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CImport::Uninit(void)
{
	//----------------------------
	// Xファイル
	//----------------------------
	for(int cnt = 0; cnt < X_MAX; cnt++)
	{
		// テクスチャポインタ解放
		SAFE_DELETE(m_xFile[cnt].tex);

		// メッシュ情報の開放
		SAFE_RELEASE(m_xFile[cnt].mesh);

		// マテリアル情報の開放
		SAFE_RELEASE(m_xFile[cnt].buffMat);
	}

	//----------------------------
	// テクスチャ
	//----------------------------
	for(int cnt = 0; cnt < TEX_MAX; cnt++)
	{
		SAFE_RELEASE(m_tex[cnt]);
	}
}
