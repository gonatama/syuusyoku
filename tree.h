//=============================================================================
//
// �؏��� [tree.h]
// Author : 
//
//=============================================================================
#ifndef _TREE_H_
#define _TREE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TREE			(2560)	// �؍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 scl;		// �X�P�[��
	D3DXVECTOR3 move;		// �X�P�[��

	D3DXCOLOR col;			// �F

	float fWidth;			// ��
	float fHeight;			// ����
	float cnt=0.f;
	int nIdxShadow;			// �eID
	//g_nAlpha;
	float						g_nAlpha;						// �A���t�@�e�X�g��臒l
	int nLife;				// ����
	bool bUse;				// �g�p���Ă��邩�ǂ���
} TREE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTree(void);
void UninitTree(void);
void UpdateTree(void);
void DrawTree(void);

int SetTree(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col);

TREE *GetTree(void);

#endif
