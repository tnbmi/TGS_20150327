//*****************************************************************************
//
// CMeshWall�N���X [meshWall.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "meshWall.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshWall::CMeshWall(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_tex		= NULL;
	m_vtxBuff	= NULL;
	m_indexBuff	= NULL;

	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_nor	= D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_vecLine = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

//=============================================================================
// ����
//=============================================================================
CMeshWall* CMeshWall::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, CScene::MESHTEX_TYPE texType)
{
	CMeshWall* mesh= new CMeshWall;

	mesh->Init(device, texture, sizeBlock, numBlock, texType);

	return mesh;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CMeshWall::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, CScene::MESHTEX_TYPE texType)
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

	// �e�N�X�`��
	m_texType = texType;

	//----------------------------
	// �t�B�[���h�ݒ�
	//----------------------------
	// �����_��
	m_numVtx = (int)NUMVERTEX(numBlock.x, numBlock.y);

	// ���_�̑��C���f�b�N�X��
	m_numVtxIndex = (int)NUMINDEX(numBlock.x, numBlock.y);

	// ���|���S����
	m_numPolygon = m_numVtxIndex - 2;

	// ������W
	m_wallLeft = D3DXVECTOR3(-(m_sizeBlock.x * m_numBlock.x) / 2.0f,
							m_sizeBlock.y * m_numBlock.y,
							0.0f);

	// �E����W
	m_wallRight = D3DXVECTOR3((m_sizeBlock.x * m_numBlock.x) / 2.0f,
								m_sizeBlock.y * m_numBlock.y,
								0.0f);

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
void CMeshWall::Uninit(void)
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
void CMeshWall::Update(void)
{

}

//=============================================================================
// �`��
//=============================================================================
void CMeshWall::Draw(void)
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
// �����蔻��
//=============================================================================
bool CMeshWall::Hitchk(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3* crossPoint)
{
	D3DXVECTOR3 vecMove			 = pos - posOld;
	D3DXVECTOR3 vecLine			 = m_wallRight - m_wallLeft;
	D3DXVECTOR3 leftToTarget	 = pos - m_wallLeft;
	D3DXVECTOR3 leftToTargetOld	 = posOld - m_wallLeft;
	D3DXVECTOR3 targetOldToLeft	 = m_wallLeft - posOld;
	D3DXVECTOR3 targetOldToRight = m_wallRight - posOld;
	D3DXVECTOR3 rightToTarget	 = pos - m_wallRight;

	//----------------------------
	// �O�ό���
	//----------------------------
	float cross[5];

	// �Ǎ���n�_�x�N�g��
	cross[0] = vecLine.x * leftToTargetOld.z - vecLine.z * leftToTargetOld.x;
	cross[1] = vecLine.x * leftToTarget.z	 - vecLine.z * leftToTarget.x;

	// �O���W�n�_�x�N�g��
	cross[2] = vecMove.x * targetOldToLeft.z  - vecMove.z * targetOldToLeft.x;
	cross[3] = vecMove.x * targetOldToRight.z - vecMove.z * targetOldToRight.x;

	// �ǉE��n�_�x�N�g��
	cross[4] = (-vecMove.x) * targetOldToRight.z - (-vecMove.z) * targetOldToRight.x;

	//----------------------------
	// ��_�擾
	//----------------------------
	// �ړ����C�����ǂ��ׂ��ł��� && �ǂ��ړ����C�����ׂ��ł���
	if(cross[0] * cross[1] <= 0.0f && cross[2] * cross[3] <= 0.0f)
	{
		// ��_�O�E��_��̔䗦
		float rate[3];
		rate[0] = abs(cross[2]);
		rate[1] = abs(cross[4]);
		if((rate[0] + rate[1]) != 0.0f)
		{
			rate[2] = rate[0] / (rate[0] + rate[1]);

			// ��_�̍��W
			crossPoint->x = m_wallLeft.x + vecLine.x * rate[2];
			crossPoint->z = m_wallLeft.z + vecLine.z * rate[2];
		}

		return true;
	}

	return false;
}

//=============================================================================
// �ǂ̉�]
//=============================================================================
void CMeshWall::RotWall(D3DXVECTOR3 oldRot, D3DXVECTOR3 nextRot)
{
	float		angle = nextRot.y - oldRot.y;
	D3DXMATRIX	workMtx;
	D3DXVECTOR3	workLeft = m_wallLeft;
	D3DXVECTOR3	workRight = m_wallRight;
	D3DXVECTOR3	workVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �����̍X�V
	m_rot = nextRot;

	// �x�N�g���̉�]
	D3DXMatrixRotationAxis(&workMtx, &workVec, angle);
	D3DXVec3TransformCoord(&m_vecLine, &m_vecLine, &workMtx);
	D3DXVec3Normalize(&m_vecLine, &m_vecLine);

	D3DXVec3TransformCoord(&m_nor, &m_nor, &workMtx);
	D3DXVec3Normalize(&m_nor, &m_nor);

	// ������W�̕ύX
	m_wallLeft.x = cosf(m_rot.y) * workLeft.x;
	m_wallLeft.z = sinf(m_rot.y) * workLeft.x;

	// �E����W�̕ύX
	m_wallRight.x = cosf(m_rot.y) * workRight.x;
	m_wallRight.z = sinf(m_rot.y) * workRight.x;
}

//=============================================================================
// �����W�̕ύX
//=============================================================================
void CMeshWall::SetLeft(void)
{
	// ������W�̕ύX
	m_wallLeft.x += m_pos.x;
	m_wallLeft.z += m_pos.z;

	// �E����W�̕ύX
	m_wallRight.x += m_pos.x;
	m_wallRight.z += m_pos.z;
}

//=============================================================================
// ���_���ݒ�
//=============================================================================
void CMeshWall::SetVertexPolygon(void)
{
	VERTEX_3D* vtx;
	int			workVtx;
	float		workDistanceX;
	float		workDistanceY;

	// �J�n
	m_vtxBuff->Lock(0, 0, (void **)&vtx, 0);

	// �t�B�[���h�S�̂̍ŉ��܂ł̋���
	workDistanceY = m_wallLeft.y;

	//----------------------------
	// ��{���_���ݒ�
	//----------------------------
	// �s�̏���
	if(m_texType == CScene::MESHTEX_FULL)
	{
		for(float cntY = 0; cntY <= m_numBlock.y; ++cntY)
		{
			D3DXVECTOR3	texcord = D3DXVECTOR3(1.0f / m_numBlock.x,
											  1.0f / m_numBlock.y,
											  0.0f);

			// �t�B�[���h�S�̂̍��[�܂ł̋���
			workDistanceX = m_wallLeft.x;

			// ��̏���
			for(float cntX = 0; cntX <= m_numBlock.x; ++cntX)
			{
				// ���_�ԍ�
				workVtx = (int)((cntY * (m_numBlock.x + 1)) + cntX);

				// ���_���W
				vtx[workVtx].vtx = D3DXVECTOR3(workDistanceX, workDistanceY, 0.0f);

				// ���_�x�N�g��
				vtx[workVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// ���ˌ�
				vtx[workVtx].diffuse = m_color;
	
				// �e�N�X�`�����W
				vtx[workVtx].tex	= D3DXVECTOR2(texcord.x * cntX, texcord.y * cntY);

				// X���̋��������̍s�ɐi�߂�
				workDistanceX += m_sizeBlock.x;
			}

			// Z���̋��������̗�ɐi�߂�
			workDistanceY -= m_sizeBlock.y;
		}
	}
	else if(m_texType == CScene::MESHTEX_PATCH)
	{
		for(float cntY = 0; cntY <= m_numBlock.y; ++cntY)
		{
			// �t�B�[���h�S�̂̍��[�܂ł̋���
			workDistanceX = m_wallLeft.x;

			// ��̏���
			for(float cntX = 0; cntX <= m_numBlock.x; ++cntX)
			{
				// ���_�ԍ�
				workVtx = (int)((cntY * (m_numBlock.x + 1)) + cntX);

				// ���_���W
				vtx[workVtx].vtx = D3DXVECTOR3(workDistanceX, workDistanceY, 0.0f);

				// ���_�x�N�g��
				vtx[workVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// ���ˌ�
				vtx[workVtx].diffuse = m_color;
	
				// �e�N�X�`�����W
				vtx[workVtx].tex	= D3DXVECTOR2(cntX, cntY);
			
				// X���̋��������̍s�ɐi�߂�
				workDistanceX += m_sizeBlock.x;
			}

			// Z���̋��������̗�ɐi�߂�
			workDistanceY -= m_sizeBlock.y;
		}
	}

	// �I��
	m_vtxBuff->Unlock();
}

//=============================================================================
// ���_�C���f�b�N�X�ݒ�
//=============================================================================
void CMeshWall::SetVertexIndexPolygon(void)
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