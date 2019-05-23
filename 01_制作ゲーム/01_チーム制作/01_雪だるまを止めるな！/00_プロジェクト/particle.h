//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : Yuto Kodama
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "scene.h"
#include "effect.h"

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CParticle : public CScene
{
public:
	typedef enum
	{
		PARTICLE_TYPE1 = 0,
		PARTICLE_MAX,
	}PARTICLE_TYPE;

	CParticle();
	CParticle(int nPriority, CScene::OBJTYPE type);

	~CParticle();

	static CParticle* Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,int nLife, PARTICLE_TYPE type);

	HRESULT Init(void) { return S_OK; };
	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife, PARTICLE_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void CreateParticlePattern();
private:
	PARTICLE_TYPE m_type;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXMATRIX m_mtxWorld;
	CEffect** m_pEffect;	//エフェクトのポインタ(必要数分だけ動的確保するためダブルポインタにしている)
	int m_nMasterLife;		//パーティクルの体力(これを超えたらエフェクトをすべて解放する)
	float m_fMoveScale;		//エフェクトの移動量に補正を掛けるとき用の変数
	int m_nEffectNum;
	int m_nCount;
};

#endif // !_PARTICLE_H_
