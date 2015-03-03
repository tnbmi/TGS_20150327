//*****************************************************************************
//
// CManagerクラス [manager.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_MANAGER_H
#define _MY_MANAGER_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CRenderer;
class CDebugproc;
class CImport;

class CInputKeyboard;

class CManager
{
public:
	CManager();
	~CManager(){};

	static	CManager* Create(HINSTANCE instance, HWND wnd, bool window);
	HRESULT	Init(HINSTANCE instance, HWND wnd, bool window);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	void	CalculateFPS(DWORD frameCnt, DWORD curTime, DWORD FPSLastTime);

private:
	CRenderer*	m_renderer;
	CDebugproc*	m_debugproc;
	CImport*	m_import;

	CInputKeyboard*	m_keyboard;
};

//=============================================================================
#endif