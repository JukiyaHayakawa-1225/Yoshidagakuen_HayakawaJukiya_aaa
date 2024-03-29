//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "debugproc.h"
#include "fade.h"
#include "pause.h"
#include "input.h"
#include "sound.h"
//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CPause *CRenderer::m_pPause = NULL;
bool CRenderer::m_bPause = NULL;

//=============================================================================
// レンダリングのコンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			// Direct3Dオブジェクト
	m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
}
//=============================================================================
// レンダリングのデストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	//全ての開放処理
	CScene::ReleaseAll();

	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount			 = 1;							// バックバッファの数
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed					 = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF, hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);					//1頂点,2ワイヤー,3通常

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1 , D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	// デバイスの破棄
	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if(m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
	RelesePause();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//モードの取得
	CManager::MODE mode;
	mode = CManager::GetMode();

	//サウンドの取得
	CSound *pSound;
	pSound = CManager::GetSound();

	//フェードの取得
	CFade::FADE fade;
	CFade *pFade = NULL;
	fade = pFade->GetFade();
#ifdef _DEBUG
	CDebugProc::Print(0, "FPS:%d\n", GetFPS());
#endif
	if (pInputKeyboard->GetTrigger(DIK_P) == true || pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_START) == true)
	{//ポーズのON/OFF
		if (fade == CFade::FADE_NONE && mode == CManager::MODE_GAME)
		{
			m_bPause = m_bPause ? false : true;
			if (m_bPause == true)
			{
				if (m_pPause == NULL)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE_PAUSE);
					m_pPause = CPause::Create();
				}
			}
			else
			{
				RelesePause();
				/*if (m_pPause != NULL)
				{
					m_pPause->Uninit();
					m_pPause = NULL;
				}*/
			}
		}
	}

	if (m_bPause == false)
	{
		//全ての更新処理
		CScene::UpdateAll();
	}
	else if (m_bPause == true)
	{
		if (m_pPause != NULL)
		{
			m_pPause->Update();
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	CFade *pFade;
	pFade = CManager::GetFade();

	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(100, 149, 237, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//全てのオブジェクトの描画処理
		CScene::DrawAll();

		pFade->Draw();
#ifdef _DEBUG
		//デバッグの表示
		CDebugProc::Draw();
#endif
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}
//=============================================================================
// ポーズの開放
//=============================================================================
void CRenderer::RelesePause(void)
{
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
		m_bPause = false;
	}
}
//=============================================================================
// ポーズの取得
//=============================================================================
bool CRenderer::GetPause(void)
{
	return m_bPause;
}