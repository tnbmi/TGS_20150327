//*****************************************************************************
//
// CListObject�N���X [listObject.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_LIST_OBJECT_H
#define _MY_LIST_OBJECT_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "scene.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CListObject
{
public:
	CListObject();
	~CListObject(){};

	static void Update(void);
	static void Draw(void);

	static void LinkObj(CScene* obj, int priority);
	static void UnlinkObj(CScene* obj);

	static void LinkUpdate(CScene* obj);
	static void UnlinkUpdate(CScene* obj);

	static void LinkDraw(CScene* obj, int priority);
	static void UnlinkDraw(CScene* obj);

private:
	static	CScene* m_updateTop;	// �X�V���X�g�̐擪�|�C���^
	static	CScene* m_updateCur;	// �X�V���X�g�̌��ݏI�[�|�C���^

	static	CScene* m_drawTop[PRIORITY_MAX];	// �`�惊�X�g�̐擪�|�C���^
	static	CScene* m_drawCur[PRIORITY_MAX];	// �`�惊�X�g�̌��ݏI�[�|�C���^
};

//=============================================================================
#endif