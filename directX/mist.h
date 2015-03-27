//*****************************************************************************
//
// CMistクラス [mist.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_MIST_H
#define _MY_MIST_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "sceneBillboard.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CMist : public CSceneBillboard
{
public:
	CMist(int priority = PRIORITY_MAX - 3, OBJTYPE objType = OBJTYPE_MIST);
	~CMist(){};

	static	CMist* Create(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 vec);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR3 vec);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

private:
	float m_time;

	D3DXVECTOR3 m_vec;
};

//=============================================================================
#endif