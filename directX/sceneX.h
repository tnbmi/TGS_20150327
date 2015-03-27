//*****************************************************************************
//
// CSceneXクラス [sceneX.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_SCENE_X_H
#define _MY_SCENE_X_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "scene.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CSceneX : public CScene
{
public:
	CSceneX(int priority = PRIORITY_MAX - 1, OBJTYPE objType = OBJTYPE_X);
	~CSceneX(){};

	static	CSceneX* Create(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	void		SetPos(D3DXVECTOR3 pos){m_pos = pos;}
	void		SetPos(float x, float y, float z){m_pos = D3DXVECTOR3(x, y, z);}
	D3DXVECTOR3	GetPos(void){return m_pos;}
	void		SetRot(D3DXVECTOR3 rot){m_rot = rot;}
	void		SetRot(float x, float y, float z){m_rot = D3DXVECTOR3(x, y, z);}
	D3DXVECTOR3	GetRot(void){return m_rot;}
	void		SetSize(D3DXVECTOR3 size){m_size = size;}
	void		SetSize(float x, float y, float z){m_size = D3DXVECTOR3(x, y, z);}
	D3DXVECTOR3	GetSize(void){return m_size;}
	void		SetColor(D3DXCOLOR color){m_color = color;}
	void		SetColor(float r, float g, float b, float a){m_color = D3DXCOLOR(r, g, b, a);}
	D3DXCOLOR	GetColor(void){return m_color;}

	D3DXVECTOR3 GetVecU(void){return m_vecU;}
	D3DXVECTOR3 GetVecF(void){return m_vecF;}
	D3DXVECTOR3 GetVecR(void){return m_vecR;}
	void		SetSpeed(D3DXVECTOR3 speed){m_speed = speed;}
	void		SetSpeed(float x, float y, float z){m_speed = D3DXVECTOR3(x, y, z);}
	D3DXVECTOR3	GetSpeed(void){return m_speed;}

	void SetParent(CSceneX* parent){m_parent = parent;}

protected:
	XFILE m_xFile;

	D3DXMATRIX	m_world;

	D3DXVECTOR3 m_vecU;
	D3DXVECTOR3 m_vecF;
	D3DXVECTOR3 m_vecR;
	D3DXVECTOR3 m_speed;

	CSceneX* m_parent;	// 親モデルポインタ
};

//=============================================================================
#endif