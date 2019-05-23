//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author :
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"

//*****************************************************************************
// メッシュフィールドクラスの構造体
//*****************************************************************************
class CMeshCylinder : public CScene
{
public:
	CMeshCylinder(int nPriority = 2, OBJTYPE type = OBJTYPE_CYLINDER);
	~CMeshCylinder();
	static CMeshCylinder *Create(D3DXVECTOR3 pos);//敵の生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; //インデックスバッファへのポインタ

	D3DXVECTOR3 m_pos;			//位置(中心座標)
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DXCOLOR m_col;			//色情報
	D3DXVECTOR2 m_TexPos;		//テクスチャ座標
	int m_nMesh_H;				//水平の分割
	int m_nMesh_V;				//垂直の分割
	int m_nVertexNum;			//頂点数
	int m_nIndexNum;			//インデックス数
	int m_nPolygonNum;			//ポリゴン数
	static int m_nSquareNum;	//四角面の数
	static int m_nVecNum;		//囲うベクトルの数
	float m_fMeshWidth;			//メッシュフィールドの幅
	float m_fMeshDepth;			//メッシュフィールドの奥行き
};

#endif
