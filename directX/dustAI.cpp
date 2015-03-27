//*****************************************************************************
//
// CDustAIAIクラス [dustAI.cpp]
// Author : YUJI MOMOI
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "dustAI.h"
#include "dust.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// コンストラクタ
//=============================================================================
CDustAI::CDustAI()
{
}

//=============================================================================
// 生成
//=============================================================================
CDustAI* CDustAI::Create(CDust* pDust)
{
	CDustAI* pointer = new CDustAI;
	pointer->Init(pDust);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CDustAI::Init(CDust* pDust)
{
	SetDust(pDust);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CDustAI::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CDustAI::Update(void)
{
	// ユニットが何もしていない時
	if(TacticsState == TACTICS_STATE_NONE)
	{
		TacticsState = TACTICS_STATE_MOVE_START;
	}

	// ユニットの移動開始時に目的地設定
	if(TacticsState == TACTICS_STATE_MOVE_START)
	{
		ThinkPosDest();
	}

	// ユニットの移動時の処理
	if(TacticsState == TACTICS_STATE_MOVE)
	{
		SerchVision();
	}

	// 攻撃準備時の処理
	if(TacticsState == TACTICS_STATE_ATTACK_POSSIBLE)
	{
		AttackPossible();
	}

	// 攻撃処理
	if(TacticsState == TACTICS_STATE_ATTACK)
	{
		Attack();
	}

	// 攻撃後の処理
	if(TacticsState == TACTICS_STATE_ATTACK_END)
	{
		AttackEnd();
	}

	// 待機時の処理 思考間隔を挟む
	if(TacticsState == TACTICS_STATE_STAY)
	{
		m_nThinkCount ++;

		if(m_nThinkCount > THINK_COOL_TIME)
		{
			m_nThinkCount = 0;
			TacticsState = TACTICS_STATE_NONE;
		}
	}
}

//=============================================================================
// 目的地を決める
//=============================================================================
void CDustAI::ThinkPosDest(void)
{
	CScene* pScene = NULL;
	D3DXVECTOR3 front = m_dust->GetFrontVector();
	D3DXVECTOR3 right = m_dust->GetRightVector();
	D3DXVECTOR3 vecTarget;
	float vecTargetMin = 1000.0f;
	bool endFlag = false;

	for(int cnt = 0; cnt < PRIORITY_MAX; ++cnt)
	{
		pScene = CScene::GetTop(cnt);

		while(pScene != NULL)
		{
			if(pScene->GetType() == CScene::OBJTYPE_WALL)
			{
				// 壁判定
				vecTarget = pScene->GetPos() - m_dust->GetPos();
				float length = D3DXVec3Length(&vecTarget);

				// 前
				if(vecTarget == front)
				{
					// 正面が壁なら
					if(length < (m_dust->GetSize().z / 2))
					{
						// 左右を調べていける方に曲がる
						endFlag = true;
					}
				}

				// 右
				if(vecTarget == right && !endFlag)
				{
				}

				// 左
			}

			pScene = pScene -> GetNext();
		}
	}
/*
	for(int cnt = 0; cnt <PRIORITY_MAX; ++cnt)
	{
		pScene = CScene::GetTop(cnt);

		while(pScene != NULL)
		{
			if(pScene -> GetType() == CScene::OBJTYPE_WALL)
			{
				D3DXVECTOR3 min ;//= pScene->GetVertexMin();	// ギミックのローカル最小位置を取得
				D3DXVECTOR3 max ;//= pScene->GetVertexMax();	// ギミックのローカル最大位置を取得
				D3DXVECTOR3 pos = pScene->GetPos();			// ギミックの座標を取得

				D3DXVECTOR3 p[4];							// ギミックの外積判定用の配列を用意

				// ギミックの外積計算用座標を計算
				p[0].x = pos.x + min.x;
				p[0].z = pos.z + max.z;
				p[1].x = pos.x + max.x;
				p[1].z = pos.z + max.z;
				p[2].x = pos.x + max.x;
				p[2].z = pos.z + min.z;
				p[3].x = pos.x + min.x;
				p[3].z = pos.z + min.z;

				D3DXVECTOR3 offset = m_dust->GetPos() - pos;			// オフセット座標(プレイヤーとギミックの距離)
				D3DXVECTOR3 rot = pScene->GetRot();			// ギミックの角度を取得
				D3DXVECTOR3 target;							// オフセット座標での回転後座標格納用

				target.x = pos.x + cosf(-rot.y) * offset.x + sinf(-rot.y) * offset.z;	// オフセット座標での回転後座標X
				target.z = pos.z + cosf(-rot.y) * offset.z - sinf(-rot.y) * offset.x;	// オフセット座標での回転後座標Z

				D3DXVECTOR3 VecLine;
				D3DXVECTOR3 VecToTarget;
				bool bHit = false;

				// 外積での当たり判定
				for(int nloop = 0;nloop < 4 && !bHit;nloop++)
				{
					VecLine = p[(nloop + 1) % 4] - p[nloop];
					VecToTarget = target - p[nloop];

					if(VecLine.z * VecToTarget.x - VecLine.x * VecToTarget.z >= 0.0f)
					{
					}
					else
					{
						bHit = true;
						break;
					}
				}

				if(!bHit)
				{

				}
			}
		}
	}*/
}

//=============================================================================
// 視界内サーチ
//=============================================================================
bool CDustAI::SerchVision(void)
{
	return false;
}

//=============================================================================
// 攻撃準備
//=============================================================================
void CDustAI::AttackPossible(void)
{
}

//=============================================================================
// 攻撃
//=============================================================================
void CDustAI::Attack(void)
{
}

//=============================================================================
// 攻撃後
//=============================================================================
void CDustAI::AttackEnd(void)
{
}

//=============================================================================
// 
//=============================================================================

