//=============================================================================
//
// オブジェクトの配置処理[3D ポリゴン] [object.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "sceneX.h"
//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAXMODEL_TYPE		(20)						//モデルの種類の最大数
#define MAX_TEXTURE_OBJ		(9)
//*****************************************************************************
// モデルクラスの構造体
//*****************************************************************************
class CObject : public CSceneX
{
public:
	typedef enum
	{
		OBJECTTYPE_COIN = 0,	//コイン
		OBJECTTYPE_KEY,			//カギ
		OBJECTTYPE_WOOD,		//木
		OBJECTTYPE_SKY,			//空
		OBJECTTYPE_CLOUD,		//雲
		OBJECTTYPE_SCAFFOLD,	//足場
		OBJECTTYPE_DUNGEON,		//ダンジョン
		OBJECTTYPE_GOAL,		//ゴール
		OBJECTTYPE_SCAFFOLD_S,	//足場
		OBJECTTYPE_MAX
	}OBJECTTYPE;
	CObject(int nPriority = 4, OBJTYPE type = OBJTYPE_OBJECT);
	~CObject();
	static HRESULT Load(void);
	static void Unload(void);
	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int type);//モデルの生成
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	static void LoadModelText(void);// モデル情報読み込み
	bool CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius);
	bool CObject::CollisionGoal(D3DXVECTOR3 *pos, D3DXVECTOR3 *move, D3DXVECTOR3 *rot);
private:
	//	読み込むモデルのデータ
	static char *m_pModelName[MAXMODEL_TYPE];		// モデルのアドレス格納用
	D3DXVECTOR3			m_vtxMin;					// モデルの最小値
	D3DXVECTOR3			m_vtxMax;					// モデルの最大値
	float m_fHeight;						//敵の高さ

	// モデルの基本情報
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE_OBJ]; //共有テクスチャのポインタ
	D3DXVECTOR3			m_pos;						// 位置
	D3DXVECTOR3			m_move;						// 位置の移動量
	D3DXVECTOR3			m_rot;						// 向き
	D3DXVECTOR3			m_rotDest;					// 目的の向き
	D3DXMATRIX			m_mtxWorld;					// ワールドマトリックス
	int					m_nIdxShadow;				// 影の番号
	int					m_nTypeModel;				// 使用するモデル
	static int			m_nMaxModelNum;				// モデルの数
	int					m_nMaxTextureNum;			// テクスチャの数
	OBJECTTYPE			m_Objecttype;
};

#endif
