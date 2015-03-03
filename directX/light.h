//*****************************************************************************
//
// ライト処理 [light.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_LIGHT_H_
#define _MY_LIGHT_H_
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CLight
{
public:
	CLight();
	~CLight(){};

	static	CLight* Create(LPDIRECT3DDEVICE9 device, int index, D3DLIGHTTYPE type, D3DXVECTOR3 vecDir, D3DCOLORVALUE diffuse, D3DCOLORVALUE specular, D3DCOLORVALUE ambient);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, int index, D3DLIGHTTYPE type, D3DXVECTOR3 vecDir, D3DCOLORVALUE diffuse, D3DCOLORVALUE specular, D3DCOLORVALUE ambient);
	void	Uninit(void);
	void	Update(void);

	void		SetVecDir(D3DXVECTOR3 vecDir){m_vecDir = vecDir;}
	D3DXVECTOR3	GetVecDir(void){return m_vecDir;}

	void			SetDiffuse(D3DCOLORVALUE diffuse){m_light.Diffuse = diffuse;}
	D3DCOLORVALUE	GetDiffuse(void){return m_light.Diffuse;}
	void			SetAmbient(D3DCOLORVALUE ambient){m_light.Ambient = ambient;}
	D3DCOLORVALUE	GetAmbient(void){return m_light.Ambient;}
	void			SetSpecular(D3DCOLORVALUE specular){m_light.Specular = specular;}
	D3DCOLORVALUE	GetSpecular(void){return m_light.Specular;}

private:
	void Reset(void);

	D3DLIGHT9	m_light;
	int			m_index;
	D3DXVECTOR3	m_vecDir;
};

//=============================================================================
#endif
