//=============================================================================
//
// オブジェクト3D処理 [scene3D.h]
// Author : Kodama Yuto
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "scene.h"

//==================================================================
//	クラスの定義
//==================================================================
//オブジェクト3Dクラス
class CScene3D : public CScene
{
public:


	CScene3D(int priority = 4, OBJTYPE type = OBJTYPE_SCENE3D);

	~CScene3D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene3D* Create(void);

	void        SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; };

	void        SetRotation(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void) { return m_rot; };

	void        SetColor(D3DXCOLOR col);
	void		SetSize(float fSize);
	void		BindTexture(LPDIRECT3DTEXTURE9 pTexture);

protected:

private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;			// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;						//ポリゴンの位置
	D3DXVECTOR3				m_rot;						//角度
	D3DXCOLOR				m_Color;					//色
	D3DXMATRIX				m_mtxWorld;					//ポリゴンのマトリックス
	float					m_fSize;
};


#endif // !_SCENE2D_H_

