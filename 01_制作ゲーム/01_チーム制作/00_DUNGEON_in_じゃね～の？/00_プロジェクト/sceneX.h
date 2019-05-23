//=============================================================================
//
// Xファイル処理[3D ポリゴン] [sceneX.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// シーン3Dのクラス構造体
//*****************************************************************************
class CSceneX : public CScene
{
public:
	CSceneX(int nPriority = 4, OBJTYPE type = OBJTYPE_SCENEX);	//コンストラクタ
	~CSceneX();	//デストラクタ
	static CSceneX *Create(D3DXVECTOR3 pos);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void BindFileName(char *cFileName);
	void BindBuff(LPD3DXBUFFER Buff);//バッファ情報割り当て
	void BindMesh(LPD3DXMESH Mesh);//メッシュ情報の割り当て
	void BindNumMat(DWORD NumMat);//マテリアルの情報の数の割り当て
	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);
	void BindTexture(LPDIRECT3DTEXTURE9 tex);//テクスチャの割り当て

private:
	char *m_pAddressModel;	//モデルの名前

	 PDIRECT3DTEXTURE9	m_pTexture;	// テクスチャへのポインタ
	LPD3DXMESH m_pMesh;					//メッシュ情報へのポインタ
	LPD3DXBUFFER m_pBuff;				//マテリアル情報へのポインタ
	DWORD m_nNumMat;					//マテリアルの情報の数
	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_rot;					//向き
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3 m_VtxMax;				//モデルの最大頂点座標
	D3DXVECTOR3 m_VtxMin;				//モデルの最小頂点座標


};

#endif