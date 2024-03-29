//=============================================================================
//
// ロゴ処理 [logo.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "scene.h"
#include "result.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLogo : public CScene
{
public:
	//メンバ関数
	CLogo(int nPriority = 6);
	~CLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 texture);
	void SetPos(D3DXVECTOR3 pos);
	void SetWidth(float fWidth);
	void SetHeight(float Height);
	void SetCol(D3DXCOLOR col);

private:
	LPDIRECT3DTEXTURE9					m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9				m_pVtxBuff;			// 頂点バッファへのポインタ
	D3DXVECTOR3							m_pos;				// 位置
	float								m_fWidth;			//幅
	float								m_fHeight;			//高さ
	D3DXCOLOR							m_col;				//色

protected:
};
#endif

//=============================================================================
//
// タイトルロゴの処理 [logo.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitleLogo : public CLogo
{
public:
	//メンバ関数
	CTitleLogo();
	~CTitleLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	//静的メンバ関数
	static CTitleLogo *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	D3DXVECTOR3		m_pos;								// 位置

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
protected:
};

//=============================================================================
//
// リザルトロゴ処理 [logo.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResultLogo : public CLogo
{
public:
	//メンバ関数
	CResultLogo();
	~CResultLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	//静的メンバ関数
	static CResultLogo *Create(D3DXVECTOR3 pos, float fWidth, float fHeight,CResult::RESULT result);

private:
	D3DXVECTOR3		m_pos;								// 位置

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_RESULT];
protected:
};

//=============================================================================
//
// ランキングロゴ処理 [logo.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRankingLogo : public CLogo
{
public:
	//メンバ関数
	CRankingLogo();
	~CRankingLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	//静的メンバ関数
	static CRankingLogo *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	D3DXVECTOR3		m_pos;								// 位置

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
protected:
};

//=============================================================================
//
// ランキングランクロゴ処理 [logo.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRankingRank : public CLogo
{
public:
	//メンバ関数
	CRankingRank();
	~CRankingRank();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	//静的メンバ関数
	static CRankingRank *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	D3DXVECTOR3		m_pos;								// 位置

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
protected:
};

//=============================================================================
//
// ターゲットロゴ処理 [logo.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTargetLogo : public CLogo
{
public:
	//メンバ関数
	CTargetLogo();
	~CTargetLogo();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	//静的メンバ関数
	static CTargetLogo *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	D3DXVECTOR3		m_pos;								// 位置
	int				m_nCounter;
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
protected:
};