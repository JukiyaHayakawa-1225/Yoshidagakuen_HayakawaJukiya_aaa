//=============================================================================
//
// ロゴの処理[2D ポリゴン] [logo.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _TUTOLOGO_H_
#define _TUTOLOGO_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TUTO_NUM (2)
#define MAX_TIUTO_TEXTURE (2)


//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CTutoLogo : public CScene
{
public:
	typedef enum
	{
		TUTOSTATE_NONE = 0,	//空の状態
		TUTOSTATE_NORMAL,		//通常状態
		TUTOSTATE_USE,			//エンターが押された状態
		TUTOSTATE_MAX			//状態の最大数
	}TUTOSTATE;
	typedef enum
	{
		TIUTOTYPE_LOGO = 0,		//タイトルロゴ
		TIUTOTYPE_PRESS,		//プレスエンター
		TIUTOTYPE_MAX			//タイトルの種類の最大数
	}TIUTOTYPE;
	CTutoLogo();				//コンストラクタ
	~CTutoLogo();				//デストラクタ
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの破棄
	static CTutoLogo *Create(void);	//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TIUTO_TEXTURE]; //共有テクスチャのポインタ
	static CScene2D *m_apScene2D[MAX_TUTO_NUM];//シーン2D
	static D3DXCOLOR m_col; //色情報
	int m_nCntFrame;//フレーム数のカウント
	float m_fWidth;//幅
	float m_fHeight;//高さ
	TUTOSTATE m_state;//状態
	TIUTOTYPE m_type;//タイトルの種類
	int m_TexNum;	//テクスチャの番号
	static int m_nJumpRankingFrame;//ランキング画面へ遷移するときのフレームカウンタ
};

#endif