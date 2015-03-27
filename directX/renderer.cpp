//*****************************************************************************
//
// CRendererクラス [renderer.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "renderer.h"
#include "debugproc.h"
#include "listObject.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CRenderer::CRenderer(void)
{

}

//=============================================================================
// 生成
//=============================================================================
CRenderer* CRenderer::Create(HWND wnd, bool window)
{
	CRenderer* pointer = new CRenderer;
	pointer->Init(wnd, window);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CRenderer::Init(HWND wnd, bool window)
{
	LPDIRECT3D9			  obj;	// Direct3Dオブジェクト
	D3DDISPLAYMODE		  dm;	// ディスプレイモード
	D3DPRESENT_PARAMETERS pp;	// デバイスのプレゼンテーションパラメータ

	//----------------------------
	// Direct3Dオブジェクトの作成
	//----------------------------
	obj = Direct3DCreate9(D3D_SDK_VERSION);
	HRESULT_POINTER(obj);

	//----------------------------
	// 現在のディスプレイモードを取得
	//----------------------------
	HRESULT_FUNC(obj->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm));

	//----------------------------
	// デバイスのプレゼンテーションパラメータの設定
	//----------------------------
	ZeroMemory(&pp, sizeof(pp));						// ワークをゼロクリア
	pp.BackBufferCount		  = 1;						// バックバッファの数
	pp.BackBufferWidth		  = SCREEN_WIDTH;			// ゲーム画面サイズ(幅)
	pp.BackBufferHeight		  = SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	pp.BackBufferFormat		  = dm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
	pp.SwapEffect			  = D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	pp.Windowed				  = window;					// ウィンドウモード
	pp.EnableAutoDepthStencil = TRUE;					// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	pp.AutoDepthStencilFormat = D3DFMT_D16;				// デプスバッファとして16bitを使う(高品質はD24S8)

	//----------------------------
	// ウィンドウ分岐
	//----------------------------
	if(window)
	{// ウィンドウモード
		pp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		pp.PresentationInterval		 = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		pp.PresentationInterval		 = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	//----------------------------
	// デバイスオブジェクト生成
	//----------------------------
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(obj->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									wnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&pp,
									&m_device)))
	{
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(obj->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										wnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&pp,
										&m_device)))
		{
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			HRESULT_FUNC(obj->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											wnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&pp,
											&m_device))
		}
	}

	//----------------------------
	// Direct3Dオブジェクト開放
	//----------------------------
	SAFE_RELEASE(obj);
	
	//----------------------------
	// デバイス設定
	//----------------------------
	// レンダーステートパラメータ
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータ
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定
	
	// テクスチャステージステート
	m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	//----------------------------
	// FPS初期化
	//----------------------------
	m_countFPS = 0;

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CRenderer::Uninit(void)
{
	// デバイスオブジェクトの開放
	SAFE_RELEASE(m_device);
}

//=============================================================================
// 更新
//=============================================================================
void CRenderer::Update(void)
{
	// オブジェクト更新
	CListObject::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CRenderer::Draw(void)
{
	if(SUCCEEDED(m_device->BeginScene()))	//開始
	{
		// バックバッファ＆Ｚバッファのクリア
		m_device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		// オブジェクト
		CListObject::Draw();

#ifdef _DEBUG
		m_debugproc->Draw();
#endif

		m_device->EndScene();	// 終了
	}

	// バックバッファとフロントバッファの入れ替え
	m_device->Present(NULL, NULL, NULL, NULL);
}