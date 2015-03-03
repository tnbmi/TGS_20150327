//*****************************************************************************
//
// CInputMouseクラス [inputMouse.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_INPUT_MOUSE_H
#define _MY_INPUT_MOUSE_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "input.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputMouse : public CInput
{
public:
	CInputMouse();
	~CInputMouse(){};

	static	CInputMouse* Create(HINSTANCE instance, HWND wnd);
	HRESULT	Init(HINSTANCE instance, HWND wnd);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	bool GetLeft(void){return (m_mouse.rgbButtons[0] & 0x80) ? true : false;}
	bool GetRight(void){return (m_mouse.rgbButtons[1] & 0x80) ? true : false;}
	bool GetCenter(void){return (m_mouse.rgbButtons[2] & 0x80) ? true : false;}

	long GetMoveX(void){return m_mouse.lX;}
	long GetMoveY(void){return m_mouse.lY;}
	long GetMoveZ(void){return m_mouse.lZ;}

private:
	DIMOUSESTATE2 m_mouse;
};

//=============================================================================
#endif