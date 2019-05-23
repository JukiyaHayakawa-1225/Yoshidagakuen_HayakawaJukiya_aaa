//=============================================================================
//
// inputx処理 [xinput.cpp]
// Author : Kishida Rei
//
//=============================================================================
#include "inputx.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_CONTROLLERS			(1)		//コントローラーの最大数
#define	NUM_JOYKEY_MAX			(128)	//キーの最大数


CInputX::InputJoyState CInputX::m_nJoypadState = INPUT_JOYSTATE_NOTPUSH;		//ジョイパッドの状態
bool CInputX::m_bConnectionJoypad = false;			//ジョイパッドの接続状態
LPDIRECTINPUTDEVICE8 m_pDevJoyPad = NULL;			//入力デバイス(ジョイパッド)へのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CInputX::CInputX()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputX::~CInputX()
{
}

//=============================================================================
// ジョイパッドの初期化
//=============================================================================
void CInputX::Init(void)
{
	m_nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	m_bConnectionJoypad = false;

	DWORD dwResult;

	for (DWORD dCnt = 0; dCnt < MAX_CONTROLLERS; dCnt++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(dCnt, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			//MessageBox(0, "コントローラーが接続されています", "", MB_OK );
			// デッドゾーンの設定
			if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
			{
				state.Gamepad.sThumbLX = 0;
				state.Gamepad.sThumbLY = 0;
			}

			if ((state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
				(state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
			{
				state.Gamepad.sThumbRX = 0;
				state.Gamepad.sThumbRY = 0;
			}

			m_bConnectionJoypad = true;
		}
		else
		{
			//MessageBox(0, "コントローラーが接続されていません", "", MB_OK);
			m_bConnectionJoypad = false;
		}
	}
}

//=============================================================================
// ジョイパッドの終了処理
//=============================================================================
void CInputX::Uninit(void)
{
	XInputEnable(false);
}

//=============================================================================
// ジョイパッドの更新処理
//=============================================================================
void CInputX::Update(void)
{

}

CInputX::InputJoyState CInputX::GetJoystate(void)
{
	return m_nJoypadState;
}

bool CInputX::GetbConnect(void)
{
	return m_bConnectionJoypad;
}




//=============================================================================
// ジョイパッドの状態取得
//=============================================================================

