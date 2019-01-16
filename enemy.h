#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

typedef struct
{

	D3DXVECTOR3			g_posEnemy;				// ���f���̈ʒu
	D3DXVECTOR3			g_rotEnemy;				// ���f���̌���(��])
	D3DXVECTOR3			g_sclEnemy;				// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3			g_moveEnemy;				// �ړ���
	D3DXVECTOR3			g_rotDestEnemy;				// �ړI�̌���


}ENEMY;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);




ENEMY	*GetEnemy(int pno);


#endif
