//=============================================================================
//
// ライト処理 [light.cpp]
// Author :岸田怜(Kishida Rei)
//
//=============================================================================
#include "light.h"
#include "renderer.h"
#include "manager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager *CLight::m_pManager = NULL;



//=============================================================================
// 関数名：コンストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CLight::CLight()
{
}

//=============================================================================
// 関数名：デストラクタ
// 関数の概要：生成されたときに呼び出される。値のクリア
//=============================================================================
CLight::~CLight()
{
}

//=============================================================================
// 関数名：ライトの初期化処理
// 関数の概要：ライト3つの生成
//=============================================================================
HRESULT CLight::Init(void)
{
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 vecDir0;	//ライトの方向[0]
	D3DXVECTOR3 vecDir1;	//ライトの方向[1]
	D3DXVECTOR3 vecDir2;	//ライトの方向[2]

	// ライトをクリアする
	ZeroMemory(&m_Light[0], sizeof(D3DLIGHT9));
	ZeroMemory(&m_Light[1], sizeof(D3DLIGHT9));
	ZeroMemory(&m_Light[2], sizeof(D3DLIGHT9));


	// ライトの種類を設定
	m_Light[0].Type = D3DLIGHT_DIRECTIONAL;
	m_Light[1].Type = D3DLIGHT_DIRECTIONAL;
	m_Light[2].Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	m_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0);
	m_Light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 0.8f);
	m_Light[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ライトの方向の設定
	vecDir0 = D3DXVECTOR3(0.2f, -0.7f, 0.4f);
	D3DXVec3Normalize(&vecDir0, &vecDir0);

	vecDir1 = D3DXVECTOR3(-0.3f, 0.8f, -0.5f);
	D3DXVec3Normalize(&vecDir1, &vecDir1);

	vecDir2 = D3DXVECTOR3(-0.3f, -0.8f, 0.3f);
	D3DXVec3Normalize(&vecDir2, &vecDir2);


	//ライトの方向の設定
	m_Light[0].Direction = vecDir0;
	m_Light[1].Direction = vecDir1;
	m_Light[2].Direction = vecDir2;

	// ライトを設定する
	pDevice->SetLight(0, &m_Light[0]);
	pDevice->SetLight(1, &m_Light[1]);
	pDevice->SetLight(2, &m_Light[2]);

	// ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	pDevice->LightEnable(1, TRUE);
	pDevice->LightEnable(2, TRUE);


	return S_OK;
}

//=============================================================================
// 関数名：ライトの終了処理
// 関数の概要：
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// 関数名：ライトの更新処理
// 関数の概要：
//=============================================================================
void CLight::Update(void)
{
}
