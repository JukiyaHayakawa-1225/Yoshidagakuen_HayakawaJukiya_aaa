//=============================================================================
//
// シーン処理 [scene.h]
// Author :岸田怜(KishidaRei)
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PRIORITY_NUMBER (8)	//オブジェクトの優先順位

//*****************************************************************************
// シーン全体のクラス構造体
//*****************************************************************************
class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,			//空の種類
		OBJTYPE_SCENE2D,			//2Dシーン
		OBJTYPE_SCENE3D,			//3Dシーン
		OBJTYPE_SCENEX,				//Xシーン
		OBJTYPE_SCENEBILLBOARD,		//ビルボードシーン
		OBJTYPE_TITLELOGO,			//タイトル
		OBJTYPE_TUTOLOGO,			//タイトル
		OBJTYPE_MESHFIELD,			//メッシュ
		OBJTYPE_SHADOW,				//影
		OBJTYPE_CYLINDER,			//シリンダー
		OBJTYPE_PLAYER,				//プレイヤー
		OBJTYPE_ENEMY,				//敵
		OBJTYPE_BG,					//背景
		OBJTYPE_UI,					//UI
		OBJTYPE_UIRESULT,			//UIリザルト
		OBJTYPE_UIRANKING,			//UIランキング
		OBJTYPE_ITEMCOUNTER,		//アイテムカウンター
		OBJTYPE_EFFECT,				//エフェクト
		OBJTYPE_TIMER,				//タイマー
		OBJTYPE_OBJECT,				//オブジェクト
		OBJTYPE_MAX					//オブジェクトの種類の最大数
	}OBJTYPE;
	CScene(int nPriority = 3, OBJTYPE type = OBJTYPE_NONE);	//コンストラクタ
	virtual ~CScene();	//デストラクタ
	virtual HRESULT Init(void) = 0;	//初期化処理
	virtual void Uninit(void) = 0;	//終了処理
	virtual void Update(void) = 0;	//更新処理
	virtual void Draw(void) = 0;	//描画処理
	void SetObjType(OBJTYPE objtype);	//オブジェクトの種類の設定
	OBJTYPE GetObjType(void);	//オブジェクトの種類の取得
	int GetPriority(void);					//優先順位の取得
	static void ReleaseSceneAll(void);		//全てのシーンの破棄
	static void UpdateAll(void);			//全てのシーンの更新
	static void DrawAll(void);				//全てのシーンの描画
	static int GetNumAll(void);				//シーンの総数の取得
	void CheckDeath(void);					//死亡フラグを調べる
	CScene *GetSceneTop(int nPriority);	//シーンの取得(インデックス)
	CScene *GetSceneNext(int nPriority);	//シーンの取得(インデックス)
protected:
	void Release(void);	//オブジェクトの破棄
private:
	int m_nPriority;	//優先順位の番号
	static int m_nNumAll;	//シーンの総数
	static CScene *m_pTop[PRIORITY_NUMBER];	//先頭のオブジェクトのポインタ
	static CScene *m_pCur[PRIORITY_NUMBER];	//現在のオブジェクトのポインタ
	CScene *m_pPrev;	//前のポインタ
	CScene *m_pNext;	//次のポインタ
	int m_nID;//自分の番号
	OBJTYPE m_Objtype;//オブジェクトの種類
	bool m_bDeath;	//死亡フラグ
};

#endif