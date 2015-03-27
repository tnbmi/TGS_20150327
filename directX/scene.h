//*****************************************************************************
//
// CSceneクラス [scene.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_SCENE_H
#define _MY_SCENE_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "import.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define PRIORITY_MAX	(5)

// メッシュ用
#define NUMVERTEX(x, z)		((x + 1) * (z + 1))					// 総頂点計算
#define NUMINDEX(x, z)		((x + 1) * (z * 2) + ((z - 1) * 2))	// 総インデックス計算
#define WORKLINE(x, z, idx)	((idx / (NUMINDEX(x, z) / z)) + 1)	// 一行のインデックス計算

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_2D,
		OBJTYPE_3D,
		OBJTYPE_X,
		OBJTYPE_BILLBOARD,
		OBJTYPE_WALL,
		OBJTYPE_FIELD,
		OBJTYPE_DOME,
		OBJTYPE_CYLINDER,
		OBJTYPE_FADE,
		OBJTYPE_PLAYER,
	} OBJTYPE;

	typedef enum
	{
		MESHTEX_FULL,
		MESHTEX_PATCH
	} MESHTEX_TYPE;

	CScene(int priority = PRIORITY_MAX - 1, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene(){};

	virtual HRESULT	Init(void){return S_OK;}
	virtual void	Uninit(void) = 0;
	virtual void	Update(void) = 0;
	virtual void	Draw(void)	 = 0;

	static void ReleaseAll(void);

	void	UnLinkScene(void);
	void	Delete(void){m_delete = true;}

	virtual void		SetPos(D3DXVECTOR3 pos) = 0;
	virtual void		SetPos(float x, float y, float z) = 0;
	virtual D3DXVECTOR3	GetPos(void) = 0;
	virtual void		SetRot(D3DXVECTOR3 rot) = 0;
	virtual void		SetRot(float x, float y, float z) = 0;
	virtual D3DXVECTOR3	GetRot(void) = 0;
	virtual void		SetSize(D3DXVECTOR3 size) = 0;
	virtual void		SetSize(float x, float y, float z) = 0;
	virtual D3DXVECTOR3	GetSize(void) = 0;
	virtual void		SetColor(D3DXCOLOR color) = 0;
	virtual void		SetColor(float r, float g, float b, float a) = 0;
	virtual D3DXCOLOR	GetColor(void) = 0;

	int		GetPriority(void){return m_priority;}
	OBJTYPE	GetType(void){return m_objType;}
	bool	GetDelete(void){return m_delete;}

	static CScene* GetTop(int priority){return m_top[priority];}
	static CScene* GetCur(int priority){return m_cur[priority];}

	CScene* GetPrev(void){return m_prev;}
	CScene* GetNext(void){return m_next;}

	void	SetUpdatePrev(CScene* obj){m_updatePrev = obj;}
	CScene* GetUpdatePrev(void){return m_updatePrev;}
	void	SetUpdateNext(CScene* obj){m_updateNext = obj;}
	CScene* GetUpdateNext(void){return m_updateNext;}

	void	SetDrawPrev(CScene* obj){m_drawPrev = obj;}
	CScene* GetDrawPrev(void){return m_drawPrev;}
	void	SetDrawNext(CScene* obj){m_drawNext = obj;}
	CScene* GetDrawNext(void){return m_drawNext;}

	virtual D3DXVECTOR3 GetVtxMin(void){return D3DXVECTOR3(0.0f, 0.0f, 0.0f);}
	virtual D3DXVECTOR3 GetVtxMax(void){return D3DXVECTOR3(0.0f, 0.0f, 0.0f);};

protected:
	LPDIRECT3DDEVICE9 m_device;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_scl;
	D3DXVECTOR3 m_size;
	D3DXCOLOR	m_color;

	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_rotOld;

	D3DXVECTOR3 m_posDest;
	D3DXVECTOR3 m_rotDest;

private:
	int		m_priority;
	OBJTYPE m_objType;
	bool	m_delete;

	static	CScene* m_top[PRIORITY_MAX];	// リストの先頭ポインタ
	static	CScene* m_cur[PRIORITY_MAX];	// リストの現在終端ポインタ

	CScene* m_prev;			// 前のオブジェクトへのポインタ
	CScene* m_next;			// 次のオブジェクトへのポインタ
	CScene* m_updatePrev;	// 前の更新オブジェクトへのポインタ
	CScene* m_updateNext;	// 次の更新オブジェクトへのポインタ
	CScene* m_drawPrev;		// 前の描画オブジェクトへのポインタ
	CScene* m_drawNext;		// 次の描画オブジェクトへのポインタ
};

//=============================================================================
#endif