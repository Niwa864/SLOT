//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE = 0,		
	SOUND_LABEL_PLAY,			
	SOUND_LABEL_BIG,			
	SOUND_LABEL_WIN,		
	SOUND_LABEL_LOSE,		
	SOUND_LABEL_SE_CALLIE,		
	SOUND_LABEL_SE_MARIE,
	SOUND_LABEL_SE_STOP,
	SOUND_LABEL_SE_WIN,
	SOUND_LABEL_SE_REEL,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
