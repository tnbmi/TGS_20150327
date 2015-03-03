//*****************************************************************************
//
// CMeshField�N���X [meshField.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "meshField.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshField::CMeshField(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_tex		= NULL;
	m_vtxBuff	= NULL;
	m_indexBuff	= NULL;

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_sizeBlock = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_numBlock  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_heightVtx  = NULL;
	m_norBuff   = NULL;
}

//=============================================================================
// ����
//=============================================================================
CMeshField* CMeshField::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, float* heightVtx, MESHTEX_TYPE texType)
{
	CMeshField* mesh= new CMeshField;

	mesh->Init(device, texture, sizeBlock, numBlock, heightVtx, texType);

	return mesh;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CMeshField::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, float* heightVtx, MESHTEX_TYPE texType)
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
	m_sizeBlock.z = sizeBlock.z;

	// �u���b�N��
	m_numBlock.x = numBlock.x;
	m_numBlock.z = numBlock.z;

	//----------------------------
	// �t�B�[���h�ݒ�
	//----------------------------
	// �e�N�X�`���^�C�v
	m_texType = texType;

	// �����_��
	m_numVtx = (int)NUMVERTEX(numBlock.x, numBlock.z);

	// ���_�̑��C���f�b�N�X��
	m_numVtxIndex = (int)NUMINDEX(numBlock.x, numBlock.z);

	// ���|���S����
	m_numPolygon = m_numVtxIndex - 2;

	// �e���_���W
	m_vtx = new D3DXVECTOR3[m_numVtx];
	for(int cnt = 0; cnt < m_numVtx; ++cnt)
	{
		m_vtx[cnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// ���_�����}�b�v
	m_heightVtx = new float[m_numVtx];
	for(int cnt = 0; cnt < m_numVtx; ++cnt)
	{
		m_heightVtx[cnt] = heightVtx[cnt];
	}

	// �@���o�b�t�@
	m_norBuff = new D3DXVECTOR3[m_numPolygon];
	for(int cnt = 0; cnt < m_numPolygon; ++cnt)
	{
		m_norBuff[cnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

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
	SetVertexPolygon(texType);

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
void CMeshField::Uninit(void)
{
	// ���X�j��
	SAFE_DELETE(m_vtx);
	SAFE_DELETE(m_heightVtx);
	SAFE_DELETE(m_norBuff);

	// �e�N�X�`���̊J��
	SAFE_RELEASE(m_tex);

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
void CMeshField::Update(void)
{

}

//=============================================================================
// �`��
//=============================================================================
void CMeshField::Draw(void)
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
									m_numVtx,		// ���_����
									0,					// �J�n�C���f�b�N�X
									m_numPolygon);		// �`�悷��v���~�e�B�u���i���|���S�����j
}

//=============================================================================
// ���_���ݒ�
//=============================================================================
void CMeshField::SetVertexPolygon(MESHTEX_TYPE texType)
{
	VERTEX_3D* vtx;
	int			workNoVtx[4];
	float		workDistanceX;
	float		workDistanceZ;
	D3DXVECTOR3	leftTop;
	int			lineBlockX = (int)(m_numBlock.x + 1);
	int			lineBlockZ = (int)(m_numBlock.z + 1);

	//----------------------------
	// ���W����
	//----------------------------
	// ������W
	leftTop = D3DXVECTOR3((m_sizeBlock.x * m_numBlock.x) / 2.0f,
							0.0f,
							(m_sizeBlock.z * m_numBlock.z) / 2.0f);

	// �t�B�[���h�S�̂̍ŉ��܂ł̋���
	workDistanceZ = leftTop.z;

	// �J�n
	m_vtxBuff->Lock(0, 0, (void **)&vtx, 0);

	//----------------------------
	// ��{���_���ݒ�
	//----------------------------
	// �s�̏���
	if(texType == MESHTEX_FULL)
	{
		D3DXVECTOR3	texcord = D3DXVECTOR3(1.0f / (float)lineBlockX,
										  1.0f / (float)lineBlockZ,
										  0.0f);

		for(int cntZ = 0; cntZ <= m_numBlock.z; ++cntZ)
		{
			// �t�B�[���h�S�̂̍��[�܂ł̋���
			workDistanceX = leftTop.x;

			// ��̏���
			for(int cntX = 0; cntX <= m_numBlock.x; ++cntX)
			{
				workNoVtx[0] = cntZ * lineBlockX + cntX;

				// ���_���W
				m_vtx[workNoVtx[0]] = vtx[workNoVtx[0]].vtx = D3DXVECTOR3(-workDistanceX, m_heightVtx[workNoVtx[0]], workDistanceZ);

				// ���_�x�N�g��
				vtx[workNoVtx[0]].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���ˌ�
				vtx[workNoVtx[0]].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
				// �e�N�X�`�����W
				vtx[workNoVtx[0]].tex = D3DXVECTOR2(texcord.x * (float)cntX, texcord.y * (float)cntZ);

				// X���̋��������̍s�ɐi�߂�
				workDistanceX -= m_sizeBlock.x;
			}

			// Z���̋��������̗�ɐi�߂�
			workDistanceZ -= m_sizeBlock.z;
		}
	}
	else if(texType == MESHTEX_PATCH)
	{
		for(int cntZ = 0; cntZ <= m_numBlock.z; ++cntZ)
		{
			// �t�B�[���h�S�̂̍��[�܂ł̋���
			workDistanceX = leftTop.x;

			// ��̏���
			for(int cntX = 0; cntX <= m_numBlock.x; ++cntX)
			{
				workNoVtx[0] = cntZ * lineBlockX + cntX;

				// ���_���W
				m_vtx[workNoVtx[0]] = vtx[workNoVtx[0]].vtx = D3DXVECTOR3(-workDistanceX, m_heightVtx[workNoVtx[0]], workDistanceZ);

				// ���_�x�N�g��
				vtx[workNoVtx[0]].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// ���ˌ�
				vtx[workNoVtx[0]].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
				// �e�N�X�`�����W
				vtx[workNoVtx[0]].tex = D3DXVECTOR2((float)cntX, (float)cntZ);

				// X���̋��������̍s�ɐi�߂�
				workDistanceX -= m_sizeBlock.x;
			}

			// Z���̋��������̗�ɐi�߂�
			workDistanceZ -= m_sizeBlock.z;
		}
	}

	//----------------------------
	// �ʖ@���x�N�g���v�Z
	//----------------------------
	D3DXVECTOR3 poly1Vec1, poly1Vec2;
	D3DXVECTOR3 poly2Vec1, poly2Vec2;
	D3DXVECTOR3 poly1Nor, poly2Nor;
	D3DXVECTOR3 centerNor, workNor;

	// �s�̏���
	for(int cntZ = 0, cntNor = 0; cntZ < m_numBlock.z; ++cntZ)
	{
		// ��̏���
		for(int cntX = 0; cntX < m_numBlock.x; ++cntX)
		{
			// �l���_�ԍ�
			workNoVtx[0] = cntZ * lineBlockX + cntX;
			workNoVtx[1] = workNoVtx[0] + 1;
			workNoVtx[2] = workNoVtx[0] + lineBlockX;
			workNoVtx[3] = workNoVtx[2] + 1;

			// ���_���Ƃ̃x�N�g��
			poly1Vec1 = vtx[workNoVtx[0]].vtx - vtx[workNoVtx[2]].vtx;
			poly1Vec2 = vtx[workNoVtx[3]].vtx - vtx[workNoVtx[2]].vtx;
			poly2Vec1 = vtx[workNoVtx[3]].vtx - vtx[workNoVtx[1]].vtx;
			poly2Vec2 = vtx[workNoVtx[0]].vtx - vtx[workNoVtx[1]].vtx;

			// �ʖ@����2�ʂ̍����@��
			D3DXVec3Cross(&poly1Nor, &poly1Vec1, &poly1Vec2);
			D3DXVec3Normalize(&poly1Nor, &poly1Nor);
			m_norBuff[cntNor] = poly1Nor;

			D3DXVec3Cross(&poly2Nor, &poly2Vec1, &poly2Vec2);
			D3DXVec3Normalize(&poly2Nor, &poly2Nor);
			m_norBuff[cntNor + 1] = poly2Nor;

			centerNor = poly1Nor + poly2Nor;
			D3DXVec3Normalize(&centerNor, &centerNor);

			// ����@��
			workNor = vtx[workNoVtx[0]].nor + centerNor;
			D3DXVec3Normalize(&workNor, &workNor);
			vtx[workNoVtx[0]].nor = workNor;

			// �E��@��
			workNor = vtx[workNoVtx[1]].nor + m_norBuff[cntNor + 1];
			D3DXVec3Normalize(&workNor, &workNor);
			vtx[workNoVtx[1]].nor = workNor;

			// �����@��
			vtx[workNoVtx[2]].nor = m_norBuff[cntNor];

			// �E���@��
			vtx[workNoVtx[3]].nor = centerNor;

			cntNor += 2;
		}
	}

	// �I��
	m_vtxBuff->Unlock();
}

//=============================================================================
// ���_�C���f�b�N�X�ݒ�
//=============================================================================
void CMeshField::SetVertexIndexPolygon(void)
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

//=============================================================================
// �I�u�W�F�N�g�̍���
//=============================================================================
float CMeshField::GetHeight(const D3DXVECTOR3 pos, D3DXVECTOR3* normal)
{
	D3DXVECTOR3 leftTop;
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 workVtx[4];
	int			workNoVtx[4];
	int			blockX, blockZ;
	int			lineBlockX = (int)(m_numBlock.x + 1);
	int			lineBlockZ = (int)(m_numBlock.z + 1);
	float		height = 0.0f;

	// �������W
	leftTop = D3DXVECTOR3((m_sizeBlock.x * m_numBlock.x) / 2.0f,
							0.0f,
							(m_sizeBlock.z * m_numBlock.z) / 2.0f);

	// ����Ă���u���b�N�v�Z
	blockX = (int)((leftTop.x + pos.x) / m_sizeBlock.x);
	blockZ = (int)((leftTop.z - pos.z) / m_sizeBlock.z);

	if(blockX >= 0 && blockX < m_numBlock.x
	&& blockZ >= 0 && blockZ < m_numBlock.z)
	{
		// �l���_�ԍ�
		workNoVtx[0] = blockZ * lineBlockX + blockX;
		workNoVtx[1] = workNoVtx[0] + 1;
		workNoVtx[2] = workNoVtx[0] + lineBlockX;
		workNoVtx[3] = workNoVtx[2] + 1;

		// �l���_���W
		workVtx[0] = m_vtx[workNoVtx[0]];
		workVtx[1] = m_vtx[workNoVtx[1]];
		workVtx[2] = m_vtx[workNoVtx[2]];
		workVtx[3] = m_vtx[workNoVtx[3]];

		if(ChkInPolygon(workVtx[2], workVtx[0], workVtx[3], pos))
		{
			height = GetHeightPolygon(workVtx[2], workVtx[0], workVtx[3], pos, normal);
		}
		else if(ChkInPolygon(workVtx[1], workVtx[3], workVtx[0], pos))
		{
			height = GetHeightPolygon(workVtx[1], workVtx[3], workVtx[0], pos, normal);
		}
	}

	return height;
}

//=============================================================================
// �ʏ�ł̍���
//=============================================================================
float CMeshField::GetHeightPolygon(const D3DXVECTOR3 P0, const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DXVECTOR3 pos, D3DXVECTOR3* normal)
{
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 workNor;

	// �ʖ@���v�Z
	vec0 = P1 - P0;
	vec1 = P2 - P0;

	D3DXVec3Cross(&workNor, &vec0, &vec1);
	D3DXVec3Normalize(&workNor, &workNor);

	// �ʂ̊O
	if(workNor.y == 0.0f)
	{
		return 0.0f;
	}

	return P0.y - (workNor.x * (pos.x - P0.x) + workNor.z * (pos.z - P0.z)) / workNor.y;
}

//=============================================================================
// �ʓ��ɂ��邩
//=============================================================================
bool CMeshField::ChkInPolygon(const D3DXVECTOR3 P0, const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vec0, vec1;
	bool		poly[3];

	poly[0] = false;
	poly[1] = false;
	poly[2] = false;

	vec0 = P1 - P0;
	vec1 = pos - P0;

	if(vec0.z * vec1.x - vec0.x * vec1.z >= 0)
	{
		poly[0] = true;
	}

	vec0 = P2 - P1;
	vec1 = pos - P1;

	if(vec0.z * vec1.x - vec0.x * vec1.z >= 0)
	{
		poly[1] = true;
	}

	vec0 = P0 - P2;
	vec1 = pos - P2;

	if(vec0.z * vec1.x - vec0.x * vec1.z >= 0)
	{
		poly[2] = true;
	}

	return poly[0] && poly[1] && poly[2];
}