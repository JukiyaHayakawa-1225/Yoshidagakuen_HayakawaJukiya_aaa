//=============================================================================
//
// 障害物(ビルボード)処理 [objBillboard.h]
// Author : Yuto Kodama
//
//=============================================================================
#ifndef _OBJ_BILLBOARD_H_
#define _OBJ_BILLBOARD_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CObjBillboard : public CBillboard
{
public:
	typedef enum
	{
		TYPE_TREE_1 = 0,
		TYPE_TREE_2,
		TYPE_ROCK,
		TYPE_GRASS,
		TYPE_MAX
	}OBJ_TYPE;

	CObjBillboard();
	CObjBillboard(int nPriority, CScene::OBJTYPE type);

	~CObjBillboard();

	static CObjBillboard* Create(OBJ_TYPE type, D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void) { return S_OK; };
	void Init(OBJ_TYPE type, D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; };

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//テクスチャ
	D3DXVECTOR3 m_pos;
	OBJ_TYPE m_Type;
};

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CItemBillboard : public CBillboard
{
public:
	typedef enum
	{
		TYPE_PRESENTBOX_0 = 0,
		TYPE_PRESENTBOX_1,
		TYPE_PRESENTBOX_2,
		TYPE_PRESENTBOX_3,
		TYPE_MAX
	}ITEM_TYPE;

	CItemBillboard();
	~CItemBillboard();

	static CItemBillboard* Create(ITEM_TYPE type, D3DXVECTOR3 pos);
	static HRESULT Load(void);
	static void Unload(void);

	HRESULT Init(void) { return S_OK; };
	void Init(ITEM_TYPE type, D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Collision(D3DXVECTOR3 pos);
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];	//テクスチャ
	ITEM_TYPE m_type;
};
#endif