//*****************************************************************************
//
// CSceneBillboardクラス [sceneBillboard.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_CSCENE_BILLBOARD_H
#define _MY_CSCENE_BILLBOARD_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "scene.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CSceneBillboard : public CScene
{
public:
	CSceneBillboard(int priority = PRIORITY_MAX - 1, OBJTYPE objType = OBJTYPE_BILLBOARD);
	~CSceneBillboard(){};

	static	CSceneBillboard* Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture);
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

	void		SetSpeed(D3DXVECTOR3 speed){m_speed = speed;}
	void		SetSpeed(float x, float y, float z){m_speed = D3DXVECTOR3(x, y, z);}
	D3DXVECTOR3	GetSpeed(void){return m_speed;}

	void		SetTexcord(int n, D3DXVECTOR2 texcord){m_texcord[n] = texcord; SetVertexPolygon();}
	D3DXVECTOR2	GetTexcord(int n){return m_texcord[n];}

	static void SetMtxView(D3DXMATRIX mtxView){m_view = mtxView;}

protected:
	void SetVertexPolygon(void);

	LPDIRECT3DTEXTURE9		m_tex;
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;
	LPDIRECT3DINDEXBUFFER9	m_indexBuff;

	D3DXMATRIX	m_world;
	int m_numVtx;		// 総頂点数
	int m_numVtxIndex;	// 頂点の総インデックス数
	int m_numPolygon;	// 総ポリゴン数

	D3DXVECTOR3 m_speed;

	static D3DXMATRIX m_view;		// ビュー行列
	D3DXVECTOR2		 m_texcord[4];	// テクスチャ座標
};

//=============================================================================
#endif