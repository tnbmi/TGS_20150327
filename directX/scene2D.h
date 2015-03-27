//*****************************************************************************
//
// CScene2Dクラス [scene2D.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_SCENE_2D_H
#define _MY_SCENE_2D_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "scene.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CScene2D : public CScene
{
public:
	typedef enum
	{
		POINT_LEFTTOP,
		POINT_CENTER
	} POINT_TYPE;

	CScene2D(int priority = PRIORITY_MAX - 1, OBJTYPE objType = OBJTYPE_2D);
	~CScene2D(){};

	static	CScene2D* Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, POINT_TYPE pointType);
	static	CScene2D* Create(LPDIRECT3DDEVICE9 device, const char* texture, POINT_TYPE pointType);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, POINT_TYPE pointType);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, const char* texture, POINT_TYPE pointType);
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

	LPDIRECT3DTEXTURE9		m_texture;
	LPDIRECT3DVERTEXBUFFER9	m_vtxBuff;

	float		m_length;		// 対角線の長さ
	float		m_angle;		// 対角線の角度
	POINT_TYPE	m_pointType;	// ポイントの位置
};

//=============================================================================
#endif