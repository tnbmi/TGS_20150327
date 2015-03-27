//*****************************************************************************
//
// CPlayerクラス [player.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_PLAYER_H
#define _MY_PLAYER_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "sceneX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define PLAER_STOP	(0.1f)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
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

	void SetKeyboard(CInputKeyboard* keyboard){m_keyboard = keyboard;}

private:
	CInputKeyboard*	m_keyboard;
};

//=============================================================================
#endif