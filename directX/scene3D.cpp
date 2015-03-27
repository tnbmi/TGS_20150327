//*****************************************************************************
//
// CScene3D�N���X [scene3D.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "scene3D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define POLIGON_SIZE (1.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene3D::CScene3D(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_tex		= NULL;
	m_vtxBuff	= NULL;
	m_indexBuff	= NULL;

	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_size	= D3DXVECTOR3(POLIGON_SIZE, POLIGON_SIZE, 0.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ����
//=============================================================================
CScene3D* CScene3D::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture)
{
	CScene3D* pointer = new CScene3D;
	pointer->Init(device, texture);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CScene3D::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture)
{
	//----------------------------
	// �f�o�C�X�擾
	//----------------------------
	m_device = device;

	//----------------------------
	// �|���S��������
	//----------------------------
	// �������m��
	HRESULT_FUNC(m_device->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &m_vtxBuff,
											  NULL))

	// ���_���ݒ�
	SetVertexPolygon();

	// �e�N�X�`���̓Ǎ���
	m_tex = CImport::GetTexture(texture);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CScene3D::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(m_vtxBuff);

	// �J��
	CScene::Delete();
}

//=============================================================================
// �X�V
//=============================================================================
void CScene3D::Update(void)
{
}

//=============================================================================
// �`��
//=============================================================================
void CScene3D::Draw(void)
{
	D3DXMATRIX scl, rot, translate;

	//----------------------------
	// ���[���h�}�g���b�N�X
	//----------------------------
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
	// ���[���h�}�g���b�N�X�̐ݒ�
	m_device->SetTransform(D3DTS_WORLD, &m_world);

	//----------------------------
	// �|���S���̕`��
	//----------------------------
	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	m_device->SetStreamSource( 0, m_vtxBuff, 0, sizeof(VERTEX_3D) );
	// ���_�t�H�[�}�b�g�̐ݒ�
	m_device->SetFVF(FVF_VERTEX_3D);
	// �e�N�X�`���̐ݒ�
	m_device->SetTexture(0, m_tex);
	// �`��ݒ�
	m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// �`����
							0,						//
							2);						// �|���S���̐�
}

//=============================================================================
// ���_���ݒ�
//=============================================================================
void CScene3D::SetVertexPolygon(void)
{
	VERTEX_3D* vtx;

	// �J�n
	m_vtxBuff->Lock(0, 0, (void **)&vtx, 0);

	// ���W
	D3DXVECTOR3 half = D3DXVECTOR3(m_size.x / 2.0f,  m_size.y / 2.0f, m_size.z / 2.0f);
	vtx[0].vtx = D3DXVECTOR3(-half.x,  half.y, half.z);
	vtx[1].vtx = D3DXVECTOR3( half.x,  half.y, half.z);
	vtx[2].vtx = D3DXVECTOR3(-half.x, -half.y, half.z);
	vtx[3].vtx = D3DXVECTOR3( half.x, -half.y, half.z);

	// ���_�x�N�g��
	vtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���ˌ�
	vtx[0].diffuse = m_color;
	vtx[1].diffuse = m_color;
	vtx[2].diffuse = m_color;
	vtx[3].diffuse = m_color;

	// �e�N�X�`�����W
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// �I��
	m_vtxBuff->Unlock();
}
