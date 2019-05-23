//=============================================================================
//
// フィールドの処理 [field.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#include "scene3D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CField : public CScene3D
{
public:	//誰からもアクセス可能
	//メンバ関数
	CField();
	~CField();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//静的メンバ関数
	static CField *Create(D3DXVECTOR3 pos,float sizeX,float sizeZ);
	static HRESULT Load(void);
	static void Unload(void);

private:	//自分だけがアクセス可能
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	//メンバ変数
};
#endif
