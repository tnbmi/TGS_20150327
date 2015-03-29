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
#include "import.h"
#include "game.h"
#include "player.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//=============================================================================
// コンストラクタ
//=============================================================================
CDustAI::CDustAI()
{
	TacticsState = TACTICS_STATE_STAY;
	m_nThinkCount = 0;
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

	// 汚す処理
	if(TacticsState == TACTICS_STATE_DEFILE)
	{
		Defile();
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
	m_dust->SetPosDest(CGame::GetPlayer()->GetPos());
	TacticsState = TACTICS_STATE_MOVE;
	m_dust->SetMove(true);
}

//=============================================================================
// 攻撃開始
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
// 視界判定
//=============================================================================
bool CDustAI::SerchVision(void)
{
	CScene* pScene = NULL;

	for(int cnt = 0;cnt < PRIORITY_MAX; ++cnt)
	{
		pScene = CScene::GetTop(cnt);

		while(pScene != NULL)
		{
			if(pScene -> GetType() == CScene::OBJTYPE_BILL)
			{
				D3DXVECTOR3 min = CImport::GetVtxMin(CScene::OBJTYPE_BILL);	// ギミックのローカル最小位置を取得
				D3DXVECTOR3 max = CImport::GetVtxMax(CScene::OBJTYPE_BILL);	// ギミックのローカル最大位置を取得
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

				D3DXVECTOR3 offset = m_dust->GetPosDest() - pos;			// オフセット座標(プレイヤーとギミックの距離)
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
					SetGoal();
					break;
				}
			}

			pScene = pScene -> GetNext();
		}
	}

	return false;
}

//=============================================================================
// 汚す処理
//=============================================================================
void CDustAI::Defile(void)
{

}

//=============================================================================
// ゴールしたことを伝える
//=============================================================================
void CDustAI::SetGoal(void)
{
	TacticsState = TACTICS_STATE_STAY;
	m_dust->SetMove(false);
}