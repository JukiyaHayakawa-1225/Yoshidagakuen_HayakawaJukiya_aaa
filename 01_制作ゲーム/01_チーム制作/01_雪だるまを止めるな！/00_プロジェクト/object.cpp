//=============================================================================
//
// 配置物の処理 [object.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "loadfilefunction.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject(int nPrioriry, OBJTYPE objtype) : CSceneX(nPrioriry, objtype)
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{


}

//=============================================================================
// オブジェクトXの生成
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, bool bHit)
{
	CObject *pObject = NULL;		//配置物のポインタ

	if (pObject == NULL)
	{
		pObject = new CObject;		//動的確保
		if (pObject != NULL)
		{
			pObject->SetPos(pos);	//位置の設置処理
			pObject->SetRot(rot);	//向きの設置処理
			pObject->SetSize(size);	//大きさの設置処理
			pObject->SetHit(bHit);	//当たり判定の設置処理
			pObject->Init();		//初期化処理
		}
	}
	return pObject;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject::Init(void)
{
	//初期化処理
	CSceneX::Init();
	//オブジェクトの種類設定
	SetObjType(OBJTYPE_OBJECT);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject::Uninit(void)
{
	CSceneX::Uninit();
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject::Draw(void)
{
	//描画処理
	CSceneX::Draw();
}


//=============================================================================
// 配置物の当たり判定
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove, D3DXVECTOR3 *VtxMin, D3DXVECTOR3 *VtxMax, bool* bHIT)
{
	bool bLand = false;	//乗っているかどうか

	D3DXVECTOR3 pos = GetPos();				//位置の取得
	D3DXVECTOR3 vtxMax = GetVtxMax();		//頂点座標最大値の取得
	D3DXVECTOR3 vtxMin = GetVtxMin();		//頂点座標最小値の取得
	bool		bHit = GetHit();			//当たり判定の取得
											//if (pPos->x >= pos.x + vtxMin.x&&
											//	pPos->x <= pos.x + vtxMax.x &&
											//	pPos->z >= pos.z + vtxMin.z &&
											//	pPos->z <= pos.z + vtxMax.z)
											//{
											//	if (pPosOld->x <= pos.x + vtxMin.x&&
											//		pPos->x >= pos.x + vtxMin.x)
											//	{//左からめり込んだ場合
											//		bLand = true;
											//		pPos->x = pos.x + vtxMin.x;
											//	}
											//	else if (pPosOld->x >= pos.x + vtxMax.x&&
											//		pPos->x <= pos.x + vtxMax.x)
											//	{//右からめり込んだ
											//		bLand = true;
											//		pPos->x = pos.x + vtxMax.x;

											//	}
											//	else if (pPosOld->z <= pos.z + vtxMin.z &&
											//		pPos->z >= pos.z + vtxMin.z)
											//	{//前からめり込んだ
											//		bLand = true;
											//		pPos->z = pos.z + vtxMin.z;

											//	}
											//	else if (pPosOld->z >= pos.z + vtxMax.z &&
											//		pPos->z <= pos.z + vtxMax.z)
											//	{//後からめり込んだ
											//		bLand = true;
											//		pPos->z = pos.z + vtxMax.z;

											//	}
											//}
	if (bHit == true)
	{
		if (/*pPos->y <= pos.y + vtxMax.y - (VtxMax->y)
			&& pPos->y >= pos.y + vtxMax.y - (VtxMax->y)
			||*/ pPos->y >= pos.y + vtxMin.y
			&& pPos->y <= pos.y + vtxMin.y
			|| pPos->y <= pos.y + vtxMax.y - (VtxMax->y)
			&& pPos->y >= pos.y + vtxMin.y)
		{// yの範囲の中
			if (pPos->z <= pos.z + vtxMax.z && pPos->z >= pos.z + vtxMin.z)
			{// zの範囲の中
				if (pPosOld->x <= pos.x + vtxMin.x
					&& pPos->x > pos.x + vtxMin.x)
				{// X座標の中に左から入った
					pPos->x = pPosOld->x;
					pMove->x = 0.0f;
				}
				else if (pPosOld->x >= pos.x + vtxMax.x
					&& pPos->x < pos.x + vtxMax.x)
				{// X座標の中に右から入った
					pPos->x = pPosOld->x;
					pMove->x = 0.0f;
				}
			}
			if (pPos->x <= pos.x + vtxMax.x && pPos->x >= pos.x + vtxMin.x)
			{// xの範囲の中
				if (pPosOld->z - (VtxMax->z) <= pos.z + vtxMin.z
					&& pPos->z  + (VtxMax->z)> pos.z + vtxMin.z)
				{// Z座標の中に手前から入った
					pPos->z = pPosOld->z;
					pMove->z = 0.0f;
					*bHIT = true;
				}
				else if (pPosOld->z >= pos.z + vtxMax.z
					&& pPos->z < pos.z + vtxMax.z)
				{// Z座標の中に後ろから入った
					pPos->z = pPosOld->z;
					pMove->z = 0.0f;
				}
			}
		}

		if (pPos->x  < pos.x + vtxMax.x - (VtxMax->x) && pPos->x > pos.x + vtxMin.x + (VtxMax->x)
			&& pPos->z <= pos.z + vtxMax.z - (VtxMax->z) && pPos->z >= pos.z + vtxMin.z + (VtxMax->z))
		{// 障害物の内側に乗った
			if (pPosOld->y >= pos.y + vtxMax.y && pPos->y < pos.y + vtxMax.y
				|| pPos->y <= pos.y + vtxMax.y && pPosOld->y > pos.y + vtxMax.y)
			{// 上からブロックに当たったとき
				bLand = true;  // 乗った判定を返す
				pPos->y = pos.y + vtxMax.y;
				pMove->y = 0.0f;  // 移動量をなくす
			}

			if (pPosOld->y  < pos.y + vtxMin.y && pPos->y >= pos.y + vtxMin.y
				|| pPos->y  > pos.y + vtxMin.y && pPosOld->y <= pos.y + vtxMin.y)
			{// 下からブロックに当たったとき
				pPos->y = pos.y + vtxMin.y;
				pMove->y = 0.0f;  // 移動量をなくす
			}
		}
	}
	return bLand;
}