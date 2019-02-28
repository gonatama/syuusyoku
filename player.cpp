//=============================================================================
//
// ���f������ [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
//#include "debugproc.h"
#include "physics.h"
#include "meshfield.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	PLAYER_PANJAN_CORE		"data/PLAYER/panjan_core.x"		// �ǂݍ��ރ��f����
#define PLAYER_PANJAN_SHAFT_R	"data/PLAYER/panjan_shaft_Right.x"
#define PLAYER_PANJAN_SHAFT_L	"data/PLAYER/panjan_shaft_Left.x"
#define PLAYER_PANJAN_WHEEL_R	"data/PLAYER/panjan_wheel_Right.x"
#define PLAYER_PANJAN_WHEEL_L	"data/PLAYER/panjan_wheel_Left.x"
#define PLAYER_PANJAN_ENJIN_R	"data/PLAYER/panjan_enjin_Right.x"
#define PLAYER_PANJAN_ENJIN_L	"data/PLAYER/panjan_enjin_Left.x"







//#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
//#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
////
#define	VALUE_MOVE_PLAYER		(0.50f)					// �ړ���
#define	RATE_MOVE_PLAYER		(0.20f)					// �ړ������W��
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.05f)			// ��]���x
#define	RATE_ROTATE_PLAYER	(0.20f)						// ��]�����W��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer;				// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH			g_pD3DXMeshPlayer;					// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatPlayer;				// �}�e���A�����ւ̃|�C���^
DWORD				g_nNumMatPlayer;					// �}�e���A�����̐�

//LPD3DXMESH		D3DXMeshPlayer_core;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
//LPD3DXBUFFER		D3DXBuffMatPlayer_core;				// ���b�V���̃}�e���A�������i�[
//DWORD				NumMatPlayer_core;					// �������̑���

LPD3DXMESH			D3DXMeshPlayer_shaft_r;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatPlayer_shaft_r;			// ���b�V���̃}�e���A�������i�[
DWORD				NumMatPlayer_shaft_r;			// �������̑���

LPD3DXMESH			D3DXMeshPlayer_shaft_l;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatPlayer_shaft_l;			// ���b�V���̃}�e���A�������i�[
DWORD				NumMatPlayer_shaft_l;				// �������̑���

LPD3DXMESH			D3DXMeshPlayer_wheel_r;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatPlayer_wheel_r;			// ���b�V���̃}�e���A�������i�[
DWORD				NumMatPlayer_wheel_r;					// �������̑���

LPD3DXMESH			D3DXMeshPlayer_wheel_l;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatPlayer_wheel_l;			// ���b�V���̃}�e���A�������i�[
DWORD				NumMatPlayer_wheel_l;					// �������̑���

LPD3DXMESH			D3DXMeshPlayer_enjin_r;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatPlayer_enjin_r;			// ���b�V���̃}�e���A�������i�[
DWORD				NumMatPlayer_enjin_r;					// �������̑���

LPD3DXMESH			D3DXMeshPlayer_enjin_l;				// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		D3DXBuffMatPlayer_enjin_l;			// ���b�V���̃}�e���A�������i�[
DWORD				NumMatPlayer_enjin_l;					// �������̑���

//D3DXVECTOR3			osPlayer;				// ���݂̈ʒu
//D3DXVECTOR3			movePlayer;				// �ړ���
//D3DXVECTOR3			rotPlayer;				// ���݂̌���
//D3DXVECTOR3			rotDestPlayer;			// �ړI�̌���





PLAYER				player[PLAYER_MAX];

//D3DXVECTOR3			g_posPlayer;				// ���f���̈ʒu
//D3DXVECTOR3			g_rotPlayer;				// ���f���̌���(��])
//D3DXVECTOR3			g_sclPlayer;				// ���f���̑傫��(�X�P�[��)

D3DXMATRIX			mtxWorld;				// ���[���h�}�g���b�N�X
D3DXMATRIX			mtxWorld_core;		// ���[���h�}�g���b�N�X(core)


int					IdxShadow;				// �eID
float				SizeShadow;				// �e�̃T�C�Y
D3DXCOLOR			colShadow;				// �e�̐F



//D3DLIGHT9			g_aLight[NUM_LIGHT];	// ���C�g���

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	player->posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->sclPlayer = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//player->osPlayer = D3DXVECTOR3(0.0f, 20.0f, 0.0f);			// ���݂̈ʒu

	player->movePlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->rotDestPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_pD3DTexturePlayer = NULL;
	g_pD3DXMeshPlayer = NULL;
	g_pD3DXBuffMatPlayer = NULL;
	g_nNumMatPlayer = 0;

	D3DXMeshPlayer_shaft_r = NULL;
	D3DXBuffMatPlayer_shaft_r = NULL;
	NumMatPlayer_shaft_r = 0;

	D3DXMeshPlayer_shaft_l = NULL;
	D3DXBuffMatPlayer_shaft_l = NULL;
	NumMatPlayer_shaft_l = 0;

	D3DXMeshPlayer_wheel_r = NULL;
	D3DXBuffMatPlayer_wheel_r = NULL;
	NumMatPlayer_wheel_r = 0;

	D3DXMeshPlayer_wheel_l = NULL;
	D3DXBuffMatPlayer_wheel_l = NULL;
	NumMatPlayer_wheel_l = 0;

	D3DXMeshPlayer_enjin_r = NULL;
	D3DXBuffMatPlayer_enjin_r = NULL;
	NumMatPlayer_enjin_r = 0;

	D3DXMeshPlayer_enjin_l = NULL;
	D3DXBuffMatPlayer_enjin_l = NULL;
	NumMatPlayer_enjin_l = 0;

	player->air = false;
	// X�t�@�C���̓ǂݍ���
	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_CORE,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_pD3DXBuffMatPlayer,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_nNumMatPlayer,		// D3DXMATERIAL�\���̂̐�
		&g_pD3DXMeshPlayer)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_SHAFT_R,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatPlayer_shaft_r,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatPlayer_shaft_r,		// D3DXMATERIAL�\���̂̐�
		&D3DXMeshPlayer_shaft_r)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_SHAFT_L,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatPlayer_shaft_l,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatPlayer_shaft_l,		// D3DXMATERIAL�\���̂̐�
		&D3DXMeshPlayer_shaft_l)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_WHEEL_R,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatPlayer_wheel_r,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatPlayer_wheel_r,		// D3DXMATERIAL�\���̂̐�
		&D3DXMeshPlayer_wheel_r)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_WHEEL_L,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatPlayer_wheel_l,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatPlayer_wheel_l ,		// D3DXMATERIAL�\���̂̐�
		&D3DXMeshPlayer_wheel_l)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_ENJIN_R,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatPlayer_enjin_r,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatPlayer_enjin_r,		// D3DXMATERIAL�\���̂̐�
		&D3DXMeshPlayer_enjin_r)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_ENJIN_L,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,		// ���b�V���̍쐬�I�v�V�������w��
		pDevice,				// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,					// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&D3DXBuffMatPlayer_enjin_l,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,					// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&NumMatPlayer_enjin_l,		// D3DXMATERIAL�\���̂̐�
		&D3DXMeshPlayer_enjin_l)))	// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
	{
		return E_FAIL;
	}


#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
		TEXTURE_FILENAME,		// �t�@�C���̖��O
		&g_pD3DTexturePlayer);	// �ǂݍ��ރ������[
#endif
		// �e�̐���
	IdxShadow = CreateShadow(player->posPlayer, 25.0f, 25.0f);
	SizeShadow = 25.0f;
	colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	if(g_pD3DTexturePlayer != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}

	if(g_pD3DXMeshPlayer != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshPlayer->Release();
		g_pD3DXMeshPlayer = NULL;
	}

	if(g_pD3DXBuffMatPlayer != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatPlayer->Release();
		g_pD3DXBuffMatPlayer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	CAMERA_WORK		*cwk = get_camerawk();
	D3DXVECTOR3		oldPos = player->posPlayer;		// ���̈ʒu
	D3DXVECTOR3		move = D3DXVECTOR3(0, 0, 0);

	//D3DXVECTOR3 rotCamera;

	float DiffRotY;

	//rotCamera = GetRotCamera();

	// �ړ�
	//if (GetKeyboardPress(DIK_U))
	//{
	//	//player->g_posPlayer.y++;
	//	player->g_posPlayer.y++;
	//	//player->g_posPlayer.z += sinf(rotCamera.y)*VALUE_MOVE;

	//}
	//else if(GetKeyboardPress(DIK_I))
	//{
	//	player->g_posPlayer.y--;

	//	//player->g_posPlayer.x += cosf(rotCamera.y)*VALUE_MOVE;
	//	//player->g_posPlayer.z += sinf(rotCamera.y)*VALUE_MOVE;
	//}

	//if (GetKeyboardPress(DIK_D))
	//{
	//	player->g_posPlayer.x += cosf(rotCamera.y)*VALUE_MOVE;
	//	player->g_posPlayer.z += sinf(rotCamera.y)*VALUE_MOVE;
	//}

	//else if (GetKeyboardPress(DIK_A))
	//{
	//	player->g_posPlayer.x -= cosf(rotCamera.y)*VALUE_MOVE;
	//	player->g_posPlayer.z += sinf(rotCamera.y)*VALUE_MOVE;
	//}
	//if (GetKeyboardPress(DIK_W))
	//{
	//	player->g_posPlayer.x += cosf(rotCamera.y)*VALUE_MOVE;
	//	player->g_posPlayer.z += sinf(rotCamera.y)*VALUE_MOVE;
	//}

	//else if (GetKeyboardPress(DIK_S))
	//{
	//	player->g_posPlayer.x += cosf(rotCamera.y)*VALUE_MOVE;
	//	player->g_posPlayer.z -= sinf(rotCamera.y)*VALUE_MOVE;
	//}

	//// ��]
	//if (GetKeyboardPress(DIK_1))
	//{
	//	player->g_rotPlayer.x++;
	//}

	//else if (GetKeyboardPress(DIK_2))
	//{
	//	player->g_rotPlayer.x--;
	//}
	//if (GetKeyboardPress(DIK_Q))
	//{
	//	player->g_rotPlayer.y += 0.06f;
	//}

	//else if (GetKeyboardPress(DIK_E))
	//{
	//	player->g_rotPlayer.y -= 0.06f;
	//}
	//if (GetKeyboardPress(DIK_5))
	//{
	//	player->g_rotPlayer.z++;
	//}

	//else if (GetKeyboardPress(DIK_6))
	//{
	//	player->g_rotPlayer.z--;
	//}

	////�g��
	//if (GetKeyboardPress(DIK_Z))
	//{
	//	player->g_sclPlayer.x += 0.01f;
	//	player->g_sclPlayer.y += 0.01f;
	//	player->g_sclPlayer.z += 0.01f;

	//}

	//else if (GetKeyboardPress(DIK_C))
	//{

	//	player->g_sclPlayer.x -= 0.01f;
	//	player->g_sclPlayer.y -= 0.01f;
	//	player->g_sclPlayer.z -= 0.01f;

	//}


	//if (GetKeyboardPress(DIK_LEFT))
	//{
	//		player->rotPlayer.x += VALUE_ROTATE_PLAYER;
	//		if (player->rotPlayer.x < -D3DX_PI)
	//		{
	//			player->rotPlayer.x += D3DX_PI * 1.5f;
	//		}

	//	if (GetKeyboardPress(DIK_UP))
	//	{// ���O�ړ�
	//		player->posPlayer.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
	//		
	//		player->rotDestPlayer.y = rotCamera.y + D3DX_PI * 0.75f;
	//		//player->rotDestPlayer.z += D3DX_PI * 2.0f;

	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// ����ړ�
	//		player->posPlayer.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

	//		player->rotDestPlayer.y = rotCamera.y + D3DX_PI * 0.25f;
	//	}
	//	else
	//	{// ���ړ�
	//		player->posPlayer.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

	//		player->rotDestPlayer.y = rotCamera.y + D3DX_PI * 0.50f;
	//	}

	//}
	//else if (GetKeyboardPress(DIK_RIGHT))
	//{
	//		player->rotPlayer.x += VALUE_ROTATE_PLAYER;
	//		if (player->rotPlayer.x < -D3DX_PI)
	//		{
	//			player->rotPlayer.x -= D3DX_PI * 1.5f;
	//		}

	//	if (GetKeyboardPress(DIK_UP))
	//	{// �E�O�ړ�
	//		player->posPlayer.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

	//		player->rotDestPlayer.y = rotCamera.y - D3DX_PI * 0.75f;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// �E��ړ�
	//		player->posPlayer.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

	//		player->rotDestPlayer.y = rotCamera.y - D3DX_PI * 0.25f;
	//	}
	//	else
	//	{// �E�ړ�
	//		player->posPlayer.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

	//		player->rotDestPlayer.y = rotCamera.y - D3DX_PI * 0.50f;

	//	}

	//}
//if (GetKeyboardPress(DIK_LEFT))
//{
//// ���ړ�
//		player->posPlayer.x += sinf(cwk->rotCameraP.y) * VALUE_MOVE_PLAYER;
//		player->posPlayer.z += cosf(cwk->rotCameraP.y) * VALUE_MOVE_PLAYER;
//
//	
//
//}
//else if (GetKeyboardPress(DIK_RIGHT))
//{
//		player->posPlayer.x -= sinf(cwk->rotCameraP.y) * VALUE_MOVE_PLAYER;
//		player->posPlayer.z -= cosf(cwk->rotCameraP.y) * VALUE_MOVE_PLAYER;
//
//}


	 if (GetKeyboardPress(DIK_UP))
		{// �O�ړ�
		player->rotPlayer.x += VALUE_ROTATE_PLAYER;
		if (player->rotPlayer.x < -D3DX_PI)
		{
			player->rotPlayer.x -= D3DX_PI * 1.5f;
		}

			player->movePlayer.x -= sinf(D3DX_PI + cwk->rotCameraP.y) * VALUE_MOVE_PLAYER;
			player->movePlayer.z -= cosf(D3DX_PI + cwk->rotCameraP.y) * VALUE_MOVE_PLAYER;

			player->rotDestPlayer.y = D3DX_PI + cwk->rotCameraP.y;
		}
	else if (GetKeyboardPress(DIK_DOWN))
		{// ��ړ�
		player->rotPlayer.x += VALUE_ROTATE_PLAYER;
		if (player->rotPlayer.x < -D3DX_PI)
		{
			player->rotPlayer.x -= D3DX_PI * 1.5f;
		}

			player->movePlayer.x -= sinf(cwk->rotCameraP.y) * VALUE_MOVE_PLAYER;
			player->movePlayer.z -= cosf(cwk->rotCameraP.y) * VALUE_MOVE_PLAYER;

			player->rotDestPlayer.y = cwk->rotCameraP.y;
		}



	//if (GetKeyboardPress(DIK_LSHIFT))
	//{// ����]
	//	player->rotPlayer.z += VALUE_ROTATE_PLAYER;
	//	if (player->rotPlayer.z < -D3DX_PI)
	//	{
	//		player->rotPlayer.z += D3DX_PI * 2.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_RSHIFT))
	//{// �E��]
	//	player->rotDestPlayer.y += VALUE_ROTATE_PLAYER;
	//	if (player->rotDestPlayer.y > D3DX_PI)
	//	{
	//		player->rotDestPlayer.y -= D3DX_PI * 2.0f;
	//	}
	//}

	if (GetKeyboardPress(DIK_LEFT))
	{
		cwk->rotCameraP.y -= VALUE_ROTATE;
		if (cwk->rotCameraP.y > D3DX_PI)
		{
			cwk->rotCameraP.y -= D3DX_PI * 2.0f;
		}
		if (cwk->rotCameraP.y < -D3DX_PI)
		{
			cwk->rotCameraP.y += D3DX_PI * 2.0f;
		}
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{// �E��]
		cwk->rotCameraP.y += VALUE_ROTATE;
		if (cwk->rotCameraP.y > D3DX_PI)
		{
			cwk->rotCameraP.y -= D3DX_PI * 2.0f;
		}
		if (cwk->rotCameraP.y < -D3DX_PI)
		{
			cwk->rotCameraP.y += D3DX_PI * 2.0f;
		}
	}

	if (GetKeyboardPress(DIK_I))
	{// �㏸
		player->movePlayer.y += VALUE_MOVE_PLAYER * 0.5f;

			if (player->posPlayer.y < 100.0f)
		{
			SizeShadow += 0.2f;
			colShadow.a -= 0.004f;
		}
	}
	if (GetKeyboardPress(DIK_K))
	{// ���~
		player->movePlayer.y -= VALUE_MOVE_PLAYER * 0.5f;

		if (player->posPlayer.y > 5.0f)
		{
			SizeShadow -= 0.2f;
			colShadow.a += 0.004f;
		}
	}
	if(GetKeyboardTrigger(DIK_SPACE)&&(player->air==false))
	{
		
			Jump(player);
	} 

	// �ړI�̊p�x�܂ł̍���
	DiffRotY = player->rotDestPlayer.y - player->rotPlayer.y;
	if (DiffRotY > D3DX_PI)
	{
		DiffRotY -= D3DX_PI * 2.0f;
	}
	if (DiffRotY < -D3DX_PI)
	{
		DiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	player->rotPlayer.y += DiffRotY * RATE_ROTATE_PLAYER;
	if (player->rotPlayer.y > D3DX_PI)
	{
		player->rotPlayer.y -= D3DX_PI * 2.0f;
	}
	if (player->rotPlayer.y < -D3DX_PI)
	{
		player->rotPlayer.y += D3DX_PI * 2.0f;
	}
	/// �ʒu�ړ�
	player->posPlayer.x += player->movePlayer.x;
	player->posPlayer.y += player->movePlayer.y;

	//if (player->posPlayer.y < 5.0f)
	//{
	//	player->posPlayer.y = 5.0f;
	//	player->air = false;
	//}
	//if (player->posPlayer.y > 150.0f)
	//{
	//	player->posPlayer.y = 150.0f;
	//}

	player->posPlayer.z += player->movePlayer.z;
	// �ړ��ʂɊ�����������
	player->movePlayer.x += (0.0f - player->movePlayer.x) * RATE_MOVE_PLAYER;
	player->movePlayer.y += (0.0f - player->movePlayer.y) * RATE_MOVE_PLAYER;
	player->movePlayer.z += (0.0f - player->movePlayer.z) * RATE_MOVE_PLAYER;

	//if (player->posPlayer.x < -310.0f)
	//{
	//	player->posPlayer.x = -310.0f;
	//}
	//if (player->posPlayer.x > 310.0f)
	//{
	//	player->posPlayer.x = 310.0f;
	//}
	//if (player->posPlayer.z < -310.0f)
	//{
	//	player->posPlayer.z = -310.0f;
	//}
	//if (player->posPlayer.z > 310.0f)
	//{
	//	player->posPlayer.z = 310.0f;
	//}



	//D3DXVECTOR3		vec = move - oldPos;
	//float			len = D3DXVec3Length(&vec);
	//if (len > 5.0f)
	//{	// �r���{�[�h�Ƃ̓����蔻��
	//	if (hitCheckBill(oldPos, move) != 0)
	//	{

	//		player->hit = true;
	//		// �������Ă���̂Ō��̈ʒu�ɖ߂�
	//		player->posPlayer = oldPos;
	//	}
	//}


	//if (player->posPlayer.y < Hitfield(player->posPlayer))
	//{

	if (player->posPlayer.y > Hitfield(player->posPlayer))
	{
		player->air=true;
	}
	else if (player->posPlayer.y< Hitfield(player->posPlayer))
	{
		player->air = false;
	}
	//	player->air = false;
	//}


	// �e�̈ʒu�ݒ�
	SetPositionShadow(IdxShadow, D3DXVECTOR3(player->posPlayer.x, 0.1f, player->posPlayer.z));

	SetVertexShadow(IdxShadow, SizeShadow, SizeShadow);
	SetColorShadow(IdxShadow, colShadow);

	//PrintDebugProc("[��s�@�̈ʒu  �F(%f : %f : %f)]\n", player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
	//PrintDebugProc("[��s�@�̌���  �F(%f) < �ړI�̌���:(%f) >]\n", player->rotPlayer.y, player->rotDestPlayer.y);
	//PrintDebugProc("[��s�@�̌���  �F(%d)\n", player->hit );

	//PrintDebugProc("\n");

	//PrintDebugProc("*** ��s�@���� ***\n");
	//PrintDebugProc("�O�ړ� : ��\n");
	//PrintDebugProc("��ړ� : ��\n");
	//PrintDebugProc("���ړ� : ��\n");
	//PrintDebugProc("�E�ړ� : ��\n");
	//PrintDebugProc("�㏸   : �h\n");
	//PrintDebugProc("���~   : �j\n");
	//PrintDebugProc("������ : LSHIFT\n");
	//PrintDebugProc("�E���� : RSHIFT\n");

}

//=============================================================================
// CORE(�e)�̕`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);




	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatPlayer->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_nNumMatPlayer; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// �`��
		g_pD3DXMeshPlayer->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	mtxWorld_core = mtxWorld;


	DrawPlayer_shaft_r();
	DrawPlayer_shaft_l();
	DrawPlayer_wheel_r();
	DrawPlayer_wheel_l();
	DrawPlayer_enjin_r();
	DrawPlayer_enjin_l();

}
//********************************************************************************************************
//***	�V���t�g�i�E�j�̕`�揈��
//********************************************************************************************************
void DrawPlayer_shaft_r(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//// �X�P�[���𔽉f
	//D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	//// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//// �ړ��𔽉f
	//D3DXMatrixTranslation(&mtxTranslate, player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


		// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	//�e�̃}�g���N�X��������
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_shaft_r->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_shaft_r; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// �`��
		D3DXMeshPlayer_shaft_r->DrawSubset(nCntMat);
	}
	//mtxWorldPlayer_body = mtxWorldPlayer;

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	�V���t�g�i���j�̕`�揈��
//********************************************************************************************************
void DrawPlayer_shaft_l(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	//�e�̃}�g���N�X��������
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_shaft_l->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_shaft_l; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// �`��
		D3DXMeshPlayer_shaft_l->DrawSubset(nCntMat);
	}
	//mtxWorldPlayer_body = mtxWorldPlayer;

	mtxWorld_core = mtxWorld;
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	�ԗցi�E�j�̕`�揈��
//********************************************************************************************************
void DrawPlayer_wheel_r(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);

	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0,0.0,0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());



	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_wheel_r->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_wheel_r; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// �`��
		D3DXMeshPlayer_wheel_r->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	�ԗցi���j�̕`�揈��
//********************************************************************************************************
void DrawPlayer_wheel_l(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);


	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());



	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_wheel_l->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_wheel_l; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// �`��
		D3DXMeshPlayer_wheel_l->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	�G���W���i�E�j�̕`�揈��
//********************************************************************************************************
void DrawPlayer_enjin_r(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_enjin_r->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_enjin_r; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// �`��
		D3DXMeshPlayer_enjin_r->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	�G���W���i���j�̕`�揈��
//********************************************************************************************************
void DrawPlayer_enjin_l(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, 0.0,0.0,0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());


	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_enjin_l->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_enjin_l; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// �`��
		D3DXMeshPlayer_enjin_l->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}


/**********************************************************************************
** GetPlayer
***********************************************************************************/
PLAYER	*GetPlayer(int pno)
{
	return &player[pno];
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return (player->posPlayer);
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return player->rotPlayer;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestPlayer(void)
{
	return player->rotDestPlayer;
}

//*******************************************************************************
//**
//*******************************************************************************
D3DXMATRIX *GetPlayerMatrix(void)
{
	return &mtxWorld_core;
}


////D3DXVECTOR3	*GetPosModel(void)
////{
////	return(&g_posModel);
////}
/////////////////////////////////////////////////
//// �N�H�[�^�j�I������]�s��ϊ�
////
//// m11-m33 : ��]�s�񐬕��i�o�́j
//// qx, qy, qz, qw : �N�H�[�^�j�I������
////
//// �����ӁF
//// �s�񐬕���DirectX�`���i�s���������̌����j�ł�
//// OpenGL�`���i����������̌����j�̏ꍇ��
//// �]�u�����l���i�[����悤�ɂ��ĉ������B
//
//void transformQuaternionToRotMat(
//	float &m11, float &m12, float &m13,
//	float &m21, float &m22, float &m23,
//	float &m31, float &m32, float &m33,
//	float qx, float qy, float qz, float qw
//) {
//	m11 = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
//	m12 = 2.0f * qx * qy + 2.0f * qw * qz;
//	m13 = 2.0f * qx * qz - 2.0f * qw * qy;
//
//	m21 = 2.0f * qx * qy - 2.0f * qw * qz;
//	m22 = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
//	m23 = 2.0f * qy * qz + 2.0f * qw * qx;
//
//	m31 = 2.0f * qx * qz + 2.0f * qw * qy;
//	m32 = 2.0f * qy * qz - 2.0f * qw * qx;
//	m33 = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;
//}
//
//
/////////////////////////////////////////////////
//// �N�H�[�^�j�I������]�s��ϊ�
////
//// m1-m3 : ��]�s�񐬕��i�o�́j
//// q : �N�H�[�^�j�I������(x,y,z,w)
////
//// �����ӁF
//// �s�񐬕���DirectX�`���i�s���������̌����j�ł�
//// OpenGL�`���i����������̌����j�̏ꍇ��
//// �]�u�����l���i�[����悤�ɂ��ĉ������B
//
//void transformQuaternionToRotMat(
//	float m1[3],
//	float m2[3],
//	float m3[3],
//	const float q[4]
//) {
//	transformQuaternionToRotMat(
//		m1[0], m1[1], m1[2],
//		m2[0], m2[1], m2[2],
//		m3[0], m3[1], m3[2],
//		q[0], q[1], q[2], q[3]
//	);
//}
//
//
/////////////////////////////////////////////////
//// �N�H�[�^�j�I������]�s��ϊ�
////
//// m : ��]�s�񐬕��i�o�́j
//// q : �N�H�[�^�j�I������(x, y, z, w)
////
//// �����ӁF
//// �s�񐬕���DirectX�`���i�s���������̌����j�ł�
//// OpenGL�`���i����������̌����j�̏ꍇ��
//// �]�u�����l���i�[����悤�ɂ��ĉ������B
//
//void transformQuaternionToRotMat(
//	float m[16],
//	const float q[4]
//) {
//	memset(m, 0, sizeof(float) * 16);
//	transformQuaternionToRotMat(
//		m[0], m[1], m[2],
//		m[4], m[5], m[6],
//		m[8], m[9], m[10],
//		q[0], q[1], q[2], q[3]
//	);
//	m[15] = 1.0f;
//}
//
//
/////////////////////////////////////////////////
//// ��]�s�񁨃N�H�[�^�j�I���ϊ�
////
//// qx, qy, qz, qw : �N�H�[�^�j�I�������i�o�́j
//// m11-m33 : ��]�s�񐬕�
////
//// �����ӁF
//// �s�񐬕���DirectX�`���i�s���������̌����j�ł�
//// OpenGL�`���i����������̌����j�̏ꍇ��
//// �]�u�����l�����ĉ������B
//
//bool transformRotMatToQuaternion(
//	float &qx, float &qy, float &qz, float &qw,
//	float m11, float m12, float m13,
//	float m21, float m22, float m23,
//	float m31, float m32, float m33
//) {
//	// �ő听��������
//	float elem[4]; // 0:x, 1:y, 2:z, 3:w
//	elem[0] = m11 - m22 - m33 + 1.0f;
//	elem[1] = -m11 + m22 - m33 + 1.0f;
//	elem[2] = -m11 - m22 + m33 + 1.0f;
//	elem[3] = m11 + m22 + m33 + 1.0f;
//
//	unsigned biggestIndex = 0;
//	for (int i = 1; i < 4; i++) {
//		if (elem[i] > elem[biggestIndex])
//			biggestIndex = i;
//	}
//
//	if (elem[biggestIndex] < 0.0f)
//		return false; // �����̍s��ɊԈႢ����I
//
//	// �ő�v�f�̒l���Z�o
//	float *q[4] = { &qx, &qy, &qz, &qw };
//	float v = sqrtf(elem[biggestIndex]) * 0.5f;
//	*q[biggestIndex] = v;
//	float mult = 0.25f / v;
//
//	switch (biggestIndex) {
//	case 0: // x
//		*q[1] = (m12 + m21) * mult;
//		*q[2] = (m31 + m13) * mult;
//		*q[3] = (m23 - m32) * mult;
//		break;
//	case 1: // y
//		*q[0] = (m12 + m21) * mult;
//		*q[2] = (m23 + m32) * mult;
//		*q[3] = (m31 - m13) * mult;
//		break;
//	case 2: // z
//		*q[0] = (m31 + m13) * mult;
//		*q[1] = (m23 + m32) * mult;
//		*q[3] = (m12 - m21) * mult;
//		break;
//	case 3: // w
//		*q[0] = (m23 - m32) * mult;
//		*q[1] = (m31 - m13) * mult;
//		*q[2] = (m12 - m21) * mult;
//		break;
//	}
//
//	return true;
//}
//
/////////////////////////////////////////////////
//// ��]�s�񁨃N�H�[�^�j�I���ϊ�
////
//// qx, qy, qz, qw : �N�H�[�^�j�I�������i�o�́j
//// m1[3] : ��]�s�񐬕� m11 - m13
//// m2[3] : ��]�s�񐬕� m21 - m23
//// m3[3] : ��]�s�񐬕� m31 - m33
////
//// �����ӁF
//// �s�񐬕���DirectX�`���i�s���������̌����j�ł�
//// OpenGL�`���i����������̌����j�̏ꍇ��
//// �]�u�����l�����ĉ������B
//
//bool transformRotMatToQuaternion(
//	float q[4],
//	const float m1[3],
//	const float m2[3],
//	const float m3[3]
//) {
//	return transformRotMatToQuaternion(
//		q[0], q[1], q[2], q[3],
//		m1[0], m1[1], m1[2],
//		m2[0], m2[1], m2[2],
//		m3[0], m3[1], m3[2]
//	);
//}
//
//////////////////////////////////////////////
//// �N�H�[�^�j�I�����ʐ��`���
////
//
//bool slerpQuaternion(
//	float out[4],
//	const float q1[4],
//	const float q2[4],
//	float t
//) {
//	// �p�x�Z�o
//	const float len1 = sqrt(q1[0] * q1[0] + q1[1] * q1[1] + q1[2] * q1[2] + q1[3] * q1[3]);
//	const float len2 = sqrt(q2[0] * q2[0] + q2[1] * q2[1] + q2[2] * q2[2] + q2[3] * q2[3]);
//
//	if (len1 == 0.0f || len2 == 0.0f)
//		return false; // �s���ȃN�H�[�^�j�I��
//
//	const float cos_val = (q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3]) / (len1 * len2);
//	const float w = acosf(cos_val);
//
//	// ���ʐ��`���
//	const float sin_w = sinf(w);
//	const float sin_t_w = sinf(t * w);
//	const float sin_inv_t_w = sinf((1.0f - t) * w);
//	const float mult_q1 = sin_inv_t_w / sin_w;
//	const float mult_q2 = sin_t_w / sin_w;
//
//	for (int i = 0; i < 4; i++)
//		out[i] = mult_q1 * q1[i] + mult_q2 * q2[i];
//
//	return true;
//}
//
//////////////////////////////////////////////
//// ��]�s��ɂ����(DirectX��p)
////
//// out : ��ԉ�]�s��i�o�́j
//// rm1 : �n�_��]�s��
//// rm2 : �I�_��]�s��
//// t : ��Ԓl�i0.0f�`1.0f�j
//
//D3DXMATRIX *slerpRotMatDX(
//	D3DXMATRIX *out,
//	const D3DXMATRIX *rm1,
//	const D3DXMATRIX *rm2,
//	float t
//) {
//	// �n�_�E�I�_�̃N�H�[�^�j�I�����Z�o
//	D3DXQUATERNION Q, Q1, Q2;
//	D3DXQuaternionRotationMatrix(&Q1, rm1);
//	D3DXQuaternionRotationMatrix(&Q2, rm2);
//
//	// �N�H�[�^�j�I���Ԃ̋��ʐ��`���
//	D3DXQuaternionSlerp(&Q, &Q1, &Q2, t);
//
//	// ��]�s��ɖ߂�
//	D3DXMatrixRotationQuaternion(out, &Q);
//
//	return out;
//}
//
//////////////////////////////////////////////
//// ��]�s��ɂ����(�ėp)
////
//// out : ��ԉ�]�s��i�o�́j
//// rm1 : �n�_��]�s��
//// rm2 : �I�_��]�s��
//// t : ��Ԓl�i0.0f�`1.0f�j
//
//void slerpRotMat(
//	float out[16],
//	const float rm1[16],
//	const float rm2[16],
//	float t
//) {
//	memset(out, 0, sizeof(float) * 16);
//
//	// �n�_�E�I�_�̃N�H�[�^�j�I�����Z�o
//	float Q[4], Q1[4], Q2[4];
//	const float *x1 = rm1 + 0;
//	const float *y1 = rm1 + 4;
//	const float *z1 = rm1 + 8;
//	const float *x2 = rm2 + 0;
//	const float *y2 = rm2 + 4;
//	const float *z2 = rm2 + 8;
//	transformRotMatToQuaternion(Q1, x1, y1, z1);
//	transformRotMatToQuaternion(Q2, x2, y2, z2);
//
//	// �N�H�[�^�j�I���Ԃ̋��ʐ��`���
//	slerpQuaternion(Q, Q1, Q2, t);
//
//	// ��]�s��ɖ߂�
//	transformQuaternionToRotMat(out, Q);
//}
