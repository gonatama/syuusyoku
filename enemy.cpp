//=============================================================================
//
// ���f������ [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_CAR		"data/PLAYER/Panjandrum.x"							// �ǂݍ��ރ��f����
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
//#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
//#define	VIEW_FAR_Z		(1000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE		(5.0f)											// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// ��]��

//#define	NUM_LIGHT		(3)												// ���C�g�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureEnemy;		// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			g_pD3DXMeshEnemy;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatEnemy;	// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatEnemy;			// �}�e���A�����̐�

ENEMY				enemy[1];

//D3DXVECTOR3			g_posEnemy;				// ���f���̈ʒu
//D3DXVECTOR3			g_rotEnemy;				// ���f���̌���(��])
//D3DXVECTOR3			g_sclEnemy;				// ���f���̑傫��(�X�P�[��)

D3DXMATRIX			enemy_mtxWorld;				// ���[���h�}�g���b�N�X


int					enemy_nIdxShadow;				// �eID
float				enemy_fSizeShadow;				// �e�̃T�C�Y
D3DXCOLOR			enemy_colShadow;				// �e�̐F

//D3DLIGHT9			g_aLight[NUM_LIGHT];	// ���C�g���

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);
	//// �J�����̏�����
	//InitCamera();

	//// ���C�g�̏�����
	//InitLight();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	enemy->g_posEnemy = D3DXVECTOR3(50.0f, 20.0f, 0.0f);
	enemy->g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemy->g_sclEnemy = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	enemy->g_moveEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemy->g_rotEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	enemy->g_rotDestEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_pD3DTextureEnemy = NULL;
	g_pD3DXMeshEnemy = NULL;
	g_pD3DXBuffMatEnemy = NULL;
	g_nNumMatEnemy = 0;

	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(PLAYER_CAR,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pD3DXBuffMatEnemy,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMatEnemy,		// D3DXMATERIAL�\���̂̐�
		&g_pD3DXMeshEnemy)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,		// �t�@�C���̖��O
		&g_pD3DTextureEnemy);	// �ǂݍ��ރ������[
#endif
		// �e�̐���
	enemy_nIdxShadow = CreateShadow(enemy->g_posEnemy, 25.0f, 25.0f);
	enemy_fSizeShadow = 25.0f;
	enemy_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	if (g_pD3DTextureEnemy != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureEnemy->Release();
		g_pD3DTextureEnemy = NULL;
	}

	if (g_pD3DXMeshEnemy != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshEnemy->Release();
		g_pD3DXMeshEnemy = NULL;
	}

	if (g_pD3DXBuffMatEnemy != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatEnemy->Release();
		g_pD3DXBuffMatEnemy = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// �ړ�
//	if (GetKeyboardPress(DIK_U))
//	{
//		enemy->g_posEnemy.y++;
//	}
//	else if (GetKeyboardPress(DIK_I))
//	{
//		enemy->g_posEnemy.y--;
//	}
//
//	if (GetKeyboardPress(DIK_D))
//	{
//		enemy->g_posEnemy.x++;
//	}
//
//	else if (GetKeyboardPress(DIK_A))
//	{
//		enemy->g_posEnemy.x--;
//	}
//	if (GetKeyboardPress(DIK_W))
//	{
//		enemy->g_posEnemy.z++;
//	}
//
//	else if (GetKeyboardPress(DIK_S))
//	{
//		enemy->g_posEnemy.z--;
//	}
//
//	// ��]
//	//if (GetKeyboardPress(DIK_1))
//	//{
//		enemy->g_rotEnemy.x++;
//	//}
//
//	//else if (GetKeyboardPress(DIK_2))
//	//{
//		enemy->g_rotEnemy.x--;
//	//}
//	if (GetKeyboardPress(DIK_Q))
//	{
//		enemy->g_rotEnemy.y += 0.06f;
//	}
//
//	else if (GetKeyboardPress(DIK_E))
//	{
//		enemy->g_rotEnemy.y -= 0.06f;
//	}
//	if (GetKeyboardPress(DIK_5))
//	{
//		enemy->g_rotEnemy.z++;
//	}
//
//	else if (GetKeyboardPress(DIK_6))
//	{
//		enemy->g_rotEnemy.z--;
//	}
//	if (GetKeyboardPress(DIK_Z))
//	{
//		enemy->g_sclEnemy.x += 0.01f;
//		enemy->g_sclEnemy.y += 0.01f;
//		enemy->g_sclEnemy.z += 0.01f;
//
//	}
//
//	else if (GetKeyboardPress(DIK_C))
//	{
//
//		enemy->g_sclEnemy.x -= 0.01f;
//		enemy->g_sclEnemy.y -= 0.01f;
//		enemy->g_sclEnemy.z -= 0.01f;
//
	}

	//// �e�̈ʒu�ݒ�
	//SetPositionShadow(enemy_nIdxShadow, D3DXVECTOR3(enemy->g_posEnemy.x, 0.1f, enemy->g_posEnemy.z));

	//SetVertexShadow(enemy_nIdxShadow, enemy_fSizeShadow, enemy_fSizeShadow);
	//SetColorShadow(enemy_nIdxShadow, enemy_colShadow);

//	//PrintDebugProc("[��s�@�̈ʒu  �F(%f : %f : %f)]\n", enemy->g_posEnemy.x, enemy->g_posEnemy.y, enemy->g_posEnemy.z);
//	//PrintDebugProc("[��s�@�̌���  �F(%f) < �ړI�̌���:(%f) >]\n", enemy->g_rotModel.y, enemy->g_rotDestModel.y);
//	//PrintDebugProc("\n");
//
//	//PrintDebugProc("*** ��s�@���� ***\n");
//	//PrintDebugProc("�O�ړ� : ��\n");
//	//PrintDebugProc("��ړ� : ��\n");
//	//PrintDebugProc("���ړ� : ��\n");
//	//PrintDebugProc("�E�ړ� : ��\n");
//	//PrintDebugProc("�㏸   : �h\n");
//	//PrintDebugProc("���~   : �j\n");
//	//PrintDebugProc("������ : LSHIFT\n");
//	//PrintDebugProc("�E���� : RSHIFT\n");
//
//}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&enemy_mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, enemy->g_sclEnemy.x, enemy->g_sclEnemy.y, enemy->g_sclEnemy.z);
	D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy->g_rotEnemy.y, enemy->g_rotEnemy.x, enemy->g_rotEnemy.z);
	D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, enemy->g_posEnemy.x, enemy->g_posEnemy.y, enemy->g_posEnemy.z);
	D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &enemy_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureEnemy);

		// �`��
		g_pD3DXMeshEnemy->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}
/**********************************************************************************
** GetEnemy
***********************************************************************************/
ENEMY	*GetEnemy(int pno)
{
	return &enemy[pno];
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionModel(void)
{
	return enemy->g_posEnemy;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationModel(void)
{
	return enemy->g_rotEnemy;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestModel(void)
{
	return enemy->g_rotDestEnemy;
}


