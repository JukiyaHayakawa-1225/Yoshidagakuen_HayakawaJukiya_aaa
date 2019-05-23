//=============================================================================
//
// 入力処理 [input.cpp]
// Author :岸田怜(Kishida Rei)
//
//=============================================================================
#include "input.h"

//*****************************************************************************
//静的メンバ変数
//*****************************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;	//入力のポインタ

//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CInput::CInput()
{//クリアな値を代入
	m_pDevice = NULL;
}


//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CInput::~CInput()
{

}
//=============================================================================
// 関数名：入力の初期化処理
// 関数の概要：DirectInputの生成
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 関数名：入力の終了処理
// 関数の概要：デバイスの破棄、入力の破棄
//=============================================================================
void CInput::Uninit(void)
{
	//デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//入力の破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}




//=============================================================================
//
//
//								CInputKeyboard
//							  ～キーボードの処理～
//
//=============================================================================
//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0;
		m_aKeyTrigger[nCntKey] = 0;
		m_aKeyRelease[nCntKey] = 0;
	}
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：最後に呼び出される
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}
//=============================================================================
// 関数名：キーボードの初期化処理
// 関数の概要：キーボードの生成
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// 関数名：キーボードの終了処理
// 関数の概要：入力の破棄
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// 関数名：キーボードの更新処理
// 関数の概要：Press, Trigger, Releaseの処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];	//トリガー
			m_aKeyRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];	//リリース
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	//キーボードの入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();	//キーボードへのアクセス権を獲得
	}
}

//=============================================================================
// 関数名：キーボードの入力情報(プレス情報)を取得
// 関数の概要：Press情報を取得
//=============================================================================

bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 関数名：キーボードの入力情報(トリガー情報)を取得
// 関数の概要：Trigger情報を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return (m_aKeyTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// 関数名：キーボードの入力情報(リリース)を取得
// 関数の概要：Trigger情報を取得
//=============================================================================
bool CInputKeyboard::GetKeyboardRelease(int nKey)
{
	return (m_aKeyRelease[nKey] & 0x80) ? true : false;
}
