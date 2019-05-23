//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : Yuto Kodama
//
//=============================================================================
#include "particle.h"

//=============================================================================
// コンストラクタ&デストラクタ
//=============================================================================
CParticle::CParticle(int nPriority, CScene::OBJTYPE type):CScene(nPriority,type)
{
	m_pEffect = NULL;
	m_nEffectNum = 0;
	m_nCount = 0;
	m_nMasterLife = 1;
}
CParticle::~CParticle()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CParticle* CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, PARTICLE_TYPE type)
{
	CParticle* pParticle = NULL;

	pParticle = new CParticle(3, OBJTYPE_EFFECT);

	if (pParticle != NULL)
	{
		pParticle->Init(pos,rot,nLife,type);
	}

	return pParticle;
}

//=============================================================================
// 初期化処理
//=============================================================================
void CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, PARTICLE_TYPE type)
{
	D3DXMATRIX mtxRot, mtxTrans;
	m_pos = pos;
	m_rot = rot;
	m_nMasterLife = nLife;
	m_type = type;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y,
		m_rot.x,
		m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	CreateParticlePattern();
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	if (m_pEffect != NULL)
	{
		for (int nCntEffect = 0; nCntEffect < m_nEffectNum; nCntEffect++)
		{
			if (m_pEffect[nCntEffect] != NULL)
			{
				m_pEffect[nCntEffect]->Uninit();
				//delete m_pEffect[nCntEffect];
				m_pEffect[nCntEffect] = NULL;
			}
		}

		delete[] m_pEffect;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	m_nCount++;
	for (int nCntEffect = 0; nCntEffect < m_nEffectNum; nCntEffect++)
	{
		m_pEffect[nCntEffect]->MoveScaling(0.925f);
		m_pEffect[nCntEffect]->Gravity(0.1f);
	}
	if (m_nCount >= m_nMasterLife)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CParticle::Draw(void)
{

}

//=============================================================================
// パーティクル設定処理
//=============================================================================
void CParticle::CreateParticlePattern(void)
{
	switch (m_type)
	{
	case PARTICLE_TYPE1:
		m_nEffectNum = 13;
		m_pEffect = new CEffect*[m_nEffectNum];

		if (m_pEffect != NULL)
		{
			m_pEffect[0] = CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 6.0f, 0.0f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[1] = CEffect::Create(m_pos, D3DXVECTOR3(4.1f, 5.9f, 0.0f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[2] = CEffect::Create(m_pos, D3DXVECTOR3(-4.2f, 5.9f, 0.0f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[3] = CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 5.9f, 4.1f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[4] = CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 5.9f, -4.3f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[5] = CEffect::Create(m_pos, D3DXVECTOR3(4.5f, 5.5f, 0.0f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[6] = CEffect::Create(m_pos, D3DXVECTOR3(-4.5f, 5.5f, 0.0f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[7] = CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 5.5f, 4.5f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[8] = CEffect::Create(m_pos, D3DXVECTOR3(0.0f, 5.5f, -4.5f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[9] = CEffect::Create(m_pos, D3DXVECTOR3(4.25f, 5.5f, 4.25f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[10] = CEffect::Create(m_pos, D3DXVECTOR3(-4.25f, 5.5f, -4.25f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[11] = CEffect::Create(m_pos, D3DXVECTOR3(4.25f, 5.5f, -4.25f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);
			m_pEffect[12] = CEffect::Create(m_pos, D3DXVECTOR3(-4.25f, 5.5f, 4.25f), CEffect::AUTHOR_PLAYER,
				3.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20);

			for (int nCntEffect = 0; nCntEffect < m_nEffectNum; nCntEffect++)
			{
				m_pEffect[nCntEffect]->SetParentMatrix(&m_mtxWorld);
			}
		}
		break;
	}
}
