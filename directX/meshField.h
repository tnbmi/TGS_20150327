//*****************************************************************************
//
// CMeshFieldクラス [meshField.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_MESH_FILED_H
#define _MY_MESH_FILED_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "scene.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CMeshField : public CScene
{
public:
	CMeshField(int priority = 0, OBJTYPE objType = OBJTYPE_FIELD);
	~CMeshField(){};

	static CMeshField* Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, float* heightVtx, MESHTEX_TYPE texType);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, float* heightVtx, MESHTEX_TYPE texType);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	void		SetPos(D3DXVECTOR3 pos){m_pos = pos; SetVertexPolygon(m_texType);}
	void		SetPos(float x, float y, float z){m_pos = D3DXVECTOR3(x, y, z); SetVertexPolygon(m_texType);}
	D3DXVECTOR3	GetPos(void){return m_pos;}
	void		SetRot(D3DXVECTOR3 rot){m_rot = rot; SetVertexPolygon(m_texType);}
	void		SetRot(float x, float y, float z){m_rot = D3DXVECTOR3(x, y, z); SetVertexPolygon(m_texType);}
	D3DXVECTOR3	GetRot(void){return m_rot;}
	void		SetSize(D3DXVECTOR3 size){m_size = size; SetVertexPolygon(m_texType);}
	void		SetSize(float x, float y, float z){m_size = D3DXVECTOR3(x, y, z); SetVertexPolygon(m_texType);}
	D3DXVECTOR3	GetSize(void){return m_size;}
	void		SetColor(D3DXCOLOR color){m_color = color; SetVertexPolygon(m_texType);}
	void		SetColor(float r, float g, float b, float a){m_color = D3DXCOLOR(r, g, b, a); SetVertexPolygon(m_texType);}
	D3DXCOLOR	GetColor(void){return m_color;}

	float GetHeight(const D3DXVECTOR3 pos, D3DXVECTOR3* normal);

private:
	void	SetVertexPolygon(MESHTEX_TYPE texType);
	void	SetVertexIndexPolygon(void);
	float	GetHeightPolygon(const D3DXVECTOR3 P0, const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DXVECTOR3 pos, D3DXVECTOR3* normal);
	bool	ChkInPolygon(const D3DXVECTOR3 P0, const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DXVECTOR3 pos);

	LPDIRECT3DTEXTURE9		m_tex;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;		// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_indexBuff;	// インデックスバッファへのポインタ

	D3DXMATRIX	 m_world;	// ワールドマトリックス
	D3DXVECTOR3* m_vtx;		// 各頂点座標
	int m_numVtx;		// 総頂点数
	int m_numVtxIndex;	// 頂点の総インデックス数
	int m_numPolygon;	// 総ポリゴン数

	D3DXVECTOR3  m_sizeBlock;	// ブロックサイズ
	D3DXVECTOR3  m_numBlock;	// ブロック数
	float*		 m_heightVtx;	// 頂点の高さ
	D3DXVECTOR3* m_norBuff;		// 面法線バッファ
	MESHTEX_TYPE m_texType;		// テクスチャタイプ(貼り方)
};

//=============================================================================
#endif
