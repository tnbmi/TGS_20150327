//*****************************************************************************
//
// CPhase�N���X [phase.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "phase.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CFade*			CPhase::m_fade		= NULL;
CInputKeyboard*	CPhase::m_keyboard	= NULL;
CInputMouse*	CPhase::m_mouse		= NULL;
CInputPadX*		CPhase::m_padX		= NULL;