//*****************************************************************************
//
// CMeshFieldクラス [meshField.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "meshField.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshField::CMeshField(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_tex		= NULL;
	m_vtxBuff	= NULL;
	m_indexBuff	= NULL;

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_sizeBlock = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_numBlock  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_heightVtx  = NULL;
	m_norBuff   = NULL;
}

//=============================================================================
// 生成
//=============================================================================
CMeshField* CMeshField::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, float* heightVtx, MESHTEX_TYPE texType)
{
	CMeshField* mesh= new CMeshField;

	mesh->Init(device, texture, sizeBlock, numBlock, heightVtx, texType);

	return mesh;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMeshField::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, D3DXVECTOR3 sizeBlock, D3DXVECTOR3 numBlock, float* heightVtx, MESHTEX_TYPE texType)
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
	m_sizeBlock.z = sizeBlock.z;

	// ブロック数
	m_numBlock.x = numBlock.x;
	m_numBlock.z = numBlock.z;

	//----------------------------
	// フィールド設定
	//----------------------------
	// テクスチャタイプ
	m_texType = texType;

	// 総頂点数
	m_numVtx = (int)NUMVERTEX(numBlock.x, numBlock.z);

	// 頂点の総インデックス数
	m_numVtxIndex = (int)NUMINDEX(numBlock.x, numBlock.z);

	// 総ポリゴン数
	m_numPolygon = m_numVtxIndex - 2;

	// 各頂点座標
	m_vtx = new D3DXVECTOR3[m_numVtx];
	for(int cnt = 0; cnt < m_numVtx; ++cnt)
	{
		m_vtx[cnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 頂点高さマップ
	m_heightVtx = new float[m_numVtx];

	if(heightVtx != nullptr)
	{
		for(int cnt = 0; cnt < m_numVtx; ++cnt)
		{
			m_heightVtx[cnt] = heightVtx[cnt];
		}
	}
	else
	{
		for(int cnt = 0; cnt < m_numVtx; ++cnt)
		{
			m_heightVtx[cnt] = 0.0f;
		}
	}

	// 法線バッファ
	m_norBuff = new D3DXVECTOR3[m_numPolygon];
	for(int cnt = 0; cnt < m_numPolygon; ++cnt)
	{
		m_norBuff[cnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

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
	SetVertexPolygon(texType);

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
void CMeshField::Uninit(void)
{
	// 諸々破棄
	SAFE_DELETE(m_vtx);
	SAFE_DELETE(m_heightVtx);
	SAFE_DELETE(m_norBuff);

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
void CMeshField::Update(void)
{

}

//=============================================================================
// 描画
//=============================================================================
void CMeshField::Draw(void)
{
	D3DXMATRIX scl, rot, translate;

	//----------------------------
	// ワールドマトリックス
	//----------------------------
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
									m_numVtx,		// 頂点総数
									0,					// 開始インデックス
									m_numPolygon);		// 描画するプリミティブ数（総ポリゴン数）
}

//=============================================================================
// 頂点情報設定
//=============================================================================
void CMeshField::SetVertexPolygon(MESHTEX_TYPE texType)
{
	VERTEX_3D* vtx;
	int			workNoVtx[4];
	float		workDistanceX;
	float		workDistanceZ;
	D3DXVECTOR3	leftTop;
	int			lineBlockX = (int)(m_numBlock.x + 1);
	int			lineBlockZ = (int)(m_numBlock.z + 1);

	//----------------------------
	// 座標準備
	//----------------------------
	// 左上座標
	leftTop = D3DXVECTOR3((m_sizeBlock.x * m_numBlock.x) / 2.0f,
							0.0f,
							(m_sizeBlock.z * m_numBlock.z) / 2.0f);

	// フィールド全体の最奥までの距離
	workDistanceZ = leftTop.z;

	// 開始
	m_vtxBuff->Lock(0, 0, (void **)&vtx, 0);

	//----------------------------
	// 基本頂点情報設定
	//----------------------------
	// 行の処理
	if(texType == MESHTEX_FULL)
	{
		D3DXVECTOR3	texcord = D3DXVECTOR3(1.0f / (float)lineBlockX,
										  1.0f / (float)lineBlockZ,
										  0.0f);

		for(int cntZ = 0; cntZ <= m_numBlock.z; ++cntZ)
		{
			// フィールド全体の左端までの距離
			workDistanceX = leftTop.x;

			// 列の処理
			for(int cntX = 0; cntX <= m_numBlock.x; ++cntX)
			{
				workNoVtx[0] = cntZ * lineBlockX + cntX;

				// 頂点座標
				m_vtx[workNoVtx[0]] = vtx[workNoVtx[0]].vtx = D3DXVECTOR3(-workDistanceX, m_heightVtx[workNoVtx[0]], workDistanceZ);

				// 頂点ベクトル
				vtx[workNoVtx[0]].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 反射光
				vtx[workNoVtx[0]].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
				// テクスチャ座標
				vtx[workNoVtx[0]].tex = D3DXVECTOR2(texcord.x * (float)cntX, texcord.y * (float)cntZ);

				// X軸の距離を次の行に進める
				workDistanceX -= m_sizeBlock.x;
			}

			// Z軸の距離を次の列に進める
			workDistanceZ -= m_sizeBlock.z;
		}
	}
	else if(texType == MESHTEX_PATCH)
	{
		for(int cntZ = 0; cntZ <= m_numBlock.z; ++cntZ)
		{
			// フィールド全体の左端までの距離
			workDistanceX = leftTop.x;

			// 列の処理
			for(int cntX = 0; cntX <= m_numBlock.x; ++cntX)
			{
				workNoVtx[0] = cntZ * lineBlockX + cntX;

				// 頂点座標
				m_vtx[workNoVtx[0]] = vtx[workNoVtx[0]].vtx = D3DXVECTOR3(-workDistanceX, m_heightVtx[workNoVtx[0]], workDistanceZ);

				// 頂点ベクトル
				vtx[workNoVtx[0]].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 反射光
				vtx[workNoVtx[0]].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	
				// テクスチャ座標
				vtx[workNoVtx[0]].tex = D3DXVECTOR2((float)cntX, (float)cntZ);

				// X軸の距離を次の行に進める
				workDistanceX -= m_sizeBlock.x;
			}

			// Z軸の距離を次の列に進める
			workDistanceZ -= m_sizeBlock.z;
		}
	}

	//----------------------------
	// 面法線ベクトル計算
	//----------------------------
	D3DXVECTOR3 poly1Vec1, poly1Vec2;
	D3DXVECTOR3 poly2Vec1, poly2Vec2;
	D3DXVECTOR3 poly1Nor, poly2Nor;
	D3DXVECTOR3 centerNor, workNor;

	// 行の処理
	for(int cntZ = 0, cntNor = 0; cntZ < m_numBlock.z; ++cntZ)
	{
		// 列の処理
		for(int cntX = 0; cntX < m_numBlock.x; ++cntX)
		{
			// 四頂点番号
			workNoVtx[0] = cntZ * lineBlockX + cntX;
			workNoVtx[1] = workNoVtx[0] + 1;
			workNoVtx[2] = workNoVtx[0] + lineBlockX;
			workNoVtx[3] = workNoVtx[2] + 1;

			// 頂点ごとのベクトル
			poly1Vec1 = vtx[workNoVtx[0]].vtx - vtx[workNoVtx[2]].vtx;
			poly1Vec2 = vtx[workNoVtx[3]].vtx - vtx[workNoVtx[2]].vtx;
			poly2Vec1 = vtx[workNoVtx[3]].vtx - vtx[workNoVtx[1]].vtx;
			poly2Vec2 = vtx[workNoVtx[0]].vtx - vtx[workNoVtx[1]].vtx;

			// 面法線と2面の合成法線
			D3DXVec3Cross(&poly1Nor, &poly1Vec1, &poly1Vec2);
			D3DXVec3Normalize(&poly1Nor, &poly1Nor);
			m_norBuff[cntNor] = poly1Nor;

			D3DXVec3Cross(&poly2Nor, &poly2Vec1, &poly2Vec2);
			D3DXVec3Normalize(&poly2Nor, &poly2Nor);
			m_norBuff[cntNor + 1] = poly2Nor;

			centerNor = poly1Nor + poly2Nor;
			D3DXVec3Normalize(&centerNor, &centerNor);

			// 左上法線
			workNor = vtx[workNoVtx[0]].nor + centerNor;
			D3DXVec3Normalize(&workNor, &workNor);
			vtx[workNoVtx[0]].nor = workNor;

			// 右上法線
			workNor = vtx[workNoVtx[1]].nor + m_norBuff[cntNor + 1];
			D3DXVec3Normalize(&workNor, &workNor);
			vtx[workNoVtx[1]].nor = workNor;

			// 左下法線
			vtx[workNoVtx[2]].nor = m_norBuff[cntNor];

			// 右下法線
			vtx[workNoVtx[3]].nor = centerNor;

			cntNor += 2;
		}
	}

	// 終了
	m_vtxBuff->Unlock();
}

//=============================================================================
// 頂点インデックス設定
//=============================================================================
void CMeshField::SetVertexIndexPolygon(void)
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

//=============================================================================
// オブジェクトの高さ
//=============================================================================
float CMeshField::GetHeight(const D3DXVECTOR3 pos, D3DXVECTOR3* normal)
{
	D3DXVECTOR3 leftTop;
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 workVtx[4];
	int			workNoVtx[4];
	int			blockX, blockZ;
	int			lineBlockX = (int)(m_numBlock.x + 1);
	int			lineBlockZ = (int)(m_numBlock.z + 1);
	float		height = 0.0f;

	// 左奥座標
	leftTop = D3DXVECTOR3((m_sizeBlock.x * m_numBlock.x) / 2.0f,
							0.0f,
							(m_sizeBlock.z * m_numBlock.z) / 2.0f);

	// 乗っているブロック計算
	blockX = (int)((leftTop.x + pos.x) / m_sizeBlock.x);
	blockZ = (int)((leftTop.z - pos.z) / m_sizeBlock.z);

	if(blockX >= 0 && blockX < m_numBlock.x
	&& blockZ >= 0 && blockZ < m_numBlock.z)
	{
		// 四頂点番号
		workNoVtx[0] = blockZ * lineBlockX + blockX;
		workNoVtx[1] = workNoVtx[0] + 1;
		workNoVtx[2] = workNoVtx[0] + lineBlockX;
		workNoVtx[3] = workNoVtx[2] + 1;

		// 四頂点座標
		workVtx[0] = m_vtx[workNoVtx[0]];
		workVtx[1] = m_vtx[workNoVtx[1]];
		workVtx[2] = m_vtx[workNoVtx[2]];
		workVtx[3] = m_vtx[workNoVtx[3]];

		if(ChkInPolygon(workVtx[2], workVtx[0], workVtx[3], pos))
		{
			height = GetHeightPolygon(workVtx[2], workVtx[0], workVtx[3], pos, normal);
		}
		else if(ChkInPolygon(workVtx[1], workVtx[3], workVtx[0], pos))
		{
			height = GetHeightPolygon(workVtx[1], workVtx[3], workVtx[0], pos, normal);
		}
	}

	return height;
}

//=============================================================================
// 面上での高さ
//=============================================================================
float CMeshField::GetHeightPolygon(const D3DXVECTOR3 P0, const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DXVECTOR3 pos, D3DXVECTOR3* normal)
{
	D3DXVECTOR3 vec0, vec1;
	D3DXVECTOR3 workNor;

	// 面法線計算
	vec0 = P1 - P0;
	vec1 = P2 - P0;

	D3DXVec3Cross(&workNor, &vec0, &vec1);
	D3DXVec3Normalize(&workNor, &workNor);

	// 面の外
	if(workNor.y == 0.0f)
	{
		return 0.0f;
	}

	return P0.y - (workNor.x * (pos.x - P0.x) + workNor.z * (pos.z - P0.z)) / workNor.y;
}

//=============================================================================
// 面内にあるか
//=============================================================================
bool CMeshField::ChkInPolygon(const D3DXVECTOR3 P0, const D3DXVECTOR3 P1, const D3DXVECTOR3 P2, const D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vec0, vec1;
	bool		poly[3];

	poly[0] = false;
	poly[1] = false;
	poly[2] = false;

	vec0 = P1 - P0;
	vec1 = pos - P0;

	if(vec0.z * vec1.x - vec0.x * vec1.z >= 0)
	{
		poly[0] = true;
	}

	vec0 = P2 - P1;
	vec1 = pos - P1;

	if(vec0.z * vec1.x - vec0.x * vec1.z >= 0)
	{
		poly[1] = true;
	}

	vec0 = P0 - P2;
	vec1 = pos - P2;

	if(vec0.z * vec1.x - vec0.x * vec1.z >= 0)
	{
		poly[2] = true;
	}

	return poly[0] && poly[1] && poly[2];
}