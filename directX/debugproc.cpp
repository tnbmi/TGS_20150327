//*****************************************************************************
//
// CDebugproc�N���X [debugproc.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "debugproc.h"

#include "inputKeyboard.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CInputKeyboard* CDebugproc::m_keyboard = NULL;
char CDebugproc::m_strDebug[STR_MAX];
bool CDebugproc::m_drawFlg;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDebugproc::CDebugproc(void)
{

}

//=============================================================================
// ����
//=============================================================================
CDebugproc* CDebugproc::Create(LPDIRECT3DDEVICE9 device)
{
	CDebugproc* pointer = new CDebugproc;
	pointer->Init(device);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CDebugproc::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �f�o�C�X�擾
	//----------------------------
	m_device = device;

	//----------------------------
	// ���\���p�t�H���g��ݒ�
	//----------------------------
	D3DXCreateFont(m_device,					// �f�o�C�X
					15,							// �����̍���
					8,							// ������
					0,							// �t�H���g�̑���
					0,							// MIPMAP�̃��x��
					FALSE,						// �C�^���b�N���ǂ���
					SHIFTJIS_CHARSET,			// �����Z�b�g
					OUT_DEFAULT_PRECIS,			// �o�͐��x
					DEFAULT_QUALITY,			// �o�͕i��
					DEFAULT_PITCH | FF_SWISS,	// �t�H���g�s�b�`�ƃt�@�~��
					"Terminal",					// �t�H���g��
					&m_font);					// �|�C���^�ւ̃A�h���X
	
	// �f�o�b�O���o�b�t�@�̃N���A
	memset(&m_strDebug, 0, STR_MAX);

	// �f�o�b�O�t���O
#ifdef _DEBUG
	m_drawFlg = true;
#else
	m_drawFlg = false;
#endif

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CDebugproc::Uninit(void)
{
	// ���\���p�t�H���g�̊J��
	SAFE_RELEASE(m_font)
}

//=============================================================================
// �X�V
//=============================================================================
void CDebugproc::Update(void)
{
	// �f�o�b�O���o�b�t�@�̃N���A
	memset(&m_strDebug, 0, STR_MAX);

	// �f�o�b�O�\���ؑ�
	if(m_keyboard->GetTrigger(DIK_F1) == true)
	{
		if(m_drawFlg)
		{
			m_drawFlg = false;
		}
		else
		{
			m_drawFlg = true;
		}
	}

	// �؂�ւ��A�i�E���X
	if(m_drawFlg)
	{
		CDebugproc::PrintDebugProc("�f�o�b�OON/OFF�F[F1]\n");
	}
}

//=============================================================================
// �`��
//=============================================================================
void CDebugproc::Draw(void)
{
	if(m_drawFlg)
	{
		RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

		m_font->DrawText(NULL, m_strDebug, -1, &rect,
						 DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}

//=============================================================================
// �f�o�b�O�\���̓o�^
//=============================================================================
void CDebugproc::PrintDebugProc(const char *fmt, ...)
{
	char	str[1024];	// ����������
	va_list	args;		// �ψ��������p

	// ���[�N������
	memset(str, 0, sizeof(str));

	//----------------------------
	// ������ǉ�
	//----------------------------
	va_start(args, fmt);	// �ψ����ւ̃A�N�Z�X�J�n

	// ������ϊ�
	vsnprintf_s(str, _TRUNCATE, fmt, args);

	// �����񌋍�
	strcat_s(m_strDebug, sizeof(m_strDebug), str);

	// ���[�N�N���A
	memset(str, 0, sizeof(str));

	va_end(args);	// �ψ����ւ̃A�N�Z�X�I��
}
