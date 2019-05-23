//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:	//誰からもアクセス可能
		//*************************************
		// サウンドファイル
		//*************************************
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// BGM0 (OP)
		SOUND_LABEL_BGM001,			// BGM1	(TUTORIAL)
		SOUND_LABEL_BGM002,			// BGM2	(GAME)
		SOUND_LABEL_BGM003,			// BGM3	(CLEAR)
		SOUND_LABEL_BGM004,			// BGM5	(RANKING)
		SOUND_LABEL_SE_DAMAGE,		//ダメージ
		SOUND_LABEL_SE_IDOU,		//移動
		SOUND_LABEL_SE_ITEM,		//アイテム
		SOUND_LABEL_SE_JUMP,		//ジャンプ
		SOUND_LABEL_SE_KAKUDAI,		//拡大
		SOUND_LABEL_SE_PAUSE,		//ポーズ
		SOUND_LABEL_SE_SYUKUSYOU,		//ポーズ
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//*****************************************************************************
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	CSound();
	~CSound();
	HRESULT InitSound(HWND hWnd);
	void UninitSound(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
private:	//自分だけがアクセス可能
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

																// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/Game.wav", -1 },		// BGM0 (OP)
		{ "data/BGM/Tutorial.wav", -1 },	// BGM1	(TUTORIAL)
		{ "data/BGM/Title.wav", -1 },		// BGM2	(GAME)
		{ "data/BGM/Result.wav", 0 },		// BGM3	(RESULT)
		{ "data/BGM/Ranking.wav", -1 },		// BGM5	(RANKING)
		{ "data/SE/Damage.wav", 0 },		// ダメージ
		{ "data/SE/Idou.wav", 0 },			// 移動
		{ "data/SE/Item.wav", 0 },			// アイテム
		{ "data/SE/Jump.wav", 0 },			// ジャンプ
		{ "data/SE/Kakudai.wav", 0 },		// 拡大
		{ "data/SE/Pause.wav", 0 },			// ポーズ
		{ "data/SE/Syukusyou.wav", 0 },			// ポーズ
	};
};
#endif
