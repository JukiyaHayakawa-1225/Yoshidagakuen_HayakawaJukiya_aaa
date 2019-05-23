//=============================================================================
//
// サウンド処理 [sound.h]
// Author :Kishida Rei
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

class CSound
{
public:
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,		// BGM(TITLE)
		SOUND_LABEL_BGM_TUTORIAL,		// BGM(TUTORIAL)
		SOUND_LABEL_BGM_GAME,			// BGM(GAME)
		SOUND_LABEL_BGM_RESULT,			// BGM(RESULT)
		SOUND_LABEL_BGM_RANKING,		// BGM(RESULT
		SOUND_LABEL_SE_PICKUP,			// 取得
		SOUND_LABEL_SE_HIT,				// ヒット
		SOUND_LABEL_SE_DECIDE,			// 決定
		SOUND_LABEL_MAX
	} SOUND_LABEL;
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;


	CSound();	//コンストラクタ
	~CSound();	//デストラクタ
	HRESULT InitSound(HWND hWnd);	//初期化
	void UninitSound(void);			//終了処理
	HRESULT PlaySound(SOUND_LABEL label);//サウンドの再生
	void StopSound(SOUND_LABEL label);//サウンドの停止
	void StopSound(void);//サウンドの停止
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/title.wav", -1 },		// タイトル
		{ "data/BGM/tutorial.wav", -1 },	// チュートリアル
		{ "data/BGM/game.wav", -1 },		// ゲーム
		{ "data/BGM/result.wav", 0 },		// リザルト
		{ "data/BGM/ranking.wav",-1 },		// ランキング
		{ "data/SE/item_pickup.wav", 0 },	// 取得
		{ "data/SE/hit.wav", 0 },			// 被ダメージ
		{ "data/SE/nyu3.wav", 0 }			// 被
	};


};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

#endif
