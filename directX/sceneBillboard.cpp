//*****************************************************************************
//
// CSceneBillboard�N���X [sceneBillboard.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "sceneBillboard.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define POLIGON_SIZE (1.0f)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
D3DXMATRIX CSceneBillboard::m_view;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneBillboard::CSceneBillboard(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_tex		= NULL;
	m_vtxBuff	= NULL;
	m_indexBuff	= NULL;

	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_size	= D3DXVECTOR3(POLIGON_SIZE, POLIGON_SIZE, 0.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_texcord[0] = D3DXVECTOR2(0.0f, 0.0f);
	m_texcord[1] = D3DXVECTOR2(1.0f, 0.0f);
	m_texcord[2] = D3DXVECTOR2(0.0f, 1.0f);
	m_texcord[3] = D3DXVECTOR2(1.0f, 1.0f);
}

//=============================================================================
// ����
//=============================================================================
CSceneBillboard* CSceneBillboard::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXMATRIX view)
{
	CSceneBillboard* pointer = new CSceneBillboard;
	pointer->Init(device, texture, view);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CSceneBillboard::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXMATRIX view)
{
	//----------------------------
	// �f�o�C�X�E�r���[�s��擾
	//----------------------------
	m_device = device;
	m_view	 = view;

	//----------------------------
	// �|���S��������
	//----------------------------
	// �������m��
	HRESULT_FUNC(m_device->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// �m�ۂ���o�b�t�@�T�C�Y
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,			// ���_�t�H�[�}�b�g
											  D3DPOOL_MANAGED,
											  &m_vtxBuff,				// ���_���
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
void CSceneBillboard::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(m_vtxBuff);

	// �J��
	CScene::Delete();
}

//=============================================================================
// �X�V
//=============================================================================
void CSceneBillboard::Update(void)
{
}

//=============================================================================
// �`��
//=============================================================================
void CSceneBillboard::Draw(void)
{
	D3DXMATRIX	scl, rot, translate;

	//----------------------------
	// ��d���I�t
	//----------------------------
	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);

	//----------------------------
	// ���[���h�}�g���b�N�X
	//----------------------------
	// ���[���h�}�g���b�N�X�̏�����(�r���{�[�h��)
	D3DXMatrixIdentity(&m_world);
	D3DXMatrixInverse(&m_world, NULL, &m_view);	// mtxView�̋t�s������߂�
	m_world._41 = 0.0f;
	m_world._42 = 0.0f;
	m_world._43 = 0.0f;

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

	//----------------------------
	// ��d���I��
	//----------------------------
	m_device->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// ���_���ݒ�
//=============================================================================
void CSceneBillboard::SetVertexPolygon(void)
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
	vtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���ˌ�
	vtx[0].diffuse = m_color;
	vtx[1].diffuse = m_color;
	vtx[2].diffuse = m_color;
	vtx[3].diffuse = m_color;

	// �e�N�X�`�����W
	vtx[0].tex = m_texcord[0];
	vtx[1].tex = m_texcord[1];
	vtx[2].tex = m_texcord[2];
	vtx[3].tex = m_texcord[3];

	// �I��
	m_vtxBuff->Unlock();
}
