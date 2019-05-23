//=============================================================================
//
// 背景処理[2D ポリゴン] [bg.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _UIRANKING_H_
#define _UIRANKING_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE_UI_RANKING (4)	//テクスチャ数
#define MAX_UI_RANKING (12)			//UI数

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CUiRanking : public CScene
{
public:
	typedef enum
	{
		UITEX_RANKING = 0,		//ランキング
		UITEX_RANK,				//順位
		UITEX_MINUTES,			//分
		UITEX_DOT,				//ドット
		UITEX_MAX				//UIの最大数
	}UITEX;//UIの種類

	typedef enum
	{
		UI_TYPE_RANKING = 0,		// リザルト
		UI_TYPE_RANK,				// 時計
		UI_TYPE_FIRSTMINUTES,		// 1st分
		UI_TYPE_FIRSTDOT,			// 1stドット
		UI_TYPE_SECONDMINUTES,		// 2nd分
		UI_TYPE_SECONDDOT,			// 2ndドット
		UI_TYPE_THIRDMINUTES,		// 3rd分
		UI_TYPE_THIRDDOT,			// 3rdドット
		UI_TYPE_FORTHMINUTES,		// 4th分
		UI_TYPE_FORTHDOT,			// 4hドット
		UI_TYPE_FIFTHMINUTES,		// 5th分
		UI_TYPE_FIFTHDOT,			// 5thドット
		UI_TYPE_MAX				//UIの最大数
	}UITYPE;//UIの種類
	CUiRanking(int nPriority = 6, OBJTYPE type = OBJTYPE_UIRESULT);	//コンストラクタ
	~CUiRanking();	//デストラクタ
	static CUiRanking *Create(void);	//背景の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);//テクスチャの読み込み
	static void Unload(void);//テクスチャの破棄

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_UI_RANKING]; //共有テクスチャのポインタ
	D3DXCOLOR m_Color;
	int m_nCntColor;	//カラーカウント用
	CScene2D *m_pScene2D[MAX_UI_RANKING];
	UITYPE m_Type;	//背景のタイプ
};

#endif