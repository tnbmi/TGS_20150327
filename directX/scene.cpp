//*****************************************************************************
//
// CScene�N���X [scene.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "scene.h"
#include "listObject.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CScene* CScene::m_top[PRIORITY_MAX];
CScene* CScene::m_cur[PRIORITY_MAX];

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene::CScene(int priority, OBJTYPE objType)
{
	//----------------------------
	// �V�[�����X�g�ǉ�
	//----------------------------
	// �v���C�I���e�B�ݒ�
	m_priority = priority;

	// �I�u�W�F�^�C�v�ݒ�
	m_objType = objType;

	// �擪�`�F�b�N
	if(m_top[m_priority] == NULL)
	{
		m_top[m_priority] = this;
		m_prev = NULL;
	}
	else
	{
		// �O�I�u�W�F�N�g����A��
		m_cur[m_priority]->m_next = this;
		m_prev = m_cur[m_priority];
	}

	// ���I�u�W�F�N�g������
	m_next = NULL;

	// �I�[�A�h���X�ݒ�
	m_cur[m_priority] = this;

	//----------------------------
	// �폜�t���O
	//----------------------------
	m_delete = false;

	//----------------------------
	// �I�u�W�F�N�g���X�g�ǉ�
	//----------------------------
	CListObject::LinkObj(this, m_priority);
}

//=============================================================================
// �S�I��
//=============================================================================
void CScene::ReleaseAll(void)
{
	CScene* cur;
	CScene* next;

	for(int cnt = 0; cnt < PRIORITY_MAX; ++cnt)
	{
		cur = m_top[cnt];

		while(cur)
		{
			next = cur->m_next;
			cur->Uninit();

			cur->UnLinkScene();
			delete cur;

			cur  = next;
		}
	}
}

//=============================================================================
// �E���X�g
//=============================================================================
void CScene::UnLinkScene(void)
{
	//----------------------------
	// �V�[�����X�g����j��
	//----------------------------
	if(m_prev != NULL)
	{
		m_prev->m_next = m_next;
	}
	else // Top�������ꍇ
	{
		m_top[m_priority] = m_next;

		if(m_top[m_priority] != NULL)
		{
			m_top[m_priority]->m_prev = NULL;
		}
	}

	if(m_next != NULL)
	{
		m_next->m_prev = m_prev;
	}
	else // Cur�������ꍇ
	{
		m_cur[m_priority] = m_prev;

		if(m_top[m_priority] != NULL)
		{
			m_cur[m_priority]->m_next = NULL;
		}
	}

	//----------------------------
	// �I�u�W�F�N�g���X�g����j��
	//----------------------------
	CListObject::UnlinkObj(this);
}