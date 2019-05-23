//=============================================================================
//
// 入力処理 [inout.h]
// Author :岸田怜(KishidaRei)
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX (256)

//*****************************************************************************
// 入力のクラス構造体
//*****************************************************************************
class CInput
{
public:
	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 m_pDevice;
};

class CInputKeyboard : public CInput
{
public:
	CInputKeyboard::CInputKeyboard();
	CInputKeyboard::~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRelease(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyTrigger[NUM_KEY_MAX];
	BYTE m_aKeyRelease[NUM_KEY_MAX];
};



#endif
