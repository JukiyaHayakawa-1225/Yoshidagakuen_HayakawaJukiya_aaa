//=============================================================================
//
// タイトルロゴ処理[2Dポリゴン] [titlelogo.cpp]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "titlelogo.h"
#include "fade.h"
#include "sound.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOGO_WIDTH (650.0f)	//タイトルロゴの幅
#define LOGO_HEIGHT (250.0f) //タイトルロゴのYの高さ
#define PRESS_WIDTH (350.0f) //PressEnterの幅
#define PRESS_HEIGHT (120.0f) //PressEnterの高さ

#define PATTERN_ANIM (5)	//テクスチャパターン数

#define ANIMATION_SPEED (4)	//アニメーションスピード
#define TEX_X (0.25f)	//テクスチャ座標_X
#define TEX_Y (0.25f)//テクスチャ座標_Y

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTitleLogo::m_apTexture[MAX_TITLE_TEXTURE] = {}; //共有テクスチャのポインタ
CScene2D *CTitleLogo::m_apScene2D[MAX_TITLE_NUM] = {};//シーン2Dのポインタ
D3DXCOLOR CTitleLogo::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色情報
int CTitleLogo::m_nJumpRankingFrame = 0;//ランキング画面へ遷移するときのフレームカウンタ

//=============================================================================
//コンストラクタ
//=============================================================================
CTitleLogo::CTitleLogo() : CScene(5)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
//テクスチャの読み込み
//=============================================================================
HRESULT CTitleLogo::Load(void)
{
	//レンダラーの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UI_TITLE.png", &m_apTexture[0]);//タイトルロゴ[パターン1]
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter2.png", &m_apTexture[1]);//タイトルロゴ[パターン2]

	return S_OK;
}

//=============================================================================
//テクスチャの破棄
//=============================================================================
void CTitleLogo::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TITLE_TEXTURE; nCntTex++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}



//=============================================================================
//タイトルロゴの生成
//=============================================================================
CTitleLogo *CTitleLogo::Create(void)
{
	//タイトルロゴのインスタンス生成
	CTitleLogo *pTitle;
	pTitle = new CTitleLogo;
	if (pTitle != NULL)
	{
		//タイトルロゴの初期化処理
		pTitle->Init();
	}

	return pTitle;
}


//=============================================================================
//初期化処理
//=============================================================================
HRESULT CTitleLogo::Init(void)
{
	//フレームの初期化
	m_nJumpRankingFrame = 0;
	m_nCntFrame = 0;
	//色情報の初期化
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_NUM; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case TITLETYPE_LOGO:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 170.0f, 0.0f), LOGO_WIDTH, LOGO_HEIGHT);
			//タイトルの種類の設定
			m_type = TITLETYPE_LOGO;
			m_state = TITLESTATE_NONE;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[0]);
			break;
		case TITLETYPE_PRESS:
			//2Dポリゴンの生成
			m_apScene2D[nCntTitle] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 250.0f, 0.0f), PRESS_WIDTH, PRESS_HEIGHT);
			m_type = TITLETYPE_PRESS;
			m_state = TITLESTATE_NORMAL;
			//テクスチャの割り当て
			m_apScene2D[nCntTitle]->BindTexture(m_apTexture[1]);
			break;
		}

			//タイトルロゴのオブジェクトの設定
			SetObjType(CScene::OBJTYPE_TITLELOGO);
	}
	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CTitleLogo::Uninit(void)
{
	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_NUM; nCntTitle++)
	{
		if (m_apScene2D[nCntTitle] != NULL)
		{
			//2Dシーンの終了処理
			m_apScene2D[nCntTitle]->Uninit();
			m_apScene2D[nCntTitle] = NULL;

		}

	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
//更新処理
//=============================================================================
void CTitleLogo::Update(void)
{
	CInputX::InputJoyState JoyState = CInputX::GetJoystate();
	bool bConnect = CInputX::GetbConnect();			//ジョイパッドの接続状態

	//状態
	XINPUT_STATE state;
	XInputGetState(0, &state);



	//ランキングへジャンプするフレームのカウント
	m_nJumpRankingFrame++;

	//フェードの取得
	CFade::FADE pFade;
	pFade = CFade::GetFade();

	//デバイスの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	D3DXVECTOR3 posTitle[MAX_TITLE_NUM];//各種位置情報


	if (pFade == CFade::FADE_NONE)
	{
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && bConnect == true))
		{//RETURNキーが押されたとき
			if (JoyState == CInputX::INPUT_JOYSTATE_NOTPUSH)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
				m_state = TITLESTATE_USE;//使用状態に
				CFade::SetFade(CManager::MODE_TUTORIAL);//メニューへ移行
			}
			JoyState = CInputX::INPUT_JOYSTATE_PUSH;
		}
		if (m_nJumpRankingFrame % 600 == 0)
		{
			CManager::SetTimer(9, 9, 9, 9, 9, 9);
			CFade::SetFade(CManager::MODE_RANKING);//ランキングへへ移行

		}
	}

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE_NUM; nCntTitle++)
	{
		posTitle[nCntTitle] = m_apScene2D[nCntTitle]->GetPosPolygon();	//位置情報の取得

		switch (m_type)
		{
		case TITLETYPE_LOGO:
			break;
		case TITLETYPE_PRESS:
			if (m_state == TITLESTATE_NORMAL)
			{//通常状態のとき
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 280;
				if (m_nCntFrame < 140)
				{
					m_col.a -= 0.01f;
				}
				if (m_nCntFrame > 140)
				{
					m_col.a += 0.01f;
				}
				//色情報の反映
				m_apScene2D[1]->SetColor(m_col);
			}
			else if (m_state == TITLESTATE_USE)
			{//ボタンが押されたとき
				m_nCntFrame++;
				m_nCntFrame = m_nCntFrame % 6;
				if (m_nCntFrame < 3)
				{
					m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//色を白に
				}
				if (m_nCntFrame > 3)
				{
					m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);//色を赤に
				}
				//色情報の反映
				m_apScene2D[1]->SetColor(m_col);
			}
			break;
		}

		//位置情報の設定
		m_apScene2D[nCntTitle]->SetPosPolygon(posTitle[nCntTitle]);

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
//描画処理
//=============================================================================
void CTitleLogo::Draw(void)
{


}

