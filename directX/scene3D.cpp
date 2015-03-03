//*****************************************************************************
//
// CScene3Dクラス [scene3D.cpp]
// Author :MAI TANABE
//
//*****************************************************************************

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// インクルードファイル
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "scene3D.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// マクロ定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define POLIGON_SIZE (1.0f)

//=============================================================================
// コンストラクタ
//=============================================================================
CScene3D::CScene3D(int priority, OBJTYPE objType) : CScene(priority, objType)
{
	m_tex		= NULL;
	m_vtxBuff	= NULL;
	m_indexBuff	= NULL;

	m_rot	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scl	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_size	= D3DXVECTOR3(POLIGON_SIZE, POLIGON_SIZE, 0.0f);
	m_color	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// 生成
//=============================================================================
CScene3D* CScene3D::Create(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture)
{
	CScene3D* pointer = new CScene3D;
	pointer->Init(device, texture);
	return pointer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CScene3D::Init(LPDIRECT3DDEVICE9 device, CImport::TEXTURES texture)
{
	//----------------------------
	// デバイス取得
	//----------------------------
	m_device = device;

	//----------------------------
	// ポリゴン初期化
	//----------------------------
	// メモリ確保
	HRESULT_FUNC(m_device->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &m_vtxBuff,
											  NULL))

	// 頂点情報設定
	SetVertexPolygon();

	// テクスチャの読込み
	m_tex = CImport::GetTexture(texture);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CScene3D::Uninit(void)
{
	// 頂点バッファの開放
	SAFE_RELEASE(m_vtxBuff);

	// 開放
	CScene::Delete();
}

//=============================================================================
// 更新
//=============================================================================
void CScene3D::Update(void)
{
}

//=============================================================================
// 描画
//=============================================================================
void CScene3D::Draw(void)
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
	// 頂点フォーマットの設定
	m_device->SetFVF(FVF_VERTEX_3D);
	// テクスチャの設定
	m_device->SetTexture(0, m_tex);
	// 描画設定
	m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP,	// 描画種類
							0,						//
							2);						// ポリゴンの数
}

//=============================================================================
// 頂点情報設定
//=============================================================================
void CScene3D::SetVertexPolygon(void)
{
	VERTEX_3D* vtx;

	// 開始
	m_vtxBuff->Lock(0, 0, (void **)&vtx, 0);

	// 座標
	D3DXVECTOR3 half = D3DXVECTOR3(m_size.x / 2.0f,  m_size.y / 2.0f, m_size.z / 2.0f);
	vtx[0].vtx = D3DXVECTOR3(-half.x,  half.y, half.z);
	vtx[1].vtx = D3DXVECTOR3( half.x,  half.y, half.z);
	vtx[2].vtx = D3DXVECTOR3(-half.x, -half.y, half.z);
	vtx[3].vtx = D3DXVECTOR3( half.x, -half.y, half.z);

	// 頂点ベクトル
	vtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
