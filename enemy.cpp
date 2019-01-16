//=============================================================================
//
// モデル処理 [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	PLAYER_CAR		"data/PLAYER/Panjandrum.x"							// 読み込むモデル名
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
//#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
//#define	VIEW_FAR_Z		(1000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE		(5.0f)											// 移動量
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)								// 回転量

//#define	NUM_LIGHT		(3)												// ライトの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureEnemy;		// テクスチャへのポインタ
LPD3DXMESH			g_pD3DXMeshEnemy;		// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatEnemy;	// マテリアル情報へのポインタ
DWORD				g_nNumMatEnemy;			// マテリアル情報の数

ENEMY				enemy[1];

//D3DXVECTOR3			g_posEnemy;				// モデルの位置
//D3DXVECTOR3			g_rotEnemy;				// モデルの向き(回転)
//D3DXVECTOR3			g_sclEnemy;				// モデルの大きさ(スケール)

D3DXMATRIX			enemy_mtxWorld;				// ワールドマトリックス


int					enemy_nIdxShadow;				// 影ID
float				enemy_fSizeShadow;				// 影のサイズ
D3DXCOLOR			enemy_colShadow;				// 影の色

//D3DLIGHT9			g_aLight[NUM_LIGHT];	// ライト情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	ENEMY *enemy = GetEnemy(0);
	//// カメラの初期化
	//InitCamera();

	//// ライトの初期化
	//InitLight();

	// 位置・回転・スケールの初期設定
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

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(PLAYER_CAR,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&g_pD3DXBuffMatEnemy,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMatEnemy,		// D3DXMATERIAL構造体の数
		&g_pD3DXMeshEnemy)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,		// ファイルの名前
		&g_pD3DTextureEnemy);	// 読み込むメモリー
#endif
		// 影の生成
	enemy_nIdxShadow = CreateShadow(enemy->g_posEnemy, 25.0f, 25.0f);
	enemy_fSizeShadow = 25.0f;
	enemy_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	if (g_pD3DTextureEnemy != NULL)
	{// テクスチャの開放
		g_pD3DTextureEnemy->Release();
		g_pD3DTextureEnemy = NULL;
	}

	if (g_pD3DXMeshEnemy != NULL)
	{// メッシュの開放
		g_pD3DXMeshEnemy->Release();
		g_pD3DXMeshEnemy = NULL;
	}

	if (g_pD3DXBuffMatEnemy != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatEnemy->Release();
		g_pD3DXBuffMatEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 移動
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
//	// 回転
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

	//// 影の位置設定
	//SetPositionShadow(enemy_nIdxShadow, D3DXVECTOR3(enemy->g_posEnemy.x, 0.1f, enemy->g_posEnemy.z));

	//SetVertexShadow(enemy_nIdxShadow, enemy_fSizeShadow, enemy_fSizeShadow);
	//SetColorShadow(enemy_nIdxShadow, enemy_colShadow);

//	//PrintDebugProc("[飛行機の位置  ：(%f : %f : %f)]\n", enemy->g_posEnemy.x, enemy->g_posEnemy.y, enemy->g_posEnemy.z);
//	//PrintDebugProc("[飛行機の向き  ：(%f) < 目的の向き:(%f) >]\n", enemy->g_rotModel.y, enemy->g_rotDestModel.y);
//	//PrintDebugProc("\n");
//
//	//PrintDebugProc("*** 飛行機操作 ***\n");
//	//PrintDebugProc("前移動 : ↑\n");
//	//PrintDebugProc("後移動 : ↓\n");
//	//PrintDebugProc("左移動 : ←\n");
//	//PrintDebugProc("右移動 : →\n");
//	//PrintDebugProc("上昇   : Ｉ\n");
//	//PrintDebugProc("下降   : Ｋ\n");
//	//PrintDebugProc("左旋回 : LSHIFT\n");
//	//PrintDebugProc("右旋回 : RSHIFT\n");
//
//}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&enemy_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, enemy->g_sclEnemy.x, enemy->g_sclEnemy.y, enemy->g_sclEnemy.z);
	D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy->g_rotEnemy.y, enemy->g_rotEnemy.x, enemy->g_rotEnemy.z);
	D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, enemy->g_posEnemy.x, enemy->g_posEnemy.y, enemy->g_posEnemy.z);
	D3DXMatrixMultiply(&enemy_mtxWorld, &enemy_mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &enemy_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatEnemy->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTextureEnemy);

		// 描画
		g_pD3DXMeshEnemy->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
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
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionModel(void)
{
	return enemy->g_posEnemy;
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationModel(void)
{
	return enemy->g_rotEnemy;
}

//=============================================================================
// 目的の向き取得
//=============================================================================
D3DXVECTOR3 GetRotationDestModel(void)
{
	return enemy->g_rotDestEnemy;
}


