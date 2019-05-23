//=============================================================================
//
// 背景処理[2D ポリゴン] [bg.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _UIRESULT_H_
#define _UIRESULT_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE_UI_RESULT (4)	//テクスチャ数
#define MAX_UI_RESULT (4)			//UI数

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CUiResult : public CScene
{
public:
	typedef enum
	{
		UI_TYPE_RESULT = 0,		// リザルト
		UI_TYPE_WATCH,			// 時計
		UI_TYPE_MINUTES,		// 分
		UI_TYPE_DOT,			// ドット
		UI_TYP_MAX				//UIの最大数
	}UITYPE;//UIの種類
	CUiResult(int nPriority = 6, OBJTYPE type = OBJTYPE_UIRESULT);	//コンストラクタ
	~CUiResult();	//デストラクタ
	static CUiResult *Create(void);	//背景の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_UI_RESULT]; //共有テクスチャのポインタ
	D3DXCOLOR m_Color;
	int m_nCntColor;	//カラーカウント用
	CScene2D *m_pScene2D[MAX_UI_RESULT];
	UITYPE m_Type;	//背景のタイプ
};

#endif