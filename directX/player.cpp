//*****************************************************************************
//
// CPlayer�N���X [player.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "player.h"

#include "game.h"
#include "debugproc.h"
#include "fade.h"

#include "inputKeyboard.h"
#include "camera.h"
#include "manager.h"

#include "mist.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SIGN(n)			((n) / abs(n))	// �����擾
#define SIGN_F(n)		((n) / fabs(n))	// �����擾(float)

#define PLAER_SPEED		(0.1f)
#define PLAER_SPEEDMAX	(5.0f)

#define STATE_MAX	(100)

#define HIT_WIDTH	(5.0f)
#define HIT_LEN		(150.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int priority, OBJTYPE objType) : CSceneX(priority, objType)
{
	m_rot  = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_size = D3DXVECTOR3(10.0f, 30.0f, 10.0f);

	m_vecF = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecR = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �Ǝ��̃��\�[�X�ݒ�
	//----------------------------
	m_mist = false;

	//----------------------------
	// �e�N���X������
	//----------------------------
	HRESULT_FUNC(CSceneX::Init(device, CImport::X_PLAYER));

	//----------------------------
	// ����������
	//----------------------------
	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CPlayer::Uninit(void)
{
	//----------------------------
	// �Ǝ��̊J��
	//----------------------------

	//----------------------------
	// �e�N���X�I��
	//----------------------------
	CSceneX::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CPlayer::Update(void)
{
	if(CPhase::GetFade()->GetState() == CFade::FADESTATE_NONE)
	{
		D3DXVECTOR3 cameraRot	 = CGame::GetCamera()->GetRot();
		D3DXMATRIX	workMtx;
		bool rotFlg = false;

		//----------------------------
		// �O��񏈗�
		//----------------------------
		// �O�X�V
		m_posOld = m_pos;
		m_rotOld = m_rot;

		//----------------------------
		// �ړ�����
		//----------------------------
		// ���ړ�
		if(m_keyboard->GetPress(DIK_W))
		{
			m_speed.x -= sinf(cameraRot.y) * PLAER_SPEED;
			m_speed.z += cosf(cameraRot.y) * PLAER_SPEED;
		}
		// ��O�ړ�
		if(m_keyboard->GetPress(DIK_S))
		{
			m_speed.x += sinf(cameraRot.y) * PLAER_SPEED;
			m_speed.z -= cosf(cameraRot.y) * PLAER_SPEED;
		}
		// ���ړ�
		if(m_keyboard->GetPress(DIK_A))
		{
			m_rot.y -= 0.03f;
		}
		// �E�ړ�
		if(m_keyboard->GetPress(DIK_D))
		{
			m_rot.y += 0.03f;
		}

		// ������x
		if(m_speed.x > PLAER_SPEEDMAX)
		{
			m_speed.x = PLAER_SPEEDMAX;
		}
		else if(m_speed.x < -PLAER_SPEEDMAX)
		{
			m_speed.x = -PLAER_SPEEDMAX;
		}

		if(m_speed.z > PLAER_SPEEDMAX)
		{
			m_speed.z = PLAER_SPEEDMAX;
		}
		else if(m_speed.z < -PLAER_SPEEDMAX)
		{
			m_speed.z = -PLAER_SPEEDMAX;
		}

		// �d��
		m_speed.x += (0 - m_speed.x) * 0.07f;
		m_speed.z += (0 - m_speed.z) * 0.07f;

		//----------------------------
		// �ړ�
		//----------------------------
		m_pos += m_speed;

		//----------------------------
		// ��]
		//----------------------------
			// �x�N�g���ϊ�
			D3DXMatrixRotationAxis(&workMtx, &m_vecU, 0.0f);
			D3DXVECTOR3 workRot = m_rot - m_rotOld;

			D3DXMatrixRotationAxis(&workMtx, &m_vecU, workRot.y);
			D3DXVec3TransformCoord(&m_vecF, &m_vecF, &workMtx);
			D3DXVec3TransformCoord(&m_vecR, &m_vecR, &workMtx);
			D3DXVec3Normalize(&m_vecU, &m_vecU);
			D3DXVec3Normalize(&m_vecF, &m_vecF);
			D3DXVec3Normalize(&m_vecR, &m_vecR);

		//----------------------------
		// ����
		//----------------------------
		if(m_keyboard->GetPress(DIK_SPACE))
		{
			D3DXVECTOR3 mistPos;
			mistPos.x = m_pos.x + m_vecF.x * 23.0f - m_vecR.x * 3.0f;
			mistPos.z = m_pos.z + m_vecF.z * 23.0f - m_vecR.z * 3.0f;
			mistPos.y = m_pos.y + 25.0f;

			CMist::Create(m_device, mistPos, m_vecF);

			m_mist = true;
		}
		else
		{
			m_mist = false;
		}

	#ifdef _DEBUG
		// �v���C���[���\��
		CDebugproc::PrintDebugProc("***�v���C���[���******\n");
		CDebugproc::PrintDebugProc("Pos  x:%f y:%f z:%f\n", m_pos.x, m_pos.y, m_pos.z);
		CDebugproc::PrintDebugProc("Speed x:%f y:%f z:%f\n", m_speed.x, m_speed.y, m_speed.z);
		CDebugproc::PrintDebugProc("VecU x:%f y:%f z:%f\n", m_vecU.x, m_vecU.y, m_vecU.z);
		CDebugproc::PrintDebugProc("VecF x:%f y:%f z:%f\n", m_vecF.x, m_vecF.y, m_vecF.z);
	#endif

		//----------------------------
		// �e�N���X�X�V
		//----------------------------
		CSceneX::Update();
	}
}

//=============================================================================
// �`��
//=============================================================================
void CPlayer::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// ����
//=============================================================================
CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 device)
{
	CPlayer* scene = new CPlayer;
	scene->Init(device);

	return scene;
}

//=============================================================================
// ��]
//=============================================================================
void CPlayer::SetRot(D3DXVECTOR3 rot)
{
	D3DXVECTOR3 oldRot = m_rot;
	D3DXMATRIX	workMtx;
	m_rot = rot;

	if(m_rot.y < -D3DX_PI || m_rot.y > D3DX_PI)	// �����ւ�
	{
		m_rot.y = m_rot.y + (-SIGN_F(m_rot.y) * D3DX_PI * 2.0f);
	}

	if(m_rot.x < -D3DX_PI || m_rot.x > D3DX_PI)	// �����ւ�
	{
		m_rot.x = m_rot.x + (-SIGN_F(m_rot.x) * D3DX_PI * 2.0f);
	}

	// �x�N�g���ϊ�
	D3DXMatrixRotationAxis(&workMtx, &m_vecU, 0.0f);
	D3DXVECTOR3 workRot = m_rot - oldRot;

	D3DXMatrixRotationAxis(&workMtx, &m_vecU, workRot.y);
	D3DXVec3TransformCoord(&m_vecF, &m_vecF, &workMtx);
	D3DXVec3TransformCoord(&m_vecR, &m_vecR, &workMtx);
	D3DXVec3Normalize(&m_vecF, &m_vecF);
	D3DXVec3Normalize(&m_vecR, &m_vecR);
}

//=============================================================================
// ���������蔻��
//=============================================================================
bool CPlayer::HitMist(D3DXVECTOR3 pos)
{
	if(m_mist)
	{
		D3DXVECTOR3 mistPos;
		mistPos.x = m_pos.x + m_vecF.x * 23.0f - m_vecR.x * 3.0f;
		mistPos.z = m_pos.z + m_vecF.z * 23.0f - m_vecR.z * 3.0f;
		mistPos.y = m_pos.y + 25.0f;

		D3DXVECTOR3 hitPos[2];
		hitPos[0].x = mistPos.x - m_vecR.x * HIT_WIDTH;
		hitPos[0].z = mistPos.z - m_vecR.x * HIT_WIDTH;
		hitPos[0].y = 0.0f;
		hitPos[1].x = mistPos.x + m_vecF.x * HIT_LEN + m_vecR.x * HIT_WIDTH;
		hitPos[1].z = mistPos.z + m_vecF.z * HIT_LEN + m_vecR.x * HIT_WIDTH;
		hitPos[1].y = 0.0f;

	#ifdef _DEBUG
		CDebugproc::PrintDebugProc("***�����蔻����******\n");
		CDebugproc::PrintDebugProc("hitPosMin x:%f y:%f z:%f\n", hitPos[0].x, hitPos[0].y, hitPos[0].z);
		CDebugproc::PrintDebugProc("hitPosMax x:%f y:%f z:%f\n", hitPos[1].x, hitPos[1].y, hitPos[1].z);
	#endif

		if((pos.x > hitPos[0].x && pos.x < hitPos[1].x)
		&& (pos.z > hitPos[0].z && pos.z < hitPos[1].z))
		{
			CDebugproc::PrintDebugProc("HIT!!!!!!!!!\n");
			return true;
		}
	}

	CDebugproc::PrintDebugProc("NOT\n");
	return false;
}
