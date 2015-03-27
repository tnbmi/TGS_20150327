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
		TEX_FIELD,
		TEX_MAX
	} TEXTURES;

	typedef enum
	{
		X_NONE = 0,
<<<<<<< HEAD
=======
		X_WALL,
		X_ENEMY,
>>>>>>> origin/dev/zyamu
		X_PLAYER,
		X_MAX
	} XFILES;

	CImport();
	~CImport(){};

	static	CImport* Create(LPDIRECT3DDEVICE9 device);
	HRESULT	Init(LPDIRECT3DDEVICE9 device);
	void	Uninit(void);

	static LPDIRECT3DTEXTURE9 GetTexture(TEXTURES tex){return m_tex[tex];}
	static XFILE			  GetXFile(XFILES x){return m_xFile[x];}

private:
	static LPDIRECT3DTEXTURE9 m_tex[TEX_MAX];
	static XFILE			  m_xFile[X_MAX];
};

//=============================================================================
#endif