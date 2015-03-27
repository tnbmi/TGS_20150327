//*****************************************************************************
//
// CScene�N���X [scene.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_SCENE_H
#define _MY_SCENE_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define PRIORITY_MAX	(5)

// ���b�V���p
#define NUMVERTEX(x, z)		((x + 1) * (z + 1))					// �����_�v�Z
#define NUMINDEX(x, z)		((x + 1) * (z * 2) + ((z - 1) * 2))	// ���C���f�b�N�X�v�Z
#define WORKLINE(x, z, idx)	((idx / (NUMINDEX(x, z) / z)) + 1)	// ��s�̃C���f�b�N�X�v�Z

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_2D,
		OBJTYPE_3D,
		OBJTYPE_X,
		OBJTYPE_BILLBOARD,
		OBJTYPE_WALL,
		OBJTYPE_FIELD,
		OBJTYPE_DOME,
		OBJTYPE_CYLINDER,
		OBJTYPE_FADE,
		OBJTYPE_ENEMY,
	} OBJTYPE;

	typedef enum
	{
		MESHTEX_FULL,
		MESHTEX_PATCH
	} MESHTEX_TYPE;

	CScene(int priority = PRIORITY_MAX - 1, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene(){};

	virtual HRESULT	Init(void){return S_OK;}
	virtual void	Uninit(void) = 0;
	virtual void	Update(void) = 0;
	virtual void	Draw(void)	 = 0;

	static void ReleaseAll(void);

	void	UnLinkScene(void);
	void	Delete(void){m_delete = true;}

	virtual void		SetPos(D3DXVECTOR3 pos) = 0;
	virtual void		SetPos(float x, float y, float z) = 0;
	virtual D3DXVECTOR3	GetPos(void) = 0;
	virtual void		SetRot(D3DXVECTOR3 rot) = 0;
	virtual void		SetRot(float x, float y, float z) = 0;
	virtual D3DXVECTOR3	GetRot(void) = 0;
	virtual void		SetSize(D3DXVECTOR3 size) = 0;
	virtual void		SetSize(float x, float y, float z) = 0;
	virtual D3DXVECTOR3	GetSize(void) = 0;
	virtual void		SetColor(D3DXCOLOR color) = 0;
	virtual void		SetColor(float r, float g, float b, float a) = 0;
	virtual D3DXCOLOR	GetColor(void) = 0;

	int		GetPriority(void){return m_priority;}
	OBJTYPE	GetType(void){return m_objType;}
	bool	GetDelete(void){return m_delete;}

	static CScene* GetTop(int priority){return m_top[priority];}
	static CScene* GetCur(int priority){return m_cur[priority];}

	CScene* GetPrev(void){return m_prev;}
	CScene* GetNext(void){return m_next;}

	void	SetUpdatePrev(CScene* obj){m_updatePrev = obj;}
	CScene* GetUpdatePrev(void){return m_updatePrev;}
	void	SetUpdateNext(CScene* obj){m_updateNext = obj;}
	CScene* GetUpdateNext(void){return m_updateNext;}

	void	SetDrawPrev(CScene* obj){m_drawPrev = obj;}
	CScene* GetDrawPrev(void){return m_drawPrev;}
	void	SetDrawNext(CScene* obj){m_drawNext = obj;}
	CScene* GetDrawNext(void){return m_drawNext;}

	virtual D3DXVECTOR3 GetVtxMin(void){return D3DXVECTOR3(0.0f, 0.0f, 0.0f);}
	virtual D3DXVECTOR3 GetVtxMax(void){return D3DXVECTOR3(0.0f, 0.0f, 0.0f);};

protected:
	LPDIRECT3DDEVICE9 m_device;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_scl;
	D3DXVECTOR3 m_size;
	D3DXCOLOR	m_color;

private:
	int		m_priority;
	OBJTYPE m_objType;
	bool	m_delete;

	static	CScene* m_top[PRIORITY_MAX];	// ���X�g�̐擪�|�C���^
	static	CScene* m_cur[PRIORITY_MAX];	// ���X�g�̌��ݏI�[�|�C���^

	CScene* m_prev;			// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene* m_next;			// ���̃I�u�W�F�N�g�ւ̃|�C���^
	CScene* m_updatePrev;	// �O�̍X�V�I�u�W�F�N�g�ւ̃|�C���^
	CScene* m_updateNext;	// ���̍X�V�I�u�W�F�N�g�ւ̃|�C���^
	CScene* m_drawPrev;		// �O�̕`��I�u�W�F�N�g�ւ̃|�C���^
	CScene* m_drawNext;		// ���̕`��I�u�W�F�N�g�ւ̃|�C���^
};

//=============================================================================
#endif