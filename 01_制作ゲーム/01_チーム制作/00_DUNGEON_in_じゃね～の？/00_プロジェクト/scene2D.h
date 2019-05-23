//=============================================================================
//
// シーン2D処理[2D ポリゴン] [scene2D.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
// シーン2Dのクラス構造体
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D::CScene2D(int nPriority = 6, OBJTYPE type = OBJTYPE_SCENE2D);	//コンストラクタ
	CScene2D::~CScene2D();	//デストラクタ
	static CScene2D *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void DrawEffect(void); //描画処理
	int GetNum2DSceneAll(void);	//2Dシーン数の取得
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て
	void SetTexture(float fTex_X, float fTex_Y);//テクスチャの設定
	void SetColor(D3DXCOLOR col);	//色の設定
	void SetPriority(int nPriority);//優先順位

	//背景スクロール
	void ScrollBg(D3DXVECTOR2 Scroll_0, D3DXVECTOR2 Scroll_1, D3DXVECTOR2 Scroll_2, D3DXVECTOR2 Scroll_3);

	//テクスチャアニメーション
	void SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim);
	void SetTextureAnimY(float fTex_X, float fTex_Y, int nPatternAnim, int nTex_Y);

	//ポリゴンの大きさの取得、設定
	void SetPolygonWidth(float fWidth);
	void SetPolygonHeight(float fWidth);

	//ポリゴンの位置情報の取得、設定
	D3DXVECTOR3 GetPosPolygon(void);
	void SetPosPolygon(D3DXVECTOR3 pos);

	//ポリゴンの角度情報の設定、設定
	D3DXVECTOR3 GetRotPolygon(void);
	void SetRotPolygon(D3DXVECTOR3 rot);

	//ポリゴンの長さの取得、設定
	float GetLengthPolygon(void);
	void SetLengthPolygon(float fLength);

	//ポリゴンの大きさの取得、設定
	float GetWidthPolygon(void);
	float GetHeightPolygon(void);


protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_posPolygon;			// ポリゴンの位置
	D3DXVECTOR3				m_rotPolygon;			// ポリゴンの回転
	float m_fLengthPolygon;	//ポリゴンの長さ
	float m_fAnglePolygon;	//ポリゴンの角度
	float m_fPolygonWidth;	//ポリゴンの幅
	float m_fPolygonHeight;	//ポリゴンの高さ
	int m_nCntflame;		//カウント
	int m_nNumScene2D;	//2Dポリゴン数
	static CManager *m_pManager;//マネージャ
	int m_nPriority;
};

#endif