//*****************************************************************************
//
// ���C������ [main.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �C���N���[�h�t�@�C��
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "manager.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �}�N����`
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// �E�B���h�E�ݒ�
//----------------------------
#define CLASS_NAME		"WindowClass"		// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"WindowCaption"		// �E�C���h�E�̃L���v�V������

//----------------------------
// ����}�N��
//----------------------------
#define CHK_INIT(init)	if(FAILED(init)){return -1;}	// �������`�F�b�N(WinMain���̂�)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �v���g�^�C�v�錾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	// �������[���[�N�`�F�b�N
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(prevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(cmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	DWORD execLastTime;	// ��������
	DWORD FPSLastTime;	// FPS���莞��
	DWORD curTime;		// ���ݎ���
	DWORD frameCnt;		// �t���[���J�E���g

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,						// �E�C���h�E�̃��C�A�E�g
		WndProc,						// �v���V�[�W���֐�
		0,
		0,
		instance,						// �C���X�^���X�n���h��
		NULL,							// �A�C�R���w��
		LoadCursor(NULL, IDC_ARROW),	// �}�E�X�J�[�\���w��
		(HBRUSH)(COLOR_WINDOW + 0),		// �w�i
		MAKEINTRESOURCE(NULL),			// ���j���[�̕\��
		CLASS_NAME,						// �E�C���h�E�N���X�̖��O
		NULL							// �������A�C�R���w��
	};
	MSG msg;	// ���b�Z�[�W
	
	//----------------------------
	// �E�B���h�E�N���X�̓o�^
	//----------------------------
	RegisterClassEx(&wcex);
	
	//----------------------------
	// �E�B���h�E�̍쐬
	//----------------------------
	HWND wnd;	// �E�C���h�E�n���h��
	int width  = SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2;
	int height = SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

	wnd = CreateWindowEx(0,
						CLASS_NAME,				// �N���X�̖��O
						WINDOW_NAME,			// �L���v�V������
						WS_OVERLAPPEDWINDOW,	// �X�^�C��
						CW_USEDEFAULT,			// X���W
						CW_USEDEFAULT,			// Y���W
						width,					// ����
						height,					// �c��
						NULL,
						NULL,
						instance,				// �C���X�^���X�n���h��
						NULL);

	//----------------------------
	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	//----------------------------
	ShowWindow(wnd, cmdShow);
	UpdateWindow(wnd);

	//----------------------------
	// ����������
	//----------------------------
	// �}�l�[�W��
	CManager* manager = CManager::Create(instance, wnd, true);

	// �t���[���J�E���g
	timeBeginPeriod(1);				// ����\��ݒ�
	execLastTime = timeGetTime();
	FPSLastTime	 = execLastTime;	// �V�X�e���������~���b�P�ʂŎ擾
	curTime	 = 0;					// ���ݎ����ƃJ�E���g�̏�����
	frameCnt	 = curTime;

	//----------------------------
	// ���b�Z�[�W���[�v
	//----------------------------
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
		{
			//----------------------------
			// Windows�̏���
			//----------------------------
			if(msg.message == WM_QUIT)
			{
				// ���[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);	// �|��
				DispatchMessage(&msg);	// ���M
			}
		}
		else
		{
			curTime = timeGetTime();	// �V�X�e���������擾

			if((curTime - FPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				// FPS�𑪒�
				manager->CalculateFPS(frameCnt, curTime, FPSLastTime);
#endif
				FPSLastTime = curTime;	// FPS�𑪒肵��������ۑ�
				frameCnt = 0;			// �J�E���g���N���A
			}

			if((curTime - execLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				execLastTime = curTime;	// ��������������ۑ�
				
				//----------------------------
				// DirectX�̏���
				//----------------------------
				// �X�V����
				manager->Update();

				// �`�揈��
				manager->Draw();

				// �J�E���g�����Z
				frameCnt++;
			}
		}
	}
	
	//----------------------------
	// �I������
	//----------------------------
	// �t���[���J�E���g
	timeEndPeriod(1);	// ����\��߂�
	
	// �}�l�[�W��
	SAFE_END(manager);

	//----------------------------
	// �E�B���h�E�o�^�̉���
	//----------------------------
	UnregisterClass( CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	//----------------------------
	// �E�C���h�E�𐶐�
	//----------------------------
	case WM_CREATE:
	{
		break;
	}

	//----------------------------
	// �E�C���h�E�j��
	//----------------------------
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//----------------------------
	// �L�[�{�[�h����
	//----------------------------
	case WM_KEYDOWN:
		switch(wParam)
		{
		// ESC�������ꂽ�Ƃ�
		case VK_ESCAPE:
			DestroyWindow(wnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(wnd, msg, wParam, lParam);
}