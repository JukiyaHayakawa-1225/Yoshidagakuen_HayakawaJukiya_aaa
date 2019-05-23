//=============================================================================
//
// リザルト処理 [manager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "result.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "player.h"
#include "fade.h"
#include "UI.h"
#include "particle2D.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CResult::RESULT CResult::m_result = RESULT_NONE;		//リザルトのポインタ

//=============================================================================
// リザルトのコンストラクタ
//=============================================================================
CResult::CResult()
{
	m_nTimer = 0;	//タイマーの初期値
}

//=============================================================================
// リザルトのデストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// リザルトのデストラクタ
//=============================================================================
CResult *CResult::Create()
{
	CResult *pResult = NULL;	//リザルトのポインタ
	if (pResult == NULL)
	{
		pResult = new CResult;	//動的確保
		if (pResult != NULL)
		{
			pResult->Init();	//初期化処理
		}
	}
	return pResult;
}

//=============================================================================
// リザルトの初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	m_nTimer = 0;	//タイマーの初期化処理

	CParticle2D::Load();

	CUI::Load();
	CUI::CreateMode(CManager::MODE_RESULT);

	//CPlayer::Create(D3DXVECTOR3( 0.0f, 0.0f, 0.0f));
	return S_OK;
}

//=============================================================================
// リザルトの終了処理
//=============================================================================
void CResult::Uninit(void)
{
	CParticle2D::Unload();
	CUI::Unload();
	//全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// リザルトの更新処理
//=============================================================================
void CResult::Update(void)
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
	 //ランキングへ遷移
		pFade->SetFade(CManager::MODE_RANKING, pFade->FADE_OUT);
	}

	//デバッグ表示
#ifdef _DEBUG
	CDebugProc::Print(1, "Result");

#endif
}

//=============================================================================
// リザルトの描画処理
//=============================================================================
void CResult::Draw(void)
{
}

//=============================================================================
// リザルトの設置処理
//=============================================================================
void CResult::SetResult(RESULT result)
{
	m_result = result;
}
//=============================================================================
// リザルトの取得処理
//=============================================================================
CResult::RESULT CResult::GetResult(void)
{
	return m_result;
}