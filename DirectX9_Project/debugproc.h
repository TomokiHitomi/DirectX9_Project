//=============================================================================
//
// デバッグ表示処理 [debugproc.h]
// Author : GP12A295 25 人見友基
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DEBUG_SCROLL_SPEED		(10)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitDebugProc(void);
void UninitDebugProc(void);
void UpdateDebugProc(void);
void DrawDebugProc(void);

void PrintDebugProc(char *fmt,...);
void SetDebugProc(void);
bool GetDebugProc(void);

#endif