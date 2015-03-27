//*****************************************************************************
//
// CScene3Dクラス [scene3D.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_SCENE_3D_H
#define _MY_SCENE_3D_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "scene.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CScene3D : public CScene
{
public:
	CScene3D(int priority = PRIORITY_MAX - 1, OBJTYPE objType = OBJTYPE_3D);
	~CScene3D(){};

	static	CScene3D* Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture);
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

protected:
	void SetVertexPolygon(void);

	LPDIRECT3DTEXTURE9		m_tex;
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;
	LPDIRECT3DINDEXBUFFER9	m_indexBuff;

	D3DXMATRIX	m_world;
	int m_numVtx;		// 総頂点数
	int m_numVtxIndex;	// 頂点の総インデックス数
	int m_numPolygon;	// 総ポリゴン数
};

//=============================================================================
#endif