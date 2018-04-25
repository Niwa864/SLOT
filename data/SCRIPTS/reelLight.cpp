//====================================================
//
// リール点灯処理[reelLight.cpp]
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "reelLight.h"

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define REELLIGHT_TEXTURE	"data/TEXTURE/reelLight_160x80.png"

#define REELLIGHT_SIZE_X	(160 * 0.8)	// ポリゴンサイズ
#define REELLIGHT_SIZE_Y	(80 * 0.8)

#define REELLIGHT_DEFAULT_X	(160 * 0.8)
#define REELLIGHT_DEFAULT_Y	(560 * 0.8)

#define REELLIGHT_MAX		(5) //4以上で死ぬ

//****************************************************
// 構造体定義
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffRLPolygon; // 頂点バッファインターフェースへのポインタ
	float fPosX, fPosY;
	float fPosXLeft, fPosXRight; // U座標
	float fPosYUp,   fPosYDown;	 // V座標
	bool bExist;
}light;

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexReelLight(LPDIRECT3DDEVICE9 pDevice);

void SetTextureRLPolygon();

//****************************************************
// グローバル変数
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureReelLight = NULL; // テクスチャへのポインタ

light g_light[REELLIGHT_MAX];

//====================================================
// 初期化処理
//====================================================
void InitReelLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		g_light[i].fPosYUp = 0.0f;
		g_light[i].fPosYDown = g_light[i].fPosYUp + REELLIGHT_SIZE_Y / REELLIGHT_DEFAULT_Y;
		g_light[i].fPosXLeft = 0.0f;
		g_light[i].fPosXRight = g_light[i].fPosXLeft + REELLIGHT_SIZE_X / REELLIGHT_DEFAULT_X;
		g_light[i].fPosX = 0.0f;
		g_light[i].fPosY = 0.0f;
		g_light[i].bExist = false;
	}

	MakeVertexReelLight(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
							  REELLIGHT_TEXTURE,		// ファイル名
							  &g_pTextureReelLight);	// 読み込み
}

//====================================================
// 終了処理
//====================================================
void UninitReelLight(void)
{
	if(g_pTextureReelLight != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTextureReelLight->Release();
		g_pTextureReelLight = NULL;
	}
	
	for(int i = 0; i < REELLIGHT_MAX; i ++)
	{
		if(g_light[i].pVtxBuffRLPolygon != NULL)
		{
			//----- 頂点バッファの解放 -----
			g_light[i].pVtxBuffRLPolygon->Release();
			g_light[i].pVtxBuffRLPolygon= NULL;
		}
	}
}

//====================================================
// 更新処理
//====================================================
void UpdateReelLight(void)
{
	//
	SetTextureRLPolygon();
}

//====================================================
// 描画処理
//====================================================
void DrawReelLight(void)
{
	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		if(!g_light[i].bExist)
		{
			continue;
		}

		g_light[0].fPosX = 43;

		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_light[i].pVtxBuffRLPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		pDevice->SetTexture(0, g_pTextureReelLight);		// テクスチャ設定
	
		//----- ポリゴン描画 -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//====================================================
// 頂点の作成
//====================================================
HRESULT MakeVertexReelLight(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
											  D3DUSAGE_WRITEONLY,			  // バッファ使用法
											  FVF_VERTEX_2D,				  // 頂点フォーマット
											  D3DPOOL_MANAGED,				  // 保持するメモリクラス
											  &g_light[i].pVtxBuffRLPolygon,  // 頂点バッファインターフェース
											  NULL)))
		{
			return E_FAIL;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
			g_light[i].pVtxBuffRLPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_light[i].fPosX, g_light[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_light[i].fPosX + REELLIGHT_SIZE_X, g_light[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_light[i].fPosX, g_light[i].fPosY + REELLIGHT_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_light[i].fPosX + REELLIGHT_SIZE_X, g_light[i].fPosY + REELLIGHT_SIZE_Y, 0.0f);

			//----- rhw設定 -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- 頂点カラー設定 -----
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//----- テクスチャ座標の設定 -----
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//----- 頂点データのアンロック -----
			g_light[i].pVtxBuffRLPolygon->Unlock();

		}
	}
	return S_OK;
}

//====================================================
// テクスチャ座標の設定
//====================================================
void SetTextureRLPolygon(void)
{
	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			g_light[i].pVtxBuffRLPolygon->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_light[i].fPosX, g_light[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_light[i].fPosX + REELLIGHT_SIZE_X, g_light[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_light[i].fPosX, g_light[i].fPosY+ REELLIGHT_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_light[i].fPosX + REELLIGHT_SIZE_X, g_light[i].fPosY + REELLIGHT_SIZE_Y, 0.0f);

			//----- rhw設定 -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- 頂点カラー設定 -----
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//----- テクスチャ座標の設定 -----
			pVtx[0].tex = D3DXVECTOR2(g_light[i].fPosXLeft,  g_light[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_light[i].fPosXRight, g_light[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_light[i].fPosXLeft,  g_light[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_light[i].fPosXRight, g_light[i].fPosYDown);

			//----- 頂点データのアンロック -----
			g_light[i].pVtxBuffRLPolygon->Unlock();

		}
	}
}


//====================================================
// リール点灯セット
//====================================================
void SetReelLight(int nNo, float fPosX, float fPosY)
{
	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		if(g_light[i].bExist)
		{
			continue;
		}
		g_light[i].fPosYUp = (REELLIGHT_SIZE_Y * nNo) / REELLIGHT_DEFAULT_Y;
		g_light[i].fPosYDown = (REELLIGHT_SIZE_Y / REELLIGHT_DEFAULT_Y) + g_light[i].fPosYUp;
		g_light[i].fPosX = fPosX;
		g_light[i].fPosY = fPosY;
		g_light[i].bExist = true;
		break;
	}
}

//====================================================
// リール消灯セット
//====================================================
void SetReelLightOut(void)
{
	for(int i = 0; i < REELLIGHT_MAX; i ++)
	{
		g_light[i].bExist = false;
	}
}