//=============================================================================
//
// マネージャ処理 [manager.cpp]
// Author :岸田怜(Kishida Rei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "scene3D.h"
#include "scene_billboard.h"
#include "sceneX.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "ui.h"
#include "scene_meshcylinder.h"
#include "itemcounter.h"
#include "number.h"
#include "meshfield.h"
#include "object.h"
#include "timer.h"
#include "fade.h"
//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CGame *CGame::m_pGame = NULL;					//ゲーム
CPlayer *CGame::m_pPlayer = NULL;				//プレイヤー
CEnemy *CGame::m_pEnemy[15] = {};				//敵
CModel *CGame::m_pModel = {};					//モデル
CObject *CGame::m_pObject = NULL;				//オブジェクト
CMeshCylinder *CGame::m_pMeshCylinder = NULL;	//シリンダー
CUi *CGame::m_pUi = NULL;						//UI
CItemCounter *CGame::m_pItemCounter = NULL;		//アイテムカウンタ
CNumber *CGame::m_pNumber = NULL;				//数字
CTimer *CGame::m_pTimer = NULL;					//タイマー
CMeshField *CGame::m_pMeshField = NULL;			//メッシュフィールド
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CGame::~CGame()
{

}




//=============================================================================
// 関数名：ゲームの初期化処理
// 関数の概要：使用するテクスチャやモデルの読み込み
//=============================================================================
HRESULT CGame::Init(void)
{


	//===============================================
	//テクスチャ・パーツの読み込み
	//===============================================
	//プレイヤーのパーツ読み込み
	m_pModel->Load();

	//UIのテクスチャ読み込み
	m_pUi->Load();

	//数字テクスチャの読み込み
	m_pNumber->Load();

	//配置するオブジェクトのテクスチャの読み込み
	m_pObject->Load();

	//プレイヤーのテクスチャの読み込み
	m_pPlayer->Load();

	//敵のテクスチャの読み込み
	CEnemy::Load();

	//===============================================
	// ゲーム内で使用する処理の生成
	//===============================================
	//UIの生成
	m_pUi = CUi::Create();

	//メッシュフィールドの生成
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//メッシュシリンダーの生成
	m_pMeshCylinder = CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//アイテムカウンターの生成
	m_pItemCounter = CItemCounter::Create();


	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(150.0f, 0.0f, -1250.0f));

	//敵の生成
	m_pEnemy[0] = CEnemy::Create(D3DXVECTOR3(600.0f, 0.0f, -800.0f), CEnemy::ENEMYTYPE_LOITERING_X);
	m_pEnemy[1] = CEnemy::Create(D3DXVECTOR3(350.0f, 0.0f, -150.0f), CEnemy::ENEMYTYPE_CHASE);
	m_pEnemy[2] = CEnemy::Create(D3DXVECTOR3(450.0f, 0.0f,-300.0f), CEnemy::ENEMYTYPE_LOITERING_Z);
	m_pEnemy[3] = CEnemy::Create(D3DXVECTOR3(200.0f, 0.0f, -900.0f), CEnemy::ENEMYTYPE_LOITERING_Z);
	m_pEnemy[4] = CEnemy::Create(D3DXVECTOR3(400, 0.0f, -1200.0f), CEnemy::ENEMYTYPE_LOITERING_Z);
	m_pEnemy[5] = CEnemy::Create(D3DXVECTOR3(600, 0.0f, -1250.0f), CEnemy::ENEMYTYPE_CHASE);
	m_pEnemy[6] = CEnemy::Create(D3DXVECTOR3(600, 0.0f, -150.0f), CEnemy::ENEMYTYPE_CHASE);

	//使用するオブジェクトモデルの読み込み・オブジェクト生成
	m_pObject->LoadModelText();

	//タイマー処理
	m_pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 -130.0f, 80.0f, 0.0f), CTimer::TIMEMODE_GAME);

	return S_OK;

}
//=============================================================================
// 関数名：タイトルの終了処理
// 関数の概要：使用するテクスチャやモデルの破棄、シーンの破棄
//=============================================================================
void CGame::Uninit()
{
	//シーンの破棄
	CScene::ReleaseSceneAll();

	//プレイヤーのパーツ破棄
	m_pModel->Unload();

	//UIのテクスチャ破棄
	m_pUi->Unload();

	//数字テクスチャの破棄
	m_pNumber->Unload();

	//配置するオブジェクトのテクスチャの破棄
	m_pObject->Unload();

	//プレイヤーのテクスチャの破棄
	m_pPlayer->Unload();

	//敵のテクスチャの破棄
	CEnemy::Unload();


}
//=============================================================================
// 関数名：ゲームの更新処理
// 関数の概要：--
//=============================================================================
void CGame::Update()
{
	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();


	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();


	//if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	//{//Rキーが押されたとき
	//	if (pFade == CFade::FADE_NONE)
	//	{
	//		CFade::SetFade(CManager::MODE_RESULT);//リザルトへ移行
	//	}
	//}
}

//=============================================================================
// 関数名：タイトルの描画処理
// 関数の概要：--
//=============================================================================
void CGame::Draw()
{

}



//=============================================================================
// 関数名：シーンメッシュフィールドクラスの取得
// 関数の概要：シーンメッシュフィールドクラスを返す
//=============================================================================
CMeshField *CGame::GetSceneMeshField(void)
{
	return m_pMeshField;
}

//=============================================================================
// 関数名：プレイヤークラスの取得
// 関数の概要：プレイヤークラスを返す
//=============================================================================
CPlayer * CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// 関数名：敵クラスの取得
// 関数の概要：敵クラスを返す
//=============================================================================
CEnemy * CGame::GetEnemy(int nIndex)
{
	return m_pEnemy[nIndex];
}

