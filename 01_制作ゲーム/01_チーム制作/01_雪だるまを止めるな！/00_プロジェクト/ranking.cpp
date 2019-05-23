//=============================================================================
//
// ランキングの処理 [ranking.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "ranking.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "billboard.h"
#include "fade.h"
#include "number.h"
#include "rankingscore.h"
#include "UI.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//=============================================================================
// ランキングのコンストラクタ
//=============================================================================
CRanking::CRanking()
{
	m_nTimer = 0;	//タイマーの初期化
}

//=============================================================================
// ランキングのデストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// ランキングのデストラクタ
//=============================================================================
CRanking *CRanking::Create()
{
	CRanking *pRanking = NULL;	//ランキングのポインタ
	if (pRanking == NULL)
	{
		pRanking = new CRanking;	//動的確保
		if (pRanking != NULL)
		{
			pRanking->Init();		//初期化処理
		}
	}
	return pRanking;
}

//=============================================================================
// ランキングの初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	//	UIの読み込み
	CUI::Load();
	//	UIの生成
	CUI::CreateMode(CManager::MODE_RANKING);
	//数字の読み込み
	CNumber::Load();
	//スコアの生成
	CRankingScore::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 450, SCREEN_HEIGHT / 2 - 160, 0.0f), 50.0f, 60.0f);

	return S_OK;
}

//=============================================================================
// ランキングの終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	//	UIの破棄
	CUI::Unload();
	//数字の破棄
	CNumber::Unload();

	//全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// ランキングの更新処理
//=============================================================================
void CRanking::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//フェードの取得
	CFade *pFade = CManager::GetFade();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_A) == true)
	{//Enterキーを押下した場合
	 //タイトルへ遷移
		pFade->SetFade(CManager::MODE_TITLE, pFade->FADE_OUT);
	}

	//デバッグ表示
#ifdef _DEBUG
	CDebugProc::Print(1, "Ranking");
#endif
}

//=============================================================================
// ランキングの描画処理
//=============================================================================
void CRanking::Draw(void)
{
}