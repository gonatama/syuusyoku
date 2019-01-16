//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : 
//
//=============================================================================
#include "meshfield.h"
#include "input.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	LOOP_MAX		(500)		// �t���N�^���̉��Z��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 D3DTextureField;			// �e�N�X�`���ǂݍ��ݏꏊ
LPDIRECT3DVERTEXBUFFER9 D3DVtxBuffField;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 D3DIdxBuffField;		// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX mtxWorldField;						// ���[���h�}�g���b�N�X
D3DXVECTOR3 posField;							// �|���S���\���ʒu�̒��S���W
D3DXVECTOR3 rotField;							// �|���S���̉�]�p

int NumBlockXField, NumBlockZField;		// �u���b�N��
int NumVertexField;							// �����_��	
int NumVertexIndexField;						// ���C���f�b�N�X��
int NumPolygonField;							// ���|���S����
float BlockSizeXField, BlockSizeZField;	// �u���b�N�T�C�Y
float Xrand, Zrand, Rrand;
float valField;
float xMax, zMax, rMax;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	xMax = (fBlockSizeX * nNumBlockX) / 2;
	zMax = (fBlockSizeX * nNumBlockX) / 2;
	rMax = 100;

	valField = (fBlockSizeX * nNumBlockX);

	// �|���S���\���ʒu�̒��S���W��ݒ�
	posField = pos;

	rotField = rot;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FIELD,		// �t�@�C���̖��O
								&D3DTextureField);	// �ǂݍ��ރ������[

	// �u���b�N���̐ݒ�
	NumBlockXField = nNumBlockX;
	NumBlockZField = nNumBlockZ;

	// ���_���̐ݒ�
	NumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	NumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	NumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	BlockSizeXField = fBlockSizeX;
	BlockSizeZField = fBlockSizeZ;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NumVertexField,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DVtxBuffField,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * NumVertexIndexField,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&D3DIdxBuffField,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / NumBlockX;
		const float texSizeZ = 1.0f / NumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffField->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int CntVtxZ = 0; CntVtxZ < (NumBlockZField + 1); CntVtxZ++)
		{
			for(int CntVtxX = 0; CntVtxX < (NumBlockXField + 1); CntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.x = -(NumBlockXField / 2.0f) * BlockSizeXField + CntVtxX * BlockSizeXField;
				pVtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.y = 0.0f;// rand() % ;
				pVtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].vtx.z = (NumBlockZField / 2.0f) * BlockSizeZField - CntVtxZ * BlockSizeZField;

				// �@���̐ݒ�
				pVtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// ���ˌ��̐ݒ�
				pVtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].tex.x = texSizeX * CntVtxX;
				pVtx[CntVtxZ * (NumBlockXField + 1) + CntVtxX].tex.y = texSizeZ * CntVtxZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		D3DVtxBuffField->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *Idx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DIdxBuffField->Lock( 0, 0, (void**)&Idx, 0 );

		int CntIdx = 0;
		for(int CntVtxZ = 0; CntVtxZ < NumBlockZField; CntVtxZ++)
		{
			if(CntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				Idx[CntIdx] = (CntVtxZ + 1) * (NumBlockXField + 1);
				CntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (NumBlockXField + 1); nCntVtxX++)
			{
				Idx[CntIdx] = (CntVtxZ + 1) * (NumBlockXField + 1) + nCntVtxX;
				CntIdx++;
				Idx[CntIdx] = CntVtxZ * (NumBlockXField + 1) + nCntVtxX;
				CntIdx++;
			}

			if(CntVtxZ < (NumBlockZField - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				Idx[CntIdx] = CntVtxZ * (NumBlockXField + 1) + NumBlockXField;
				CntIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		D3DIdxBuffField->Unlock();
	}

	for (int i = 0; i < LOOP_MAX; i++)
	{


		//if (GetKeyboardPress(DIK_B))
		//{

		Xrand = (float)(rand() % (int)(valField)) - xMax;
		Zrand = (float)(rand() % (int)(valField)) - zMax;
		Rrand = (float)(rand() % 200 * (D3DX_PI)) / 100.0f;

		D3DXVECTOR3 vecRand, vecMesh, cross;
		D3DXVECTOR3 tmp;
		VERTEX_3D	*Vtx;
		D3DVtxBuffField->Lock(0, 0, (void**)&Vtx, 0);


		vecRand.x = cosf(Rrand);
		vecRand.z = sinf(Rrand);
		vecRand.y = 0.0f;

		for (int z = 0; z < (NumBlockZField + 1); z++)
		{
			for (int x = 0; x < (NumBlockXField + 1); x++)
			{
				tmp.x = Vtx[z*(NumBlockXField + 1) + x].vtx.x;
				tmp.z = Vtx[z*(NumBlockXField + 1) + x].vtx.z;

				vecMesh.x = tmp.x - Xrand;
				vecMesh.z = tmp.z - Zrand;
				vecMesh.y = 0.0f;

				D3DXVec3Cross(&cross, &vecRand, &vecMesh);

				if (cross.y > 0.0f)
				{
					Vtx[z*(NumBlockXField + 1) + x].vtx.y += 5.0f;
				}
				else
				{
					Vtx[z*(NumBlockXField + 1) + x].vtx.y -= 0.0f;

				}

			}

		}

		D3DIdxBuffField->Unlock();

		//}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	if(D3DVtxBuffField)
	{// ���_�o�b�t�@�̊J��
		D3DVtxBuffField->Release();
		D3DVtxBuffField = NULL;
	}

	if(D3DIdxBuffField)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		D3DIdxBuffField->Release();
		D3DIdxBuffField = NULL;
	}

	if(D3DTextureField)
	{// �e�N�X�`���̊J��
		D3DTextureField->Release();
		D3DTextureField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorldField);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotField.y, rotField.x, rotField.z);
	D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, posField.x, posField.y, posField.z);
	D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	Device->SetTransform(D3DTS_WORLD, &mtxWorldField);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	Device->SetStreamSource(0, D3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	Device->SetIndices(D3DIdxBuffField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	Device->SetTexture(0, D3DTextureField);

	// �|���S���̕`��
	Device->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, NumVertexField, 0, NumPolygonField);
}
//
////=============================================================================
//// ����
////=============================================================================
//float dotProduct(D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
//{
//
//	float ans = (vl->x*vr->x) + (vl->y*vr->y) + (vl->z*vr->z);
//
//
//	return(ans);
//}
//
//
////=============================================================================
//// �O��
////=============================================================================
//void crossProduct(D3DXVECTOR3 *ret, D3DXVECTOR3 *vl, D3DXVECTOR3 *vr)
//{
//	//#if 0
//
//	ret->x = (vl->y * vr->z) - (vl->z * vr->y);
//	ret->y = (vl->z * vr->x) - (vl->x * vr->z);
//	ret->z = (vl->x * vr->y) - (vl->y * vr->x);
//	//#else
//		//�֐�
//		//D3DXVECTOR3 ans = D3DXVec3Cross(ret, vl, vr);
//
//
//	//#endif
//
//}
//

////=============================================================================
//// �|���S���Ɛ����Ƃ̓����蔻��
//// p0  :�|���S���̒��_1
//// p1  :�|���S���̒��_2
//// p2  :�|���S���̒��_3
//// pos0:�n�_�i�ړ��O�j
//// pos1:�I�_�i�ړ���j
////=============================================================================
//int hitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	D3DXVECTOR3		nor;		// �|���S���̖@��
//
//	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�B�����Đ��K�����Ă����B
//		D3DXVECTOR3		vec01 = p1 - p0;
//		D3DXVECTOR3		vec02 = p2 - p0;
//		crossProduct(&nor, &vec01, &vec02);
//		D3DXVec3Normalize(&nor, &nor);
//
//
//	}
//
//	{	// ����(�O�p�`�|���S��)�Ɛ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
//		// ���߂��@���ƃx�N�g���Q�i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
//		D3DXVECTOR3		vec1 = pos0 - p0;
//		D3DXVECTOR3		vec2 = pos1 - p0;
//		float a = dotProduct(&vec1, &nor);
//		float b = dotProduct(&vec2, &nor);
//		if ((a * b) > 0)
//		{
//			// �������Ă���\���͖���
//			return(0);
//		}
//	}
//
//
//	{	// �|���S���Ɛ����̌�_�����߂�
//		//D3DXVECTOR3		vec1 = pos0 - p0;
//		//D3DXVECTOR3		vec2 = pos0 - p0;
//		//float			d1;					// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
//		//float			d2;					// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
//		//float			a;					// ������
//		//D3DXVECTOR3		vec3;
//		//D3DXVECTOR3		p3;					// ��_
//
//
//		D3DXVECTOR3		vec1 = pos0 - p0;
//		D3DXVECTOR3		vec2 = pos1 - p0;
//		float			d1 = fabs(dotProduct(&nor, &vec1));
//		float			d2 = fabs(dotProduct(&nor, &vec2));
//		float			a = d1 / (d1 + d2);
//		D3DXVECTOR3		vec3 = (1 - a)*vec1 + a * vec2;
//		D3DXVECTOR3		p3 = p0 + vec3;
//
//
//
//
//		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�
//
//			// �|���S���̊e�ӂ̃x�N�g��
//			D3DXVECTOR3		v1 = p1 - p0;
//			D3DXVECTOR3		v2 = p2 - p1;
//			D3DXVECTOR3		v3 = p3 = p0;
//
//			// �e���_�ƌ�_�Ƃ̃x�N�g��
//			D3DXVECTOR3		v4 = p3 - p1;
//			D3DXVECTOR3		v5 = p3 - p2;
//			D3DXVECTOR3		v6 = p3 - p0;
//
//			// �e�ӂƂ̊O�ςŖ@�������߂�
//			D3DXVECTOR3		n1, n2, n3;
//			crossProduct(&n1, &v1, &v4);
//			crossProduct(&n2, &v2, &v5);
//			crossProduct(&n3, &v3, &v6);
//
//
//
//			// ���ꂼ��̃x�N�g���̌�������ςŃ`�F�b�N
//			if (dotProduct(&n1, &nor) < 0) return(0);
//			if (dotProduct(&n2, &nor) < 0) return(0);
//			if (dotProduct(&n3, &nor) < 0) return(0);
//
//
//
//		}
//	}
//
//	return(1);
//}
//
//
////=============================================================================
//// �r���{�[�h�Ƃ̓����蔻��
//// pos0:�n�_�i�ړ��O�j
//// pos1:�I�_�i�ړ���j
////=============================================================================
//int hitCheckBill(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
//{
//	PLAYER *player = GetPlayer(0);
//	int				ans = 0;	// ��悸�������Ă��Ȃ����Z�b�g���Ă���
//
//	D3DXVECTOR3		pos[4];
//	D3DXVECTOR3		nor;		// �|���S���̖@��
//
//								// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
//	VERTEX_3D		*pVtx;
//	D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int i = 0; i < 1; i++, pVtx += 4)
//	{	// �܂��́A�|���S���̒��_�����߂�
//
//		// ���_���W�̐ݒ�
//		pos[0] = pVtx[0].vtx;
//		pos[1] = pVtx[1].vtx;
//		pos[2] = pVtx[2].vtx;
//		pos[3] = pVtx[3].vtx;
//
//		// �������|���S���Ɛ����̓����蔻��
//		ans = hitCheck(pos[0], pos[2], pos[3], pos0, pos1);
//		if (ans != 0) break;
//
//		// �E�㑤�|���S���Ɛ����̓����蔻��
//		ans = hitCheck(pos[0], pos[3], pos[1], pos0, pos1);
//		if (ans != 0) break;
//
//	}
//
//	// ���_�f�[�^���A�����b�N����
//	D3DVtxBuffField->Unlock();
//
//
//
//
//	return(ans);
//
//}


/*************************************************************************
*�O�_����
**************************************************************************/
FLOAT Hitfield(D3DXVECTOR3 pos)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

	D3DXVECTOR3 cross;
	D3DXVECTOR3 vec1, vec2;
	float pos_high, fx, fz;


	fx = (NumBlockXField / 2.0 +  (pos.x / BlockSizeXField));
	fz = (NumBlockZField / 2.0 -  (pos.z / BlockSizeZField));

	int x =(int)floorf(fx);
	int z =(int)floorf(fz);

	int vtx0, vtx1, vtx2, vtx3;

	vtx0 = z * (NumBlockXField +1)+x;
	vtx1 = vtx0 + 1;
	vtx2 = vtx1+ NumBlockXField;
	vtx3 = vtx2 + 1;



		//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		D3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);


		if (fx - x > fz - z)
		{
		
			vec1 = pVtx[vtx1].vtx - pVtx[vtx0].vtx;
			vec2 = pVtx[vtx3].vtx - pVtx[vtx0].vtx;

			D3DXVec3Cross(&cross, &vec1, &vec2);
		}
		else
		{
			vec1 = pVtx[vtx3].vtx - pVtx[vtx0].vtx;
			vec2 = pVtx[vtx2].vtx - pVtx[vtx0].vtx;

			D3DXVec3Cross(&cross, &vec1, &vec2);

		}



	pos_high = pVtx[vtx0].vtx.y - (cross.x*(pos.x - pVtx[vtx0].vtx.x) + cross.z*(pos.z - pVtx[vtx0].vtx.z)) / cross.y;

	// ���_�f�[�^���A�����b�N����
	D3DVtxBuffField->Unlock();
	

	return (pos_high);
}

	/**
	vtx[no].x





	*/