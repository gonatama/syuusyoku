//=============================================================================
//
// �J����
//
//=============================================================================
#define	_CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "camera.h"
#include "input.h"
#include "player.h"
#include "billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA_WORK		cwk[1];

int				g_debug = 1;

//=============================================================================
// �J�����̏�����
//=============================================================================
void InitCamera(void)
{
	//cwk->posCameraP = D3DXVECTOR3(0.0f, 500.0f, -250.0f);
	//cwk->posCameraR = D3DXVECTOR3(0.0f, 0.0f, 250.0f);
	cwk->posCameraP = D3DXVECTOR3(0.0f, 0.0f, 200.0f);
	cwk->posCameraR = D3DXVECTOR3(0.0f, 0.0f, 400.0f);
	cwk->posCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// �J�������_�̉�]�p�x������
	cwk->rotCameraP = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �����_�Ǝ��_�̋���
	cwk->cam_len = 150.0f;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�������_�ړ�����
//=============================================================================

// �J�����̈ʒu = �J�����̒����_ + �����_����̑��ΓI�ʒu
// �J�����̈ʒu = �J�����̒����_ + (�����_����̊p�x * ���_�܂ł̋���)

void calc_posCameraP(D3DXVECTOR3 pno)
{
	D3DXVECTOR3 vec(0.0f, 0.25f, -1.0f);		// ���K�����ꂽ�����_����̌���
	D3DXMATRIX	mtx;

	cwk->posCameraR = pno;

	// �J�����̑��ΓI�ȉ�]�p�x���s��ɕϊ�����
	D3DXMatrixRotationYawPitchRoll(&mtx, cwk->rotCameraP.y, cwk->rotCameraP.x, cwk->rotCameraP.z);

	// ���K�����ꂽ�_�ɉ�]�s����|����
	D3DXVec3TransformCoord(&vec, &vec, &mtx);

	// �J�����̈ʒu = �J�����̒����_ + (�����_����̊p�x * ���_�܂ł̋���)
	cwk->posCameraP = cwk->posCameraR + (vec * cwk->cam_len);


}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *player = GetPlayer(0);

	if (GetKeyboardPress(DIK_Q))
	{// Y������]
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

	if (GetKeyboardPress(DIK_E))
	{// Y���E��]
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

	//if(GetKeyboardPress(DIK_C))
	//{// X������]
	//	cwk->rotCameraP.x -= VALUE_ROTATE;
	//	if(cwk->rotCameraP.x > D3DX_PI)
	//	{
	//		cwk->rotCameraP.x -= D3DX_PI * 2.0f;
	//	}
	//	if(cwk->rotCameraP.x < -D3DX_PI)
	//	{
	//		cwk->rotCameraP.x += D3DX_PI * 2.0f;
	//	}
	//}

	//if(GetKeyboardPress(DIK_V))
	//{// x���E��]
	//	cwk->rotCameraP.x += VALUE_ROTATE;
	//	if(cwk->rotCameraP.x > D3DX_PI)
	//	{
	//		cwk->rotCameraP.x -= D3DX_PI * 2.0f;
	//	}
	//	if(cwk->rotCameraP.x < -D3DX_PI)
	//	{
	//		cwk->rotCameraP.x += D3DX_PI * 2.0f;
	//	}
	//}

	if (GetKeyboardPress(DIK_Z))
	{
		// ���ړ�
		cwk->cam_len -= VALUE_MOVE_CAMERA;
	}
	else if (GetKeyboardPress(DIK_C))
	{
		// �E�ړ�
		cwk->cam_len += VALUE_MOVE_CAMERA;
	}

	calc_posCameraP(player->posPlayer);
	
// �f�o�b�O�\���̂n�m/�n�e�e
	if (GetKeyboardTrigger(DIK_P))
	{
		g_debug = (g_debug + 1) % 2;
	}
}

//=============================================================================
// �J�����̕`�揈��
//=============================================================================
void DrawCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&cwk->mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&cwk->mtxView,
		&cwk->posCameraP,		// �J�����̎��_
		&cwk->posCameraR,		// �J�����̒����_
		&cwk->posCameraU);		// �J�����̏����

// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &cwk->mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&cwk->mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&cwk->mtxProjection,
		VIEW_ANGLE,										// ����p
		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT),	// �A�X�y�N�g��
		VIEW_NEAR_Z,									// �r���[���ʂ�NearZ�l
		VIEW_FAR_Z);									// �r���[���ʂ�FarZ�l

// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &cwk->mtxProjection);
}

//=============================================================================
// �J�������[�N�̃A�h���X���擾
//=============================================================================

CAMERA_WORK *get_camerawk(void)
{
	return(cwk);
}


//=============================================================================
// �r���{�[�h�̃f�o�b�O�\��
//=============================================================================
int DrawDebugCamera(LPD3DXFONT font, RECT rect)
{
	char str[256];

	if (g_debug != 1) return(rect.top);

	rect.top += 16;

	// ���W�\��
	sprintf(str, "�J�����������W�F%.4f %.4f %.4f", cwk->posCameraR.x, cwk->posCameraR.y, cwk->posCameraR.z);
	font->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	rect.top += 16;

	// ���W�\��
	sprintf(str, "�J�������_���W�F%.4f %.4f %.4f", cwk->posCameraP.x, cwk->posCameraP.y, cwk->posCameraP.z);
	font->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	rect.top += 16;

	// ���W�\��
	sprintf(str, "�J�������_��]�F%.4f %.4f %.4f ZXCV", cwk->rotCameraP.x, cwk->rotCameraP.y, cwk->rotCameraP.z);
	font->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	rect.top += 16;

	// �����\��
	sprintf(str, "�����F%.4f BN", cwk->cam_len);
	font->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	rect.top += 16;

	return(rect.top);

}



//=============================================================================
// �J�����̌����̎擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return cwk->posCameraR;
}


////=============================================================================
////
//// �J�������� [camera.cpp]
//// Author : 
////
////=============================================================================
//#include "camera.h"
//#include "player.h"
//#include "input.h"
//#include "debugproc.h"
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// ����p
//#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
//#define	VIEW_NEAR_Z			(10.0f)					// �r���[���ʂ�NearZ�l
//#define	VIEW_FAR_Z			(1000.0f)				// �r���[���ʂ�FarZ�l
//#define	VALUE_MOVE_CAMERA	(2.0f)					// �J�����̈ړ���
//#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// �J�����̉�]��
//
//#define	INTERVAL_CAMERA_R	(12.5f)					// ���f���̎����̐�܂ł̋���
//#define	RATE_CHASE_CAMERA_P	(0.35f)					// �J�����̎��_�ւ̕␳�W��
//#define	RATE_CHASE_CAMERA_R	(0.20f)					// �J�����̒����_�ւ̕␳�W��
//
//#define	CHASE_HEIGHT_P		(100.0f)				// �ǐՎ��̎��_�̍���
//#define	CHASE_HEIGHT_R		(10.0f)					// �ǐՎ��̒����_�̍���
//
////*****************************************************************************
//// �v���g�^�C�v�錾
////*****************************************************************************
//
//
////*****************************************************************************
//// �O���[�o���ϐ�
////*****************************************************************************
//D3DXVECTOR3		posCameraP;				// �J�����̎��_
//D3DXVECTOR3		posCameraR;				// �J�����̒����_
//D3DXVECTOR3		posCameraU;				// �J�����̏����
//D3DXVECTOR3		posCameraPDest;			// �J�����̎��_�̖ړI�ʒu
//D3DXVECTOR3		posCameraRDest;			// �J�����̒����_�̖ړI�ʒu
//D3DXVECTOR3		rotCamera;				// �J�����̉�]
//float			LengthIntervalCamera;	// �J�����̎��_�ƒ����_�̋���
//D3DXMATRIX		mtxView;					// �r���[�}�g���b�N�X
//D3DXMATRIX		mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X
//
////=============================================================================
//// �J�����̏�����
////=============================================================================
//HRESULT InitCamera(void)
//{
//	posCameraP = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
//	posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	posCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//	posCameraPDest = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
//	posCameraRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	float vx,vz;
//	vx = posCameraP.x - posCameraR.x;
//	vz = posCameraP.z - posCameraR.z;
//	LengthIntervalCamera = sqrtf(vx * vx + vz * vz);
//
//	return S_OK;
//}
//
////=============================================================================
//// �J�����̏I������
////=============================================================================
//void UninitCamera(void)
//{
//
//
//}
//
////=============================================================================
//// �J�����̍X�V����
////=============================================================================
//void UpdateCamera(void)
//{
//	if(GetKeyboardPress(DIK_A))
//	{
//		if(GetKeyboardPress(DIK_W))
//		{// ���O�ړ�
//			posCameraP.x -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//			posCameraP.z += sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//		}
//		else if(GetKeyboardPress(DIK_S))
//		{// ����ړ�
//			posCameraP.x -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//			posCameraP.z += sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//		}
//		else
//		{// ���ړ�
//			posCameraP.x -= cosf(rotCamera.y) * VALUE_MOVE_CAMERA;
//			posCameraP.z += sinf(rotCamera.y) * VALUE_MOVE_CAMERA;
//		}
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	else if(GetKeyboardPress(DIK_D))
//	{
//		if(GetKeyboardPress(DIK_W))
//		{// �E�O�ړ�
//			posCameraP.x += cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//			posCameraP.z -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//		}
//		else if(GetKeyboardPress(DIK_S))
//		{// �E��ړ�
//			posCameraP.x += cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//			posCameraP.z -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
//		}
//		else
//		{// �E�ړ�
//			posCameraP.x += cosf(rotCamera.y) * VALUE_MOVE_CAMERA;
//			posCameraP.z -= sinf(rotCamera.y) * VALUE_MOVE_CAMERA;
//		}
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	else if(GetKeyboardPress(DIK_W))
//	{// �O�ړ�
//		posCameraP.x += sinf(rotCamera.y) * VALUE_MOVE_CAMERA;
//		posCameraP.z += cosf(rotCamera.y) * VALUE_MOVE_CAMERA;
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	else if(GetKeyboardPress(DIK_S))
//	{// ��ړ�
//		posCameraP.x -= sinf(rotCamera.y) * VALUE_MOVE_CAMERA;
//		posCameraP.z -= cosf(rotCamera.y) * VALUE_MOVE_CAMERA;
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//
//	if(GetKeyboardPress(DIK_Z))
//	{// ���_����u���v
//		rotCamera.y += VALUE_ROTATE_CAMERA;
//		if(rotCamera.y > D3DX_PI)
//		{
//			rotCamera.y -= D3DX_PI * 2.0f;
//		}
//
//		posCameraP.x = posCameraR.x - sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraP.z = posCameraR.z - cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	if(GetKeyboardPress(DIK_C))
//	{// ���_����u�E�v
//		rotCamera.y -= VALUE_ROTATE_CAMERA;
//		if(rotCamera.y < -D3DX_PI)
//		{
//			rotCamera.y += D3DX_PI * 2.0f;
//		}
//
//		posCameraP.x = posCameraR.x - sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraP.z = posCameraR.z - cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	if(GetKeyboardPress(DIK_Y))
//	{// ���_�ړ��u��v
//		posCameraP.y += VALUE_MOVE_CAMERA;
//	}
//	if(GetKeyboardPress(DIK_N))
//	{// ���_�ړ��u���v
//		posCameraP.y -= VALUE_MOVE_CAMERA;
//	}
//
//	if(GetKeyboardPress(DIK_Q))
//	{// �����_����u���v
//		rotCamera.y -= VALUE_ROTATE_CAMERA;
//		if(rotCamera.y < -D3DX_PI)
//		{
//			rotCamera.y += D3DX_PI*2.0f;
//		}
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	if(GetKeyboardPress(DIK_E))
//	{// �����_����u�E�v
//		rotCamera.y += VALUE_ROTATE_CAMERA;
//		if(rotCamera.y > D3DX_PI)
//		{
//			rotCamera.y -= D3DX_PI*2.0f;
//		}
//
//		posCameraR.x = posCameraP.x + sinf(rotCamera.y) * LengthIntervalCamera;
//		posCameraR.z = posCameraP.z + cosf(rotCamera.y) * LengthIntervalCamera;
//	}
//	if(GetKeyboardPress(DIK_T))
//	{// �����_�ړ��u��v
//		posCameraR.y += VALUE_MOVE_CAMERA;
//	}
//	if(GetKeyboardPress(DIK_B))
//	{// �����_�ړ��u���v
//		posCameraR.y -= VALUE_MOVE_CAMERA;
//	}
//
//	PrintDebugProc("[�J�����̎��_  �F(%f : %f : %f)]\n", posCameraP.x, posCameraP.y, posCameraP.z);
//	PrintDebugProc("[�J�����̒����_�F(%f : %f : %f)]\n", posCameraR.x, posCameraR.y, posCameraR.z);
//	PrintDebugProc("[�J�����̌���  �F(%f)]\n", rotCamera.y);
//	PrintDebugProc("\n");
//
//	PrintDebugProc("*** ���_���� ***\n");
//	PrintDebugProc("�O�ړ� : W\n");
//	PrintDebugProc("��ړ� : S\n");
//	PrintDebugProc("���ړ� : A\n");
//	PrintDebugProc("�E�ړ� : D\n");
//	PrintDebugProc("��ړ� : Y\n");
//	PrintDebugProc("���ړ� : N\n");
//	PrintDebugProc("������ : Z\n");
//	PrintDebugProc("�E���� : C\n");
//	PrintDebugProc("\n");
//
//	PrintDebugProc("*** �����_���� ***\n");
//	PrintDebugProc("��ړ� : T\n");
//	PrintDebugProc("���ړ� : B\n");
//	PrintDebugProc("������ : Q\n");
//	PrintDebugProc("�E���� : E\n");
//	PrintDebugProc("\n");
//}
//
////=============================================================================
//// �J�����̐ݒ菈��
////=============================================================================
//void SetCamera(void)
//{
//	LPDIRECT3DDEVICE9 Device = GetDevice();
//	PLAYER *player = GetPlayer(0);
//	// �r���[�}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&cwk->mtxView);
//
//	// �r���[�}�g���b�N�X�̍쐬
//	D3DXMatrixLookAtLH(&cwk->mtxView,
//						&posCameraP,		// �J�����̎��_
//						&player->posPlayer,		// �J�����̒����_
//						&posCameraU);		// �J�����̏����
//	// �r���[�}�g���b�N�X�̐ݒ�
//	Device->SetTransform(D3DTS_VIEW, &cwk->mtxView);
//
//
//	// �v���W�F�N�V�����}�g���b�N�X�̏�����
//	D3DXMatrixIdentity(&mtxProjection);
//
//	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
//	D3DXMatrixPerspectiveFovLH(&mtxProjection,
//								VIEW_ANGLE,			// ����p
//								VIEW_ASPECT,		// �A�X�y�N�g��
//								VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
//								VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l
//
//	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
//	Device->SetTransform(D3DTS_PROJECTION, &mtxProjection);
//}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return cwk->mtxView;
}

////=============================================================================
//// �J�������[�N�̃A�h���X���擾
////=============================================================================
//
//CAMERA_WORK *get_camerawk(void)
//{
//	return(cwk);
//}
