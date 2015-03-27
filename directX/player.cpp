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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SIGN(n)			((n) / abs(n))	// �����擾
#define SIGN_F(n)		((n) / fabs(n))	// �����擾(float)

#define PLAER_SPEED		(0.05f)
#define PLAER_SPEEDMAX	(1.0f)

#define STATE_MAX	(100)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int priority, OBJTYPE objType) : CSceneX(priority, objType)
{
<<<<<<< HEAD
	m_rot  = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	m_size = D3DXVECTOR3(5.0f, 15.0f, 5.0f);

=======
	m_size = D3DXVECTOR3(5.0f, 15.0f, 5.0f);
>>>>>>> origin/dev/zyamu
	m_vecF = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vecR = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_speed = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �Ǝ��̃��\�[�X�ݒ�
	//----------------------------

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

			rotFlg = true;
		}
		// ��O�ړ�
		if(m_keyboard->GetPress(DIK_S))
		{
			m_speed.x += sinf(cameraRot.y) * PLAER_SPEED;
			m_speed.z -= cosf(cameraRot.y) * PLAER_SPEED;

			rotFlg = true;
		}
		// ���ړ�
		if(m_keyboard->GetPress(DIK_A))
		{
			m_speed.x -= cosf(cameraRot.y) * PLAER_SPEED;
			m_speed.z -= sinf(cameraRot.y) * PLAER_SPEED;

			rotFlg = true;
		}
		// �E�ړ�
		if(m_keyboard->GetPress(DIK_D))
		{
			m_speed.x += cosf(cameraRot.y) * PLAER_SPEED;
			m_speed.z += sinf(cameraRot.y) * PLAER_SPEED;

			rotFlg = true;
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
		//m_speed.y += -0.07f;

		//----------------------------
		// �ړ�
		//----------------------------
		m_pos += m_speed;

		//----------------------------
		// ��]
		//----------------------------
		if(rotFlg)
		{
			float diffRot;		// ��]�ړI�܂ł̍���
			float excessRot;	// ��]���ߕ�

			// �������ړ����Ă���
			if(m_pos.x != m_posOld.x || m_pos.z != m_posOld.z)
			{
				D3DXVECTOR3 Def;			// �
				D3DXVECTOR3 End;			// �ړI(�I�_)
				float		fVec1, fVec2;	// �x�N�g����Βl
				float		fInner;			// ����
	
				// �x�N�g������
				// �(z���̉�)
				Def.x = 0;
				Def.z = D3DX_PI;
				// �ړI(�ړ��O�ƌ�̍�)
				End.x = m_speed.x;
				End.z = -m_speed.z;

				// �x�N�g����Βl
				// �
				fVec1 = sqrt(Def.x*Def.x + Def.z*Def.z);
				// �ړI
				fVec2 = sqrt(End.x*End.x + End.z*End.z);

				// ����
				fInner = (Def.x*End.x + Def.z*End.z);

				// �ړI�̊p�x
				m_rotDest.y = acos(fInner / (fVec1 * fVec2));

				if(End.x != 0)	// x�����ړ����Ă���
				{
					m_rotDest.y *= -SIGN(End.x);
				}
			}

			// �ړI�̌����܂ł̍���
			diffRot =	m_rotDest.y - m_rot.y;
			if(diffRot < -D3DX_PI || diffRot > D3DX_PI)	// �����ւ�
			{
				excessRot	= diffRot - (SIGN_F(diffRot) * D3DX_PI);

				diffRot	= (SIGN_F(diffRot) * D3DX_PI) - excessRot;
				diffRot	= (diffRot * (-1)) + (SIGN_F(diffRot) * 0.01f);
			}

			// �������X�V
			m_rot.y +=	diffRot * 0.1f;
			if(m_rot.y < -D3DX_PI || m_rot.y > D3DX_PI)	// �����ւ�
			{
				m_rot.y = (m_rot.y * (-1)) + (SIGN_F(m_rot.y) * 0.1f);
			}

			// �x�N�g���ϊ�
			D3DXMatrixRotationAxis(&workMtx, &m_vecU, 0.0f);
			D3DXVECTOR3 workRot = m_rot - m_rotOld;

			D3DXMatrixRotationAxis(&workMtx, &m_vecU, workRot.y);
			D3DXVec3TransformCoord(&m_vecF, &m_vecF, &workMtx);
			D3DXVec3TransformCoord(&m_vecR, &m_vecR, &workMtx);
			D3DXVec3Normalize(&m_vecU, &m_vecU);
			D3DXVec3Normalize(&m_vecF, &m_vecF);
			D3DXVec3Normalize(&m_vecR, &m_vecR);
		}

	#ifdef _DEBUG
		// �v���C���[���\��
		CDebugproc::PrintDebugProc("***�v���C���[���******\n");
		CDebugproc::PrintDebugProc("Pos  x:%f y:%f z:%f\n", m_pos.x, m_pos.y, m_pos.z);
		CDebugproc::PrintDebugProc("Speed x:%f y:%f z:%f\n", m_speed.x, m_speed.y, m_speed.z);
<<<<<<< HEAD
		CDebugproc::PrintDebugProc("Vec U:%f F:%f R:%f\n", m_vecU, m_vecF, m_vecR);
=======
>>>>>>> origin/dev/zyamu
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
