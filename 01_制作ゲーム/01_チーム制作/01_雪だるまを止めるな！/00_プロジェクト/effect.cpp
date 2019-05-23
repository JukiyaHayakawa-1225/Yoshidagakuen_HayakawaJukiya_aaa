//=============================================================================
//
// プレイヤー処理 [effect.cpp]
// Author : Kodama Yuto
//
//=============================================================================
#include "effect.h"
#include "Manager.h"
#include "input.h"
#include "renderer.h"
//==================================================================
// マクロ定義
//==================================================================
#define EFFECT_LIFE (30)

//==================================================================
// 静的メンバ変数宣言
//==================================================================
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[2];				//共有テクスチャへのポインタ

//==================================================================
// コンストラクタ&デストラクタ
//==================================================================
CEffect::CEffect(int nPriority, CScene::OBJTYPE type) : CBillboard(nPriority,type)
{
	m_nCount = 0;
	m_nLife = 1;

}
CEffect::~CEffect()
{
}

//==================================================================
// 共有テクスチャの読み込み&解放
//==================================================================
HRESULT CEffect::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_apTexture[0]);

	return S_OK;
}
void CEffect::Unload(void)
{
	for (int nCntTex = 0; nCntTex < 2; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//==================================================================
// 生成処理
//==================================================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, AUTHORITY author, float fSize, D3DXCOLOR col, int nLife)
{
	CEffect* pEffect = NULL;
	pEffect = new CEffect(7,OBJTYPE_EFFECT);

	if (pEffect != NULL)
	{
		pEffect->Init(pos,move,col,fSize,nLife);	//オーバーロードしたInitで初期化
		pEffect->m_Author = author;
	}

	return pEffect;
}

//==================================================================
// 初期化処理
//==================================================================
HRESULT CEffect::Init(void)
{
	return S_OK;
}
void CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXCOLOR col, float fSize, int nLife)
{
	//レンダラの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//ポインタの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	m_pos = pos;
	m_move = move;
	m_Col = col;
	m_nCount = 0;
	m_nLife = nLife;

	CBillboard::SetPos(pos);
	CBillboard::SetWidth(fSize);
	CBillboard::SetHeight(fSize);
	CBillboard::Init();
	CBillboard::BindTexture(m_apTexture[0]);
}

//==================================================================
// 終了処理
//==================================================================
void CEffect::Uninit(void)
{
	CBillboard::Uninit();
}

//==================================================================
// 更新処理
//==================================================================
void CEffect::Update(void)
{
	CBillboard::Update();
	m_nCount++;

	D3DXVECTOR3 pos = CBillboard::GetPos();
	m_pos += m_move;

	SetPos(m_pos);
	//SetCol(D3DXCOLOR(m_Col.r, m_Col.g, m_Col.b,(m_nLife - m_nCount) * (1.0f / m_nLife)));
	//if (m_nCount == m_nLife)
	//{
	//	Uninit();
	//}
}

//==================================================================
// 描画処理
//==================================================================
void CEffect::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CBillboard::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

void CEffect::MoveScaling(float fScaling)
{
	m_move *= fScaling;
}
void CEffect::Gravity(float fGravity)
{
	m_move.y -= fGravity;
}

void CEffect::SetParentMatrix(D3DXMATRIX* mtxParent)
{
	m_mtxParent = mtxParent;
	CBillboard::SetmtxParent(m_mtxParent);
}
