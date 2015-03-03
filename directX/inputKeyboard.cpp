//*****************************************************************************
//
// CInputKeyboard�N���X [inputKeyboard.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "inputKeyboard.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define REPEAT_MIN		(30)	// ���s�[�g�J�n�J�E���g��
#define REPEAT_BETWEEN	(3)		// ���s�[�g�Ԋu�J�E���g��

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard(void)
{
	for(int cnt = 0; cnt < KEYBOARD_MAX; ++cnt)
	{
		m_state[cnt]	 = 0x00;
		m_trigger[cnt]	 = 0x00;
		m_release[cnt]	 = 0x00;
		m_repeat[cnt]	 = 0x00;
		m_cntRepeat[cnt] = 0;
	}

}

//=============================================================================
// ����
//=============================================================================
CInputKeyboard* CInputKeyboard::Create(HINSTANCE instance, HWND wnd)
{
	CInputKeyboard* pointer = new CInputKeyboard;
	pointer->Init(instance, wnd);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE instance, HWND wnd)
{
	//----------------------------
	// ����
	//----------------------------
	// DirectInput�I�u�W�F�N�g
	HRESULT_FUNC(CInput::Init(instance, wnd))

	// �f�o�C�X�̐���
	if(FAILED(m_dInput->CreateDevice(GUID_SysKeyboard, &m_dIDevice, NULL)))
	{
		MessageBox(NULL, "�f�o�C�X�������ł��܂���ł���", "�G���[", (MB_OK | MB_ICONERROR));
		return E_FAIL;
	}

	//----------------------------
	// �ݒ�
	//----------------------------
	// �f�[�^�t�H�[�}�b�g
	if(FAILED(m_dIDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(NULL, "�f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł���", "�G���[", (MB_OK | MB_ICONERROR));
		return E_FAIL;
	}

	// �������[�h
	if(FAILED(m_dIDevice->SetCooperativeLevel(wnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(NULL, "�������[�h��ݒ�ł��܂���ł���", "�G���[", (MB_OK | MB_ICONERROR));
		return E_FAIL;
	}

	//----------------------------
	// �A�N�Z�X���擾
	//----------------------------
	m_dIDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// �I�u�W�F�J��
	CInput::Uninit();

	// �A�N�Z�X���J��
	if(m_dIDevice != NULL)
	{
		m_dIDevice->Unacquire();
		m_dIDevice->Release();
		m_dIDevice = NULL;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[KEYBOARD_MAX];

	if(SUCCEEDED(m_dIDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for(int cnt = 0; cnt < KEYBOARD_MAX; ++cnt)
		{
			// ����擾
			aKeyState[cnt] &= 0x80;

			//----------------------------
			// ��ԏ��쐬
			//----------------------------
			// �g���K�[
			m_trigger[cnt] = (aKeyState[cnt] ^ m_state[cnt]) & aKeyState[cnt];

			// �����[�X
			m_release[cnt] = (aKeyState[cnt] ^ m_state[cnt]) & m_state[cnt];

			// ���s�[�g
			if(aKeyState[cnt])
			{
				// �J�E���g�A�b�v�E�L�[���Z�b�g
				++m_cntRepeat[cnt];
				m_repeat[cnt] = 0;

				if((m_cntRepeat[cnt] >= REPEAT_MIN && (m_cntRepeat[cnt] % REPEAT_BETWEEN == 0)) || m_trigger[cnt])
				{
					// ���s�[�g�J�n
					m_repeat[cnt] = aKeyState[cnt];
				}
			}
			else
			{
				m_repeat[cnt] = aKeyState[cnt];
				m_cntRepeat[cnt] = 0;
			}

			// �v���X
			m_state[cnt] = aKeyState[cnt];
		}
	}
	else
	{
		// �A�N�Z�X����蒼��
		m_dIDevice->Acquire();
	}
}