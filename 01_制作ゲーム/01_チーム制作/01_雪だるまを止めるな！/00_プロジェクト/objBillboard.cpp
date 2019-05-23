//=============================================================================
//
// 障害物(ビルボード)処理 [objBillboard.cpp]
// Author : Yuto Kodama
//
//=============================================================================
#include "objBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "score.h"
#include "game.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CObjBillboard::m_apTexture[CObjBillboard::TYPE_MAX] = {};

CObjBillboard::CObjBillboard()
{
}

//=============================================================================
// コンストラクタ&デストラクタ
//=============================================================================
CObjBillboard::CObjBillboard(int nPriority, CScene::OBJTYPE type) : CBillboard(nPriority, type)
{

}

CObjBillboard::~CObjBillboard()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CObjBillboard* CObjBillboard::Create(CObjBillboard::OBJ_TYPE type, D3DXVECTOR3 pos)
{
	CObjBillboard* pBillboard = NULL;

	pBillboard = new CObjBillboard(3, CScene::OBJTYPE_BILLBOARD);

	if (pBillboard != NULL)
	{
		pBillboard->SetHeight(50.0f);
		pBillboard->SetWidth(50.0f);
		pBillboard->Init(type, pos);
	}

	return pBillboard;
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CObjBillboard::Load(void)
{
	//レンダラの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//ポインタの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/f_tree941.png", &m_apTexture[TYPE_TREE_1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/christmas-tree.png", &m_apTexture[TYPE_TREE_2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/", &m_apTexture[TYPE_ROCK]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/grass.png", &m_apTexture[TYPE_GRASS]);

	return S_OK;
}

//=============================================================================
// テクスチャ解放処理
//=============================================================================
void CObjBillboard::Unload(void)
{
	//解放
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
void CObjBillboard::Init(CObjBillboard::OBJ_TYPE type, D3DXVECTOR3 pos)
{

	//親クラスの初期化
	CBillboard::Init();

	//変数に値を入れる
	m_Type = type;
	m_pos = pos;

	//親クラスに値を設定
	CBillboard::SetPos(pos);
	CBillboard::BindTexture(m_apTexture[type]);
}

//=============================================================================
// 終了処理
//=============================================================================
void CObjBillboard::Uninit(void)
{
	//親クラスの終了処理
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObjBillboard::Update(void)
{
	//親クラスの更新処理
	CBillboard::Update();


	//親クラスに新しい値を設定
	CBillboard::SetPos(m_pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void CObjBillboard::Draw(void)
{
	//親クラスの描画処理
	CBillboard::Draw();
}


//=============================================================================
//
// アイテム(ビルボード)処理 [ItemBillboard.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CItemBillboard::m_apTexture[CItemBillboard::TYPE_MAX] = {};

//=============================================================================
// コンストラクタ&デストラクタ
//=============================================================================
CItemBillboard::CItemBillboard()
{
	m_type = TYPE_PRESENTBOX_0;
}

CItemBillboard::~CItemBillboard()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CItemBillboard* CItemBillboard::Create(CItemBillboard::ITEM_TYPE type, D3DXVECTOR3 pos)
{
	CItemBillboard* pBillboard = NULL;

	pBillboard = new CItemBillboard;

	if (pBillboard != NULL)
	{
		pBillboard->SetHeight(25.0f);
		pBillboard->SetWidth(25.0f);
		pBillboard->Init(type, pos);
	}

	return pBillboard;
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CItemBillboard::Load(void)
{
	//レンダラの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//ポインタの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/box.png", &m_apTexture[TYPE_PRESENTBOX_0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/christmas-tree.png", &m_apTexture[TYPE_PRESENTBOX_1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/", &m_apTexture[TYPE_PRESENTBOX_2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/grass.png", &m_apTexture[TYPE_PRESENTBOX_3]);

	return S_OK;
}

//=============================================================================
// テクスチャ解放処理
//=============================================================================
void CItemBillboard::Unload(void)
{
	//開放
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
void CItemBillboard::Init(CItemBillboard::ITEM_TYPE type, D3DXVECTOR3 pos)
{
	//オブジェクト種類の設置処理
	SetObjType(OBJTYPE_ITEM_BILLBOARD);


	//親クラスの初期化
	CBillboard::Init();

	//変数に値を入れる
	SetPos(pos);
	m_type = type;

	//親クラスに値を設定
	CBillboard::SetPos(pos);
	CBillboard::BindTexture(m_apTexture[type]);
}

//=============================================================================
// 終了処理
//=============================================================================
void CItemBillboard::Uninit(void)
{
	//親クラスの終了処理
	CBillboard::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CItemBillboard::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	//親クラスの更新処理
	CBillboard::Update();


	//親クラスに新しい値を設定
	CBillboard::SetPos(pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void CItemBillboard::Draw(void)
{
	//親クラスの描画処理
	CBillboard::Draw();
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CItemBillboard::Collision(D3DXVECTOR3 pos)
{
	//スコアの取得
	CScore *pScore = CGame::GetScore();
	//位置の取得
	D3DXVECTOR3 ItemPos = GetPos();
	//距離の変数
	float fLength = 0.0f;

	//距離の計算
	fLength = sqrtf((pos.x - ItemPos.x) * (pos.x - ItemPos.x) + (pos.z - ItemPos.z) * (pos.z - ItemPos.z));

	if (fLength >= 0.0f && fLength <= 20.0f)
	{//一定の範囲内の場合
	 //スコア加算して終了処理
		pScore->AddScore(10000);
		Uninit();
	}
}
