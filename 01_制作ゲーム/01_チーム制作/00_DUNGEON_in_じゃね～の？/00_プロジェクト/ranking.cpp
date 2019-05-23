//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : 箱﨑彩花
//
//=============================================================================
#include "main.h"				//メイン
#include "scene.h"				//シーン
#include "renderer.h"			//レンダリング
#include "scene2D.h"			//シーン2D
#include "manager.h"			//マネージャー
#include "input.h"				//入力
#include "number.h"				// ナンバー
#include "timer.h"				// タイマー
#include "game.h"				//ゲーム
#include "ranking.h"			//ランキング
#include "DebugProc.h"			//デバッグ表示
#include "fade.h"				//フェード
#include "ui_ranking.h"
#include "meshfield.h"
#include "object.h"
#include "inputx.h"
#include "sound.h"
//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CUiRanking *CRanking::m_pUiRanking = NULL;
CObject *CRanking::m_pObject = NULL;				//オブジェクト
CMeshField *CRanking::m_pMeshField = NULL;			//メッシュフィールド
int CRanking::m_nJumpTitleFrame = 0;//タイトル画面へ遷移するときのフレームカウンタ

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{	//何も書かない
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	//フレームの初期化
	m_nJumpTitleFrame = 0;


	//数字のテクスチャの読み込み
	CNumber::Load();

	//UIのテクスチャの読み込み
	m_pUiRanking->Load();

	//配置するオブジェクトのテクスチャの読み込み
	m_pObject->Load();


	//===============================================
	// ゲーム内で使用する処理の生成
	//===============================================
	//メッシュフィールドの生成
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	//使用するオブジェクトモデルの読み込み・オブジェクト生成
	m_pObject->LoadModelText();



	//UIの生成
	m_pUiRanking->Create();

	//生成処理
	CTimer::Create(D3DXVECTOR3(500.0f, 100.0f, 0.0f), CTimer::TIMEMODE_RANKING);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	CScene::ReleaseSceneAll();

	//数字のテクスチャの破棄
	CNumber::Unload();

	//UIのテクスチャの読み込み
	m_pUiRanking->Unload();

}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	m_nJumpTitleFrame++;

	CInputX::InputJoyState JoyState = CInputX::GetJoystate();
	bool bConnect = CInputX::GetbConnect();			//ジョイパッドの接続状態

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && bConnect == true))
	{//エンターキーが押されたとき
		if (JoyState == CInputX::INPUT_JOYSTATE_NOTPUSH)
		{
			if (pFade == CFade::FADE_NONE)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
				CFade::SetFade(CManager::MODE_TITLE);//ランキングへ移行
			}
		}
		JoyState = CInputX::INPUT_JOYSTATE_PUSH;

	}

	if (m_nJumpTitleFrame % 600 == 0)
	{
		if (pFade == CFade::FADE_NONE)
		{
			CFade::SetFade(CManager::MODE_TITLE);//ランキングへへ移行
		}
	}

	if (JoyState == CInputX::INPUT_JOYSTATE_PUSH)
	{
		JoyState = CInputX::INPUT_JOYSTATE_NONE;
	}
	else if (JoyState == CInputX::INPUT_JOYSTATE_NONE)
	{
		JoyState = CInputX::INPUT_JOYSTATE_NOTPUSH;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{
}