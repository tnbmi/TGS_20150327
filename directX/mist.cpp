//*****************************************************************************
//
// CMist�N���X [mist.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "mist.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define LIFE_TIME	(16)
#define GRAVITY		(0.1f)
#define POLIGON_SIZE (5.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMist::CMist(int priority, OBJTYPE objType) : CSceneBillboard(priority, objType)
{
	m_size = D3DXVECTOR3(POLIGON_SIZE, POLIGON_SIZE, 0.0f);
	m_speed = D3DXVECTOR3(7.0f, 1.0f, 7.0f);

	m_pos.x = (float)(rand() % 6) - 3 * 0.1f;
	m_pos.y = (float)(rand() % 6) - 3 * 0.1f;
	m_pos.z = (float)(rand() % 6) - 3 * 0.1f;
}

//=============================================================================
// ����
//=============================================================================
CMist* CMist::Create(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 vec)
{
	CMist* pointer = new CMist;
	pointer->Init(device, pos, vec);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CMist::Init(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 vec)
{
	//----------------------------
	// �Ǝ��̃��\�[�X�ݒ�
	//----------------------------
	m_time = LIFE_TIME;
	m_pos += pos;
	m_vec = vec;

	//----------------------------
	// �e�N���X������
	//----------------------------
	HRESULT_FUNC(CSceneBillboard::Init(device, CImport::TEX_MIST))

	//----------------------------
	// ����������
	//----------------------------
	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CMist::Uninit(void)
{
	//----------------------------
	// �e�N���X�I��
	//----------------------------
	CSceneBillboard::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CMist::Update(void)
{
	//----------------------------
	// �ړ�
	//----------------------------
	m_pos.x	+= m_vec.x * m_speed.x;
	m_pos.z	+= m_vec.z * m_speed.z;
	m_pos.y += m_speed.y;
	
	m_speed.y += -0.2f;

	//----------------------------
	// �J�E���g�_�E��
	//----------------------------
	m_time--;
	if(m_time < 0 || m_pos.y < 0.0f)
	{
		Delete();
	}

	//----------------------------
	// �e�N���X�X�V
	//----------------------------
	CSceneBillboard::Update();
}

//=============================================================================
// �`��
//=============================================================================
void CMist::Draw(void)
{
	// ���Z�����_�[�X�e�[�g
	m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);		// �ʏ퍇��
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	// ���\�[�X�J���[�̎w��
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);		// ���f�X�e�B�l�[�V�����J���[�̎w��

	m_device->SetRenderState(D3DRS_ZENABLE, FALSE);

	// �`��
	CSceneBillboard::Draw();

	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);

	// �f�t�H���g�����_�[�X�e�[�g
	m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// �ʏ퍇��
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��
}