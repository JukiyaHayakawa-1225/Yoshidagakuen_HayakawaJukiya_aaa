//=============================================================================
//
// 数字処理[2D ポリゴン] [number.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//前方宣言 --スペルミスに注意！(なんでも通るから！)
class CNumber
{
public:

	CNumber();	//コンストラクタ
	~CNumber();	//デストラクタ
	static CNumber *Create(D3DXVECTOR3 pos, float width, float height);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static HRESULT Load(void);
	static void Unload(void);
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て
	void SetTexture(float fTex_X, float fTex_Y);//テクスチャの設定
	void SetColor(D3DXCOLOR col);	//色の設定

	void SetNumber(int nNumber);


	//テクスチャアニメーション
	void SetTextureAnim(float fTex_X, float fTex_Y, int nPatternAnim);

	//ポリゴンの大きさの取得、設定
	void SetNumberWidth(float fWidth);
	void SetNumberHeight(float fWidth);

	//ポリゴンの位置情報の取得、設定
	D3DXVECTOR3 GetPosNumber(void);
	void SetPosNumber(D3DXVECTOR3 pos);

	//ポリゴンの角度情報の設定、設定
	D3DXVECTOR3 GetRotNumber(void);
	void SetRotNumber(D3DXVECTOR3 rot);

	//ポリゴンの長さの取得、設定
	float GetLengthNumber(void);
	void SetLengthNumber(float fLength);

	//ポリゴンの大きさの取得、設定
	float GetWidthNumber(void);
	float GetHeightNumber(void);



private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// テクスチャへのポインタ
	 LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXVECTOR3				m_posNumber;			// ポリゴンの位置
	D3DXVECTOR3				m_rotNumber;			// ポリゴンの回転
	float m_fLengthNumber;	//ポリゴンの長さ
	float m_fAngleNumber;	//ポリゴンの角度
	float m_fNumberWidth;	//ポリゴンの幅
	float m_fNumberHeight;	//ポリゴンの高さ
	int m_nNumber;
};

#endif