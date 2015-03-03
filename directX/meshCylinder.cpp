//*****************************************************************************
//
// CMeshCylinderクラス [meshCylinder.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "meshCylinder.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshCylinder::CMeshCylinder(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	// メンバ初期化
	m_tex		= NULL;
	m_vtxBuff	= NULL;
	m_indexBuff	= NULL;

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//=============================================================================
// 生成
//=============================================================================
CMeshCylinder* CMeshCylinder::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock)
{
	CMeshCylinder* mesh= new CMeshCylinder;

	mesh->Init(device, texture, sizeBlock, numBlock);

	return mesh;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMeshCylinder::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock)
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

	//----------------------------
	// フィールド設定
	//----------------------------
	// 総頂点数
	m_numVtx = (int)NUMVERTEX(numBlock.x, numBlock.y);

	// 頂点の総インデックス数
	m_numVtxIndex = (int)NUMINDEX(numBlock.x, numBlock.y);

	// 総ポリゴン数
	m_numPolygon = m_numVtxIndex - 2;

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
void CMeshCylinder::Uninit(void)
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
void CMeshCylinder::Update(void)
{
}

//=============================================================================
// 描画
//=============================================================================
void CMeshCylinder::Draw(void)
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
// 頂点情報設定
//=============================================================================
void CMeshCylinder::SetVertexPolygon(void)
{
	VERTEX_3D* vtx;
	int			workVtx;
	float		workDistanceY;
	float		workAngleX = D3DX_PI * 2.0f / m_numBlock.x;
	D3DXVECTOR3	texcord = D3DXVECTOR3(1.0f / (float)m_numBlock.x,
									  1.0f / (float)m_numBlock.y,
									  0.0f);

	// 開始
	m_vtxBuff->Lock(0, 0, (void **)&vtx, 0);

	// フィールド全体の最奥までの距離
	workDistanceY = m_sizeBlock.y * m_numBlock.y;

	// 行の処理
	for(float cntY = 0; cntY <= m_numBlock.y; ++cntY)
	{
		// 列の処理
		for(float cntX = 0; cntX <= m_numBlock.x; ++cntX)
		{
			// 頂点番号
			workVtx = (int)((cntY * (m_numBlock.x + 1)) + cntX);

			// 頂点座標
			vtx[workVtx].vtx.x =  cos(workAngleX * cntX) * 490.0f;
			vtx[workVtx].vtx.z = -sin(workAngleX * cntX) * 490.0f;
			vtx[workVtx].vtx.y = workDistanceY;

			// 頂点ベクトル
			vtx[workVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 反射光
			vtx[workVtx].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			vtx[workVtx].tex = D3DXVECTOR2(texcord.x * (float)cntX, texcord.y * (float)cntY);
		}

		// Z軸の距離を次の列に進める
		workDistanceY -= m_sizeBlock.y;
	}

	// 終了
	m_vtxBuff->Unlock();
}

//=============================================================================
// 頂点インデックス設定
//=============================================================================
void CMeshCylinder::SetVertexIndexPolygon(void)
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