//*****************************************************************************
//
// CMeshCylinderクラス [meshCylinder.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_MESH_CYLINDER_H
#define _MY_MESH_CYLINDER_H
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
class CMeshCylinder : public CScene
{
public:
	CMeshCylinder(int priority = 0, OBJTYPE objType = OBJTYPE_CYLINDER);
	~CMeshCylinder(){};

	static CMeshCylinder* Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	void		SetPos(D3DXVECTOR3 pos){m_pos = pos; SetVertexPolygon();}
	void		SetPos(float x, float y, float z){m_pos = D3DXVECTOR3(x, y, z); SetVertexPolygon();}
	D3DXVECTOR3	GetPos(void){return m_pos;}
	void		SetRot(D3DXVECTOR3 rot){m_rot = rot; SetVertexPolygon();}
	void		SetRot(float x, float y, float z){m_rot = D3DXVECTOR3(x, y, z); SetVertexPolygon();}
	D3DXVECTOR3	GetRot(void){return m_rot;}
	void		SetSize(D3DXVECTOR3 size){m_size = size; SetVertexPolygon();}
	void		SetSize(float x, float y, float z){m_size = D3DXVECTOR3(x, y, z); SetVertexPolygon();}
	D3DXVECTOR3	GetSize(void){return m_size;}
	void		SetColor(D3DXCOLOR color){m_color = color; SetVertexPolygon();}
	void		SetColor(float r, float g, float b, float a){m_color = D3DXCOLOR(r, g, b, a); SetVertexPolygon();}
	D3DXCOLOR	GetColor(void){return m_color;}

private:
	void SetVertexPolygon(void);
	void SetVertexIndexPolygon(void);

	LPDIRECT3DTEXTURE9		m_tex;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;		// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_indexBuff;	// インデックスバッファへのポインタ

	D3DXMATRIX	m_world;	// ワールドマトリックス
	int m_numVtx;		// 総頂点数
	int m_numVtxIndex;	// 頂点の総インデックス数
	int m_numPolygon;	// 総ポリゴン数

	D3DXVECTOR3 m_sizeBlock;	// ブロックサイズ
	D3DXVECTOR3 m_numBlock;		// ブロック数
};

//=============================================================================
#endif
