// レンダリング処理 [renderer.h]
// Author :岸田怜(Kishida Rei)
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

class CFade;
class CPause;
//*****************************************************************************
// 描画のクラス構造体
//*****************************************************************************
class CRenderer
{
public:
	CRenderer::CRenderer();	//コンストラクタ
	CRenderer::~CRenderer();	//デストラクタ
	HRESULT Init(HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	LPDIRECT3DDEVICE9 GetDevice(void);	//デバイスの取得
	static void SetPause(void);
	static bool GetPause(void);

private:
#ifdef _DEBUG
	void DrawFPS(void);			//FPS表示処理
#endif

	LPDIRECT3D9				m_pD3D;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)

	int m_nCntFPS;//FPS

#ifdef _DEBUG
	LPD3DXFONT				m_pFont;			// フォントへのポインタ
#endif
	static CFade *m_pFade;//フェード
	static CPause *m_pPause;//ポーズ
	static bool m_bPause;	//ポーズしているか


};

#endif