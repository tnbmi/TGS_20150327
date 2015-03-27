//*****************************************************************************
//
// �J�������� [camera.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "camera.h"
#include "debugproc.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �A�X�y�N�g��
#define RADIAN(a)	((D3DX_PI / 180.0f) * a)						// ���W�A���v�Z

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera(void)
{
	m_at		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_distance	= DISTANCE_3D(m_pos.x, m_pos.y, m_pos.z);

	m_atDest		= m_at;
	m_posDest		= m_pos;
	m_rotDest		= m_rot;
	m_distanceDest	= m_distance;

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecF = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecR = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

//=============================================================================
// ����
//=============================================================================
CCamera* CCamera::Create(D3DXVECTOR3 at, D3DXVECTOR3 pos)
{
	CCamera* pointer = new CCamera;
	pointer->Init(at, pos);
	return pointer;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(D3DXVECTOR3 at, D3DXVECTOR3 pos)
{
	//----------------------------
	// �ʒu�Ƌ���
	//----------------------------
	m_pos		= pos;
	m_at		= at;
	m_distance	= DISTANCE_3D(pos.x, pos.y, pos.z);

	//----------------------------
	// ��]
	//----------------------------
	m_rot.y = atan2(m_pos.x, -m_pos.z);
	m_rot.x = atan2(m_pos.y, (float)hypot(m_pos.z, m_pos.x));

	if(m_rot.y < -D3DX_PI || m_rot.y > D3DX_PI)
	{
		m_rot.y = m_rot.y + (-SIGN_F(m_rot.y) * D3DX_PI * 2.0f);
	}

	if(m_rot.x < -D3DX_PI || m_rot.x > D3DX_PI)
	{
		m_rot.x = m_rot.x + (-SIGN_F(m_rot.x) * D3DX_PI * 2.0f);
	}

	//----------------------------
	// �ڕW�l
	//----------------------------
	m_posDest		= m_pos;
	m_atDest		= m_at;
	m_rotDest		= m_rot;
	m_distanceDest	= m_distance;

	// ����������
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::SetCamera(LPDIRECT3DDEVICE9 device)
{
	D3DXVECTOR3 eye = m_pos + m_at;

	//----------------------------
	// �r���[�}�g���b�N�X
	//----------------------------
	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_view);
	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_view, &eye, &m_at, &m_vecU);
	// �r���[�}�g���b�N�X�̐ݒ�
	device->SetTransform(D3DTS_VIEW, &m_view);

	//----------------------------
	// �v���W�F�N�V�����}�g���b�N�X
	//----------------------------
	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_projection);
	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_projection,
								RADIAN(45),	// ����p
								ASPECT,		// �A�X�y�N�g��
								10.0f,		// near�l
								10000.0f);	// far�l
	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	device->SetTransform(D3DTS_PROJECTION, &m_projection);

#ifdef _DEBUG
	// �J�������\��
	CDebugproc::PrintDebugProc("***�J�������**********\nPos x:%f y:%f z:%f\nAt  x:%f y:%f z:%f\nRot x:%f y:%f z:%f\n",
								m_pos.x, m_pos.y, m_pos.z,
								m_at.x,  m_at.y,  m_at.z,
								m_rot.x, m_rot.y, m_rot.z);
#endif
}

//=============================================================================
// ���_�Đݒ�
//=============================================================================
void CCamera::ResetPos(void)
{
	m_pos.x = m_at.x + cosf(m_rot.y) * cosf(m_rot.x) * m_distance;
	m_pos.z = m_at.z + sinf(m_rot.y) * cosf(m_rot.x) * m_distance;
	m_pos.y = m_at.y + sinf(m_rot.x) * m_distance;
}

//=============================================================================
// ��]�p�Đݒ�
//=============================================================================
void CCamera::ResetRot(void)
{
	m_rot.y = atan2(m_pos.x, -m_pos.z);
	m_rot.x = atan2(m_pos.y, (float)hypot(m_pos.z, m_pos.x));

	if(m_rot.y < -D3DX_PI || m_rot.y > D3DX_PI)	// �����ւ�
	{
		m_rot.y = m_rot.y + (-SIGN_F(m_rot.y) * D3DX_PI * 2.0f);
	}

	if(m_rot.x < -D3DX_PI || m_rot.x > D3DX_PI)	// �����ւ�
	{
		m_rot.x = m_rot.x + (-SIGN_F(m_rot.x) * D3DX_PI * 2.0f);
	}
}

//=============================================================================
// �ړI���_�Đݒ�
//=============================================================================
void CCamera::ResetPosDest(void)
{
	m_posDest.x = m_atDest.x + cosf(m_rotDest.y - D3DX_PI * 0.5f) * cosf(m_rotDest.x) * m_distance;
	m_posDest.z = m_atDest.z + sinf(m_rotDest.y - D3DX_PI * 0.5f) * cosf(m_rotDest.x) * m_distance;
	m_posDest.y = m_atDest.y + sinf(m_rotDest.x) * m_distance;
}

//=============================================================================
// �ړI��]�p�Đݒ�
//=============================================================================
void CCamera::ResetRotDest(void)
{
	m_rotDest.x = -(atan2f(m_posDest.y - m_atDest.y, m_posDest.z - m_atDest.z) - D3DX_PI);
	m_rotDest.y = -(atan2f(m_posDest.x - m_atDest.x, m_posDest.z - m_atDest.z) - D3DX_PI);

	if(m_rotDest.y < -D3DX_PI || m_rotDest.y > D3DX_PI)	// �����ւ�
	{
		m_rotDest.y = m_rotDest.y + (-SIGN_F(m_rotDest.y) * D3DX_PI * 2.0f);
	}
}

//=============================================================================
// �����Đݒ�
//=============================================================================
void CCamera::ResetDistance(void)
{
	m_distance = DISTANCE_3D(m_pos.x, m_pos.y, m_pos.z);
}
