//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "fade.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON (2)
#define BG_WIDTH		(SCREEN_WIDTH)				//背景の幅
#define BG_HEIGHT		(SCREEN_HEIGHT)				//背景の高さ
#define BG_POS_X		(0)							//背景左上X座標
#define BG_POS_Y		(0)							//背景左上Y座標
#define PATTERN_ANIM (11)	//テクスチャパターン数

#define ANIMATION_SPEED (4)	//アニメーションスピード

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		CFade::m_pTexture = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9  CFade::m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
CFade::FADE CFade::m_fade = FADE_NONE;
D3DXCOLOR CFade::m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
CManager::MODE CFade::m_modeNext = CManager::MODE_TITLE;
int CFade::m_nPatternAnim = 0;
int CFade::m_nCounterAnim = 0;
int CFade ::m_nTex_Y = 1;
//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade()
{
	m_nPatternAnim = 10;	//アニメーションパターン
	m_nCounterAnim = 0;//アニメーションカウンター
	m_nTex_Y = 1;
}
//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init(CManager::MODE modeNext)
{
	m_nPatternAnim = 10;	//アニメーションパターン
	m_nCounterAnim = 0;//アニメーションカウンター
	m_nTex_Y = 1;

	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 値の初期化
	m_fade = FADE_IN;
	m_modeNext = modeNext;
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//黒い画面にしておく

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/fade_tex.png", &m_pTexture);


	VERTEX_2D*pVtx;	//頂点情報へのポインタ
					// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファの情報を設定
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, BG_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);

	//1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.2f, 0.5f);


	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	SetTexture(0.8f, 1.0f, 0.5f, 1.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			SetTexture(0.8f, 1.0f, 0.5f, 1.0f);


			m_nCounterAnim++;

			if (m_nCounterAnim % ANIMATION_SPEED == 0)
			{
				m_nPatternAnim = (m_nPatternAnim - 1) % PATTERN_ANIM;//パターンNo.更新
				if (m_nPatternAnim == 4)
				{
					m_nTex_Y--;
				}
				if (m_nPatternAnim == 0)
				{
					m_fade = FADE_NONE;//何もしていない状態
					m_nPatternAnim = 0;
				}
			}

		}
		else if (m_fade == FADE_OUT)
		{
			SetTexture(0.0f, 0.2f, 0.0f, 0.5f);

			m_nCounterAnim++;
			if (m_nCounterAnim % ANIMATION_SPEED == 0)
			{
				m_nPatternAnim = (m_nPatternAnim + 1) % PATTERN_ANIM;//パターンNo.更新
				if (m_nPatternAnim == 5)
				{
					m_nTex_Y++;
				}
				if (m_nPatternAnim == 10)
				{
					m_fade = FADE_IN;//何もしていない状態
					m_nPatternAnim = 9;
					CManager::SetMode(m_modeNext);
				}
			}
		}


		//頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//頂点カラー
		pVtx[0].col = m_color;
		pVtx[1].col = m_color;
		pVtx[2].col = m_color;
		pVtx[3].col = m_color;

		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0 + (0.2f * m_nPatternAnim), (0.5f * m_nTex_Y));
		pVtx[1].tex = D3DXVECTOR2(0.2f + (0.2f * m_nPatternAnim), (0.5f * m_nTex_Y));
		pVtx[2].tex = D3DXVECTOR2(0 + (0.2f * m_nPatternAnim), 0.5f * (m_nTex_Y + 1));
		pVtx[3].tex = D3DXVECTOR2(0.2f + (0.2f * m_nPatternAnim), 0.5f * (m_nTex_Y + 1));

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}


}

//=============================================================================
// タイトル画面
//=============================================================================
void CFade::Draw(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}



//=============================================================================
// フェードの設定
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_color = m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//黒い画面にしておく(透明)


}

//=============================================================================
// フェードの取得
//=============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

void CFade::SetTexture(float fTex_X_0, float fTex_X_1, float fTex_Y_0, float fTex_Y_1)
{

	VERTEX_2D*pVtx;	//頂点情報へのポインタ


	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(fTex_X_0, fTex_Y_0);
	pVtx[1].tex = D3DXVECTOR2(fTex_X_1, fTex_Y_0);
	pVtx[2].tex = D3DXVECTOR2(fTex_X_0, fTex_Y_1);
	pVtx[3].tex = D3DXVECTOR2(fTex_X_1, fTex_Y_1);


	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

void CFade::AddTex()
{
	VERTEX_2D*pVtx;	//頂点情報へのポインタ


	if (m_fade != FADE_NONE)
	{
		m_nPatternAnim++;
		if (m_nCounterAnim % ANIMATION_SPEED == 0)
		{
			m_nPatternAnim = (m_nPatternAnim + 1) % PATTERN_ANIM;//パターンNo.更新
			if (m_nPatternAnim == 5)
			{
				m_nTex_Y++;
			}
			if (m_nPatternAnim == 10)
			{
				m_fade = FADE_IN;//何もしていない状態
				m_nPatternAnim = 9;
				CManager::SetMode(CManager::MODE_RESULT);
			}


		}



	}


	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0 + (0.2f * m_nPatternAnim), (0.5f * m_nTex_Y));
	pVtx[1].tex = D3DXVECTOR2(0.2f + (0.2f * m_nPatternAnim), (0.5f * m_nTex_Y));
	pVtx[2].tex = D3DXVECTOR2(0 + (0.2f * m_nPatternAnim), 0.5f * (m_nTex_Y + 1));
	pVtx[3].tex = D3DXVECTOR2(0.2f + (0.2f * m_nPatternAnim), 0.5f * (m_nTex_Y + 1));

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();




}


