//*****************************************************************************
//
// CInputMouse�N���X [inputMouse.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "inputMouse.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputMouse::CInputMouse(void)
{

}

//=============================================================================
// ����
//=============================================================================
CInputMouse* CInputMouse::Create(HINSTANCE instance, HWND wnd)
{
	CInputMouse* pointer = new CInputMouse;
	pointer->Init(instance, wnd);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE instance, HWND wnd)
{
	//----------------------------
	// ����
	//----------------------------
	// DirectInput�I�u�W�F�N�g
	HRESULT_FUNC(CInput::Init(instance, wnd))

	// �f�o�C�X�̐���
	if(FAILED(m_dInput->CreateDevice(GUID_SysMouse, &m_dIDevice, NULL)))
	{
		MessageBox(NULL, "�f�o�C�X�������ł��܂���ł���", "�G���[", (MB_OK | MB_ICONERROR));
		return E_FAIL;
	}
	
	//----------------------------
	// �ݒ�
	//----------------------------
	// �f�[�^�t�H�[�}�b�g
	if(FAILED(m_dIDevice->SetDataFormat(&c_dfDIMouse2)))
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

	// �����[�h
	DIPROPDWORD diprop;

	diprop.diph.dwSize		 = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj		 = 0;
	diprop.diph.dwHow		 = DIPH_DEVICE;
	diprop.dwData	= DIPROPAXISMODE_REL;	// ���Βl���[�h
	//diprop.dwData	= DIPROPAXISMODE_ABS;	// ��Βl���[�h

	if(FAILED(m_dIDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		MessageBox(NULL, "�����[�h��ݒ�ł��܂���ł���", "�G���[", (MB_OK | MB_ICONERROR));
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
void CInputMouse::Uninit(void)
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
void CInputMouse::Update(void)
{
	if(FAILED(m_dIDevice->GetDeviceState(sizeof(DIMOUSESTATE2),&m_mouse)))
	{
		// �A�N�Z�X����蒼��
		m_dIDevice->Acquire();
	}
}
