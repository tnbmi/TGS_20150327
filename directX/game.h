//*****************************************************************************
//
// CGame�N���X [game.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_GAME_H
#define _MY_GAME_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "phase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CCamera;
class CLight;

class CMeshField;

class CGame : CPhase
{
public:
	CGame(void){};
	~CGame(){};

	HRESULT	Init(LPDIRECT3DDEVICE9 device);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CCamera* GetCamera(void){return m_camera;}

private:
	void InitObject(LPDIRECT3DDEVICE9 device);
	void UpdateCamera(void);
	void Debug(void);

	static CCamera*		m_camera;
	CLight*	m_light[3];

	CMeshField*	m_field;
};

//=============================================================================
#endif
