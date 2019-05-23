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
#include "ui_result.h"
#include "timer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define COIN_WIDTH (200)			//コインポリゴンの幅
#define COIN_HEIGHT (90)			//コインポリゴンのYの高さ

#define KEY_WIDTH (200)				//カギポリゴンの幅
#define KEY_HEIGHT (90)				//カギポリゴンのYの高さ

#define SLASH_WIDTH (30)			//スラッシュポリゴンの幅
#define SLASH_HEIGHT (70)			//スラッシュポリゴンのYの高さ

#define MINUTES_WIDTH (60)			//分ポリゴンの幅
#define MINUTES_HEIGHT (90)			//分ポリゴンのYの高さ

#define DOT_WIDTH (30)				//ドットポリゴンの幅
#define DOT_HEIGHT (30)				//ドットポリゴンのYの高さ

#define EMOTE_WIDTH (100)			//思考ポリゴンの幅
#define EMOTE_HEIGHT (100)			//思考ポリゴンのYの高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CUiResult::m_pTexture[MAX_TEXTURE_UI_RESULT] = {}; //共有テクスチャのポインタ
//=============================================================================
//コンストラクタ
//=============================================================================
CUiResult::CUiResult(int nPriority, OBJTYPE type) : CScene(nPriority, type)
{//クリアな値を代入

}
//=============================================================================
//デストラクタ
//=============================================================================
CUiResult::~CUiResult()
{


}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CUiResult::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/result.png", &m_pTexture[0]);		// リザルト
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/watch.png", &m_pTexture[1]);		// カギ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI_COLON.png", &m_pTexture[2]);	// 分
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI_DOT.png", &m_pTexture[3]);		// ドット


	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CUiResult::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_UI_RESULT; nCntTex++)
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
CUiResult *CUiResult::Create(void)
{
	//背景の生成
	CUiResult *pUi;
	pUi = new CUiResult;

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
HRESULT CUiResult::Init(void)
{
	//色情報設定
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nCntColor = 0;

	for (int nCntScene2D = 0; nCntScene2D < MAX_UI_RESULT; nCntScene2D++)
	{
		switch (nCntScene2D)
		{
		case UI_TYPE_RESULT:// リザルト
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(150.0f, 90.0f, 0.0f), COIN_WIDTH, COIN_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UI_TYPE_RESULT]);
			break;
		case UI_TYPE_WATCH:// 時計
			//2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH/2 - 300.0f, SCREEN_HEIGHT/ 2 - 150.0f, 0.0f), KEY_WIDTH, KEY_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UI_TYPE_WATCH]);
			break;

		case UI_TYPE_MINUTES://分
							 //2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(700.0f, 210.0f, 0.0f), MINUTES_WIDTH, MINUTES_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UI_TYPE_MINUTES]);
			break;

		case UI_TYPE_DOT://どっと
							 //2Dポリゴンの生成
			m_pScene2D[nCntScene2D] = CScene2D::Create(D3DXVECTOR3(930.0f, 230.0f, 0.0f), DOT_WIDTH, DOT_HEIGHT);
			//テクスチャの割り当て
			m_pScene2D[nCntScene2D]->BindTexture(m_pTexture[UI_TYPE_DOT]);
			break;
		}
		m_pScene2D[nCntScene2D]->SetObjType(CScene::OBJTYPE_UI);
	}

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CUiResult::Uninit(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_UI_RESULT; nCntScene2D++)
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
void CUiResult::Update(void)
{
	for (int nCntScene2D = 0; nCntScene2D < MAX_UI_RESULT; nCntScene2D++)
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
void CUiResult::Draw(void)
{

}

