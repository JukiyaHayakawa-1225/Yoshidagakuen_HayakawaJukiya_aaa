//=============================================================================
//
// チュートリアルの処理 [tutorial.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "tutorial.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "debugproc.h"
#include "fade.h"
#include "meshfield.h"
#include "UI.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//=============================================================================
// チュートリアルのコンストラクタ
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// チュートリアルのデストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// チュートリアルの生成
//=============================================================================
CTutorial *CTutorial::Create()
{
	CTutorial *pTutorial = NULL;	//チュートリアルのポインタ
	if (pTutorial == NULL)
	{
		pTutorial = new CTutorial;	//動的確保
		if (pTutorial != NULL)
		{
			pTutorial->Init();		//初期化処理
		}
	}
	return pTutorial;
}

//=============================================================================
// チュートリアルの初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{

	CUI::Load();
	CUI::CreateMode(CManager::MODE_TUTORIAL);

	CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 15.0f, 15.0f);
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	return S_OK;
}

//=============================================================================
// チュートリアルの終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	CUI::Unload();
	//全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// チュートリアルの更新処理
//=============================================================================
void CTutorial::Update(void)
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
	 //ゲームへ遷移
		pFade->SetFade(CManager::MODE_GAME, pFade->FADE_OUT);

	}

	//デバッグ表示
#ifdef _DEBUG
	CDebugProc::Print(1, "Tutorial");
#endif
}

//=============================================================================
// チュートリアルの描画処理
//=============================================================================
void CTutorial::Draw(void)
{
}