//*****************************************************************************
//
// CImport�N���X [import.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �e�N�X�`��
const char* TEX_PATH[] =
{
	NULL,
	"./data/TEXTURE/fade.jpg",
	"./data/TEXTURE/sky000.jpg",
	"./data/TEXTURE/concrete.jpg",
	"./data/TEXTURE/dirt.png",
	"./data/TEXTURE/mist.jpg",
	"./data/TEXTURE/title.jpg",
	"./data/TEXTURE/result.jpg",
};

// X�t�@�C��
const char* X_PATH[] =
{
	NULL,
	"./data/MODEL/yogore_souzi.x",
	"./data/MODEL/wall.x",
	"./data/MODEL/dust.x",
	"./data/MODEL/Bill.x",
	"./data/MODEL/Three_Bill.x",
	"./data/MODEL/L_Bill.x"
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LPDIRECT3DTEXTURE9	CImport::m_tex[TEX_MAX];
XFILE				CImport::m_xFile[X_MAX];

D3DXVECTOR3 CImport::m_VtxMin[];
D3DXVECTOR3 CImport::m_VtxMax[];

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CImport::CImport(void)
{
	// �e�N�X�`��
	for(int cnt = 0; cnt < TEX_MAX; ++cnt)
	{
		m_tex[cnt] = nullptr;
	}

	// X�t�@�C��
	for(int cnt = 0; cnt < TEX_MAX; ++cnt)
	{
		m_xFile[cnt] = XFILE();
	}

	// �T�C�Y
	for(int cnt = 0; cnt < TEX_MAX; ++cnt)
	{
		m_VtxMin[cnt] = D3DXVECTOR3(0.0f,0.0f,0.0f);
		m_VtxMax[cnt] = D3DXVECTOR3(0.0f,0.0f,0.0f);
	}
}

//=============================================================================
// ����
//=============================================================================
CImport* CImport::Create(LPDIRECT3DDEVICE9 device)
{
	CImport* pointer = new CImport;
	pointer->Init(device);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CImport::Init(LPDIRECT3DDEVICE9 device)
{
	//----------------------------
	// �e�N�X�`��
	//----------------------------
	for(int cnt = 1; cnt < TEX_MAX; ++cnt)
	{
		D3DXCreateTextureFromFile(device, TEX_PATH[cnt], &m_tex[cnt]);
	}

	//----------------------------
	// X�t�@�C��
	//----------------------------
	for(int cnt = 1; cnt < X_MAX; ++cnt)
	{
		// ���f���ǂݍ���
		if(FAILED(D3DXLoadMeshFromX(X_PATH[cnt],
									D3DXMESH_SYSTEMMEM,
									device,
									NULL,
									&m_xFile[cnt].buffMat,
									NULL,
									&m_xFile[cnt].numMat,
									&m_xFile[cnt].mesh)))
		{
			continue;
		}

		// �}�e���A���̐ݒ�
		m_xFile[cnt].mat = (D3DXMATERIAL *)m_xFile[cnt].buffMat->GetBufferPointer();	// �}�e���A���̐擪�A�h���X�擾
		m_xFile[cnt].tex = new LPDIRECT3DTEXTURE9[m_xFile[cnt].numMat];

		for(unsigned int cntMat = 0; cntMat < m_xFile[cnt].numMat; cntMat++)
		{
			//�e�N�X�`�������[�h
			m_xFile[cnt].tex[cntMat] = NULL;

			if(m_xFile[cnt].mat[cntMat].pTextureFilename != NULL)
			{
				HRESULT hr = D3DXCreateTextureFromFile(device, m_xFile[cnt].mat[cntMat].pTextureFilename, &m_xFile[cnt].tex[cntMat]);
				if(FAILED(hr))
				{
					m_xFile[cnt].tex[cntMat] = NULL;
				}
			}
		}

		// �T�C�Y�̎擾
		m_VtxMin[cnt] = GetVtxMin(cnt);
		m_VtxMax[cnt] = GetVtxMax(cnt);
	}

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CImport::Uninit(void)
{
	//----------------------------
	// X�t�@�C��
	//----------------------------
	for(int cnt = 1; cnt < X_MAX; cnt++)
	{
		// �e�N�X�`���|�C���^���
		SAFE_DELETE(m_xFile[cnt].tex);

		// ���b�V�����̊J��
		SAFE_RELEASE(m_xFile[cnt].mesh);

		// �}�e���A�����̊J��
		SAFE_RELEASE(m_xFile[cnt].buffMat);
	}

	//----------------------------
	// �e�N�X�`��
	//----------------------------
	for(int cnt = 1; cnt < TEX_MAX; cnt++)
	{
		// �e�N�X�`���̊J��
		SAFE_RELEASE(m_tex[cnt]);
	}
}


//=============================================================================
// ���_���W���擾(�ŏ��l)
// ����:��
// �ߒl:��
//=============================================================================
D3DXVECTOR3 CImport::GetVtxMin(int index)
{
	D3DXVECTOR3 pos[2];
	BYTE *pVertexBuffer;								// ���_�o�b�t�@
	WORD *pIndexBuffer;									// �C���f�b�N�X�o�b�t�@
	D3DXVECTOR3 Vertex;									// ���_
	DWORD NumFaces = m_xFile[index].mesh->GetNumFaces();		// �|���S�����̎擾
	DWORD NumVertex = m_xFile[index].mesh->GetNumVertices();	// ���_���̎擾
	DWORD FVF = m_xFile[index].mesh->GetFVF();					// ���_�t�H�[�}�b�g���擾
	DWORD VertexSize = D3DXGetFVFVertexSize(FVF);		// ���_�T�C�Y�̎擾
	
	// ���b�N�i���_�o�b�t�@�j
	// ���b�N�i�C���f�b�N�X�o�b�t�@�j
	m_xFile[index].mesh->LockVertexBuffer(D3DLOCK_READONLY,(LPVOID*)&pVertexBuffer);
	m_xFile[index].mesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&pIndexBuffer);
	
	// �|���S�����������[�v
	DWORD VerNum[3];				// ���_�ԍ�(�C���f�b�N�X)
	D3DXVECTOR3 VerPos[3];				// ���_���W
	D3DXVECTOR3 VerPos2[3];			// ���_���W
	D3DXVECTOR3 Ver[3];				// ���_���W

	// ���_�ԍ��擾(���Ԗڂ̒��_��)
	VerNum[0] = pIndexBuffer[0];	// ���_1
	VerNum[1] = pIndexBuffer[1];	// ���_2
	VerNum[2] = pIndexBuffer[2];	// ���_3
	
	// ���_���W�擾
	VerPos[0] = VerPos2[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
	VerPos[1] = VerPos2[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
	VerPos[2] = VerPos2[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

	Ver[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
	Ver[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
	Ver[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

	for( unsigned int i = 0; i < NumFaces; i++ )
	{
		// ���_�ԍ��擾(���Ԗڂ̒��_��)
		VerNum[0] = pIndexBuffer[i*3 + 0];	// ���_1
		VerNum[1] = pIndexBuffer[i*3 + 1];	// ���_2
		VerNum[2] = pIndexBuffer[i*3 + 2];	// ���_3
	
		// ���_���W�擾
		Ver[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
		Ver[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
		Ver[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);
		
		if( VerPos[0].x > Ver[0].x )
		{
			VerPos[0].x = Ver[0].x;
		}

		if( VerPos[1].x > Ver[1].x )
		{
			VerPos[1].x = Ver[1].x;
		}

		if( VerPos[2].x > Ver[2].x )
		{
			VerPos[2].x = Ver[2].x;
		}

		if( VerPos[0].y > Ver[0].y )
		{
			VerPos[0].y = Ver[0].y;
		}

		if( VerPos[1].y > Ver[1].y )
		{
			VerPos[1].y = Ver[1].y;
		}

		if( VerPos[2].y > Ver[2].y )
		{
			VerPos[2].y = Ver[2].y;
		}

		if( VerPos[0].z > Ver[0].z )
		{
			VerPos[0].z = Ver[0].z;
		}

		if( VerPos[1].z > Ver[1].z )
		{
			VerPos[1].z = Ver[1].z;
		}

		if( VerPos[2].z > Ver[2].z )
		{
			VerPos[2].z = Ver[2].z;
		}
	}

	pos[0] = VerPos[0];

	// �A�����b�N�i���_�o�b�t�@�j
	// �A�����b�N�i�C���f�b�N�X�o�b�t�@�j
	m_xFile[index].mesh->UnlockVertexBuffer();
	m_xFile[index].mesh->UnlockIndexBuffer();

	return pos[0];
}

//=============================================================================
// ���_���W���擾(�ő�l)
// ����:��
// �ߒl:��
//=============================================================================
D3DXVECTOR3 CImport::GetVtxMax(int index)
{
	D3DXVECTOR3 pos[2];
	BYTE *pVertexBuffer;								// ���_�o�b�t�@
	WORD *pIndexBuffer;									// �C���f�b�N�X�o�b�t�@
	D3DXVECTOR3 Vertex;									// ���_
	DWORD NumFaces = m_xFile[index].mesh->GetNumFaces();		// �|���S�����̎擾
	DWORD NumVertex = m_xFile[index].mesh->GetNumVertices();	// ���_���̎擾
	DWORD FVF = m_xFile[index].mesh->GetFVF();					// ���_�t�H�[�}�b�g���擾
	DWORD VertexSize = D3DXGetFVFVertexSize(FVF);		// ���_�T�C�Y�̎擾
	
	// ���b�N�i���_�o�b�t�@�j
	// ���b�N�i�C���f�b�N�X�o�b�t�@�j
	m_xFile[index].mesh->LockVertexBuffer(D3DLOCK_READONLY,(LPVOID*)&pVertexBuffer);
	m_xFile[index].mesh->LockIndexBuffer(D3DLOCK_READONLY, (LPVOID*)&pIndexBuffer);
	
	// �|���S�����������[�v
	DWORD VerNum[3];				// ���_�ԍ�(�C���f�b�N�X)
	D3DXVECTOR3 VerPos[3];				// ���_���W
	D3DXVECTOR3 VerPos2[3];			// ���_���W
	D3DXVECTOR3 Ver[3];				// ���_���W

	// ���_�ԍ��擾(���Ԗڂ̒��_��)
	VerNum[0] = pIndexBuffer[0];	// ���_1
	VerNum[1] = pIndexBuffer[1];	// ���_2
	VerNum[2] = pIndexBuffer[2];	// ���_3
	
	// ���_���W�擾
	VerPos[0] = VerPos2[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
	VerPos[1] = VerPos2[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
	VerPos[2] = VerPos2[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

	Ver[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
	Ver[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
	Ver[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

	for( unsigned int i = 0; i < NumFaces; i++ )
	{
		// ���_�ԍ��擾(���Ԗڂ̒��_��)
		VerNum[0] = pIndexBuffer[i*3 + 0];	// ���_1
		VerNum[1] = pIndexBuffer[i*3 + 1];	// ���_2
		VerNum[2] = pIndexBuffer[i*3 + 2];	// ���_3
	
		// ���_���W�擾
		Ver[0] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[0]*VertexSize);
		Ver[1] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[1]*VertexSize);
		Ver[2] = *(D3DXVECTOR3*)(pVertexBuffer + VerNum[2]*VertexSize);

		if( VerPos2[0].x < Ver[0].x )
		{
			VerPos2[0].x = Ver[0].x;
		}

		if( VerPos2[1].x < Ver[1].x )
		{
			VerPos2[1].x = Ver[1].x;
		}

		if( VerPos2[2].x < Ver[2].x )
		{
			VerPos2[2].x = Ver[2].x;
		}

		if( VerPos2[0].y < Ver[0].y )
		{
			VerPos2[0].y = Ver[0].y;
		}

		if( VerPos2[1].y < Ver[1].y )
		{
			VerPos2[1].y = Ver[1].y;
		}

		if( VerPos2[2].y < Ver[2].y )
		{
			VerPos2[2].y = Ver[2].y;
		}

		if( VerPos2[0].z < Ver[0].z )
		{
			VerPos2[0].z = Ver[0].z;
		}

		if( VerPos2[1].z < Ver[1].z )
		{
			VerPos2[1].z = Ver[1].z;
		}

		if( VerPos2[2].z < Ver[2].z )
		{
			VerPos2[2].z = Ver[2].z;
		}
	}

	pos[1] = VerPos2[0];

	// �A�����b�N�i���_�o�b�t�@�j
	// �A�����b�N�i�C���f�b�N�X�o�b�t�@�j
	m_xFile[index].mesh->UnlockVertexBuffer();
	m_xFile[index].mesh->UnlockIndexBuffer();

	return pos[1];
}