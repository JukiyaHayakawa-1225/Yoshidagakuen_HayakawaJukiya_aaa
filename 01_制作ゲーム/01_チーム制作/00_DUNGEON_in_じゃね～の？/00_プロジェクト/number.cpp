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
#include "number.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ANIMATION_SPEED (8)	//アニメーションのスピード
#define TEX_X (0.25f)	//テクスチャ座標_X
#define TEX_Y (1.0f)	//テクスチャ座標_Y

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL; //共有テクスチャのポインタ


//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CNumber::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI_NUMBER.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CNumber::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//コンストラクタ
//=============================================================================
CNumber::CNumber()
{
	m_rotNumber = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//角度
	m_posNumber = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//位置
	m_fNumberWidth = 0.0f;	//ポリゴンの幅
	m_fNumberHeight = 0.0f;	//ポリゴンの高さ
	m_pVtxBuff = NULL;
}

//=============================================================================
//デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
//2Dシーンの生成
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, float width, float height)
{
	CNumber  *pNumber;
	pNumber = new CNumber;//シーンクラスの生成
	if (pNumber != NULL)
	{
		pNumber->m_posNumber = pos;
		pNumber->m_fNumberWidth = width;
		pNumber->m_fNumberHeight = height;
		pNumber->Init();
	}
	return pNumber;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CNumber::Init(void)
{


	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成

	////ポリゴンの長さ
	m_fLengthNumber = sqrtf((m_fNumberWidth / 2 + m_fNumberWidth / 2) * (m_fNumberHeight / 2 + m_fNumberHeight / 2));

	//ポリゴンの角度
	m_fAngleNumber = atan2f(m_fNumberWidth / 2, m_fNumberHeight / 2);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	pVtx[0].pos = D3DXVECTOR3(m_posNumber.x - m_fNumberWidth, m_posNumber.y - m_fNumberHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posNumber.x + m_fNumberWidth, m_posNumber.y - m_fNumberHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posNumber.x - m_fNumberWidth, m_posNumber.y + m_fNumberHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posNumber.x + m_fNumberWidth, m_posNumber.y + m_fNumberHeight, 0.0f);


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
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CNumber::Uninit(void)
{

	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//=============================================================================
//更新処理
//=============================================================================
void CNumber::Update(void)
{



}

//=============================================================================
//描画処理
//=============================================================================
void CNumber::Draw(void)
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
//ポリゴンの位置の取得
//=============================================================================
D3DXVECTOR3 CNumber::GetPosNumber(void)
{
	return m_posNumber;
}

//=============================================================================
//ポリゴンの位置の設定
//=============================================================================
void CNumber::SetPosNumber(D3DXVECTOR3 pos)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	//位置情報を代入
	m_posNumber = pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posNumber.x + sinf(-D3DX_PI + m_fAngleNumber + m_rotNumber.x) * m_fLengthNumber,
		m_posNumber.y + cosf(-D3DX_PI + m_fAngleNumber + m_rotNumber.y) * m_fLengthNumber,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posNumber.x + sinf(D3DX_PI - m_fAngleNumber + m_rotNumber.x) * m_fLengthNumber,
		m_posNumber.y + cosf(D3DX_PI - m_fAngleNumber + m_rotNumber.y) * m_fLengthNumber,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posNumber.x + sinf(-m_fAngleNumber + m_rotNumber.x) * m_fLengthNumber,
		m_posNumber.y + cosf(-m_fAngleNumber + m_rotNumber.y) * m_fLengthNumber,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posNumber.x + sinf(m_fAngleNumber + m_rotNumber.x) * m_fLengthNumber,
		m_posNumber.y + cosf(m_fAngleNumber + m_rotNumber.y) * m_fLengthNumber,
		0.0f);


	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ポリゴンの角度の取得
//=============================================================================
D3DXVECTOR3 CNumber::GetRotNumber(void)
{
	return m_rotNumber;
}

//=============================================================================
//ポリゴンの角度の設定
//=============================================================================
void CNumber::SetRotNumber(D3DXVECTOR3 rot)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	m_rotNumber = rot;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_posNumber.x + sinf(-D3DX_PI + m_fAngleNumber + m_rotNumber.x) * m_fLengthNumber,
		m_posNumber.y + cosf(-D3DX_PI + m_fAngleNumber + m_rotNumber.y) * m_fLengthNumber,
		0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posNumber.x + sinf(D3DX_PI - m_fAngleNumber + m_rotNumber.x) * m_fLengthNumber,
		m_posNumber.y + cosf(D3DX_PI - m_fAngleNumber + m_rotNumber.y) * m_fLengthNumber,
		0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posNumber.x + sinf(-m_fAngleNumber + m_rotNumber.x) * m_fLengthNumber,
		m_posNumber.y + cosf(-m_fAngleNumber + m_rotNumber.y) * m_fLengthNumber,
		0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posNumber.x + sinf(m_fAngleNumber + m_rotNumber.x) * m_fLengthNumber,
		m_posNumber.y + cosf(m_fAngleNumber + m_rotNumber.y) * m_fLengthNumber,
		0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//ポリゴンの長さの取得
//=============================================================================
float CNumber::GetLengthNumber(void)
{
	return m_fLengthNumber;
}

//=============================================================================
//ポリゴンの長さの設定
//=============================================================================
void CNumber::SetLengthNumber(float fLength)
{
	m_fLengthNumber = fLength;
}

//=============================================================================
//ポリゴンの長さの設定
//=============================================================================
void CNumber::SetNumberWidth(float fWidth)
{
	m_fNumberWidth = fWidth;
}
//=============================================================================
//ポリゴンの長さの設定
//=============================================================================
void CNumber::SetNumberHeight(float fHeight)
{
	m_fNumberHeight = fHeight;

}
//=============================================================================
//テクスチャの割り当て
//=============================================================================
void CNumber::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;

}

//=============================================================================
//テクスチャの設定
//=============================================================================
void CNumber::SetTexture(float fTex_X, float fTex_Y)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

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
//テクスチャの設定
//=============================================================================
void CNumber::SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

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

//============================================================================================
//色の設定
//============================================================================================
void CNumber::SetColor(D3DXCOLOR col)
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

//============================================================================================
//数字の設定
//============================================================================================
void CNumber::SetNumber(int nNumber)
{
	m_nNumber = nNumber;

	// 頂点情報を設定
	VERTEX_2D *pVtx; // 頂点情報の作成


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * m_nNumber), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * m_nNumber), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * m_nNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * m_nNumber), 1.0f);
	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}
//============================================================================================
//幅の取得
//============================================================================================
float CNumber::GetWidthNumber(void)
{
	return m_fNumberWidth;
}
//============================================================================================
//幅の取得
//============================================================================================
float CNumber::GetHeightNumber(void)
{
	return m_fNumberHeight;

}


