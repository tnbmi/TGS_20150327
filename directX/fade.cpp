//*****************************************************************************
//
// CFade�N���X [fade.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "fade.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
const int FADE_CNG[CFade::FADESTATE_MAX] =
{
	0,
	1,
	0,
	-1,
	0
};

const CFade::FADESTATE FADE_NEXT[CFade::FADESTATE_MAX] =
{
	CFade::FADESTATE_NONE,
	CFade::FADESTATE_OUTEND,
	CFade::FADESTATE_NONE,
	CFade::FADESTATE_INEND,
	CFade::FADESTATE_NONE
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFade::CFade(int priority, OBJTYPE objType) : CScene2D(priority, objType)
{
	// �����o������
	m_state = FADESTATE_NONE;
	m_time	= 0;
	m_alpha = 0.0f;
	m_color = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_alpha);
	m_flg	= false;
}

//=============================================================================
// ����
//=============================================================================
CFade* CFade::Create(LPDIRECT3DDEVICE9 device)
{
	CFade* pointer = new CFade;
	pointer->Init(device);

	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CFade::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �Ǝ��̃��\�[�X�ݒ�
	//----------------------------

	//----------------------------
	// �e�N���X������
	//----------------------------
	HRESULT_FUNC(CScene2D::Init(device, CImport::TEX_FADE, CScene2D::POINT_CENTER))
	SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));

	//----------------------------
	// ����������
	//----------------------------
	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CFade::Uninit(void)
{
	//----------------------------
	// �Ǝ��̊J��
	//----------------------------

	//----------------------------
	// �e�N���X�I��
	//----------------------------
	CScene2D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CFade::Update(void)
{
	//----------------------------
	// �t�F�[�h����
	//----------------------------
	if(m_flg && m_time >= 0)
	{
		// �A���t�@�l�ύX
		m_alpha += m_cngAlpha * FADE_CNG[m_state];
		if(m_alpha < 0.0f)
		{
			m_alpha = 0.0f;
		}
		else if(m_alpha > 1.0f)
		{
			m_alpha = 1.0f;
		}

		m_color = D3DXCOLOR(m_color.r, m_color.g, m_color.b, m_alpha);
		CScene2D::SetColor(m_color);

		// �J�E���g�_�E��
		m_time--;
		if(m_time < 0)
		{
			// ���[�h�X�V
			m_state = FADE_NEXT[m_state];
			m_flg = false;
		}
	}
	else if(m_state == FADESTATE_INEND)
	{
		m_state = FADESTATE_NONE;
	}

	//----------------------------
	// �e�N���X�X�V
	//----------------------------
	CScene2D::Update();
}

//=============================================================================
// �`��
//=============================================================================
void CFade::Draw(void)
{
	if(m_state != FADESTATE_NONE)
	{
		// ���e�X�g�I�t
		m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// �`��
		CScene2D::Draw();

		// ���e�X�g�I��
		m_device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	}
}

//=============================================================================
// �J�n
//=============================================================================
void CFade::Start(FADESTATE state, int time, float r, float g, float b, float a)
{
	m_state		= state;
	m_time		= time * 60;
	m_alpha		= a;
	m_color		= D3DXCOLOR(r, g, b, a);
	m_cngAlpha	= 1.0f / (float)m_time;
	CScene2D::SetColor(m_color);

	if(state == FADESTATE_IN || state == FADESTATE_OUT)
	{
		m_flg = true;
	}
}