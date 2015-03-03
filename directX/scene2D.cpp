//*****************************************************************************
//
// CScene2Dクラス [scene2D.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "scene2D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define POLIGON_SIZE (1.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_texture	= NULL;
	m_vtxBuff	= NULL;

	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size	= D3DXVECTOR3(POLIGON_SIZE, POLIGON_SIZE, 0.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR2 half = D3DXVECTOR2(m_size.x / 2.0f, m_size.y / 2.0f);
	m_length = DISTANCE_2D(half.x, half.y);
	m_angle	 = atan2f(half.x, half.y);

}

//=============================================================================
// 生成
//=============================================================================
CScene2D* CScene2D::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, POINT_TYPE pointType)
{
	CScene2D* pointer = new CScene2D;
	pointer->Init(device, texture, pointType);
	return pointer;
}

CScene2D* CScene2D::Create(LPDIRECT3DDEVICE9 device, const char* texture, POINT_TYPE pointType)
{
	CScene2D* pointer = new CScene2D;
	pointer->Init(device, texture, pointType);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CScene2D::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture, POINT_TYPE pointType)
{
	//----------------------------
	// デバイス取得
	//----------------------------
	m_device = device;

	//----------------------------
	// ポイントのタイプ取得
	//----------------------------
	m_pointType = pointType;

	//----------------------------
	// ポリゴン初期化
	//----------------------------
	// メモリ確保
	HRESULT_FUNC(m_device->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_2D,
											  D3DPOOL_MANAGED,
											  &m_vtxBuff,
											  NULL))

	// 頂点情報設定
	SetVertexPolygon();

	// テクスチャ取得
	m_texture = CImport::GetTexture(texture);

	return S_OK;
}

HRESULT CScene2D::Init(LPDIRECT3DDEVICE9 device, const char* texture, POINT_TYPE pointType)
{
	//----------------------------
	// デバイス取得
	//----------------------------
	m_device = device;

	//----------------------------
	// ポイントのタイプ取得
	//----------------------------
	m_pointType = pointType;

	//----------------------------
	// ポリゴン初期化
	//----------------------------
	// メモリ確保
	HRESULT_FUNC(m_device->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_2D,			// 頂点フォーマット
											  D3DPOOL_MANAGED,
											  &m_vtxBuff,				// 頂点情報
											  NULL))

	// 頂点情報設定
	SetVertexPolygon();

	// テクスチャ取得
	D3DXCreateTextureFromFile(device, texture, &m_texture);
	
	//----------------------------
	// 初期化成功
	//----------------------------
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの開放
	SAFE_RELEASE(m_vtxBuff);

	// 開放
	CScene::Delete();
}

//=============================================================================
// 更新
//=============================================================================
void CScene2D::Update(void)
{
}

//=============================================================================
// 描画
//=============================================================================
void CScene2D::Draw(void)
{
	//頂点バッファをデータストリームにバインド
	m_device->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	m_device->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	m_device->SetTexture(0, m_texture);

	//描画設定
	m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//描画種類
							0,						//
							2);						//ポリゴンの数
}

//=============================================================================
// 頂点情報設定
//=============================================================================
void CScene2D::SetVertexPolygon(void)
{
	VERTEX_2D* vtx;
	D3DXVECTOR3	work[4];

	// 座標設定
	switch(m_pointType)
	{
	case POINT_LEFTTOP:
		{
			// 各頂点の座標計算
			work[0].x = m_pos.x;
			work[0].y = m_pos.y;
			work[1].x = m_pos.x + m_size.x;
			work[1].y = m_pos.y;
			work[2].x = m_pos.x;
			work[2].y = m_pos.y + m_size.y;
			work[3].x = m_pos.x + m_size.x;
			work[3].y = m_pos.y + m_size.y;
			break;
		}

	case POINT_CENTER:
		{
			// 頂点距離・角度更新
			D3DXVECTOR2 half = D3DXVECTOR2(m_size.x / 2.0f, m_size.y / 2.0f);
			m_length = DISTANCE_2D(half.x, half.y);
			m_angle	 = atan2f(half.x, half.y);

			// 各頂点の座標計算
			work[0].x = m_pos.x - sinf(m_angle - m_rot.z) * m_length;
			work[0].y = m_pos.y - cosf(m_angle - m_rot.z) * m_length;
			work[1].x = m_pos.x + sinf(m_angle + m_rot.z) * m_length;
			work[1].y = m_pos.y - cosf(m_angle + m_rot.z) * m_length;
			work[2].x = m_pos.x - sinf(m_angle + m_rot.z) * m_length;
			work[2].y = m_pos.y + cosf(m_angle + m_rot.z) * m_length;
			work[3].x = m_pos.x + sinf(m_angle - m_rot.z) * m_length;
			work[3].y = m_pos.y + cosf(m_angle - m_rot.z) * m_length;
			break;
		}
	}

	// 開始
	m_vtxBuff->Lock(0, 0, (void **)&vtx, 0);

	// 座標
	vtx[0].vtx = D3DXVECTOR3(work[0].x, work[0].y, 0.0f);
	vtx[1].vtx = D3DXVECTOR3(work[1].x, work[1].y, 0.0f);
	vtx[2].vtx = D3DXVECTOR3(work[2].x, work[2].y, 0.0f);
	vtx[3].vtx = D3DXVECTOR3(work[3].x, work[3].y, 0.0f);

	// 1.0f固定
	vtx[0].rhw = 1.0f;
	vtx[1].rhw = 1.0f;
	vtx[2].rhw = 1.0f;
	vtx[3].rhw = 1.0f;

	// 反射光
	vtx[0].diffuse = m_color;
	vtx[1].diffuse = m_color;
	vtx[2].diffuse = m_color;
	vtx[3].diffuse = m_color;

	// テクスチャ座標
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 終了
	m_vtxBuff->Unlock();
}
