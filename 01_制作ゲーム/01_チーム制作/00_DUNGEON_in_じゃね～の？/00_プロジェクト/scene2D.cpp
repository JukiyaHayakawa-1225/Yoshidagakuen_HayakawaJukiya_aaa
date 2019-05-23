//=============================================================================
//
// シーン処理[2Dポリゴン] [scene2D.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLYGON_WIDTH (50.0f)
#define POLYGON_HEIGHT (50.0f)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager *CScene2D::m_pManager = NULL;

//=============================================================================
// 関数名：2Dシーンの生成
// 関数の概要：2Dシーンを生成する
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CScene2D  *pScene2D;
	pScene2D = new CScene2D;//シーンクラスの生成
	if (pScene2D != NULL)
	{
		pScene2D->m_posPolygon = pos;//位置情報の代入
		pScene2D->m_fPolygonWidth = fWidth;
		pScene2D->m_fPolygonHeight = fHeight;
		pScene2D->Init();
	}
	return pScene2D;
}

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CScene2D::CScene2D(int nPriority, OBJTYPE type) :  CScene(nPriority, type)
{
	m_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	m_posPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	m_fPolygonWidth = 0.0f;	//ポリゴンの幅
	m_fPolygonHeight = 0.0f;	//ポリゴンの高さ
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_nNumScene2D++;
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 関数名：2Dシーンの初期化処理
// 関数の概要：ポリゴンの設定
//=============================================================================
HRESULT CScene2D::Init(void)
{


	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	VERTEX_2D *pVtx; // 頂点情報の作成
	m_fLengthPolygon = sqrtf((m_fPolygonWidth / 2 + m_fPolygonWidth / 2) * (m_fPolygonHeight / 2 + m_fPolygonHeight / 2));

	m_fAnglePolygon = atan2f(m_fPolygonWidth / 2, m_fPolygonHeight / 2);

	// 頂点座標の設定
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);

	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + sinf(D3DX_PI + m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(D3DX_PI + m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);

	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);

	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);


	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();


	return S_OK;
}

//=============================================================================
// 関数名：2Dシーンの終了処理
// 関数の概要：テクスチャ、頂点バッファの破棄、自身の破棄
//=============================================================================
void CScene2D::Uninit(void)
{

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクト(自分自身)の破棄
	Release();

}

//=============================================================================
// 関数名：2Dシーンの更新処理
// 関数の概要：--
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// 関数名：2Dシーンの描画処理
// 関数の概要：--
//=============================================================================
void CScene2D::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	// デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わなくても書く！)
	pDevice->SetTexture(0, m_pTexture);


	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
//=============================================================================
// 関数名：エフェクト描画処理
// 関数の概要：--
//=============================================================================
void CScene2D::DrawEffect(void)
{

	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	// デバイスの取得
	pDevice = pRenderer->GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));


	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定(使わなくても書く！)
	pDevice->SetTexture(0, m_pTexture);


	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// 関数名：2Dシーンの総数を取得
// 関数の概要：2Dシーンの総数を返す
//=============================================================================
int CScene2D::GetNum2DSceneAll(void)
{
	return m_nNumScene2D;
}

//=============================================================================
// 関数名：ポリゴンの位置を取得
// 関数の概要：ポリゴンの位置を返す
//=============================================================================
D3DXVECTOR3 CScene2D::GetPosPolygon(void)
{
	return m_posPolygon;
}

//=============================================================================
// 関数名：ポリゴンの位置を設定
// 関数の概要：ポリゴンの位置を設定する
//=============================================================================
void CScene2D::SetPosPolygon(D3DXVECTOR3 pos)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	//位置情報を代入
	m_posPolygon = pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + sinf(D3DX_PI - m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(D3DX_PI - m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + sinf(m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 関数名：ポリゴンの角度を取得
// 関数の概要：ポリゴンの角度を返す
//=============================================================================
D3DXVECTOR3 CScene2D::GetRotPolygon(void)
{
	return m_rotPolygon;
}

//=============================================================================
// 関数名：ポリゴンの角度を設定
// 関数の概要：ポリゴンの角度を設定
//=============================================================================
void CScene2D::SetRotPolygon(D3DXVECTOR3 rot)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	m_rotPolygon = rot;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-D3DX_PI + m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + sinf(D3DX_PI - m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(D3DX_PI - m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x + sinf(-m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(-m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + sinf(m_fAnglePolygon + m_rotPolygon.x) * m_fLengthPolygon,
		m_posPolygon.y + cosf(m_fAnglePolygon + m_rotPolygon.y) * m_fLengthPolygon,
		0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 関数名：ポリゴンの長さを取得
// 関数の概要：ポリゴンの長さを返す
//=============================================================================
float CScene2D::GetLengthPolygon(void)
{
	return m_fLengthPolygon;
}

//=============================================================================
// 関数名：ポリゴンの長さを設定
// 関数の概要：ポリゴンの長さを設定する
//=============================================================================
void CScene2D::SetLengthPolygon(float fLength)
{
	m_fLengthPolygon = fLength;
}

//=============================================================================
// 関数名：ポリゴンの幅を設定
// 関数の概要：ポリゴンの幅を設定する
//=============================================================================
void CScene2D::SetPolygonWidth(float fWidth)
{
	m_fPolygonWidth = fWidth;
}
//=============================================================================
// 関数名：ポリゴンの高さを設定
// 関数の概要：ポリゴンの高さを設定する
//=============================================================================
void CScene2D::SetPolygonHeight(float fHeight)
{
	m_fPolygonHeight = fHeight;

}
//=============================================================================
// 関数名：テクスチャの設定
// 関数の概要：テクスチャを割り当てる
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
// 関数名：テクスチャの座標設定
// 関数の概要：テクスチャの座標を設定する
//=============================================================================
void CScene2D::SetTexture(float fTex_X, float fTex_Y)
{

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X, fTex_Y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
// 関数名：テクスチャアニメーション
// 関数の概要：アニメーションを動かす
//=============================================================================
void CScene2D::SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim)
{
	//デバイスの取得
	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0 + (fTex_X*nPatternAnim), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y);
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
// 関数名：テクスチャアニメーション(Y軸)
// 関数の概要：アニメーションを動かす
//=============================================================================
void CScene2D::SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y)
{

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


					 // 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0 + (fTex_X*nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[1].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), (fTex_Y * nTex_Y));
	pVtx[2].tex = D3DXVECTOR2(0 + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));
	pVtx[3].tex = D3DXVECTOR2(fTex_X + (fTex_X * nPatternAnim), fTex_Y * (nTex_Y + 1));

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
// 関数名：背景スクロール
// 関数の概要：テクスチャをうごかす
//=============================================================================
void CScene2D::ScrollBg(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3)
{

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = Scroll_0;
	pVtx[1].tex = Scroll_1;
	pVtx[2].tex = Scroll_2;
	pVtx[3].tex = Scroll_3;


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 関数名：ポリゴンの色設定
// 関数の概要：ポリゴンの色を設定する
//=============================================================================
void CScene2D::SetColor(D3DXCOLOR col)
{
	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 関数名：優先順位の設定
// 関数の概要：描画順を設定する
//=============================================================================
void CScene2D::SetPriority(int nPriority)
{
	m_nPriority = nPriority;
}
//=============================================================================
// 関数名：ポリゴンの幅を取得
// 関数の概要：ポリゴンの幅を返す
//=============================================================================
float CScene2D::GetWidthPolygon(void)
{
	return m_fPolygonWidth;
}
//=============================================================================
// 関数名：ポリゴンのを高さ取得
// 関数の概要：ポリゴンの幅を返す
//=============================================================================
float CScene2D::GetHeightPolygon(void)
{
	return m_fPolygonHeight;

}




