//=============================================================================
//
// ビルボードゲージ処理 [billboardgauge.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "billboardgauge.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "enemy.h"
#include "score.h"
#include "billboard.h"
#include "player.h"
#include "game.h"
#include "ui.h"
#include "enemy.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CBillboardLifeGauge::m_pTexture = NULL;	//テクスチャへのポインタ

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CBillboardLifeGauge::CBillboardLifeGauge() : CBillboard()
{
	m_nLife = 0;								//体力の初期値
}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CBillboardLifeGauge::~CBillboardLifeGauge()
{

}
//=============================================================================
// オブジェクトのテクスチャ読み込み
//=============================================================================
HRESULT CBillboardLifeGauge::Load(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gauge\\GAUGE_LIFE.png",
		&m_pTexture);

	return S_OK;
}

//=============================================================================
// オブジェクトのテクスチャ破棄
//=============================================================================
void CBillboardLifeGauge::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ビルボード体力ゲージ生成
//=============================================================================
CBillboardLifeGauge *CBillboardLifeGauge::Create(D3DXVECTOR3 pos, int nLife, int nMaxLife)
{
	CBillboardLifeGauge *pBillboardLifeGauge = NULL;
	if (pBillboardLifeGauge == NULL)
	{
		pBillboardLifeGauge = new CBillboardLifeGauge;						//シーンの動的確保
		if (pBillboardLifeGauge != NULL)
		{
			pBillboardLifeGauge->SetPos(pos);						//位置の設置処理
			pBillboardLifeGauge->m_nLife = nLife;					//体力の設置処理
			pBillboardLifeGauge->m_nMaxLife = nMaxLife;				//体力の最大値設置処理
			pBillboardLifeGauge->SetWidth(MAX_BILLBOARDGAGE_WIDTH);	//幅の設置処理
			pBillboardLifeGauge->SetHeight(2.5f);					//高さの設置処理
			pBillboardLifeGauge->Init();							//初期化処理
			pBillboardLifeGauge->CutSide(0);						//左から減算処理
			pBillboardLifeGauge->BindTexture(m_pTexture);			//テクスチャの貼り付け
		}
	}
	return pBillboardLifeGauge;									//値を返す
}

//=============================================================================
// オブジェクトの初期化処理
//=============================================================================
HRESULT CBillboardLifeGauge::Init(void)
{
	//ビルボードの初期化処理
	CBillboard::Init();
	//色の設置処理
	SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));;
	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CBillboardLifeGauge::Uninit(void)
{
	//ビルボードの終了処理
	CBillboard::Uninit();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CBillboardLifeGauge::Update(void)
{

}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CBillboardLifeGauge::Draw(void)
{
	//CScene2Dの描画処理
	CBillboard::Draw();
}

//=============================================================================
// UI体力ゲージ減算処理
//=============================================================================
void CBillboardLifeGauge::CutLifeGauge(int nCut)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//敵の取得
	CEnemy *pEnemy;
	pEnemy = CGame::GetEnemy();

	int nLife = 0;			//体力
	int nMaxLife = 0;		//体力の最大値
	float fData;			//データの格納

	m_nLife -= nCut;		//体力の減算

	D3DXVECTOR3 pos = GetPos();	//位置の取得処理
	float fWidth = GetWidth();	//幅の取得処理

	//ゲージの割合処理
	fData = MAX_BILLBOARDGAGE_WIDTH * m_nLife / m_nMaxLife;

	if (fWidth >= 0.0f)
	{//幅が０以上の場合
		SetWidth(fData);		//幅の設置処理
		CutSide(0);				//左から減算処理
		if (fWidth <= MAX_BILLBOARDGAGE_WIDTH && fWidth >= (MAX_BILLBOARDGAGE_WIDTH / 2 + 1))
		{//幅が半分より大きいの場合
			SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));	//色の設置処理
		}
		else if (fWidth <= MAX_BILLBOARDGAGE_WIDTH / 2 && fWidth >= (MAX_BILLBOARDGAGE_WIDTH / 4 + 1))
		{//幅が半分より小さい場合
			SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));	//色の設置処理
		}
		else if (fWidth <= MAX_BILLBOARDGAGE_WIDTH / 4 && fWidth >= 0)
		{//幅が４割以下の場合
			SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	//色の設置処理
		}
	}
	else
	{
		fWidth = 0.0f;		//値の固定
		SetWidth(fWidth);	//幅の設置処理
		CutSide(0);			//右から減算処理
	}
}
