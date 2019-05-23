//=============================================================================
//
// UIの処理[2Dポリゴン] [ui.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================5
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "ui_ranking.h"
#include "timer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_WIDTH (200)			//ランキングポリゴンの幅
#define RANKING_HEIGHT (90)			//ランキングポリゴンのYの高さ

#define RANK_WIDTH (180)			//ランクポリゴンの幅
#define RANK_HEIGHT (620)			//ランクポリゴンのYの高さ

#define MINUTES_WIDTH (40)			//分ポリゴンの幅
#define MINUTES_HEIGHT (90)			//分ポリゴンのYの高さ

#define DOT_WIDTH (20)				//ドットポリゴンの幅
#define DOT_HEIGHT (20)				//ドットポリゴンのYの高さ

#define MINUTES_POS_X (640.0f)		//分の位置(X)
#define DOT_POS_X (830.0f)		//分の位置(X)

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CUiRanking::m_pTexture[MAX_TEXTURE_UI_RANKING] = {}; //共有テクスチャのポインタ
//=============================================================================
//コンストラクタ
//=============================================================================
CUiRanking::CUiRanking(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{//クリアな値を代入

}
//=============================================================================
//デストラクタ
//=============================================================================
CUiRanking::~CUiRanking()
{


}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CUiRanking::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ranking.png", &m_pTexture[0]);		// リザルト
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI_RANKING.png", &m_pTexture[1]);		// カギ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI_COLON.png", &m_pTexture[2]);	// 分
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI_DOT.png", &m_pTexture[3]);		// ドット


	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CUiRanking::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_UI_RANKING; nCntTex++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}


//=============================================================================
//背景の生成
//=============================================================================
CUiRanking *CUiRanking::Create(void)
{
	//背景の生成
	CUiRanking *pUi;
	pUi = new CUiRanking;

	if (pUi != NULL)
	{
		//背景の初期化処理
		pUi->Init();
	}

	return pUi;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CUiRanking::Init(void)
{
	//色情報設定
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCntColor = 0;

	for (int nCntScene2D = 0; nCntScene2D < MAX_UI_RANKING; nCntScene2D++)
	{
		switch (nCntScene2D)
		{
		case UI_TYPE_RANKING:// ランキング
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(150.0f, 90.0f, 0.0f), RANKING_WIDTH, RANKING_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_RANKING]);
			break;
		case UI_TYPE_RANK:// 時計
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250.0f, SCREEN_HEIGHT / 2 -30.0f, 0.0f), RANK_WIDTH, RANK_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_RANK]);
			break;

		case UI_TYPE_FIRSTMINUTES://1st分
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(MINUTES_POS_X, 100.0f, 0.0f), MINUTES_WIDTH, MINUTES_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_MINUTES]);
			break;

		case UI_TYPE_FIRSTDOT://1stドット
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(DOT_POS_X, 130.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_DOT]);
			break;

		case UI_TYPE_SECONDMINUTES://2nd分
			 //2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(MINUTES_POS_X, 220.0f, 0.0f), MINUTES_WIDTH, MINUTES_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_MINUTES]);
			break;

		case UI_TYPE_SECONDDOT://2ndドット
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(DOT_POS_X, 250.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_DOT]);
			break;

		case UI_TYPE_THIRDMINUTES://3rd分
			 //2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(MINUTES_POS_X, 340.0f, 0.0f), MINUTES_WIDTH, MINUTES_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_MINUTES]);
			break;

		case UI_TYPE_THIRDDOT://3rdドット
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(DOT_POS_X, 360.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_DOT]);
			break;
		case UI_TYPE_FORTHMINUTES://4th分
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(MINUTES_POS_X, 450.0f, 0.0f), MINUTES_WIDTH, MINUTES_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_MINUTES]);
			break;

		case UI_TYPE_FORTHDOT://4thドット
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(DOT_POS_X, 480.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_DOT]);
			break;
		case UI_TYPE_FIFTHMINUTES://5th分
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(MINUTES_POS_X, 570.0f, 0.0f), MINUTES_WIDTH, MINUTES_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_MINUTES]);
			break;

		case UI_TYPE_FIFTHDOT://4thドット
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(DOT_POS_X, 600.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UITEX_DOT]);
			break;

		}
		m_pScene2D[nCntScene2D]->SetObjType(CScene::OBJTYPE_UI);
	}

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CUiRanking::Uninit(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_UI_RANKING; nCntScene2D++)
	{
		if (m_pScene2D != NULL)
		{//シーン2Dが空じゃないとき
		 //2Dシーンの終了処理
			m_pScene2D[nCntScene2D]->Uninit();
			//delete m_pScene2D[nCntScene2D];
			m_pScene2D[nCntScene2D] = NULL;
		}
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CUiRanking::Update(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_UI_RANKING; nCntScene2D++)
	{
		if (m_pScene2D[nCntScene2D] != NULL)
		{//シーン2Dが空じゃないとき
			D3DXVECTOR3 posBg = m_pScene2D[nCntScene2D]->GetPosPolygon();	//プレイヤーの位置情報

																			//位置情報の設定
			m_pScene2D[nCntScene2D]->SetPosPolygon(posBg);
		}
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CUiRanking::Draw(void)
{

}

