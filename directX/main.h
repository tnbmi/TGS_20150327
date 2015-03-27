//*****************************************************************************
//
// メイン処理 [main.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_MAIN_H
#define _MY_MAIN_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// メモリーリークチェック準備
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif
#include <stdlib.h>
#include <crtdbg.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800) // 警告対策用
#include "dinput.h"
#include "XInput.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// メモリーリークチェック
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)
#define new DBG_NEW
#endif
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ライブラリのリンク
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "XInput.lib")

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// 画面サイズ
//----------------------------
#define SCREEN_WIDTH	(800)	// ウインドウの幅
#define SCREEN_HEIGHT	(600)	// ウインドウの高さ

//----------------------------
// 分岐マクロ
//----------------------------
#define HRESULT_FUNC(func)	if(FAILED(func)){return E_FAIL;}	// 関数のHRESULTリターン
#define HRESULT_POINTER(p)	if(p==NULL){return E_FAIL;}			// NULLのHRESULTリターン

#define SAFE_RELEASE(p)		 if(p!=NULL){p->Release(); p=NULL;}				// 開放保護
#define SAFE_DELETE(p)		 if(p!=NULL){delete p; p=NULL;}					// 破棄保護
#define SAFE_DELETE_ARRAY(p) if(p!=NULL){delete[] p; p=NULL;}				// 配列破棄保護
#define SAFE_UNINIT(p)		 if(p!=NULL){p->Uninit(); p=NULL;}				// 終了保護
#define SAFE_END(p)			 if(p!=NULL){p->Uninit(); delete p; p=NULL;}	// 最終保護

//----------------------------
// その他共通マクロ
//----------------------------
#define DISTANCE_2D(x,y)	sqrt(x*x + y*y)			// 2次元距離計算
#define DISTANCE_3D(x,y,z)	sqrt(x*x + y*y + z*z)	// 3次元距離計算
#define SIGN_F(n)			((n) / fabs(n))			// 符号取得(float)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// フォーマット
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//----------------------------
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
//----------------------------
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//----------------------------
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
//----------------------------
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//----------------------------
// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
//----------------------------
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float		rhw;		// テクスチャのパースペクティブコレクト用
	D3DCOLOR	diffuse;	// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

//----------------------------
// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
//----------------------------
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR	diffuse;	// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

//=============================================================================

#endif