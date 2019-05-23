//=============================================================================
//
// ゲームの処理 [game.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//前方宣言
class CMeshField;
class CPlayer;
class CCommand;
class CMap;
class CScore;
class CObjBillboard;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame
{
public:	//誰からもアクセス可能
	//メンバ関数
	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CGame			*Create(void);
	static CMeshField		*GetMeshField(void);
	static CPlayer			*GetPlayer(void);
	static CScore	*GetScore(void);


private:	//自分だけがアクセス可能
	//静的メンバ変数
	static CMeshField		*m_pMeshField;	//メッシュフィールドのポインタ
	static CPlayer			*m_pPlayer;		//プレイヤーのポインタ
	static CMap				*m_pMap;		//マップのポインタ
	static CScore	*m_pScore;
	static CObjBillboard *m_p3Dporigon;

};
#endif