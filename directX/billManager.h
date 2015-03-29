//*****************************************************************************
//
// CBillManagerクラス [import.h]
// Author :KOUTAROU NISHIDA
//
//*****************************************************************************

#ifndef _BILL_MANAGER_H
#define _BILL_MANAGER_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 構造体定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define BILL_MAX (4)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CBill;
class CBillManager
{
public:
	CBillManager();
	~CBillManager();
	HRESULT Init(LPDIRECT3DDEVICE9 device);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBillManager *Create(LPDIRECT3DDEVICE9 device);

	static D3DXVECTOR3 GetPos(int nindex){ return m_Pos[nindex]; }
	static D3DXVECTOR3 SetPos(int nindex){ return m_Pos[nindex]; }
	static D3DXVECTOR3 GetRot(int nindex){ return m_Rot[nindex]; }
	static D3DXVECTOR3 SetRot(int nindex){ return m_Rot[nindex]; }

	CBill *GetBill(int index){ return m_pBill[index]; }

	int GetNumBill(void){return m_nNumBill;}

private:
	static const D3DXVECTOR3 m_Pos[];
	static const D3DXVECTOR3 m_Rot[];
	int		m_nNumBill;

	CBill *m_pBill[BILL_MAX];

};

//=============================================================================
#endif