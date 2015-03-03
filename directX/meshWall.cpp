//*****************************************************************************
//
// CMeshWallクラス [meshWall.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "meshWall.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshWall::CMeshWall(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_tex		= NULL;
	m_vtxBuff	= NULL;
	m_indexBuff	= NULL;

	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_nor	= D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_vecLine = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}

//=============================================================================
// 生成
//=============================================================================
CMeshWall* CMeshWall::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, CScene::MESHTEX_TYPE texType)
{
	CMeshWall* mesh= new CMeshWall;

	mesh->Init(device, texture, sizeBlock, numBlock, texType);

	return mesh;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMeshWall::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, CScene::MESHTEX_TYPE texType)
{
	//----------------------------
	// デバイス取得
	//----------------------------
	m_device = device;

	//----------------------------
	// ブロック設定
	//----------------------------
	// ブロックサイズ
	m_sizeBlock.x = sizeBlock.x;
	m_sizeBlock.y = sizeBlock.y;

	// ブロック数
	m_numBlock.x = numBlock.x;
	m_numBlock.y = numBlock.y;

	// テクスチャ
	m_texType = texType;

	//----------------------------
	// フィールド設定
	//----------------------------
	// 総頂点数
	m_numVtx = (int)NUMVERTEX(numBlock.x, numBlock.y);

	// 頂点の総インデックス数
	m_numVtxIndex = (int)NUMINDEX(numBlock.x, numBlock.y);

	// 総ポリゴン数
	m_numPolygon = m_numVtxIndex - 2;

	// 左上座標
	m_wallLeft = D3DXVECTOR3(-(m_sizeBlock.x * m_numBlock.x) / 2.0f,
							m_sizeBlock.y * m_numBlock.y,
							0.0f);

	// 右上座標
	m_wallRight = D3DXVECTOR3((m_sizeBlock.x * m_numBlock.x) / 2.0f,
								m_sizeBlock.y * m_numBlock.y,
								0.0f);

	//----------------------------
	// ポリゴン初期化
	//----------------------------
	// メモリ確保
	HRESULT_FUNC(m_device->CreateVertexBuffer(sizeof(VERTEX_3D) * m_numVtx,	// 確保するバッファサイズ
											D3DUSAGE_WRITEONLY,
											FVF_VERTEX_3D,				// 頂点フォーマット
											D3DPOOL_MANAGED,
											&m_vtxBuff,					// 頂点情報
											NULL))

	HRESULT_FUNC(m_device->CreateIndexBuffer(sizeof(WORD) * m_numVtxIndex,	// 確保するバッファサイズ
											D3DUSAGE_WRITEONLY,
											D3DFMT_INDEX16,				// 頂点フォーマット
											D3DPOOL_MANAGED,
											&m_indexBuff,				// 頂点情報
											NULL))

	// 頂点情報設定
	SetVertexPolygon();

	// 頂点インデックス設定
	SetVertexIndexPolygon();

	// テクスチャの読込み
	m_tex = CImport::GetTexture(texture);

	//----------------------------
	// 初期化成功
	//----------------------------
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CMeshWall::Uninit(void)
{
	// テクスチャの開放
	SAFE_RELEASE(m_tex);

	// 頂点バッファの開放
	SAFE_RELEASE(m_vtxBuff);

	// インデックスバッファの開放
	SAFE_RELEASE(m_indexBuff);

	// 開放
	CScene::Delete();
}

//=============================================================================
// 更新
//=============================================================================
void CMeshWall::Update(void)
{

}

//=============================================================================
// 描画
//=============================================================================
void CMeshWall::Draw(void)
{
	//----------------------------
	// ワールドマトリックス
	//----------------------------
	D3DXMATRIX scl, rot, translate;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_world);
	// スケールの反映
	D3DXMatrixScaling(&scl, m_scl.x, m_scl.y, m_scl.z);
	D3DXMatrixMultiply(&m_world, &m_world, &scl);
	// 回転の反映
	D3DXMatrixRotationYawPitchRoll(&rot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_world, &m_world, &rot);
	// 位置の反映
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_world, &m_world, &translate);
	// ワールドマトリックスの設定
	m_device->SetTransform(D3DTS_WORLD, &m_world);

	//----------------------------
	// ポリゴンの描画
	//----------------------------
	// 頂点バッファをデータストリームにバインド
	m_device->SetStreamSource( 0, m_vtxBuff, 0, sizeof(VERTEX_3D) );
	// インデックスバッファをバインド
	m_device->SetIndices(m_indexBuff);

	// 頂点フォーマットの設定
	m_device->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	m_device->SetTexture(0, m_tex);

	// インデックスバッファを使用したポリゴンの描画
	m_device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	// 描画種類
									0,					// 
									0,
									m_numVtx,			// 頂点総数
									0,					// 開始インデックス
									m_numPolygon);		// 描画するプリミティブ数（総ポリゴン数）
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CMeshWall::Hitchk(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3* crossPoint)
{
	D3DXVECTOR3 vecMove			 = pos - posOld;
	D3DXVECTOR3 vecLine			 = m_wallRight - m_wallLeft;
	D3DXVECTOR3 leftToTarget	 = pos - m_wallLeft;
	D3DXVECTOR3 leftToTargetOld	 = posOld - m_wallLeft;
	D3DXVECTOR3 targetOldToLeft	 = m_wallLeft - posOld;
	D3DXVECTOR3 targetOldToRight = m_wallRight - posOld;
	D3DXVECTOR3 rightToTarget	 = pos - m_wallRight;

	//----------------------------
	// 外積結果
	//----------------------------
	float cross[5];

	// 壁左上始点ベクトル
	cross[0] = vecLine.x * leftToTargetOld.z - vecLine.z * leftToTargetOld.x;
	cross[1] = vecLine.x * leftToTarget.z	 - vecLine.z * leftToTarget.x;

	// 前座標始点ベクトル
	cross[2] = vecMove.x * targetOldToLeft.z  - vecMove.z * targetOldToLeft.x;
	cross[3] = vecMove.x * targetOldToRight.z - vecMove.z * targetOldToRight.x;

	// 壁右上始点ベクトル
	cross[4] = (-vecMove.x) * targetOldToRight.z - (-vecMove.z) * targetOldToRight.x;

	//----------------------------
	// 交点取得
	//----------------------------
	// 移動ラインが壁を跨いでいる && 壁が移動ラインを跨いでいる
	if(cross[0] * cross[1] <= 0.0f && cross[2] * cross[3] <= 0.0f)
	{
		// 交点前・交点後の比率
		float rate[3];
		rate[0] = abs(cross[2]);
		rate[1] = abs(cross[4]);
		if((rate[0] + rate[1]) != 0.0f)
		{
			rate[2] = rate[0] / (rate[0] + rate[1]);

			// 交点の座標
			crossPoint->x = m_wallLeft.x + vecLine.x * rate[2];
			crossPoint->z = m_wallLeft.z + vecLine.z * rate[2];
		}

		return true;
	}

	return false;
}

//=============================================================================
// 壁の回転
//=============================================================================
void CMeshWall::RotWall(D3DXVECTOR3 oldRot, D3DXVECTOR3 nextRot)
{
	float		angle = nextRot.y - oldRot.y;
	D3DXMATRIX	workMtx;
	D3DXVECTOR3	workLeft = m_wallLeft;
	D3DXVECTOR3	workRight = m_wallRight;
	D3DXVECTOR3	workVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 向きの更新
	m_rot = nextRot;

	// ベクトルの回転
	D3DXMatrixRotationAxis(&workMtx, &workVec, angle);
	D3DXVec3TransformCoord(&m_vecLine, &m_vecLine, &workMtx);
	D3DXVec3Normalize(&m_vecLine, &m_vecLine);

	D3DXVec3TransformCoord(&m_nor, &m_nor, &workMtx);
	D3DXVec3Normalize(&m_nor, &m_nor);

	// 左上座標の変更
	m_wallLeft.x = cosf(m_rot.y) * workLeft.x;
	m_wallLeft.z = sinf(m_rot.y) * workLeft.x;

	// 右上座標の変更
	m_wallRight.x = cosf(m_rot.y) * workRight.x;
	m_wallRight.z = sinf(m_rot.y) * workRight.x;
}

//=============================================================================
// 左座標の変更
//=============================================================================
void CMeshWall::SetLeft(void)
{
	// 左上座標の変更
	m_wallLeft.x += m_pos.x;
	m_wallLeft.z += m_pos.z;

	// 右上座標の変更
	m_wallRight.x += m_pos.x;
	m_wallRight.z += m_pos.z;
}

//=============================================================================
// 頂点情報設定
//=============================================================================
void CMeshWall::SetVertexPolygon(void)
{
	VERTEX_3D* vtx;
	int			workVtx;
	float		workDistanceX;
	float		workDistanceY;

	// 開始
	m_vtxBuff->Lock(0, 0, (void **)&vtx, 0);

	// フィールド全体の最奥までの距離
	workDistanceY = m_wallLeft.y;

	//----------------------------
	// 基本頂点情報設定
	//----------------------------
	// 行の処理
	if(m_texType == CScene::MESHTEX_FULL)
	{
		for(float cntY = 0; cntY <= m_numBlock.y; ++cntY)
		{
			D3DXVECTOR3	texcord = D3DXVECTOR3(1.0f / m_numBlock.x,
											  1.0f / m_numBlock.y,
											  0.0f);

			// フィールド全体の左端までの距離
			workDistanceX = m_wallLeft.x;

			// 列の処理
			for(float cntX = 0; cntX <= m_numBlock.x; ++cntX)
			{
				// 頂点番号
				workVtx = (int)((cntY * (m_numBlock.x + 1)) + cntX);

				// 頂点座標
				vtx[workVtx].vtx = D3DXVECTOR3(workDistanceX, workDistanceY, 0.0f);

				// 頂点ベクトル
				vtx[workVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// 反射光
				vtx[workVtx].diffuse = m_color;
	
				// テクスチャ座標
				vtx[workVtx].tex	= D3DXVECTOR2(texcord.x * cntX, texcord.y * cntY);

				// X軸の距離を次の行に進める
				workDistanceX += m_sizeBlock.x;
			}

			// Z軸の距離を次の列に進める
			workDistanceY -= m_sizeBlock.y;
		}
	}
	else if(m_texType == CScene::MESHTEX_PATCH)
	{
		for(float cntY = 0; cntY <= m_numBlock.y; ++cntY)
		{
			// フィールド全体の左端までの距離
			workDistanceX = m_wallLeft.x;

			// 列の処理
			for(float cntX = 0; cntX <= m_numBlock.x; ++cntX)
			{
				// 頂点番号
				workVtx = (int)((cntY * (m_numBlock.x + 1)) + cntX);

				// 頂点座標
				vtx[workVtx].vtx = D3DXVECTOR3(workDistanceX, workDistanceY, 0.0f);

				// 頂点ベクトル
				vtx[workVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// 反射光
				vtx[workVtx].diffuse = m_color;
	
				// テクスチャ座標
				vtx[workVtx].tex	= D3DXVECTOR2(cntX, cntY);
			
				// X軸の距離を次の行に進める
				workDistanceX += m_sizeBlock.x;
			}

			// Z軸の距離を次の列に進める
			workDistanceY -= m_sizeBlock.y;
		}
	}

	// 終了
	m_vtxBuff->Unlock();
}

//=============================================================================
// 頂点インデックス設定
//=============================================================================
void CMeshWall::SetVertexIndexPolygon(void)
{
	WORD*	index;

	// 開始
	m_indexBuff->Lock(0, 0, (void**)&index, 0);

	// インデックス設定
	for(int cnt = 0, cnt2 = 0; cnt < m_numVtxIndex; ++cnt, ++cnt2)
	{
		// 0じゃない かつ 重複している
		if( cnt != 0
		&& (((int)(cnt - NUMVERTEX(m_numBlock.x, 1)) % (int)(NUMVERTEX(m_numBlock.x, 1) + 2)) == 0
		 || ((int)(cnt - (NUMVERTEX(m_numBlock.x, 1) + 2)) % (int)(NUMVERTEX(m_numBlock.x, 1) + 2)) == 0))
		{
			// 重複の処理
			index[cnt] = index[cnt - 1];
			cnt2--;
		}
		else
		{
			// 重複しない処理
			index[cnt] = (WORD)(((m_numBlock.x + 1) + (cnt2 / 2)) - ((cnt2 % 2) * (m_numBlock.x + 1)));
		}
	}

	// 終了
	m_indexBuff->Unlock();	// 他のデータが入れるようにする
}