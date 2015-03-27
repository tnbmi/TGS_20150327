//*****************************************************************************
//
// CDebugproc�N���X [debugproc.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_DEBUGPROC_H
#define _MY_DEBUGPROC_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define STR_MAX	(2048)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputKeyboard;

class CDebugproc
{
public:
	CDebugproc();
	~CDebugproc(){};

	static	CDebugproc* Create(LPDIRECT3DDEVICE9 device);
	HRESULT	Init(LPDIRECT3DDEVICE9 device);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static void SetKeyboard(CInputKeyboard* keyboard){m_keyboard = keyboard;}

	static void PrintDebugProc(const char *fmt, ...);

private:
	LPDIRECT3DDEVICE9 m_device;
	static CInputKeyboard* m_keyboard;

	LPD3DXFONT	m_font;
	static char	m_strDebug[STR_MAX];
	static bool	m_drawFlg;
};

//=============================================================================
#endif