//=============================================================================
//
// リザルトの処理 [result.h]
// Author : 箱﨑彩花
//
//=============================================================================
#include "main.h"				//メイン
#include "scene.h"				//シーン
#include "renderer.h"			//レンダリング
#include "scene2D.h"			//シーン2D
#include "manager.h"			//マネージャー
#include "input.h"				//入力
#include "result.h"				//タイトル
#include "DebugProc.h"			// デバッグ表示
#include "number.h"				// ナンバー
#include "timer.h"				// タイマー
#include "fade.h"				//フェード
#include "ui_result.h"
#include "meshfield.h"
#include "object.h"
#include "inputx.h"
#include "sound.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CUiResult *CResult::m_pUiResult = NULL;
CObject *CResult::m_pObject = NULL;				//オブジェクト
CMeshField *CResult::m_pMeshField = NULL;			//メッシュフィールド

//=============================================================================
// コンストラクタ
//=============================================================================
CResult::CResult()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult()
{	//何も書かない
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	// テクスチャの読み込み（必ずCreateの前で読み込み！）
	CNumber::Load();					// ナンバー

	//テクスチャの読み込み
	m_pUiResult->Load();

	//配置するオブジェクトのテクスチャの読み込み
	m_pObject->Load();


	//===============================================
	// ゲーム内で使用する処理の生成
	//===============================================
	//メッシュフィールドの生成
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	//使用するオブジェクトモデルの読み込み・オブジェクト生成
	m_pObject->LoadModelText();




	// 生成
	CTimer::Create(D3DXVECTOR3(500.0f, 200.0f, 0.0f), CTimer::TIMEMODE_RESULT);

	// リザルトUIの生成
	m_pUiResult->Create();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// テクスチャの破棄
	CNumber::Unload();

	//テクスチャの読み込み
	m_pUiResult->Unload();




	//シーンの破棄
	CScene::ReleaseSceneAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	CInputX::InputJoyState JoyState = CInputX::GetJoystate();
	bool bConnect = CInputX::GetbConnect();			//ジョイパッドの接続状態

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();


	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();


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
				CFade::SetFade(CManager::MODE_RANKING);//ランキングへ移行
			}
		}
		JoyState = CInputX::INPUT_JOYSTATE_PUSH;
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
void CResult::Draw(void)
{
}