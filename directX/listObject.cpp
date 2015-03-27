//*****************************************************************************
//
// CListObject�N���X [listObject.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "listObject.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CScene* CListObject::m_updateTop = nullptr;
CScene* CListObject::m_updateCur = nullptr;
CScene* CListObject::m_drawTop[PRIORITY_MAX] = {nullptr};
CScene* CListObject::m_drawCur[PRIORITY_MAX] = {nullptr};

//=============================================================================
// �X�V
//=============================================================================
void CListObject::Update(void)
{
	CScene* cur = m_updateTop;
	CScene* next;

	while(cur)
	{
		cur->Update();

		next = cur->GetUpdateNext();

		if(cur->GetDelete())
		{
			cur->UnLinkScene();
			delete cur;
		}

		cur = next;
	}
}

//=============================================================================
// �`��
//=============================================================================
void CListObject::Draw(void)
{
	CScene* cur;

	for(int cnt = 0; cnt < PRIORITY_MAX; ++cnt)
	{
		cur = m_drawTop[cnt];

		while(cur)
		{
			cur->Draw();

			cur = cur->GetDrawNext();
		}
	}
}

//=============================================================================
// ���X�g�ǉ�
//=============================================================================
void CListObject::LinkObj(CScene* obj, int priority)
{
	// �X�V���X�g
	LinkUpdate(obj);

	// �`�惊�X�g
	LinkDraw(obj, priority);
}

//=============================================================================
// ���X�g����j��
//=============================================================================
void CListObject::UnlinkObj(CScene* obj)
{
	// �X�V���X�g
	UnlinkUpdate(obj);

	// �`�惊�X�g
	UnlinkDraw(obj);
}

//=============================================================================
// �X�V���X�g�ǉ�
//=============================================================================
void CListObject::LinkUpdate(CScene* obj)
{
	// �擪�`�F�b�N
	if(m_updateTop == NULL)
	{
		m_updateTop = obj;
		obj->SetUpdatePrev(NULL);
	}
	else
	{
		// �O�I�u�W�F�N�g����A��
		m_updateCur->SetUpdateNext(obj);
		obj->SetUpdatePrev(m_updateCur);
	}

	// ���I�u�W�F�N�g������
	obj->SetUpdateNext(NULL);
	// �I�[�A�h���X�ݒ�
	m_updateCur = obj;
}

//=============================================================================
// �X�V���X�g����j��
//=============================================================================
void CListObject::UnlinkUpdate(CScene* obj)
{
	CScene* prev = obj->GetUpdatePrev();
	CScene* next = obj->GetUpdateNext();

	if(prev != NULL)
	{
		prev->SetUpdateNext(next);
	}
	else // Top�������ꍇ
	{
		m_updateTop = next;

		if(m_updateTop != NULL)
		{
			m_updateTop->SetUpdatePrev(NULL);
		}
	}

	if(next != NULL)
	{
		next->SetUpdatePrev(prev);
	}
	else // Cur�������ꍇ
	{
		m_updateCur = prev;

		if(m_updateTop != NULL)
		{
			m_updateCur->SetUpdateNext(NULL);
		}
	}
}

//=============================================================================
// �`�惊�X�g�ǉ�
//=============================================================================
void CListObject::LinkDraw(CScene* obj, int priority)
{
	// �擪�`�F�b�N
	if(m_drawTop[priority] == NULL)
	{
		m_drawTop[priority] = obj;
		obj->SetDrawPrev(NULL);
	}
	else
	{
		// �O�I�u�W�F�N�g����A��
		m_drawCur[priority]->SetDrawNext(obj);
		obj->SetDrawPrev(m_drawCur[priority]);
	}

	// ���I�u�W�F�N�g������
	obj->SetDrawNext(NULL);
	// �I�[�A�h���X�ݒ�
	m_drawCur[priority] = obj;
}

//=============================================================================
// �`�惊�X�g����j��
//=============================================================================
void CListObject::UnlinkDraw(CScene* obj)
{
	CScene* prev = obj->GetDrawPrev();
	CScene* next = obj->GetDrawNext();
	int priority = obj->GetPriority();

	if(prev != NULL)
	{
		prev->SetDrawNext(next);
	}
	else // Top�������ꍇ
	{
		m_drawTop[priority] = next;

		if(m_drawTop[priority] != NULL)
		{
			m_drawTop[priority]->SetDrawPrev(NULL);
		}
	}

	if(next != NULL)
	{
		next->SetDrawPrev(prev);
	}
	else // Cur�������ꍇ
	{
		m_drawCur[priority] = prev;

		if(m_drawTop[priority] != NULL)
		{
			m_drawCur[priority]->SetDrawNext(NULL);
		}
	}
}
