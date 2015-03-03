//*****************************************************************************
//
// CSceneX�N���X [sceneX.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "sceneX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_xFile.mesh	= NULL;
	m_xFile.buffMat = NULL;

	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_size	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_parent = NULL;
}

//=============================================================================
// ����
//=============================================================================
CSceneX* CSceneX::Create(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile)
{
	CSceneX* pointer = new CSceneX;
	pointer->Init(device, xFile);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CSceneX::Init(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile)
{
	//----------------------------
	// �f�o�C�X�擾
	//----------------------------
	m_device = device;

	//----------------------------
	// ���f���擾
	//----------------------------
	m_xFile = CImport::GetXFile(xFile);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CSceneX::Uninit(void)
{
	// �J��
	CScene::Delete();
}

//=============================================================================
// �X�V
//=============================================================================
void CSceneX::Update(void)
{
}

//=============================================================================
// �`��
//=============================================================================
void CSceneX::Draw(void)
{
	//----------------------------
	// ���[���h�}�g���b�N�X
	//----------------------------
	D3DXMATRIX	scl, rot, translate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_world);
	// �X�P�[���̔��f
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&m_world, &m_world, &scl);
	// ��]�̔��f
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_world, &m_world, &rot);
	// �ʒu�̔��f
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_world, &m_world, &translate);

	//�e�����݂���ꍇ�͐e�}�g���N�X������
	if(m_parent)
	{
		D3DXMatrixMultiply(&m_world, &m_world, &m_parent->m_world);
	}

	// ���[���h�}�g���b�N�X�̐ݒ�
	m_device->SetTransform(D3DTS_WORLD, &m_world);

	//----------------------------
	// ���f���̕`��
	//----------------------------
	D3DMATERIAL9 matDef;

	m_device->GetMaterial(&matDef);	// ���݂̃}�e���A�����擾

	for(unsigned int cntMat = 0; cntMat < m_xFile.numMat; cntMat++)
	{
		m_device->SetMaterial(&m_xFile.mat[cntMat].MatD3D);
		m_device->SetTexture(0, m_xFile.tex[cntMat]);
		m_xFile.mesh->DrawSubset(cntMat);
	}

	m_device->SetMaterial(&matDef);	// �}�e���A�������ɖ߂�
}