//*****************************************************************************
//
// CListObjectクラス [listObject.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_LIST_OBJECT_H
#define _MY_LIST_OBJECT_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "scene.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CListObject
{
public:
	CListObject();
	~CListObject(){};

	static void Update(void);
	static void Draw(void);

	static void LinkObj(CScene* obj, int priority);
	static void UnlinkObj(CScene* obj);

	static void LinkUpdate(CScene* obj);
	static void UnlinkUpdate(CScene* obj);

	static void LinkDraw(CScene* obj, int priority);
	static void UnlinkDraw(CScene* obj);

private:
	static	CScene* m_updateTop;	// 更新リストの先頭ポインタ
	static	CScene* m_updateCur;	// 更新リストの現在終端ポインタ

	static	CScene* m_drawTop[PRIORITY_MAX];	// 描画リストの先頭ポインタ
	static	CScene* m_drawCur[PRIORITY_MAX];	// 描画リストの現在終端ポインタ
};

//=============================================================================
#endif