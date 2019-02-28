//=============================================================================
//
// モデル処理 [player.cpp]
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
// マクロ定義
//*****************************************************************************
#define	PLAYER_PANJAN_CORE		"data/PLAYER/panjan_core.x"		// 読み込むモデル名
#define PLAYER_PANJAN_SHAFT_R	"data/PLAYER/panjan_shaft_Right.x"
#define PLAYER_PANJAN_SHAFT_L	"data/PLAYER/panjan_shaft_Left.x"
#define PLAYER_PANJAN_WHEEL_R	"data/PLAYER/panjan_wheel_Right.x"
#define PLAYER_PANJAN_WHEEL_L	"data/PLAYER/panjan_wheel_Left.x"
#define PLAYER_PANJAN_ENJIN_R	"data/PLAYER/panjan_enjin_Right.x"
#define PLAYER_PANJAN_ENJIN_L	"data/PLAYER/panjan_enjin_Left.x"







//#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
//#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
////
#define	VALUE_MOVE_PLAYER		(0.50f)					// 移動量
#define	RATE_MOVE_PLAYER		(0.20f)					// 移動慣性係数
#define	VALUE_ROTATE_PLAYER	(D3DX_PI * 0.05f)			// 回転速度
#define	RATE_ROTATE_PLAYER	(0.20f)						// 回転慣性係数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTexturePlayer;				// テクスチャへのポインタ
LPD3DXMESH			g_pD3DXMeshPlayer;					// メッシュ情報へのポインタ
LPD3DXBUFFER		g_pD3DXBuffMatPlayer;				// マテリアル情報へのポインタ
DWORD				g_nNumMatPlayer;					// マテリアル情報の数

//LPD3DXMESH		D3DXMeshPlayer_core;				// ID3DXMeshインターフェイスへのポインタ
//LPD3DXBUFFER		D3DXBuffMatPlayer_core;				// メッシュのマテリアル情報を格納
//DWORD				NumMatPlayer_core;					// 属性情報の総数

LPD3DXMESH			D3DXMeshPlayer_shaft_r;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXBuffMatPlayer_shaft_r;			// メッシュのマテリアル情報を格納
DWORD				NumMatPlayer_shaft_r;			// 属性情報の総数

LPD3DXMESH			D3DXMeshPlayer_shaft_l;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXBuffMatPlayer_shaft_l;			// メッシュのマテリアル情報を格納
DWORD				NumMatPlayer_shaft_l;				// 属性情報の総数

LPD3DXMESH			D3DXMeshPlayer_wheel_r;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXBuffMatPlayer_wheel_r;			// メッシュのマテリアル情報を格納
DWORD				NumMatPlayer_wheel_r;					// 属性情報の総数

LPD3DXMESH			D3DXMeshPlayer_wheel_l;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXBuffMatPlayer_wheel_l;			// メッシュのマテリアル情報を格納
DWORD				NumMatPlayer_wheel_l;					// 属性情報の総数

LPD3DXMESH			D3DXMeshPlayer_enjin_r;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXBuffMatPlayer_enjin_r;			// メッシュのマテリアル情報を格納
DWORD				NumMatPlayer_enjin_r;					// 属性情報の総数

LPD3DXMESH			D3DXMeshPlayer_enjin_l;				// ID3DXMeshインターフェイスへのポインタ
LPD3DXBUFFER		D3DXBuffMatPlayer_enjin_l;			// メッシュのマテリアル情報を格納
DWORD				NumMatPlayer_enjin_l;					// 属性情報の総数

//D3DXVECTOR3			osPlayer;				// 現在の位置
//D3DXVECTOR3			movePlayer;				// 移動量
//D3DXVECTOR3			rotPlayer;				// 現在の向き
//D3DXVECTOR3			rotDestPlayer;			// 目的の向き





PLAYER				player[PLAYER_MAX];

//D3DXVECTOR3			g_posPlayer;				// モデルの位置
//D3DXVECTOR3			g_rotPlayer;				// モデルの向き(回転)
//D3DXVECTOR3			g_sclPlayer;				// モデルの大きさ(スケール)

D3DXMATRIX			mtxWorld;				// ワールドマトリックス
D3DXMATRIX			mtxWorld_core;		// ワールドマトリックス(core)


int					IdxShadow;				// 影ID
float				SizeShadow;				// 影のサイズ
D3DXCOLOR			colShadow;				// 影の色



//D3DLIGHT9			g_aLight[NUM_LIGHT];	// ライト情報

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER *player = GetPlayer(0);

	// 位置・回転・スケールの初期設定
	player->posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->sclPlayer = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	//player->osPlayer = D3DXVECTOR3(0.0f, 20.0f, 0.0f);			// 現在の位置

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
	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_CORE,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&g_pD3DXBuffMatPlayer,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&g_nNumMatPlayer,		// D3DXMATERIAL構造体の数
		&g_pD3DXMeshPlayer)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_SHAFT_R,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatPlayer_shaft_r,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatPlayer_shaft_r,		// D3DXMATERIAL構造体の数
		&D3DXMeshPlayer_shaft_r)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_SHAFT_L,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatPlayer_shaft_l,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatPlayer_shaft_l,		// D3DXMATERIAL構造体の数
		&D3DXMeshPlayer_shaft_l)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_WHEEL_R,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatPlayer_wheel_r,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatPlayer_wheel_r,		// D3DXMATERIAL構造体の数
		&D3DXMeshPlayer_wheel_r)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_WHEEL_L,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatPlayer_wheel_l,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatPlayer_wheel_l ,		// D3DXMATERIAL構造体の数
		&D3DXMeshPlayer_wheel_l)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_ENJIN_R,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatPlayer_enjin_r,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatPlayer_enjin_r,		// D3DXMATERIAL構造体の数
		&D3DXMeshPlayer_enjin_r)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}

	if (FAILED(D3DXLoadMeshFromX(PLAYER_PANJAN_ENJIN_L,				// 読み込むモデルファイル名(Xファイル)
		D3DXMESH_SYSTEMMEM,		// メッシュの作成オプションを指定
		pDevice,				// IDirect3DDevice9インターフェイスへのポインタ
		NULL,					// 隣接性データを含むバッファへのポインタ
		&D3DXBuffMatPlayer_enjin_l,	// マテリアルデータを含むバッファへのポインタ
		NULL,					// エフェクトインスタンスの配列を含むバッファへのポインタ
		&NumMatPlayer_enjin_l,		// D3DXMATERIAL構造体の数
		&D3DXMeshPlayer_enjin_l)))	// ID3DXMeshインターフェイスへのポインタのアドレス
	{
		return E_FAIL;
	}


#if 0
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					// デバイスへのポインタ
		TEXTURE_FILENAME,		// ファイルの名前
		&g_pD3DTexturePlayer);	// 読み込むメモリー
#endif
		// 影の生成
	IdxShadow = CreateShadow(player->posPlayer, 25.0f, 25.0f);
	SizeShadow = 25.0f;
	colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	if(g_pD3DTexturePlayer != NULL)
	{// テクスチャの開放
		g_pD3DTexturePlayer->Release();
		g_pD3DTexturePlayer = NULL;
	}

	if(g_pD3DXMeshPlayer != NULL)
	{// メッシュの開放
		g_pD3DXMeshPlayer->Release();
		g_pD3DXMeshPlayer = NULL;
	}

	if(g_pD3DXBuffMatPlayer != NULL)
	{// マテリアルの開放
		g_pD3DXBuffMatPlayer->Release();
		g_pD3DXBuffMatPlayer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	CAMERA_WORK		*cwk = get_camerawk();
	D3DXVECTOR3		oldPos = player->posPlayer;		// 元の位置
	D3DXVECTOR3		move = D3DXVECTOR3(0, 0, 0);

	//D3DXVECTOR3 rotCamera;

	float DiffRotY;

	//rotCamera = GetRotCamera();

	// 移動
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

	//// 回転
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

	////拡大
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
	//	{// 左前移動
	//		player->posPlayer.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
	//		
	//		player->rotDestPlayer.y = rotCamera.y + D3DX_PI * 0.75f;
	//		//player->rotDestPlayer.z += D3DX_PI * 2.0f;

	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// 左後移動
	//		player->posPlayer.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

	//		player->rotDestPlayer.y = rotCamera.y + D3DX_PI * 0.25f;
	//	}
	//	else
	//	{// 左移動
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
	//	{// 右前移動
	//		player->posPlayer.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_PLAYER;

	//		player->rotDestPlayer.y = rotCamera.y - D3DX_PI * 0.75f;
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN))
	//	{// 右後移動
	//		player->posPlayer.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_PLAYER;

	//		player->rotDestPlayer.y = rotCamera.y - D3DX_PI * 0.25f;
	//	}
	//	else
	//	{// 右移動
	//		player->posPlayer.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;
	//		player->posPlayer.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_PLAYER;

	//		player->rotDestPlayer.y = rotCamera.y - D3DX_PI * 0.50f;

	//	}

	//}
//if (GetKeyboardPress(DIK_LEFT))
//{
//// 左移動
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
		{// 前移動
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
		{// 後移動
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
	//{// 左回転
	//	player->rotPlayer.z += VALUE_ROTATE_PLAYER;
	//	if (player->rotPlayer.z < -D3DX_PI)
	//	{
	//		player->rotPlayer.z += D3DX_PI * 2.0f;
	//	}
	//}
	//if (GetKeyboardPress(DIK_RSHIFT))
	//{// 右回転
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
	{// 右回転
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
	{// 上昇
		player->movePlayer.y += VALUE_MOVE_PLAYER * 0.5f;

			if (player->posPlayer.y < 100.0f)
		{
			SizeShadow += 0.2f;
			colShadow.a -= 0.004f;
		}
	}
	if (GetKeyboardPress(DIK_K))
	{// 下降
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

	// 目的の角度までの差分
	DiffRotY = player->rotDestPlayer.y - player->rotPlayer.y;
	if (DiffRotY > D3DX_PI)
	{
		DiffRotY -= D3DX_PI * 2.0f;
	}
	if (DiffRotY < -D3DX_PI)
	{
		DiffRotY += D3DX_PI * 2.0f;
	}

	// 目的の角度まで慣性をかける
	player->rotPlayer.y += DiffRotY * RATE_ROTATE_PLAYER;
	if (player->rotPlayer.y > D3DX_PI)
	{
		player->rotPlayer.y -= D3DX_PI * 2.0f;
	}
	if (player->rotPlayer.y < -D3DX_PI)
	{
		player->rotPlayer.y += D3DX_PI * 2.0f;
	}
	/// 位置移動
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
	// 移動量に慣性をかける
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
	//{	// ビルボードとの当たり判定
	//	if (hitCheckBill(oldPos, move) != 0)
	//	{

	//		player->hit = true;
	//		// 当たっているので元の位置に戻す
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


	// 影の位置設定
	SetPositionShadow(IdxShadow, D3DXVECTOR3(player->posPlayer.x, 0.1f, player->posPlayer.z));

	SetVertexShadow(IdxShadow, SizeShadow, SizeShadow);
	SetColorShadow(IdxShadow, colShadow);

	//PrintDebugProc("[飛行機の位置  ：(%f : %f : %f)]\n", player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
	//PrintDebugProc("[飛行機の向き  ：(%f) < 目的の向き:(%f) >]\n", player->rotPlayer.y, player->rotDestPlayer.y);
	//PrintDebugProc("[飛行機の向き  ：(%d)\n", player->hit );

	//PrintDebugProc("\n");

	//PrintDebugProc("*** 飛行機操作 ***\n");
	//PrintDebugProc("前移動 : ↑\n");
	//PrintDebugProc("後移動 : ↓\n");
	//PrintDebugProc("左移動 : ←\n");
	//PrintDebugProc("右移動 : →\n");
	//PrintDebugProc("上昇   : Ｉ\n");
	//PrintDebugProc("下降   : Ｋ\n");
	//PrintDebugProc("左旋回 : LSHIFT\n");
	//PrintDebugProc("右旋回 : RSHIFT\n");

}

//=============================================================================
// CORE(親)の描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);




	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatPlayer->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_nNumMatPlayer; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// 描画
		g_pD3DXMeshPlayer->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
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
//***	シャフト（右）の描画処理
//********************************************************************************************************
void DrawPlayer_shaft_r(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//// スケールを反映
	//D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	//// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//// 移動を反映
	//D3DXMatrixTranslation(&mtxTranslate, player->posPlayer.x, player->posPlayer.y, player->posPlayer.z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


		// スケールを反映
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	//親のマトリクスをかける
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_shaft_r->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_shaft_r; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// 描画
		D3DXMeshPlayer_shaft_r->DrawSubset(nCntMat);
	}
	//mtxWorldPlayer_body = mtxWorldPlayer;

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	シャフト（左）の描画処理
//********************************************************************************************************
void DrawPlayer_shaft_l(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);

	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	//親のマトリクスをかける
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_shaft_l->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_shaft_l; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// 描画
		D3DXMeshPlayer_shaft_l->DrawSubset(nCntMat);
	}
	//mtxWorldPlayer_body = mtxWorldPlayer;

	mtxWorld_core = mtxWorld;
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	車輪（右）の描画処理
//********************************************************************************************************
void DrawPlayer_wheel_r(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);

	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0,0.0,0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());



	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_wheel_r->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_wheel_r; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// 描画
		D3DXMeshPlayer_wheel_r->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	車輪（左）の描画処理
//********************************************************************************************************
void DrawPlayer_wheel_l(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);


	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());



	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_wheel_l->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_wheel_l; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// 描画
		D3DXMeshPlayer_wheel_l->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	エンジン（右）の描画処理
//********************************************************************************************************
void DrawPlayer_enjin_r(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_enjin_r->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_enjin_r; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// 描画
		D3DXMeshPlayer_enjin_r->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//********************************************************************************************************
//***	エンジン（左）の描画処理
//********************************************************************************************************
void DrawPlayer_enjin_l(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;


	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, player->sclPlayer.x, player->sclPlayer.y, player->sclPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, player->rotPlayer.y, player->rotPlayer.x, player->rotPlayer.z);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0, 0.0, 0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, 0.0,0.0,0.0);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);


	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, GetPlayerMatrix());


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)D3DXBuffMatPlayer_enjin_l->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)NumMatPlayer_enjin_l; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pD3DTexturePlayer);

		// 描画
		D3DXMeshPlayer_enjin_l->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
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
// 位置取得
//=============================================================================
D3DXVECTOR3 GetPositionPlayer(void)
{
	return (player->posPlayer);
}

//=============================================================================
// 向き取得
//=============================================================================
D3DXVECTOR3 GetRotationPlayer(void)
{
	return player->rotPlayer;
}

//=============================================================================
// 目的の向き取得
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
//// クォータニオン→回転行列変換
////
//// m11-m33 : 回転行列成分（出力）
//// qx, qy, qz, qw : クォータニオン成分
////
//// ※注意：
//// 行列成分はDirectX形式（行方向が軸の向き）です
//// OpenGL形式（列方向が軸の向き）の場合は
//// 転置した値を格納するようにして下さい。
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
//// クォータニオン→回転行列変換
////
//// m1-m3 : 回転行列成分（出力）
//// q : クォータニオン成分(x,y,z,w)
////
//// ※注意：
//// 行列成分はDirectX形式（行方向が軸の向き）です
//// OpenGL形式（列方向が軸の向き）の場合は
//// 転置した値を格納するようにして下さい。
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
//// クォータニオン→回転行列変換
////
//// m : 回転行列成分（出力）
//// q : クォータニオン成分(x, y, z, w)
////
//// ※注意：
//// 行列成分はDirectX形式（行方向が軸の向き）です
//// OpenGL形式（列方向が軸の向き）の場合は
//// 転置した値を格納するようにして下さい。
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
//// 回転行列→クォータニオン変換
////
//// qx, qy, qz, qw : クォータニオン成分（出力）
//// m11-m33 : 回転行列成分
////
//// ※注意：
//// 行列成分はDirectX形式（行方向が軸の向き）です
//// OpenGL形式（列方向が軸の向き）の場合は
//// 転置した値を入れて下さい。
//
//bool transformRotMatToQuaternion(
//	float &qx, float &qy, float &qz, float &qw,
//	float m11, float m12, float m13,
//	float m21, float m22, float m23,
//	float m31, float m32, float m33
//) {
//	// 最大成分を検索
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
//		return false; // 引数の行列に間違いあり！
//
//	// 最大要素の値を算出
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
//// 回転行列→クォータニオン変換
////
//// qx, qy, qz, qw : クォータニオン成分（出力）
//// m1[3] : 回転行列成分 m11 - m13
//// m2[3] : 回転行列成分 m21 - m23
//// m3[3] : 回転行列成分 m31 - m33
////
//// ※注意：
//// 行列成分はDirectX形式（行方向が軸の向き）です
//// OpenGL形式（列方向が軸の向き）の場合は
//// 転置した値を入れて下さい。
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
//// クォータニオン球面線形補間
////
//
//bool slerpQuaternion(
//	float out[4],
//	const float q1[4],
//	const float q2[4],
//	float t
//) {
//	// 角度算出
//	const float len1 = sqrt(q1[0] * q1[0] + q1[1] * q1[1] + q1[2] * q1[2] + q1[3] * q1[3]);
//	const float len2 = sqrt(q2[0] * q2[0] + q2[1] * q2[1] + q2[2] * q2[2] + q2[3] * q2[3]);
//
//	if (len1 == 0.0f || len2 == 0.0f)
//		return false; // 不正なクォータニオン
//
//	const float cos_val = (q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3]) / (len1 * len2);
//	const float w = acosf(cos_val);
//
//	// 球面線形補間
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
//// 回転行列による補間(DirectX専用)
////
//// out : 補間回転行列（出力）
//// rm1 : 始点回転行列
//// rm2 : 終点回転行列
//// t : 補間値（0.0f～1.0f）
//
//D3DXMATRIX *slerpRotMatDX(
//	D3DXMATRIX *out,
//	const D3DXMATRIX *rm1,
//	const D3DXMATRIX *rm2,
//	float t
//) {
//	// 始点・終点のクォータニオンを算出
//	D3DXQUATERNION Q, Q1, Q2;
//	D3DXQuaternionRotationMatrix(&Q1, rm1);
//	D3DXQuaternionRotationMatrix(&Q2, rm2);
//
//	// クォータニオン間の球面線形補間
//	D3DXQuaternionSlerp(&Q, &Q1, &Q2, t);
//
//	// 回転行列に戻す
//	D3DXMatrixRotationQuaternion(out, &Q);
//
//	return out;
//}
//
//////////////////////////////////////////////
//// 回転行列による補間(汎用)
////
//// out : 補間回転行列（出力）
//// rm1 : 始点回転行列
//// rm2 : 終点回転行列
//// t : 補間値（0.0f～1.0f）
//
//void slerpRotMat(
//	float out[16],
//	const float rm1[16],
//	const float rm2[16],
//	float t
//) {
//	memset(out, 0, sizeof(float) * 16);
//
//	// 始点・終点のクォータニオンを算出
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
//	// クォータニオン間の球面線形補間
//	slerpQuaternion(Q, Q1, Q2, t);
//
//	// 回転行列に戻す
//	transformQuaternionToRotMat(out, Q);
//}
