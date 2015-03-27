//*****************************************************************************
//
// CPlayer�N���X [player.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_PLAYER_H
#define _MY_PLAYER_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "sceneX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define PLAER_STOP	(0.1f)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputKeyboard;
class CInputPadX;

class CPlayer : public CSceneX
{
public:
	CPlayer(int priority = PRIORITY_MAX - 2, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer(){};

	static CPlayer* Create(LPDIRECT3DDEVICE9 device);
	HRESULT	Init(LPDIRECT3DDEVICE9 device);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	void SetRot(D3DXVECTOR3 rot);

<<<<<<< HEAD
=======
	D3DXVECTOR3 GetSize(void){return m_size;}
	D3DXVECTOR3 GetVecU(void){return m_vecU;}
	D3DXVECTOR3 GetVecF(void){return m_vecF;}
	D3DXVECTOR3 GetVecR(void){return m_vecR;}

>>>>>>> origin/dev/zyamu
	void SetKeyboard(CInputKeyboard* keyboard){m_keyboard = keyboard;}

private:
	CInputKeyboard*	m_keyboard;
};

//=============================================================================
#endif