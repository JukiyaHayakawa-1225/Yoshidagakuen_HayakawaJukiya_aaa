//=============================================================================
//
// UI処理 [ui.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene2d.h"	// シーン2D
#include "manager.h"	// マネージャー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define UI_TEXMAX		(16)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CUI : public CScene2D
{// UI クラス
public:
	CUI();								// コンストラクタ
	~CUI();								// デストラクタ

	static HRESULT Load(void);			// ロード
	static void Unload(void);			// アンロード
										// 生成
	static CUI *Create(D3DXVECTOR3 pos, float height, float width, D3DXCOLOR col, D3DXVECTOR2 uv, int nType);
	// 初期化処理
	HRESULT Init(D3DXVECTOR3 pos, float height, float width, D3DXCOLOR col, D3DXVECTOR2 uv, int nType);
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理
	static void CreateMode(CManager::MODE mode);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[UI_TEXMAX];		//  テクスチャ情報へのポインタ
	int m_nType;											//  種類
	int m_nCounterAnim;										//	アニメーション用カウンター
	int m_nPatternAnim;										//	アニメーションパターン
	float fWidthOld;										//	初期の大きさの取得
	bool bFlash;											//	点滅
	bool bCol;												//	色の変更点滅
	int nFlashType;											//	点滅の種類
	int nSizeCount;											//	大きさのカウント

protected:

};

#endif