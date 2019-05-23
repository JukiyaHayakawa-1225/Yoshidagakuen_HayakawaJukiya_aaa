//=============================================================================
//
// ゲーム処理 [manager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "game.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "player.h"
#include "meshfield.h"
#include "debugproc.h"
#include "fade.h"
#include "object.h"
#include "map.h"
#include "pause.h"
#include "camera.h"
#include "field.h"
#include "objBillboard.h"
#include "number.h"
#include "score.h"
#include "UI.h"
#include "effect.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CPlayer		*CGame::m_pPlayer = NULL;				//プレイヤーのポインタ情報
CMeshField	*CGame::m_pMeshField = NULL;			//メッシュフィールドのポインタ情報
CMap		*CGame::m_pMap = NULL;					//マップのポインタ情報
CScore		*CGame::m_pScore = NULL;
CObjBillboard *CGame::m_p3Dporigon = NULL;


//=============================================================================
// マクロ定義
//=============================================================================


//=============================================================================
// ゲームのコンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame::~CGame()
{
	m_pPlayer = NULL;		//プレイヤーを初期化
	m_pMeshField = NULL;	//メッシュフィールドを初期化
	m_pMap = NULL;			//マップを初期化
}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame *CGame::Create()
{
	CGame *pGame = NULL;	//ゲームのポインタ
	if (pGame == NULL)
	{
		pGame = new CGame;	//動的確保
		if (pGame != NULL)
		{
			pGame->Init();	//初期化処理
		}
	}
	return pGame;
}

//=============================================================================
// ゲームの初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	CCamera* pCamera = CManager::GetCamera();

	CUI::Load();
	CUI::CreateMode(CManager::MODE_GAME);

	//ポーズの読み込み
	CPause::Load();

	//数字の読み込み
	CNumber::Load();

	//プレイヤーの読み込み
	CPlayer::Load();

	//フィールドの読み込み
	CField::Load();

	//オブジェクトビルボードの読み込み
	CObjBillboard::Load();

	//アイテムビルボートの読み込み
	CItemBillboard::Load();

	//エフェクトの読み込み
	CEffect::Load();

	//マップを生成
	m_pMap = CMap::Create();

	//CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f, 100.0f, 200.0f),100,100);

	//フィールドの生成
	CField::Create(D3DXVECTOR3(0.0f, 0.0f, 1500.0f), 400.0f, 3000.0f);

	//メッシュフィールドの生成
	//CMeshField::Create(D3DXVECTOR3(0.0f,1.0f,0.0f));

	//スコアの生成
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 150.0f, 50.0f, 0.0f), 23.0f, 23.0f);

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 10.0f, 0.0f));

	pCamera->SetTarget(m_pPlayer);

	CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f,50.0f,300));
	CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f, 50.0f, 900));
	CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f, 50.0f, 1500));
	CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f, 50.0f, 2100));
	CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f, 50.0f, 2700));
	CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f, 50.0f, 3300));
	CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f, 50.0f, 3900));
	CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f, 50.0f, 4500));
	CItemBillboard::Create(CItemBillboard::TYPE_PRESENTBOX_0, D3DXVECTOR3(0.0f, 50.0f, 5100));


	for (int nCount = 0; nCount < 50; nCount++)
	{
		m_p3Dporigon = CObjBillboard::Create(CObjBillboard::TYPE_TREE_1, D3DXVECTOR3(100.0f, 100, (nCount*100.0f)));
	}
	for (int nCount = 0; nCount < 50; nCount++)
	{
		m_p3Dporigon = CObjBillboard::Create(CObjBillboard::TYPE_TREE_1, D3DXVECTOR3(-100.0f, 100, (nCount*100.0f)));
	}
	return S_OK;
}

//=============================================================================
// ゲームの終了処理
//=============================================================================
void CGame::Uninit(void)
{

	//アイテムビルボートの読み込み
	CItemBillboard::Unload();

	CUI::Unload();
	//オブジェクトビルボードの破棄
	CObjBillboard::Unload();
	//プレイヤーの破棄
	CPlayer::Unload();
	//ポーズの破棄
	CPause::Unload();
	//数字の破棄
	CNumber::Unload();

	//マップの終了処理
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();
		delete m_pMap;
		m_pMap = NULL;
	}

	//全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// ゲームの更新処理
//=============================================================================
void CGame::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//フェードの取得
	CFade *pFade = CManager::GetFade();
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{//Enterキーを押下した場合
	 //リザルトへ遷移
		//pFade->SetFade(CManager::MODE_RESULT, pFade->FADE_OUT);

	}

	if (m_pPlayer->GetState() == CPlayer::PLAYER_STATE_GOAL)
	{
		pFade->SetFade(CManager::MODE_RESULT, pFade->FADE_OUT);
	}

	//デバッグ表示
#ifdef _DEBUG
	CDebugProc::Print(1, "Game");
#endif // _DEBUG
}

//=============================================================================
// ゲームの描画処理
//=============================================================================
void CGame::Draw(void)
{
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;				//値を返す
}

//=============================================================================
// スコアの取得
//=============================================================================
CScore * CGame::GetScore(void)
{
	return m_pScore;
}

//=============================================================================
// オブジェクト3Dの取得
//=============================================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;				//値を返す
}
