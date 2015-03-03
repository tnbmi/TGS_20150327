//*****************************************************************************
//
// メイン処理 [main.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "manager.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// ウィンドウ設定
//----------------------------
#define CLASS_NAME		"WindowClass"		// ウインドウのクラス名
#define WINDOW_NAME		"WindowCaption"		// ウインドウのキャプション名

//----------------------------
// 分岐マクロ
//----------------------------
#define CHK_INIT(init)	if(FAILED(init)){return -1;}	// 初期化チェック(WinMain内のみ)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// プロトタイプ宣言
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	// メモリーリークチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(prevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(cmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	DWORD execLastTime;	// 処理時刻
	DWORD FPSLastTime;	// FPS測定時刻
	DWORD curTime;		// 現在時刻
	DWORD frameCnt;		// フレームカウント

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,						// ウインドウのレイアウト
		WndProc,						// プロシージャ関数
		0,
		0,
		instance,						// インスタンスハンドル
		NULL,							// アイコン指定
		LoadCursor(NULL, IDC_ARROW),	// マウスカーソル指定
		(HBRUSH)(COLOR_WINDOW + 0),		// 背景
		MAKEINTRESOURCE(NULL),			// メニューの表示
		CLASS_NAME,						// ウインドウクラスの名前
		NULL							// 小さいアイコン指定
	};
	MSG msg;	// メッセージ
	
	//----------------------------
	// ウィンドウクラスの登録
	//----------------------------
	RegisterClassEx(&wcex);
	
	//----------------------------
	// ウィンドウの作成
	//----------------------------
	HWND wnd;	// ウインドウハンドル
	int width  = SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2;
	int height = SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);

	wnd = CreateWindowEx(0,
						CLASS_NAME,				// クラスの名前
						WINDOW_NAME,			// キャプション名
						WS_OVERLAPPEDWINDOW,	// スタイル
						CW_USEDEFAULT,			// X座標
						CW_USEDEFAULT,			// Y座標
						width,					// 横幅
						height,					// 縦幅
						NULL,
						NULL,
						instance,				// インスタンスハンドル
						NULL);

	//----------------------------
	// 初期化処理
	//----------------------------
	// マネージャ
	CManager* manager = CManager::Create(instance, wnd, true);

	// フレームカウント
	timeBeginPeriod(1);				// 分解能を設定
	execLastTime = timeGetTime();
	FPSLastTime	 = execLastTime;	// システム時刻をミリ秒単位で取得
	curTime	 = 0;					// 現在時刻とカウントの初期化
	frameCnt	 = curTime;

	//----------------------------
	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	//----------------------------
	ShowWindow(wnd, cmdShow);
	UpdateWindow(wnd);
	
	//----------------------------
	// メッセージループ
	//----------------------------
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // メッセージを取得しなかった場合"0"を返す
		{
			//----------------------------
			// Windowsの処理
			//----------------------------
			if(msg.message == WM_QUIT)
			{
				// ループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);	// 翻訳
				DispatchMessage(&msg);	// 送信
			}
		}
		else
		{
			curTime = timeGetTime();	// システム時刻を取得

			if((curTime - FPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				// FPSを測定
				manager->CalculateFPS(frameCnt, curTime, FPSLastTime);
#endif
				FPSLastTime = curTime;	// FPSを測定した時刻を保存
				frameCnt = 0;			// カウントをクリア
			}

			if((curTime - execLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				execLastTime = curTime;	// 処理した時刻を保存
				
				//----------------------------
				// DirectXの処理
				//----------------------------
				// 更新処理
				manager->Update();

				// 描画処理
				manager->Draw();

				// カウントを加算
				frameCnt++;
			}
		}
	}
	
	//----------------------------
	// 終了処理
	//----------------------------
	// フレームカウント
	timeEndPeriod(1);	// 分解能を戻す
	
	// マネージャ
	SAFE_END(manager);

	//----------------------------
	// ウィンドウ登録の解除
	//----------------------------
	UnregisterClass( CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	//----------------------------
	// ウインドウを生成
	//----------------------------
	case WM_CREATE:
	{
		break;
	}

	//----------------------------
	// ウインドウ破棄
	//----------------------------
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//----------------------------
	// キーボード処理
	//----------------------------
	case WM_KEYDOWN:
		switch(wParam)
		{
		// ESCが押されたとき
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