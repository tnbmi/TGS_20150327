//*****************************************************************************
//
// CInputPadXクラス [inputPadX.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "inputPadX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define REPEAT_MIN		(30)	// リピート開始カウント数
#define REPEAT_BETWEEN	(3)		// リピート間隔カウント数
#define BUTTON_NUM		(14)	// ボタンの数
const WORD PAD_BUTTON[BUTTON_NUM] =
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};

//=============================================================================
// コンストラクタ
//=============================================================================
CInputPadX::CInputPadX(void)
{

}

//=============================================================================
// 生成
//=============================================================================
CInputPadX* CInputPadX::Create(void)
{
	CInputPadX* pointer = new CInputPadX;
	pointer->Init();
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CInputPadX::Init(void)
{
	//----------------------------
	// パッド情報取得
	//----------------------------
	for(DWORD cnt = 0; cnt < PAD_NUM; ++cnt)
	{
		if(XInputGetState(cnt, &m_pad) != ERROR_SUCCESS)
		{
			m_connected = false;
		}
		else
		{
			m_no = cnt;
			m_padFlg[m_no] = true;
			m_connected	  = true;
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CInputPadX::Uninit(void)
{
	if(m_connected == true)
	{
		m_padFlg[m_no] = true;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CInputPadX::Update(void)
{
	// パッド情報更新
	if(XInputGetState(m_no, &m_pad) != ERROR_SUCCESS)
	{
		m_connected = false;
	}
	else
	{
		m_connected	  = true;
	}

}
