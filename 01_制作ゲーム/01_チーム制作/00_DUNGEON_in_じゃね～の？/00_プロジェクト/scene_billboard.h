//=============================================================================
//
// ビルボード処理[3D ポリゴン] [billboard.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENEBILLBOARD_H_
#define _SCENEBILLBOARD_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//前方宣言 --スペルミスに注意！(なんでも通るから！)
//*****************************************************************************
// シーン3Dのクラス構造体
//*****************************************************************************
class CSceneBillBoard : public CScene
{
public:
	CSceneBillBoard(int nPriority = 4, OBJTYPE type = OBJTYPE_SCENEBILLBOARD);	//コンストラクタ
	~CSceneBillBoard();	//デストラクタ
	static CSceneBillBoard *Create(D3DXVECTOR3 pos);//敵の生成
	HRESULT Init(void);					//初期化処理
	void Uninit(void);					//終了処理
	void Update(void);					//更新処理
	void Draw(void);					//描画処理
	void DrawEffect(void);				//描画処理
	int GetNumSceneNillBoardAll(void);	//2Dシーン数の取得
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て
	void SetColor(D3DXCOLOR col);	//色の設定

	//ポリゴンの位置情報の取得、設定
	D3DXVECTOR3 GetPos(void);
	void SetPos(D3DXVECTOR3 pos);

	//ポリゴンの角度情報の取得、設定
	D3DXVECTOR3 GetRot(void);
	void SetRot(D3DXVECTOR3 pos);

private:
	LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	D3DXMATRIX				m_mtxWorld;			//ポリゴンのマトリックス情報
	D3DXVECTOR3				m_pos;			// ポリゴンの位置
	D3DXVECTOR3				m_rot;			// ポリゴンの回転
	static CManager *m_pManager;//マネージャ
	int m_nCntflame;		//カウント
	int m_nNumSceneBillBoard;	//2Dポリゴン数
	int m_nPriority;//優先度
	float m_fWidth;	//ポリゴンの幅
	float m_fHeight; //ポリゴンの奥行き
	float m_fnor; //ポリゴンの法線
};

#endif