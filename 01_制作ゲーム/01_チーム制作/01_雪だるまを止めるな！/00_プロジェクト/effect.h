//=============================================================================
//
// 弾クラス処理 [effect.h]
// Author : Kodama Yuto
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "billboard.h"

//==================================================================
//	クラスの定義
//==================================================================
//プレイヤークラス
class CEffect : public CBillboard
{
public:

	typedef enum
	{//弾の管理者分け用
		AUTHOR_NONE,
		AUTHOR_PLAYER,
		AUTHOR_ENEMY,
	}AUTHORITY;

	//コンストラクタ&デストラクタ
	CEffect();
	CEffect(int nPriority,CScene::OBJTYPE type);
	~CEffect();

	static HRESULT Load(void);
	static void    Unload(void);

	HRESULT Init(void);
	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSize, int nLife);

	void    Uninit(void);
	void    Update(void);
	void    Draw(void);
	void	SetParentMatrix(D3DXMATRIX* mtxParent);
	static CEffect* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, AUTHORITY author,float fSize,D3DXCOLOR col,int nLife);
	AUTHORITY GetAuthor(void) { return m_Author; };

	void MoveScaling(float fScaling);
	void Gravity(float fGravity);
private:
	D3DXMATRIX*				  m_mtxParent;		//親パーティクルのマトリックス(自身のマトリックスと掛け合わせる)
	static LPDIRECT3DTEXTURE9 m_apTexture[2];	//共有テクスチャへのポインタ
	AUTHORITY				m_Author;			//エフェクトの権利者
	D3DXVECTOR3				m_pos;				//座標
	D3DXVECTOR3				m_move;				//移動量
	D3DXCOLOR				m_Col;				//色
	int						m_nCount;			//カウンタ
	int						m_nLife;			//体力
	float					m_fSize;			//サイズ
protected:
};
#endif // !_PLAYER_H_
