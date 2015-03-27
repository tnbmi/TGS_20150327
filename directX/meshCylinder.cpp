//*****************************************************************************
//
// CMeshCylinder�N���X [meshCylinder.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "meshCylinder.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshCylinder::CMeshCylinder(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	// �����o������
	m_tex		= NULL;
	m_vtxBuff	= NULL;
	m_indexBuff	= NULL;

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// ����
//=============================================================================
CMeshCylinder* CMeshCylinder::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock)
{
	CMeshCylinder* mesh= new CMeshCylinder;

	mesh->Init(device, texture, sizeBlock, numBlock);

	return mesh;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CMeshCylinder::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock)
{
	//----------------------------
	// �f�o�C�X�擾
	//----------------------------
	m_device = device;

	//----------------------------
	// �u���b�N�ݒ�
	//----------------------------
	// �u���b�N�T�C�Y
	m_sizeBlock.x = sizeBlock.x;
	m_sizeBlock.y = sizeBlock.y;

	// �u���b�N��
	m_numBlock.x = numBlock.x;
	m_numBlock.y = numBlock.y;

	//----------------------------
	// �t�B�[���h�ݒ�
	//----------------------------
	// �����_��
	m_numVtx = (int)NUMVERTEX(numBlock.x, numBlock.y);

	// ���_�̑��C���f�b�N�X��
	m_numVtxIndex = (int)NUMINDEX(numBlock.x, numBlock.y);

	// ���|���S����
	m_numPolygon = m_numVtxIndex - 2;

	//----------------------------
	// �|���S��������
	//----------------------------
	// �������m��
	HRESULT_FUNC(m_device->CreateVertexBuffer(sizeof(VERTEX_3D) * m_numVtx,	// �m�ۂ���o�b�t�@�T�C�Y
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,				// ���_�t�H�[�}�b�g
											D3DPOOL_MANAGED,
											&m_vtxBuff,					// ���_���
											NULL))

	HRESULT_FUNC(m_device->CreateIndexBuffer(sizeof(WORD) * m_numVtxIndex,	// �m�ۂ���o�b�t�@�T�C�Y
											D3DUSAGE_WRITEONLY,
											D3DFMT_INDEX16,				// ���_�t�H�[�}�b�g
											D3DPOOL_MANAGED,
											&m_indexBuff,				// ���_���
											NULL))

	// ���_���ݒ�
	SetVertexPolygon();

	// ���_�C���f�b�N�X�ݒ�
	SetVertexIndexPolygon();

	// �e�N�X�`���̓Ǎ���
	m_tex = CImport::GetTexture(texture);

	//----------------------------
	// ����������
	//----------------------------
	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CMeshCylinder::Uninit(void)
{
	// ���_�o�b�t�@�̊J��
	SAFE_RELEASE(m_vtxBuff);

	// �C���f�b�N�X�o�b�t�@�̊J��
	SAFE_RELEASE(m_indexBuff);

	// �J��
	CScene::Delete();
}

//=============================================================================
// �X�V
//=============================================================================
void CMeshCylinder::Update(void)
{
}

//=============================================================================
// �`��
//=============================================================================
void CMeshCylinder::Draw(void)
{
	//----------------------------
	// ���[���h�}�g���b�N�X
	//----------------------------
	D3DXMATRIX scl, rot, translate;

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
	// �C���f�b�N�X�o�b�t�@���o�C���h
	m_device->SetIndices(m_indexBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	m_device->SetTexture(0, m_tex);

	// �C���f�b�N�X�o�b�t�@���g�p�����|���S���̕`��
	m_device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	// �`����
									0,					// 
									0,
									m_numVtx,			// ���_����
									0,					// �J�n�C���f�b�N�X
									m_numPolygon);		// �`�悷��v���~�e�B�u���i���|���S�����j
}

//=============================================================================
// ���_���ݒ�
//=============================================================================
void CMeshCylinder::SetVertexPolygon(void)
{
	VERTEX_3D* vtx;
	int			workVtx;
	float		workDistanceY;
	float		workAngleX = D3DX_PI * 2.0f / m_numBlock.x;
	D3DXVECTOR3	texcord = D3DXVECTOR3(1.0f / (float)m_numBlock.x,
									  1.0f / (float)m_numBlock.y,
									  0.0f);

	// �J�n
	m_vtxBuff->Lock(0, 0, (void **)&vtx, 0);

	// �t�B�[���h�S�̂̍ŉ��܂ł̋���
	workDistanceY = m_sizeBlock.y * m_numBlock.y;

	// �s�̏���
	for(float cntY = 0; cntY <= m_numBlock.y; ++cntY)
	{
		// ��̏���
		for(float cntX = 0; cntX <= m_numBlock.x; ++cntX)
		{
			// ���_�ԍ�
			workVtx = (int)((cntY * (m_numBlock.x + 1)) + cntX);

			// ���_���W
			vtx[workVtx].vtx.x =  cos(workAngleX * cntX) * 490.0f;
			vtx[workVtx].vtx.z = -sin(workAngleX * cntX) * 490.0f;
			vtx[workVtx].vtx.y = workDistanceY;

			// ���_�x�N�g��
			vtx[workVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���ˌ�
			vtx[workVtx].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W
			vtx[workVtx].tex = D3DXVECTOR2(texcord.x * (float)cntX, texcord.y * (float)cntY);
		}

		// Z���̋��������̗�ɐi�߂�
		workDistanceY -= m_sizeBlock.y;
	}

	// �I��
	m_vtxBuff->Unlock();
}

//=============================================================================
// ���_�C���f�b�N�X�ݒ�
//=============================================================================
void CMeshCylinder::SetVertexIndexPolygon(void)
{
	WORD*	index;

	// �J�n
	m_indexBuff->Lock(0, 0, (void**)&index, 0);

	// �C���f�b�N�X�ݒ�
	for(int cnt = 0, cnt2 = 0; cnt < m_numVtxIndex; ++cnt, ++cnt2)
	{
		// 0����Ȃ� ���� �d�����Ă���
		if( cnt != 0
		&& (((int)(cnt - NUMVERTEX(m_numBlock.x, 1)) % (int)(NUMVERTEX(m_numBlock.x, 1) + 2)) == 0
		 || ((int)(cnt - (NUMVERTEX(m_numBlock.x, 1) + 2)) % (int)(NUMVERTEX(m_numBlock.x, 1) + 2)) == 0))
		{
			// �d���̏���
			index[cnt] = index[cnt - 1];
			cnt2--;
		}
		else
		{
			// �d�����Ȃ�����
			index[cnt] = (WORD)(((m_numBlock.x + 1) + (cnt2 / 2)) - ((cnt2 % 2) * (m_numBlock.x + 1)));
		}
	}

	// �I��
	m_indexBuff->Unlock();	// ���̃f�[�^�������悤�ɂ���
}