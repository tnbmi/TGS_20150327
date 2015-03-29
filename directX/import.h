//*****************************************************************************
//
// CImportクラス [import.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_IMPORT_H
#define _MY_IMPORT_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 構造体定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct XFILE
{
	XFILE()
	{
		buffMat	= nullptr;
		numMat	= 0;
		mesh	= nullptr;
		mat		= nullptr;
		tex		= nullptr;
	}

	LPD3DXBUFFER		buffMat;	// マテリアル情報へのポインタ
	DWORD				numMat;		// マテリアル情報の数
	LPD3DXMESH			mesh;		// メッシュ情報へのポインタ
	D3DXMATERIAL*		mat;		// マテリアルデータ先頭ポインタ
	LPDIRECT3DTEXTURE9*	tex;		// テクスチャデータ先頭ポインタ
} XFILE;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CImport
{
public:
	typedef enum
	{
		TEX_NONE = 0,
		TEX_FADE,
		TEX_DOME,
		TEX_FIELD,
		TEX_WALL,
		TEX_MIST,
		TEX_TITLE,
		TEX_RESULT,
		TEX_MAX
	} TEXTURES;

	typedef enum
	{
		X_NONE = 0,
		X_PLAYER,
		X_WALL,
		X_ENEMY,
		X_BILL,
		X_THREE_BILL,
		X_L_BILL,
		X_MAX
	} XFILES;

	CImport();
	~CImport(){};

	static	CImport* Create(LPDIRECT3DDEVICE9 device);
	HRESULT	Init(LPDIRECT3DDEVICE9 device);
	void	Uninit(void);

	static LPDIRECT3DTEXTURE9 GetTexture(TEXTURES tex){return m_tex[tex];}
	static XFILE			  GetXFile(XFILES x){return m_xFile[x];}

	static D3DXVECTOR3 GetVtxSizeMin(XFILES index){return m_VtxMin[index];}
	static D3DXVECTOR3 GetVtxSizeMax(XFILES index){return m_VtxMax[index];}
	static D3DXVECTOR3 GetVtxMin(int index);
	static D3DXVECTOR3 GetVtxMax(int index);

private:
	static LPDIRECT3DTEXTURE9 m_tex[TEX_MAX];
	static XFILE			  m_xFile[X_MAX];

	static D3DXVECTOR3        m_VtxMin[X_MAX];
	static D3DXVECTOR3        m_VtxMax[X_MAX];
};

//=============================================================================
#endif