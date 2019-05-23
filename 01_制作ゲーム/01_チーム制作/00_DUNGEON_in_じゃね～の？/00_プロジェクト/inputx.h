//=============================================================================
//
// inputx処理 [inputx.h]
// Author : komatsu keisuke
//
//=============================================================================
#ifndef _INPUTX_H_
#define _INPUTX_H_

#include "main.h"

//=============================================================================
// ジョイパッドの構造体
//=============================================================================
class CInputX
{

public:
	typedef enum
	{
		INPUT_JOYSTATE_NOTPUSH = 0,
		INPUT_JOYSTATE_PUSH,
		INPUT_JOYSTATE_NONE,
		INPUT_JOYSTATE_MAX
	}InputJoyState;


	CInputX();
	~CInputX();
	void Init(void);
	void Uninit(void);
	void Update(void);

	static InputJoyState GetJoystate(void);
	static bool GetbConnect(void);
private:
	static InputJoyState m_nJoypadState;		//ジョイパッドの状態
	static bool m_bConnectionJoypad;			//ジョイパッドの接続状態
	static LPDIRECTINPUTDEVICE8 m_pDevJoyPad;			//入力デバイス(ジョイパッド)へのポインタ

};

#endif