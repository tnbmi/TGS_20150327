//*****************************************************************************
//
// CPhaseクラス [phase.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_PHAZE_H
#define _MY_PHAZE_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CFade;
class CInputKeyboard;
class CInputMouse;
class CInputPadX;

class CPhase
{
public:
	CPhase(void){};
	~CPhase(){};

	virtual HRESULT	Init(LPDIRECT3DDEVICE9 device) = 0;
	virtual void	Uninit(void) = 0;
	virtual void	Update(void) = 0;
	virtual void	Draw(void)	 = 0;

	static CFade*	GetFade(void){return m_fade;}

	void SetKeyboard(CInputKeyboard* keyboard){m_keyboard = keyboard;}
	void SetMouse(CInputMouse* mouse){m_mouse = mouse;}
	void SetPadX(CInputPadX* padX){m_padX = padX;}

protected:
	LPDIRECT3DDEVICE9 m_device;	// Deviceオブジェクト(デバイスへのポインタ)

	static CFade* m_fade;

	static CInputKeyboard*	m_keyboard;
	static CInputMouse*		m_mouse;
	static CInputPadX*		m_padX;
};

//=============================================================================
#endif
