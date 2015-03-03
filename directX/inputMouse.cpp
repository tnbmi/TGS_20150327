//*****************************************************************************
//
// CInputMouseクラス [inputMouse.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "inputMouse.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// コンストラクタ
//=============================================================================
CInputMouse::CInputMouse(void)
{

}

//=============================================================================
// 生成
//=============================================================================
CInputMouse* CInputMouse::Create(HINSTANCE instance, HWND wnd)
{
	CInputMouse* pointer = new CInputMouse;
	pointer->Init(instance, wnd);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE instance, HWND wnd)
{
	//----------------------------
	// 生成
	//----------------------------
	// DirectInputオブジェクト
	HRESULT_FUNC(CInput::Init(instance, wnd))

	// デバイスの生成
	if(FAILED(m_dInput->CreateDevice(GUID_SysMouse, &m_dIDevice, NULL)))
	{
		MessageBox(NULL, "デバイスが生成できませんでした", "エラー", (MB_OK | MB_ICONERROR));
		return E_FAIL;
	}
	
	//----------------------------
	// 設定
	//----------------------------
	// データフォーマット
	if(FAILED(m_dIDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(NULL, "データフォーマットを設定できませんでした", "エラー", (MB_OK | MB_ICONERROR));
		return E_FAIL;
	}

	// 協調モード
	if(FAILED(m_dIDevice->SetCooperativeLevel(wnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(NULL, "協調モードを設定できませんでした", "エラー", (MB_OK | MB_ICONERROR));
		return E_FAIL;
	}

	// 軸モード
	DIPROPDWORD diprop;

	diprop.diph.dwSize		 = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj		 = 0;
	diprop.diph.dwHow		 = DIPH_DEVICE;
	diprop.dwData	= DIPROPAXISMODE_REL;	// 相対値モード
	//diprop.dwData	= DIPROPAXISMODE_ABS;	// 絶対値モード

	if(FAILED(m_dIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		MessageBox(NULL, "軸モードを設定できませんでした", "エラー", (MB_OK | MB_ICONERROR));
		return E_FAIL;
	}

	//----------------------------
	// アクセス権取得
	//----------------------------
	m_dIDevice->Acquire();

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CInputMouse::Uninit(void)
{
	// オブジェ開放
	CInput::Uninit();

	// アクセス権開放
	if(m_dIDevice != NULL)
	{
		m_dIDevice->Unacquire();
		m_dIDevice->Release();
		m_dIDevice = NULL;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CInputMouse::Update(void)
{
	if(FAILED(m_dIDevice->GetDeviceState(sizeof(DIMOUSESTATE2),&m_mouse)))
	{
		// アクセス権取り直し
		m_dIDevice->Acquire();
	}
}
