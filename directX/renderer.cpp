//*****************************************************************************
//
// CRenderer�N���X [renderer.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "renderer.h"
#include "debugproc.h"
#include "listObject.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRenderer::CRenderer(void)
{

}

//=============================================================================
// ����
//=============================================================================
CRenderer* CRenderer::Create(HWND wnd, bool window)
{
	CRenderer* pointer = new CRenderer;
	pointer->Init(wnd, window);
	return pointer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CRenderer::Init(HWND wnd, bool window)
{
	LPDIRECT3D9			  obj;	// Direct3D�I�u�W�F�N�g
	D3DDISPLAYMODE		  dm;	// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS pp;	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^

	//----------------------------
	// Direct3D�I�u�W�F�N�g�̍쐬
	//----------------------------
	obj = Direct3DCreate9(D3D_SDK_VERSION);
	HRESULT_POINTER(obj);

	//----------------------------
	// ���݂̃f�B�X�v���C���[�h���擾
	//----------------------------
	HRESULT_FUNC(obj->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm));

	//----------------------------
	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	//----------------------------
	ZeroMemory(&pp, sizeof(pp));						// ���[�N���[���N���A
	pp.BackBufferCount		  = 1;						// �o�b�N�o�b�t�@�̐�
	pp.BackBufferWidth		  = SCREEN_WIDTH;			// �Q�[����ʃT�C�Y(��)
	pp.BackBufferHeight		  = SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	pp.BackBufferFormat		  = dm.Format;				// �o�b�N�o�b�t�@�t�H�[�}�b�g�̓f�B�X�v���C���[�h�ɍ��킹�Ďg��
	pp.SwapEffect			  = D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	pp.Windowed				  = window;					// �E�B���h�E���[�h
	pp.EnableAutoDepthStencil = TRUE;					// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	pp.AutoDepthStencilFormat = D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��(���i����D24S8)

	//----------------------------
	// �E�B���h�E����
	//----------------------------
	if(window)
	{// �E�B���h�E���[�h
		pp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		pp.PresentationInterval		 = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{// �t���X�N���[�����[�h
		pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		pp.PresentationInterval		 = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	//----------------------------
	// �f�o�C�X�I�u�W�F�N�g����
	//----------------------------
	// [�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs�Ȃ�
	if(FAILED(obj->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									wnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&pp,
									&m_device)))
	{
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs�Ȃ�
		if(FAILED(obj->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										wnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&pp,
										&m_device)))
		{
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs�Ȃ�
			HRESULT_FUNC(obj->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											wnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&pp,
											&m_device))
		}
	}

	//----------------------------
	// Direct3D�I�u�W�F�N�g�J��
	//----------------------------
	SAFE_RELEASE(obj);
	
	//----------------------------
	// �f�o�C�X�ݒ�
	//----------------------------
	// �����_�[�X�e�[�g�p�����[�^
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�p�����[�^
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�
	
	// �e�N�X�`���X�e�[�W�X�e�[�g
	m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �A���t�@�u�����f�B���O������ݒ�
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �ŏ��̃A���t�@����
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �Q�Ԗڂ̃A���t�@����

	//----------------------------
	// FPS������
	//----------------------------
	m_countFPS = 0;

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CRenderer::Uninit(void)
{
	// �f�o�C�X�I�u�W�F�N�g�̊J��
	SAFE_RELEASE(m_device);
}

//=============================================================================
// �X�V
//=============================================================================
void CRenderer::Update(void)
{
	// �I�u�W�F�N�g�X�V
	CListObject::Update();
}

//=============================================================================
// �`��
//=============================================================================
void CRenderer::Draw(void)
{
	if(SUCCEEDED(m_device->BeginScene()))	//�J�n
	{
		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		m_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		// �I�u�W�F�N�g
		CListObject::Draw();

#ifdef _DEBUG
		m_debugproc->Draw();
#endif

		m_device->EndScene();	// �I��
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_device->Present(NULL, NULL, NULL, NULL);
}