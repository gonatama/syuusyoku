#include "ligth.h"
#include "input.h"

//�}�N��
// ���C�g�̐�
D3DLIGHT9 Light[NUM_LIGHT];		// ���C�g�̃��[�N


//=============================================================================
// ���C�g�̏���������
//=============================================================================
HRESULT InitLight(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	D3DXVECTOR3 vecDir;

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[0], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	Light[0].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ����
	Light[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.20f, -0.60f, 0.80f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[0].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	Device->SetLight(0, &Light[0]);

	// ���C�g�̐ݒ�
	Device->LightEnable(0, TRUE);


	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[1], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	Light[1].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	Light[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// ����
	Light[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[1].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	Device->SetLight(1, &Light[1]);

	// ���C�g�̐ݒ�
	Device->LightEnable(1, TRUE);

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory(&Light[2], sizeof(D3DLIGHT9));

	// ���C�g�̃^�C�v�̐ݒ�
	Light[2].Type = D3DLIGHT_DIRECTIONAL;

	// �g�U��
	Light[2].Diffuse = D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);

	// ����
	Light[2].Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

	// ���C�g�̕����̐ݒ�
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&Light[2].Direction, &vecDir);

	// ���C�g�������_�����O�p�C�v���C���ɐݒ�
	Device->SetLight(2, &Light[2]);

	// ���C�g�̐ݒ�
	Device->LightEnable(2, TRUE);


	// ���C�e�B���O���[�h
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void UninitLigth(void)
{

}
//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void UpdateLigth(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

}
