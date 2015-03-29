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
class CPlayer;
class CDustManager;
class CBillManager;

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
	static CPlayer* GetPlayer(void){return m_player;}
	static CDustManager* GetDustManager(void){return m_dustManager;}
	static CBillManager* GetBillManager(void){return m_billManager;}

private:
	void InitObject(LPDIRECT3DDEVICE9 device);
	void UpdateCamera(void);
	void Debug(void);

	static CCamera*		m_camera;
	CLight*	m_light[3];

	bool	 m_dcFlg;
	CCamera* m_debugCamera;

	CMeshField*			 m_field;
	static CPlayer*		 m_player;
	static CDustManager* m_dustManager;
	static CBillManager* m_billManager;
};

//=============================================================================
#endif
