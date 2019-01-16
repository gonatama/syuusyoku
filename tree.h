//=============================================================================
//
// 木処理 [tree.h]
// Author : 
//
//=============================================================================
#ifndef _TREE_H_
#define _TREE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TREE			(2560)	// 木最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 scl;		// スケール
	D3DXVECTOR3 move;		// スケール

	D3DXCOLOR col;			// 色

	float fWidth;			// 幅
	float fHeight;			// 高さ
	float cnt=0.f;
	int nIdxShadow;			// 影ID
	//g_nAlpha;
	float						g_nAlpha;						// アルファテストの閾値
	int nLife;				// 寿命
	bool bUse;				// 使用しているかどうか
} TREE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTree(void);
void UninitTree(void);
void UpdateTree(void);
void DrawTree(void);

int SetTree(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col);

TREE *GetTree(void);

#endif
