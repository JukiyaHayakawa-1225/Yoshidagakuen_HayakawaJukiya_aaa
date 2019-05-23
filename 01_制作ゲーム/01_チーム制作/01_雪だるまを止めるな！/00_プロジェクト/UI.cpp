//=============================================================================
//
// UI処理 [UIe.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ui.h"				// タイトルフレーム
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "particle2D.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TITLE000		"data\\TEXTURE\\UI\\titlelogo.png"			// タイトルロゴテクスチャ
#define	TEXTURE_PRESS			"data\\TEXTURE\\UI\\press.png"				// PRESS
#define	TEXTURE_TUTORIAL000		"data\\TEXTURE\\7.png"			// チュートリアル
#define	TEXTURE_GAME000			"data\\TEXTURE\\UI\\playersizeui.png"		// プレイヤーのサイズのUI
#define	TEXTURE_GAME001			"data\\TEXTURE\\UI\\minmapplayer.png"		// ミニマップのプレイヤー
#define	TEXTURE_GAME002			"data\\TEXTURE\\UI\\minmap.png"				// ミニマップ
#define	TEXTURE_RESULT000		"data\\TEXTURE\\UI\\resultlogo.png"			// フィニッシュ
#define	TEXTURE_PLAYER			"data\\TEXTURE\\UI\\player.png"				// プレイヤー

#define	TEXTURE_GAMEMOVEUI000	"data\\TEXTURE\\UI\\move.png"				// 移動キー
#define	TEXTURE_GAMEMOVEUI001	"data\\TEXTURE\\UI\\jump.png"				// ジャンプ
#define	TEXTURE_GAMEMOVEUI002	"data\\TEXTURE\\UI\\small.png"				// 縮小
#define	TEXTURE_GAMEMOVEUI003	"data\\TEXTURE\\UI\\bikku.png"				// 拡大

#define	TEXTURE_RANKING			"data\\TEXTURE\\UI\\rankinglogo.png"		// ランキングロゴ
#define	TEXTURE_RANKING1		"data\\TEXTURE\\UI\\ranking1.png"		// 1
#define	TEXTURE_RANKING2		"data\\TEXTURE\\UI\\ranking2.png"		// 2
#define	TEXTURE_RANKING345		"data\\TEXTURE\\UI\\ranking345.png"		// 345


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CUI::m_pTexture[UI_TEXMAX] = {};			// テクスチャ情報へのポインタ

																//=============================================================================
																// コンストラクタ
																//=============================================================================
CUI::CUI() : CScene2D(7, OBJTYPE_SCENE2D)
{
	m_nType = 0;								//	種類
	m_nCounterAnim = 0;							//	アニメーション用カウンター
	m_nPatternAnim = 0;							//	アニメーションパターン
	fWidthOld = 0.0f;							//	初期の大きさの位置
	bFlash = false;								//	点滅しない
	nSizeCount = 0;								//	大きさのカウント
	nFlashType = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CUI::~CUI()
{

}
//=============================================================================
// ロード処理
//=============================================================================
HRESULT CUI::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE000, &m_pTexture[0]);		//	タイトルロゴ
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PRESS, &m_pTexture[1]);			//	PRESS
	D3DXCreateTextureFromFile(pDevice, TEXTURE_TUTORIAL000, &m_pTexture[2]);	//	タイトルロゴ
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME000, &m_pTexture[3]);		//	プレイヤーのサイズのUI
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME001, &m_pTexture[4]);		//	プレイヤーのサイズのUI
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME002, &m_pTexture[5]);		//	ミニマップ
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RESULT000, &m_pTexture[6]);		//	フィニッシュ
	D3DXCreateTextureFromFile(pDevice, TEXTURE_PLAYER, &m_pTexture[7]);			//	プレイヤー

	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAMEMOVEUI000, &m_pTexture[8]);	//	移動
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAMEMOVEUI001, &m_pTexture[9]);	//	ジャンプ
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAMEMOVEUI002, &m_pTexture[10]);	//	縮小
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAMEMOVEUI003, &m_pTexture[11]);	//	拡大

	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING, &m_pTexture[12]);		//	ランキングロゴ
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING1, &m_pTexture[13]);		//	1
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING2, &m_pTexture[14]);		//	2
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING345, &m_pTexture[15]);	//	345

	return S_OK;
}

//=============================================================================
// アンロード処理
//=============================================================================
void CUI::Unload(void)
{
	for (int nCntTex = 0; nCntTex < UI_TEXMAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{// NULLの場合
		 // テクスチャの生成
			m_pTexture[nCntTex]->Release();		// 解放
			m_pTexture[nCntTex] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CUI *CUI::Create(D3DXVECTOR3 pos, float height, float width, D3DXCOLOR col, D3DXVECTOR2 uv, int nType)
{
	CUI *pUI = {};				// Uiポインタ

	if (pUI == NULL)
	{// NULLの場合// メモリ確保
		pUI = new CUI;
		if (pUI != NULL)
		{// NULL以外の場合
			pUI->Init(pos, height, width, col, uv, nType);			// 初期化処理
		}
	}
	return pUI;
}

//=============================================================================
// UI作成するモード
//=============================================================================
void CUI::CreateMode(CManager::MODE mode)
{
	switch (mode)
	{
	case CManager::MODE_TITLE://		タイトル			-------------------------------------------------------------------------------------------------------------------------------------
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400, SCREEN_HEIGHT / 2 - 100, 0.0f), 200.0f, 200.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 7);		//	プレイヤー
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400, SCREEN_HEIGHT / 2 - 100, 0.0f), 200.0f, 200.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 7);		//	プレイヤー
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f), 200.0f, 400.0f, D3DXCOLOR(0.8f, 0.8f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 0);			//	タイトルロゴ
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), 100.0f, 400.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 1);				//	PRESS
		break;
	case CManager::MODE_TUTORIAL://		チュートリアル		-------------------------------------------------------------------------------------------------------------------------------------
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 2);	//	チュートリアル
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT / 2 + 300, 0.0f), 100.0f, 400.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 1);		//	PRESS
		break;
	case CManager::MODE_GAME://			ゲーム				-------------------------------------------------------------------------------------------------------------------------------------
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 450, SCREEN_HEIGHT / 2 - 200, 0.0f), 100.0f, 100.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 3);		//	プレイヤーのサイズのUI
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 550, SCREEN_HEIGHT / 2 - 150, 0.0f), 150.0f, 30.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 5);			//	ミニマップ
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 550, SCREEN_HEIGHT / 2, 0.0f), 30.0f, 30.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 4);				//	プレイヤーのサイズのUI

		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT / 2 + 60, 0.0f), 40, 70, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 8);					//	移動
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT / 2 + 140, 0.0f), 40, 70, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 9);				//	ジャンプ
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT / 2 + 210, 0.0f), 40, 70, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 10);				//	縮小
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT / 2 + 280, 0.0f), 40, 70, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 11);				//	拡大

		break;
	case CManager::MODE_RESULT://		リザルト			-------------------------------------------------------------------------------------------------------------------------------------
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f), 100.0f, 400.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 6);				//	フィニッシュ
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT / 2 + 300, 0.0f), 100.0f, 400.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 1);		//	PRESS
		break;
	case CManager::MODE_RANKING://		ランキング			-------------------------------------------------------------------------------------------------------------------------------------
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400, SCREEN_HEIGHT / 2, 0.0f), 350.0f, 300.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 7);				//	プレイヤー
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 290, 0.0f), 80.0f, 350.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 12);				//	ランキングロゴ
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 180, 0.0f), 90.0f, 110.0f, D3DXCOLOR(0.5, 0.5, 0.5, 1.0), D3DXVECTOR2(1.0f, 1.0f), 13);			//	1
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, 0.0f), 80.0f, 100.0f, D3DXCOLOR(0.5, 0.5, 0.5, 1.0), D3DXVECTOR2(1.0f, 1.0f), 14);			//	2
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 160, 0.0f), 160.0f, 100.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 15);			//	345
		CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250, SCREEN_HEIGHT / 2 + 350, 0.0f), 50.0f, 200.0f, D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 1);			//	PRESS

		break;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUI::Init(D3DXVECTOR3 pos, float height, float width, D3DXCOLOR col, D3DXVECTOR2 uv, int nType)
{
	CScene2D::Init();							//	初期化処理
	CScene2D::SetHeight(height);				//	幅の設定
	CScene2D::SetWidth(width);					//	高さの設定
	CScene2D::SetVtxPos(pos);					//	位置の設定
	CScene2D::SetCol(col);						//	色の設定
	CScene2D::BindTexture(m_pTexture[nType]);	//	テクスチャ割り当て
												//	種類の設定
	m_nType = nType;
	//	幅の初期位置
	fWidthOld = width;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUI::Uninit(void)
{
	CScene2D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理
//=============================================================================
void CUI::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	D3DXVECTOR3 pos = CScene2D::GetPos();		//	位置の取得
	float fWidth = CScene2D::GetWidth();		//	幅の取得
	float fHeight = CScene2D::GetHeight();		//	大きさの取得
	D3DXCOLOR col = CScene2D::GetCol();			//	色の取得

	m_nCounterAnim++;

	if (m_nType == 1)
	{//	Press
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoypad->GetTrigger(pInputJoypad->DIJS_BUTTON_A) == true)
		{
			nFlashType = 1;
		}
		switch (nFlashType)
		{
		case 0:	//	点滅早くする
			if ((m_nCounterAnim % 60) == 0)
			{
				bFlash = bFlash ? false : true;
			}
			break;
		case 1:	//	点滅遅くする
			if ((m_nCounterAnim % 6) == 0)
			{
				bFlash = bFlash ? false : true;
			}
			break;
		}
		if (bFlash == true)
		{//	サイズをなくす
			fWidth = 0;
		}
		else if (bFlash == false)
		{//	サイズを初期の大木さんイ戻す
			fWidth = fWidthOld;
		}
	}
	if (m_nType == 3)
	{//	プレイヤーの大きさUI
		if (pInputKeyboard->GetTrigger(DIK_K) == true || pInputJoypad->GetTrigger(pInputJoypad->DIJS_BUTTON_RB) == true)
		{//	拡大
			if (nSizeCount < 2)
			{
				nSizeCount += 1;
				fWidth += 15;
				fHeight += 15;
			}
		}
		else if (pInputKeyboard->GetTrigger(DIK_J) == true || pInputJoypad->GetTrigger(pInputJoypad->DIJS_BUTTON_LB) == true)
		{//	縮小
			if (nSizeCount >= 0)
			{
				nSizeCount -= 1;
				fWidth -= 15;
				fHeight -= 15;
			}
		}
	}
	if (m_nType == 4)
	{//	ミニマップのプレイヤー

		if (pos.y >= 100.0f)
		{
			pos.y -= 0.2f;
		}
	}
	if (m_nType == 6)
	{//	フィニッシュ
		if ((m_nCounterAnim % 30) == 0)
		{
			bCol = bCol ? false : true;
		}
		if (bCol == true)
		{
			col.r = 0.1f;
			fWidth += 2;
			fHeight += 2;
		}
		else if (bCol == false)
		{
			CParticle2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2, 0.0f),
				D3DXVECTOR3((sinf((rand() % 628) / 100.0f) * ((rand() % (int)10))), (cosf((rand() % 628) / 100.0f) * ((rand() % (int)10))), 0.0f), D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), 50);
			CParticle2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2, 0.0f),
				D3DXVECTOR3((sinf((rand() % 628) / 100.0f) * ((rand() % (int)10))), (cosf((rand() % 628) / 100.0f) * ((rand() % (int)10))), 0.0f), D3DXCOLOR(1.0, 1.0, 1.0f, 1.0), 50);
			col.r = 1.0f;
			fWidth -= 2;
			fHeight -= 2;
		}
	}
	if (m_nType == 7)
	{//	プレイヤーロゴ
		if ((m_nCounterAnim % 6) == 0)
		{
			bFlash = bFlash ? false : true;
		}
		if (bFlash == true)
		{//	プレイヤー大きさ変更
			fHeight += 2;
			fWidth -= 2;
		}
		if (bFlash == false)
		{//	プレイヤー大きさ変更
			fHeight -= 2;
			fWidth += 2;
		}
	}
	if (m_nType == 8)
	{//	移動キー
		if (pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_D) == true
			|| pInputJoypad->GetPress(pInputJoypad->DIJS_BUTTON_LSTICK_LEFT) == true
			|| pInputJoypad->GetPress(pInputJoypad->DIJS_BUTTON_LSTICK_RIGHT) == true
			|| pInputJoypad->GetPress(pInputJoypad->DIJS_BUTTON_LEFT) == true
			|| pInputJoypad->GetPress(pInputJoypad->DIJS_BUTTON_RIGHT) == true)
		{//	色最大値
			col.r = 1.0f;
			col.g = 1.0f;
			col.b = 1.0f;
		}
		else
		{//	色半減
			col.r = 0.5f;
			col.g = 0.5f;
			col.b = 0.5f;
		}
	}
	if (m_nType == 9)
	{//	ジャンプ
		if (pInputKeyboard->GetPress(DIK_SPACE) == true || pInputJoypad->GetPress(pInputJoypad->DIJS_BUTTON_A) == true)
		{
			col.r = 1.0f;
			col.g = 1.0f;
			col.b = 1.0f;
		}
		else
		{//	色半減
			col.r = 0.5f;
			col.g = 0.5f;
			col.b = 0.5f;
		}
	}
	if (m_nType == 10)
	{//	縮小
		if (pInputKeyboard->GetPress(DIK_J) == true || pInputJoypad->GetPress(pInputJoypad->DIJS_BUTTON_LB) == true)
		{
			col.r = 1.0f;
			col.g = 1.0f;
			col.b = 1.0f;
		}
		else
		{//	色半減
			col.r = 0.5f;
			col.g = 0.5f;
			col.b = 0.5f;
		}
	}
	if (m_nType == 11)
	{//	拡大
		if (pInputKeyboard->GetPress(DIK_K) == true || pInputJoypad->GetPress(pInputJoypad->DIJS_BUTTON_RB) == true)
		{
			col.r = 1.0f;
			col.g = 1.0f;
			col.b = 1.0f;
		}
		else
		{//	色半減
			col.r = 0.5f;
			col.g = 0.5f;
			col.b = 0.5f;
		}
	}

	if (m_nType == 13)
	{//	1位
		if ((m_nCounterAnim % 6) == 0)
		{
			bFlash = bFlash ? false : true;
		}
		if (bFlash == true)
		{//	プレイヤー大きさ変更
			fHeight += 2;
			fWidth -= 2;
		}
		if (bFlash == false)
		{//	プレイヤー大きさ変更
			fHeight -= 2;
			fWidth += 2;
			col.r = 1.0f;
		}
	}
	if (m_nType == 14)
	{//	2位
		if ((m_nCounterAnim % 12) == 0)
		{
			bFlash = bFlash ? false : true;
		}
		if (bFlash == true)
		{//	プレイヤー大きさ変更
			fHeight += 2;
			fWidth -= 2;
		}
		if (bFlash == false)
		{//	プレイヤー大きさ変更
			fHeight -= 2;
			fWidth += 2;
			col.b = 1.0f;
		}
	}

	CScene2D::SetVtxPos(pos);					//	位置の設定
	CScene2D::SetWidth(fWidth);					//	幅の設定
	CScene2D::SetHeight(fHeight);				//	高さの設定
	CScene2D::SetCol(col);						//	色の設定
}

//=============================================================================
// 描画処理
//=============================================================================
void CUI::Draw(void)
{
	CScene2D::Draw();						// 描画処理
}