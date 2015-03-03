//*****************************************************************************
//
// �J�������� [camera.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_CAMERA_H
#define _MY_CAMERA_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputKeyboard;
class CPlayer;

class CCamera
{
public:
	CCamera();
	~CCamera(){};

	static	CCamera* Create(D3DXVECTOR3 at, D3DXVECTOR3 pos);
	HRESULT	Init(D3DXVECTOR3 at, D3DXVECTOR3 pos);
	void	Uninit(void);
	void	Update(void);
	void	SetCamera(LPDIRECT3DDEVICE9 device);

	D3DXMATRIX GetMtxView(void){return m_view;}

	void		SetPos(D3DXVECTOR3 pos){m_pos = pos; ResetRot();}
	D3DXVECTOR3	GetPos(void){return m_pos;}
	void		SetAt(D3DXVECTOR3 at){m_at = at;}
	D3DXVECTOR3	GetAt(void){return m_at;}
	D3DXVECTOR3	GetRot(void){return m_rot;}
	void		SetDistance(float distance){m_distance = distance;}
	void		SetDistance(void){m_distance = DISTANCE_3D(m_pos.x, m_pos.y, m_pos.z);}
	float		GetDistance(void){return m_distance;}

	void		SetPosDest(D3DXVECTOR3 posDest){m_posDest = posDest;}
	D3DXVECTOR3	GetPosDest(void){return m_posDest;}
	void		SetAtDest(D3DXVECTOR3 atDest){m_atDest = atDest;}
	D3DXVECTOR3	GetAtDest(void){return m_atDest;}
	D3DXVECTOR3	GetRotDest(void){return m_rotDest;}
	void		SetDistanceDest(float distanceDest){m_distanceDest = distanceDest;}
	float		GetDistanceDest(void){return m_distanceDest;}

private:
	void ResetPos(void);
	void ResetRot(void);
	void ResetPosDest(void);
	void ResetRotDest(void);
	void ResetDistance(void);

	D3DXMATRIX	 m_view;		// �r���[�}�g���b�N�X
	D3DXMATRIX	 m_projection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DVIEWPORT9 m_viewport;	// �r���[�|�[�g

	D3DXVECTOR3	m_pos;		// ���_
	D3DXVECTOR3	m_at;		// �����_
	D3DXVECTOR3	m_rot;		// ����(��]�p)
	float		m_distance;	// ����(���_���璍���_�܂�)

	D3DXVECTOR3	m_posDest;		// �ړI�̎��_
	D3DXVECTOR3	m_atDest;		// �ړI�̒����_
	D3DXVECTOR3	m_rotDest;		// �ړI�̌���(��]�p)
	float		m_distanceDest;	// �ړI�̋���

	D3DXVECTOR3	m_vecU;	// ������x�N�g��
	D3DXVECTOR3	m_vecF;	// �O�����x�N�g��
	D3DXVECTOR3	m_vecR;	// �E�����x�N�g��
};

//=============================================================================
#endif