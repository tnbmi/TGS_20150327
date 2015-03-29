//*****************************************************************************
//
// CDustクラス [dust.h]
// Author : YUJI MOMOI
//
//*****************************************************************************

#ifndef _DUST_H
#define _DUST_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"
#include "sceneX.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define DUST_MOVE_SPEED (0.4f)
#define DUST_ROTATION_SPEED (0.4f)
#define DUST_HP (3)
#define DUST_INVINCIBLE_TIME (30)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 構造体定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 前方宣言
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CDustAI;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CDust : public CSceneX
{
public:

	CDust(int priority = PRIORITY_MAX - 1, OBJTYPE objType = OBJTYPE_ENEMY);
	~CDust(){};

	static	CDust* Create(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile);
	HRESULT	Init(LPDIRECT3DDEVICE9 device, CImport::XFILES xFile);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	D3DXVECTOR3 GetFrontVector(void){return m_FrontVector;}
	void SetFrontVector(D3DXVECTOR3 flont){m_FrontVector = flont;}
	D3DXVECTOR3 GetRightVector(void){return m_RightVector;}
	void SetRightVector(D3DXVECTOR3 right){m_RightVector = right;}

	void SetMove(bool bMove){m_move = bMove;}
	void SetDamege(void);
	int GetHP(void){return m_HP;}

private:

	D3DXVECTOR3 m_FrontVector;
	D3DXVECTOR3 m_RightVector;
	D3DXVECTOR3 m_RotPower;

	CDustAI*	m_AI;
	bool		m_move;
	int			m_HP;
	int			m_InvincibleTime;
	bool		m_damege;
};

//=============================================================================
#endif