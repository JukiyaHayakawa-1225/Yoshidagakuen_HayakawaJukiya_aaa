//=============================================================================
//
// フェード処理 [fade.h]
// Author : Kishida Rei
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "manager.h"
//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CManager;

class CFade
{
public:
	CFade();
	~CFade();
	typedef enum
	{
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;

	//static CFade *Create(D3DXVECTOR3 pos, CManager::MODE modeNext);	//プレイヤーの生成
	//HRESULT Init(D3DXVECTOR3 pos, CManager::MODE modeNext);	//初期化処理
	//void Uninit(void);	//終了処理
	//void Update(void);	//更新処理
	//void Draw(void);	//描画処理

	HRESULT Init(CManager::MODE modeNext);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetFade(CManager::MODE modeNext);
	static FADE GetFade(void);
	void SetTexture(float fTex_X_0, float fTex_X_1, float fTex_Y_0, float fTex_Y_1);
	static void AddTex();
private:
	static LPDIRECT3DTEXTURE9		m_pTexture;	// テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9  m_pVtxBuff;	// 頂点バッファへのポインタ
	static FADE					m_fade;					// フェード状態
	static CManager::MODE			m_modeNext;				// 次のモード
	static D3DXCOLOR				m_color;			// フェード色
	static int m_nPatternAnim;	//アニメーションパターン
	static int m_nCounterAnim;//アニメーションカウンター
	static int m_nTex_Y;

};


#endif
