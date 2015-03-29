//*****************************************************************************
//
// CDust�N���X [dust.cpp]
// Author : YUJI MOMOI
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "dust.h"
#include "dustAI.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDust::CDust(int priority, OBJTYPE objType) : CSceneX(priority, objType)
{
	m_xFile.mesh	= NULL;
	m_xFile.buffMat = NULL;

	m_rot= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posDest = m_pos;
	m_posOld = m_pos;
	m_scl= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_size= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_parent= NULL;
	m_AI= NULL;

	m_FrontVector = D3DXVECTOR3(0.0f,0.0f,-1.0f);
	m_RightVector = D3DXVECTOR3(-1.0f,0.0f,0.0f);
	m_RotPower = D3DXVECTOR3(0,0,0);
	m_move = false;

	m_HP = DUST_HP;
	m_InvincibleTime = 0;
	m_damege = false;
}

//=============================================================================
// ����
//=============================================================================
CDust* CDust::Create(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile)
{
	CDust* pointer = new CDust;
	pointer->Init(device, xFile);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CDust::Init(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile)
{
	//----------------------------
	// �f�o�C�X�擾
	//----------------------------
	m_device = device;

	//----------------------------
	// ���f���擾
	//----------------------------
	m_xFile = CImport::GetXFile(xFile);

	//----------------------------
	// AI����
	//----------------------------
	m_AI = CDustAI::Create(this);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CDust::Uninit(void)
{
	// �J��
	CScene::Delete();

	// AI���
	SAFE_DELETE(m_AI);
}

//=============================================================================
// �X�V
//=============================================================================
void CDust::Update(void)
{
	m_posOld = m_pos;

	// AI�X�V
	m_AI->Update();

	if(!m_move)
	{
		return;
	}

	// �ړ�
	D3DXVECTOR3 front = m_FrontVector;
	D3DXVECTOR3 right = m_RightVector;
	D3DXVECTOR3 vecTarget = m_posDest - m_pos;

	// �ړI�n�͈͓̔��Ȃ�
	D3DXVECTOR3 vecLength = m_posDest - m_pos;
	float length = sqrtf(vecLength.x * vecLength.x
						+ vecLength.y * vecLength.y
						+ vecLength.z * vecLength.z);
	if(length < 10)
	{
		m_AI->SetGoal();
		m_move = false;
		return;
	}

	D3DXVec3Normalize(&vecTarget,&vecTarget);
	float dot = D3DXVec3Dot(&right,&vecTarget);
	m_RotPower.y += dot * DUST_ROTATION_SPEED;

	float frontDot = D3DXVec3Dot(&front,&vecTarget);
	if(frontDot < -0.95f)
	{
	m_RotPower.y += DUST_ROTATION_SPEED;
	}

	if(frontDot > -0.2f)
	{
	m_pos += front * DUST_MOVE_SPEED;
	}

	front.x = sinf(-m_RotPower.y);
	front.z = cosf(m_RotPower.y - D3DX_PI);

	right.x = cosf(m_RotPower.y - D3DX_PI);
	right.z = sinf(m_RotPower.y);

	m_FrontVector = front;
	m_RightVector = right;
	m_rot = m_RotPower;

	if(m_HP <= 0)
	{
		Uninit();
	}

	if(m_damege)
	{
		++m_InvincibleTime;

		if(m_InvincibleTime > DUST_INVINCIBLE_TIME)
		{
			m_InvincibleTime = 0;
			m_damege = false;
		}
	}
}

//=============================================================================
// �`��
//=============================================================================
void CDust::Draw(void)
{
	if(m_HP > 0)
	{
		CSceneX::Draw();
	}
}

//=============================================================================
// �_���[�W����
//=============================================================================
void CDust::SetDamege(void)
{
	if(!m_damege)
	{
		-- m_HP;
		m_damege = true;
	}
}
