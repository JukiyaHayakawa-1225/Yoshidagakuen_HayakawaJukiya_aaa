//=============================================================================
//
// マップの処理[map.h]
// Auther:Jukiya Hayakawa
//
//=============================================================================
#ifndef _MAP_H_
#define _MAP_H_

#include "main.h"

//前方宣言
class CModelCreate;

//*****************************************************************************
// マクロ定義
//*****************************************************************************

class CObject;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMap
{
public:    // 誰からもアクセス可能
	CMap();
	~CMap();
	void Init(void);
	void Uninit(void);

	//静的メンバ関数
	static CMap *Create(void);
private:   //自分だけがアクセス可能
	void SaveObject(void);
	void FieldLoad(char *pFieldFileName, char *pStrCur, char *pLine);
	void LoadObject(char *pFileNameObject, char *pStrCur, char *pLine);

	CModelCreate		*m_pModelCreate;		// モデル生成のポインタ
};

#endif