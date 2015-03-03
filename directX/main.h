//*****************************************************************************
//
// ���C������ [main.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_MAIN_H
#define _MY_MAIN_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �������[���[�N�`�F�b�N����
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif
#include <stdlib.h>
#include <crtdbg.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800) // �x���΍��p
#include "dinput.h"
#include "XInput.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �������[���[�N�`�F�b�N
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define new DBG_NEW
#endif
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ���C�u�����̃����N
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "XInput.lib")

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// ��ʃT�C�Y
//----------------------------
#define SCREEN_WIDTH	(800)	// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(600)	// �E�C���h�E�̍���

//----------------------------
// ����}�N��
//----------------------------
#define HRESULT_FUNC(func)	if(FAILED(func)){return E_FAIL;}	// �֐���HRESULT���^�[��
#define HRESULT_POINTER(p)	if(p==NULL){return E_FAIL;}			// NULL��HRESULT���^�[��

#define SAFE_RELEASE(p)		 if(p!=NULL){p->Release(); p=NULL;}				// �J���ی�
#define SAFE_DELETE(p)		 if(p!=NULL){delete p; p=NULL;}					// �j���ی�
#define SAFE_DELETE_ARRAY(p) if(p!=NULL){delete[] p; p=NULL;}				// �z��j���ی�
#define SAFE_UNINIT(p)		 if(p!=NULL){p->Uninit(); p=NULL;}				// �I���ی�
#define SAFE_END(p)			 if(p!=NULL){p->Uninit(); delete p; p=NULL;}	// �ŏI�ی�

//----------------------------
// ���̑����ʃ}�N��
//----------------------------
#define DISTANCE_2D(x,y)	sqrt(x*x + y*y)			// 2���������v�Z
#define DISTANCE_3D(x,y,z)	sqrt(x*x + y*y + z*z)	// 3���������v�Z
#define SIGN_F(n)			((n) / fabs(n))			// �����擾(float)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �t�H�[�}�b�g
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ� / �e�N�X�`�����W )
//----------------------------
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//----------------------------
// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���ˌ� / �e�N�X�`�����W )
//----------------------------
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//----------------------------
// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
//----------------------------
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	float		rhw;		// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	D3DCOLOR	diffuse;	// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_2D;

//----------------------------
// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
//----------------------------
typedef struct
{
	D3DXVECTOR3 vtx;		// ���_���W
	D3DXVECTOR3 nor;		// �@���x�N�g��
	D3DCOLOR	diffuse;	// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
} VERTEX_3D;

//=============================================================================
#endif