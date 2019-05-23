//=============================================================================
//
// Xファイル処理[3D ポリゴン] [sceneX.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTS (29)
#define MAX_TEXTURE (2)
//*****************************************************************************
// シーン3Dのクラス構造体
//*****************************************************************************
class CModel
{
public:
	typedef enum
	{
		MODELTYPE_PLAYER = 0,	//プレイヤー
		MODELTYPE_ENEMY,		//敵
		MODELTYPE_MAX			//モデルの種類の最大数
	}MODELTYPE;
	CModel();	//コンストラクタ
	~CModel();	//デストラクタ
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumParts, MODELTYPE type);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetPos(D3DXVECTOR3 pos);	//位置情報の代入
	void SetRot(D3DXVECTOR3 rot);	//角度情報の代入
	void SetMtx(D3DXMATRIX mtx);	//角度情報の代入
	D3DXMATRIX GetMatrix(void);//マトリックスのしぃとく
	void Load(void);//使用するパーツの読み込み
	void Unload(void);//使用するパーツの破棄
	void SetParent(CModel *pModel);//親子関係の設定
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て

private:
	static char						 m_acFileName[MAX_PARTS][256];		// 読み込む
	//static LPDIRECT3DTEXTURE9		 *m_apTexture[MAX_TEXTURE];		// テクスチャへのポインタ
	 LPDIRECT3DTEXTURE9				 m_pTexture;					// テクスチャへのポインタ
	static LPD3DXMESH				 m_apMesh[MAX_PARTS];			//メッシュ情報へのポインタ
	static LPD3DXBUFFER				 m_apBuffMat[MAX_PARTS];		//マテリアル情報へのポインタ
	static DWORD					 m_aNumMat[MAX_PARTS];			//マテリアルの情報の数
	CModel							*m_pParent;						//親モデルのポインタ
	int m_nNumParts;
	int m_nMaxParts;
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	MODELTYPE m_type;	//モデルの種類
};

#endif