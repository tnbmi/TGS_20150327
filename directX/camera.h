//*****************************************************************************
//
// カメラ処理 [camera.h]
// Author :MAI TANABE
//
//*****************************************************************************

#ifndef _MY_CAMERA_H
#define _MY_CAMERA_H
//=============================================================================

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "main.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// クラス定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class CInputKeyboard;
class CPlayer;

class CCamera
{
public:
	CCamera();
	~CCamera(){};

	static	CCamera* Create(D3DXVECTOR3 at, D3DXVECTOR3 pos);
	HRESULT	Init(D3DXVECTOR3 at, D3DXVECTOR3 pos);
	void	Uninit(void);
	void	Update(void);
	void	SetCamera(LPDIRECT3DDEVICE9 device);

	D3DXMATRIX GetMtxView(void){return m_view;}

	void		SetPos(D3DXVECTOR3 pos){m_pos = pos; ResetRot();}
	D3DXVECTOR3	GetPos(void){return m_pos;}
	void		SetAt(D3DXVECTOR3 at){m_at = at;}
	D3DXVECTOR3	GetAt(void){return m_at;}
	D3DXVECTOR3	GetRot(void){return m_rot;}
	void		SetDistance(float distance){m_distance = distance;}
	void		SetDistance(void){m_distance = DISTANCE_3D(m_pos.x, m_pos.y, m_pos.z);}
	float		GetDistance(void){return m_distance;}

	void		SetPosDest(D3DXVECTOR3 posDest){m_posDest = posDest;}
	D3DXVECTOR3	GetPosDest(void){return m_posDest;}
	void		SetAtDest(D3DXVECTOR3 atDest){m_atDest = atDest;}
	D3DXVECTOR3	GetAtDest(void){return m_atDest;}
	D3DXVECTOR3	GetRotDest(void){return m_rotDest;}
	void		SetDistanceDest(float distanceDest){m_distanceDest = distanceDest;}
	float		GetDistanceDest(void){return m_distanceDest;}

private:
	void ResetPos(void);
	void ResetRot(void);
	void ResetPosDest(void);
	void ResetRotDest(void);
	void ResetDistance(void);

	D3DXMATRIX	 m_view;		// ビューマトリックス
	D3DXMATRIX	 m_projection;	// プロジェクションマトリックス
	D3DVIEWPORT9 m_viewport;	// ビューポート

	D3DXVECTOR3	m_pos;		// 視点
	D3DXVECTOR3	m_at;		// 注視点
	D3DXVECTOR3	m_rot;		// 向き(回転角)
	float		m_distance;	// 距離(視点から注視点まで)

	D3DXVECTOR3	m_posDest;		// 目的の視点
	D3DXVECTOR3	m_atDest;		// 目的の注視点
	D3DXVECTOR3	m_rotDest;		// 目的の向き(回転角)
	float		m_distanceDest;	// 目的の距離

	D3DXVECTOR3	m_vecU;	// 上方向ベクトル
	D3DXVECTOR3	m_vecF;	// 前方向ベクトル
	D3DXVECTOR3	m_vecR;	// 右方向ベクトル
};

//=============================================================================
#endif