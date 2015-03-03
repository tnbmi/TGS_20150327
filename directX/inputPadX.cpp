//*****************************************************************************
//
// CInputPadX�N���X [inputPadX.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "inputPadX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define REPEAT_MIN		(30)	// ���s�[�g�J�n�J�E���g��
#define REPEAT_BETWEEN	(3)		// ���s�[�g�Ԋu�J�E���g��
#define BUTTON_NUM		(14)	// �{�^���̐�
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
// �R���X�g���N�^
//=============================================================================
CInputPadX::CInputPadX(void)
{

}

//=============================================================================
// ����
//=============================================================================
CInputPadX* CInputPadX::Create(void)
{
	CInputPadX* pointer = new CInputPadX;
	pointer->Init();
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CInputPadX::Init(void)
{
	//----------------------------
	// �p�b�h���擾
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
// �I��
//=============================================================================
void CInputPadX::Uninit(void)
{
	if(m_connected == true)
	{
		m_padFlg[m_no] = true;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CInputPadX::Update(void)
{
	// �p�b�h���X�V
	if(XInputGetState(m_no, &m_pad) != ERROR_SUCCESS)
	{
		m_connected = false;
	}
	else
	{
		m_connected	  = true;
	}

}
