//*****************************************************************************
//
// CDustAIAI�N���X [dustAI.cpp]
// Author : YUJI MOMOI
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "dustAI.h"
#include "dust.h"
#include "import.h"
#include "game.h"
#include "player.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDustAI::CDustAI()
{
	TacticsState = TACTICS_STATE_STAY;
	m_nThinkCount = 0;
}

//=============================================================================
// ����
//=============================================================================
CDustAI* CDustAI::Create(CDust* pDust)
{
	CDustAI* pointer = new CDustAI;
	pointer->Init(pDust);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CDustAI::Init(CDust* pDust)
{
	SetDust(pDust);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CDustAI::Uninit(void)
{
}

//=============================================================================
// �X�V
//=============================================================================
void CDustAI::Update(void)
{
	// ���j�b�g���������Ă��Ȃ���
	if(TacticsState == TACTICS_STATE_NONE)
	{
		TacticsState = TACTICS_STATE_MOVE_START;
	}

	// ���j�b�g�̈ړ��J�n���ɖړI�n�ݒ�
	if(TacticsState == TACTICS_STATE_MOVE_START)
	{
		ThinkPosDest();
	}

	// ���j�b�g�̈ړ����̏���
	if(TacticsState == TACTICS_STATE_MOVE)
	{
		SerchVision();
	}

	// �U���������̏���
	if(TacticsState == TACTICS_STATE_ATTACK_POSSIBLE)
	{
		AttackPossible();
	}

	// �U������
	if(TacticsState == TACTICS_STATE_ATTACK)
	{
		Attack();
	}

	// �U����̏���
	if(TacticsState == TACTICS_STATE_ATTACK_END)
	{
		AttackEnd();
	}

	// ��������
	if(TacticsState == TACTICS_STATE_DEFILE)
	{
		Defile();
	}

	// �ҋ@���̏��� �v�l�Ԋu������
	if(TacticsState == TACTICS_STATE_STAY)
	{
		m_nThinkCount ++;

		if(m_nThinkCount > THINK_COOL_TIME)
		{
			m_nThinkCount = 0;
			TacticsState = TACTICS_STATE_NONE;
		}
	}
}

//=============================================================================
// �ړI�n�����߂�
//=============================================================================
void CDustAI::ThinkPosDest(void)
{
	m_dust->SetPosDest(CGame::GetPlayer()->GetPos());
	TacticsState = TACTICS_STATE_MOVE;
	m_dust->SetMove(true);
}

//=============================================================================
// �U���J�n
//=============================================================================
void CDustAI::AttackPossible(void)
{
}

//=============================================================================
// �U��
//=============================================================================
void CDustAI::Attack(void)
{
}

//=============================================================================
// �U����
//=============================================================================
void CDustAI::AttackEnd(void)
{
}

//=============================================================================
// ���E����
//=============================================================================
bool CDustAI::SerchVision(void)
{
	CScene* pScene = NULL;

	for(int cnt = 0;cnt < PRIORITY_MAX; ++cnt)
	{
		pScene = CScene::GetTop(cnt);

		while(pScene != NULL)
		{
			if(pScene -> GetType() == CScene::OBJTYPE_BILL)
			{
				D3DXVECTOR3 min = CImport::GetVtxMin(CScene::OBJTYPE_BILL);	// �M�~�b�N�̃��[�J���ŏ��ʒu���擾
				D3DXVECTOR3 max = CImport::GetVtxMax(CScene::OBJTYPE_BILL);	// �M�~�b�N�̃��[�J���ő�ʒu���擾
				D3DXVECTOR3 pos = pScene->GetPos();			// �M�~�b�N�̍��W���擾

				D3DXVECTOR3 p[4];							// �M�~�b�N�̊O�ϔ���p�̔z���p��

				// �M�~�b�N�̊O�όv�Z�p���W���v�Z
				p[0].x = pos.x + min.x;
				p[0].z = pos.z + max.z;
				p[1].x = pos.x + max.x;
				p[1].z = pos.z + max.z;
				p[2].x = pos.x + max.x;
				p[2].z = pos.z + min.z;
				p[3].x = pos.x + min.x;
				p[3].z = pos.z + min.z;

				D3DXVECTOR3 offset = m_dust->GetPosDest() - pos;			// �I�t�Z�b�g���W(�v���C���[�ƃM�~�b�N�̋���)
				D3DXVECTOR3 rot = pScene->GetRot();			// �M�~�b�N�̊p�x���擾
				D3DXVECTOR3 target;							// �I�t�Z�b�g���W�ł̉�]����W�i�[�p

				target.x = pos.x + cosf(-rot.y) * offset.x + sinf(-rot.y) * offset.z;	// �I�t�Z�b�g���W�ł̉�]����WX
				target.z = pos.z + cosf(-rot.y) * offset.z - sinf(-rot.y) * offset.x;	// �I�t�Z�b�g���W�ł̉�]����WZ

				D3DXVECTOR3 VecLine;
				D3DXVECTOR3 VecToTarget;
				bool bHit = false;

				// �O�ςł̓����蔻��
				for(int nloop = 0;nloop < 4 && !bHit;nloop++)
				{
					VecLine = p[(nloop + 1) % 4] - p[nloop];
					VecToTarget = target - p[nloop];

					if(VecLine.z * VecToTarget.x - VecLine.x * VecToTarget.z >= 0.0f)
					{
					}
					else
					{
						bHit = true;
						break;
					}
				}

				if(!bHit)
				{
					SetGoal();
					break;
				}
			}

			pScene = pScene -> GetNext();
		}
	}

	return false;
}

//=============================================================================
// ��������
//=============================================================================
void CDustAI::Defile(void)
{

}

//=============================================================================
// �S�[���������Ƃ�`����
//=============================================================================
void CDustAI::SetGoal(void)
{
	TacticsState = TACTICS_STATE_STAY;
	m_dust->SetMove(false);
}