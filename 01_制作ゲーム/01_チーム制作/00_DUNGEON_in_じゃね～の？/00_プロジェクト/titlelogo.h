//=============================================================================
//
// ロゴの処理[2D ポリゴン] [logo.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TITLE_NUM (2)
#define MAX_TITLE_TEXTURE (2)

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CTitleLogo : public CScene
{
public:
	typedef enum
	{
		TITLESTATE_NONE = 0,	//空の状態
		TITLESTATE_NORMAL,		//通常状態
		TITLESTATE_USE,			//エンターが押された状態
		TITLESTATE_MAX			//状態の最大数
	}TITLESTATE;
	typedef enum
	{
		TITLETYPE_LOGO = 0,		//タイトルロゴ
		TITLETYPE_PRESS,		//プレスエンター
		TITLETYPE_MAX			//タイトルの種類の最大数
	}TITLETYPE;
	CTitleLogo();				//コンストラクタ
	~CTitleLogo();				//デストラクタ
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの破棄
	static CTitleLogo *Create(void);	//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TITLE_TEXTURE]; //共有テクスチャのポインタ
	static CScene2D *m_apScene2D[MAX_TITLE_NUM];//シーン2D
	static D3DXCOLOR m_col; //色情報
	int m_nCntFrame;//フレーム数のカウント
	float m_fWidth;//幅
	float m_fHeight;//高さ
	TITLESTATE m_state;//状態
	TITLETYPE m_type;//タイトルの種類
	int m_TexNum;	//テクスチャの番号
	static int m_nJumpRankingFrame;//ランキング画面へ遷移するときのフレームカウンタ
};

#endif