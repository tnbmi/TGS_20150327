//*****************************************************************************
//
// CImport�N���X [import.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_IMPORT_H
#define _MY_IMPORT_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �\���̒�`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct XFILE
{
	XFILE()
	{
		buffMat	= nullptr;
		numMat	= 0;
		mesh	= nullptr;
		mat		= nullptr;
		tex		= nullptr;
	}

	LPD3DXBUFFER		buffMat;	// �}�e���A�����ւ̃|�C���^
	DWORD				numMat;		// �}�e���A�����̐�
	LPD3DXMESH			mesh;		// ���b�V�����ւ̃|�C���^
	D3DXMATERIAL*		mat;		// �}�e���A���f�[�^�擪�|�C���^
	LPDIRECT3DTEXTURE9*	tex;		// �e�N�X�`���f�[�^�擪�|�C���^
} XFILE;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++z++++++++++++++++++++++++++++++++++
class CImport
{
public:
	typedef enum
	{
		TEX_NONE = 0,
		TEX_MAX
	} TEXTURES;

	typedef enum
	{
		X_NONE = 0,
		X_MAX
	} XFILES;

	CImport();
	~CImport(){};

	static	CImport* Create(LPDIRECT3DDEVICE9 device);
	HRESULT	Init(LPDIRECT3DDEVICE9 device);
	void	Uninit(void);

	static LPDIRECT3DTEXTURE9 GetTexture(TEXTURES tex){return m_tex[tex];}
	static XFILE			  GetXFile(XFILES x){return m_xFile[x];}

private:
	static LPDIRECT3DTEXTURE9 m_tex[TEX_MAX];
	static XFILE			  m_xFile[X_MAX];
};

//=============================================================================
#endif