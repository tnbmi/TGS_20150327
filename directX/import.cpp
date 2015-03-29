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
	"./data/TEXTURE/fade.jpg",
	"./data/TEXTURE/sky000.jpg",
	"./data/TEXTURE/concrete.jpg",
	"./data/TEXTURE/dirt.png",
	"./data/TEXTURE/mist.jpg",
	"./data/TEXTURE/title.jpg",
	"./data/TEXTURE/result.jpg",
};

// Xファイル
const char* X_PATH[] =
{
	NULL,
	"./data/MODEL/yogore_souzi.x",
	"./data/MODEL/wall.x",
	"./data/MODEL/dust.x",
	"./data/MODEL/Bill.x",
	"./data/MODEL/Three_Bill.x",
	"./data/MODEL/L_Bill.x"
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 静的変数
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LPDIRECT3DTEXTURE9	CImport::m_tex[TEX_MAX];
XFILE				CImport::m_xFile[X_MAX];

D3DXVECTOR3 CImport::m_VtxMin[];
D3DXVECTOR3 CImport::m_VtxMax[];

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

	// サイズ
	for(int cnt = 0; cnt < TEX_MAX; ++cnt)
	{
		m_VtxMin[cnt] = D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_VtxMax[cnt] = D3DXVECTOR3(0.0f,0.0f,0.0f);
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
	for(int cnt = 1; cnt < TEX_MAX; ++cnt)
	{
		D3DXCreateTextureFromFile(device, TEX_PATH[cnt], &m_tex[cnt]);
	}

	//----------------------------
	// Xファイル
	//----------------------------
	for(int cnt = 1; cnt < X_MAX; ++cnt)
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

		// サイズの取得
		m_VtxMin[cnt] = GetVtxMin(cnt);
		m_VtxMax[cnt] = GetVtxMax(cnt);
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
	for(int cnt = 1; cnt < X_MAX; cnt++)
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
	for(int cnt = 1; cnt < TEX_MAX; cnt++)
	{
		// テクスチャの開放
		SAFE_RELEASE(m_tex[cnt]);
	}
}


//=============================================================================
// 頂点座標を取得(最小値)
// 引数:無
// 戻値:無
//=============================================================================
D3DXVECTOR3 CImport::GetVtxMin(int index)
{
	D3DXVECTOR3 pos[2];
	BYTE *pVertexBuffer;								// 頂点バッファ
	WORD *pIndexBuffer;									// インデックスバッファ
	D3DXVECTOR3 Vertex;									// 頂点
	DWORD NumFaces = m_xFile[index].mesh->GetNumFaces();		// ポリゴン数の取得
	DWORD NumVertex = m_xFile[index].mesh->GetNumVertices();	// 頂点数の取得
	DWORD FVF = m_xFile[index].mesh->GetFVF();					// 頂点フォーマットを取得
	DWORD VertexSize = D3DXGetFVFVertexSize(FVF);		// 頂点サイズの取得
	
	// ロック（頂点バッファ）
	// ロック（インデックスバッファ）
	m_xFile[index].mesh->LockVertexBuffer(D3DLOCK_READONLY,(LPVOID*)&pVertexBuffer);
	m_xFile[index].mesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&pIndexBuffer);
	
	// ポリゴン数だけループ
	DWORD VerNum[3];				// 頂点番号(インデックス)
	D3DXVECTOR3 VerPos[3];				// 頂点座標
	D3DXVECTOR3 VerPos2[3];			// 頂点座標
	D3DXVECTOR3 Ver[3];				// 頂点座標

	// 頂点番号取得(何番目の頂点化)
	VerNum[0] = pIndexBuffer[0];	// 頂点1
	VerNum[1] = pIndexBuffer[1];	// 頂点2
	VerNum[2] = pIndexBuffer[2];	// 頂点3
	
	// 頂点座標取得
	VerPos[0] = VerPos2[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
	VerPos[1] = VerPos2[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
	VerPos[2] = VerPos2[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

	Ver[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
	Ver[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
	Ver[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

	for( unsigned int i = 0; i < NumFaces; i++ )
	{
		// 頂点番号取得(何番目の頂点化)
		VerNum[0] = pIndexBuffer[i*3 + 0];	// 頂点1
		VerNum[1] = pIndexBuffer[i*3 + 1];	// 頂点2
		VerNum[2] = pIndexBuffer[i*3 + 2];	// 頂点3
	
		// 頂点座標取得
		Ver[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
		Ver[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
		Ver[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);
		
		if( VerPos[0].x > Ver[0].x )
		{
			VerPos[0].x = Ver[0].x;
		}

		if( VerPos[1].x > Ver[1].x )
		{
			VerPos[1].x = Ver[1].x;
		}

		if( VerPos[2].x > Ver[2].x )
		{
			VerPos[2].x = Ver[2].x;
		}

		if( VerPos[0].y > Ver[0].y )
		{
			VerPos[0].y = Ver[0].y;
		}

		if( VerPos[1].y > Ver[1].y )
		{
			VerPos[1].y = Ver[1].y;
		}

		if( VerPos[2].y > Ver[2].y )
		{
			VerPos[2].y = Ver[2].y;
		}

		if( VerPos[0].z > Ver[0].z )
		{
			VerPos[0].z = Ver[0].z;
		}

		if( VerPos[1].z > Ver[1].z )
		{
			VerPos[1].z = Ver[1].z;
		}

		if( VerPos[2].z > Ver[2].z )
		{
			VerPos[2].z = Ver[2].z;
		}
	}

	pos[0] = VerPos[0];

	// アンロック（頂点バッファ）
	// アンロック（インデックスバッファ）
	m_xFile[index].mesh->UnlockVertexBuffer();
	m_xFile[index].mesh->UnlockIndexBuffer();

	return pos[0];
}

//=============================================================================
// 頂点座標を取得(最大値)
// 引数:無
// 戻値:無
//=============================================================================
D3DXVECTOR3 CImport::GetVtxMax(int index)
{
	D3DXVECTOR3 pos[2];
	BYTE *pVertexBuffer;								// 頂点バッファ
	WORD *pIndexBuffer;									// インデックスバッファ
	D3DXVECTOR3 Vertex;									// 頂点
	DWORD NumFaces = m_xFile[index].mesh->GetNumFaces();		// ポリゴン数の取得
	DWORD NumVertex = m_xFile[index].mesh->GetNumVertices();	// 頂点数の取得
	DWORD FVF = m_xFile[index].mesh->GetFVF();					// 頂点フォーマットを取得
	DWORD VertexSize = D3DXGetFVFVertexSize(FVF);		// 頂点サイズの取得
	
	// ロック（頂点バッファ）
	// ロック（インデックスバッファ）
	m_xFile[index].mesh->LockVertexBuffer(D3DLOCK_READONLY,(LPVOID*)&pVertexBuffer);
	m_xFile[index].mesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&pIndexBuffer);
	
	// ポリゴン数だけループ
	DWORD VerNum[3];				// 頂点番号(インデックス)
	D3DXVECTOR3 VerPos[3];				// 頂点座標
	D3DXVECTOR3 VerPos2[3];			// 頂点座標
	D3DXVECTOR3 Ver[3];				// 頂点座標

	// 頂点番号取得(何番目の頂点化)
	VerNum[0] = pIndexBuffer[0];	// 頂点1
	VerNum[1] = pIndexBuffer[1];	// 頂点2
	VerNum[2] = pIndexBuffer[2];	// 頂点3
	
	// 頂点座標取得
	VerPos[0] = VerPos2[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
	VerPos[1] = VerPos2[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
	VerPos[2] = VerPos2[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

	Ver[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
	Ver[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
	Ver[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

	for( unsigned int i = 0; i < NumFaces; i++ )
	{
		// 頂点番号取得(何番目の頂点化)
		VerNum[0] = pIndexBuffer[i*3 + 0];	// 頂点1
		VerNum[1] = pIndexBuffer[i*3 + 1];	// 頂点2
		VerNum[2] = pIndexBuffer[i*3 + 2];	// 頂点3
	
		// 頂点座標取得
		Ver[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
		Ver[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
		Ver[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

		if( VerPos2[0].x < Ver[0].x )
		{
			VerPos2[0].x = Ver[0].x;
		}

		if( VerPos2[1].x < Ver[1].x )
		{
			VerPos2[1].x = Ver[1].x;
		}

		if( VerPos2[2].x < Ver[2].x )
		{
			VerPos2[2].x = Ver[2].x;
		}

		if( VerPos2[0].y < Ver[0].y )
		{
			VerPos2[0].y = Ver[0].y;
		}

		if( VerPos2[1].y < Ver[1].y )
		{
			VerPos2[1].y = Ver[1].y;
		}

		if( VerPos2[2].y < Ver[2].y )
		{
			VerPos2[2].y = Ver[2].y;
		}

		if( VerPos2[0].z < Ver[0].z )
		{
			VerPos2[0].z = Ver[0].z;
		}

		if( VerPos2[1].z < Ver[1].z )
		{
			VerPos2[1].z = Ver[1].z;
		}

		if( VerPos2[2].z < Ver[2].z )
		{
			VerPos2[2].z = Ver[2].z;
		}
	}

	pos[1] = VerPos2[0];

	// アンロック（頂点バッファ）
	// アンロック（インデックスバッファ）
	m_xFile[index].mesh->UnlockVertexBuffer();
	m_xFile[index].mesh->UnlockIndexBuffer();

	return pos[1];
}