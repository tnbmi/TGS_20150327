//*****************************************************************************
//
// CInputPadXクラス [inputPadX.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_INPUT_PADX_H
#define _MY_INPUT_PADX_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "input.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputPadX : public CInput
{
public:
	CInputPadX();
	~CInputPadX(){};

	static	CInputPadX* Create(void);
	HRESULT	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	bool GetButton(WORD wButton){return (m_pad.Gamepad.wButtons & wButton) ? true : false;}
	bool GetLeftTrigger(void){return (m_pad.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}
	bool GetRightrigger(void){return (m_pad.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;}

	SHORT GetThumbLX(void){return (abs(m_pad.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? m_pad.Gamepad.sThumbLX : 0;}
	SHORT GetThumbLY(void){return (abs(m_pad.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ? m_pad.Gamepad.sThumbLY : 0;}
	SHORT GetThumbRX(void){return (abs(m_pad.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? m_pad.Gamepad.sThumbRX : 0;}
	SHORT GetThumbRY(void){return (abs(m_pad.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ? m_pad.Gamepad.sThumbRY : 0;}

private:
	bool		 m_connected;
	int			 m_no;
	XINPUT_STATE m_pad;
};

//=============================================================================
#endif