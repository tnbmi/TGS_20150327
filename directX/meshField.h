//*****************************************************************************
//
// CMeshField�N���X [meshField.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_MESH_FILED_H
#define _MY_MESH_FILED_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "scene.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �N���X��`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CMeshField : public CScene
{
public:
	CMeshField(int priority = 0, OBJTYPE objType = OBJTYPE_FIELD);
	~CMeshField(){};

	static CMeshField* Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, float* heightVtx, MESHTEX_TYPE texType);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, float* heightVtx, MESHTEX_TYPE texType);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	void		SetPos(D3DXVECTOR3 pos){m_pos = pos; SetVertexPolygon(m_texType);}
	void		SetPos(float x, float y, float z){m_pos = D3DXVECTOR3(x, y, z); SetVertexPolygon(m_texType);}
	D3DXVECTOR3	GetPos(void){return m_pos;}
	void		SetRot(D3DXVECTOR3 rot){m_rot = rot; SetVertexPolygon(m_texType);}
	void		SetRot(float x, float y, float z){m_rot = D3DXVECTOR3(x, y, z); SetVertexPolygon(m_texType);}
	D3DXVECTOR3	GetRot(void){return m_rot;}
	void		SetSize(D3DXVECTOR3 size){m_size = size; SetVertexPolygon(m_texType);}
	void		SetSize(float x, float y, float z){m_size = D3DXVECTOR3(x, y, z); SetVertexPolygon(m_texType);}
	D3DXVECTOR3	GetSize(void){return m_size;}
	void		SetColor(D3DXCOLOR color){m_color = color; SetVertexPolygon(m_texType);}
	void		SetColor(float r, float g, float b, float a){m_color = D3DXCOLOR(r, g, b, a); SetVertexPolygon(m_texType);}
	D3DXCOLOR	GetColor(void){return m_color;}

	float GetHeight(const D3DXVECTOR3 pos, D3DXVECTOR3* normal);

private:
	void	SetVertexPolygon(MESHTEX_TYPE texType);
	void	SetVertexIndexPolygon(void);
	float	GetHeightPolygon(const D3DXVECTOR3 P0, const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DXVECTOR3 pos, D3DXVECTOR3* normal);
	bool	ChkInPolygon(const D3DXVECTOR3 P0, const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DXVECTOR3 pos);

	LPDIRECT3DTEXTURE9		m_tex;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_indexBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX	 m_world;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3* m_vtx;		// �e���_���W
	int m_numVtx;		// �����_��
	int m_numVtxIndex;	// ���_�̑��C���f�b�N�X��
	int m_numPolygon;	// ���|���S����

	D3DXVECTOR3  m_sizeBlock;	// �u���b�N�T�C�Y
	D3DXVECTOR3  m_numBlock;	// �u���b�N��
	float*		 m_heightVtx;	// ���_�̍���
	D3DXVECTOR3* m_norBuff;		// �ʖ@���o�b�t�@
	MESHTEX_TYPE m_texType;		// �e�N�X�`���^�C�v(�\���)
};

//=============================================================================
#endif
