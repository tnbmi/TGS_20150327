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
	"./data/TEXTURE/fade000.png",
	"./data/TEXTURE/concrete.jpg",
};

// X�t�@�C��
const char* X_PATH[] =
{
	NULL,
	"./data/MODEL/yogore_souzi.x",
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �ÓI�ϐ�
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LPDIRECT3DTEXTURE9	CImport::m_tex[TEX_MAX];
XFILE				CImport::m_xFile[X_MAX];

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
