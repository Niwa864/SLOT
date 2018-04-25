//====================================================
//
// リザルト処理[result.cpp]
//
// ～更新履歴～
// 2016/08/08 制作開始
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "result.h"	//ポリゴン処理用のヘッダファイル
#include <stdio.h>

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define TEX_RESULT	"data/TEXTURE/result_610x480.png"
#define TEX_ARROW	"data/TEXTURE/allow_30x40.png"
#define TEX_RETRY	"data/TEXTURE/retry_300x260.png"
#define TEX_GOTITLE	"data/TEXTURE/title_300x260.png"

#define RESULT_SIZE_X	(610 * 0.8)	// ポリゴンサイズ
#define RESULT_SIZE_Y	(480 * 0.8)
#define ARROW_SIZE_X	(30 * 0.8)	// ポリゴンサイズ
#define ARROW_SIZE_Y	(40 * 0.8)
#define RETRY_SIZE_X		(300 * 0.8)	// ポリゴンサイズ
#define RETRY_SIZE_Y		(260 * 0.8)
#define GOTITLE_SIZE_X		(300 * 0.8)	// ポリゴンサイズ
#define GOTITLE_SIZE_Y		(260 * 0.8)

#define RETRY_DEFAULT_SIZE_X	(600 * 0.8)	// テクスチャのもともとの大きさ
#define RETRY_DEFAULT_SIZE_Y	(260 * 0.8)	
#define GOTITLE_DEFAULT_SIZE_X		(600 * 0.8)	// テクスチャのもともとの大きさ
#define GOTITLE_DEFAULT_SIZE_Y		(260 * 0.8)	

#define TEXTURE_MAX	(4)


//****************************************************
// 構造体定義
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffPolygon; // 頂点バッファインターフェースへのポインタ
	LPDIRECT3DTEXTURE9 pTextureFile;	// テクスチャへのポインタ
	float fPolyPosX, fPolyPosY;
	float fPolySizeX, fPolySizeY;
	float fTexDefaultX, fTexDefaultY;
	float fPosXLeft, fPosXRight;		// U座標(0.0f ~ 1.0f)
	float fPosYUp, fPosYDown;			// V座標(0.0f ~ 1.0f)
}tex;

//****************************************************
// 列挙型
//****************************************************

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);

void RorGSetTexture(void);

//****************************************************
// グローバル変数
//****************************************************
tex g_TexR[TEXTURE_MAX];

int g_nRorG;

//====================================================
// 初期化処理
//====================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	//----- テクスチャの初期化 -----
	g_TexR[0].fPosXLeft    = 0.0f;
	g_TexR[0].fPosXRight   = 1.0f;
	g_TexR[0].fPosYUp	  = 0.0f;
	g_TexR[0].fPosYDown    = 1.0f;
	g_TexR[0].fPolySizeX   = RESULT_SIZE_X;
	g_TexR[0].fPolySizeY   = RESULT_SIZE_Y;
	g_TexR[0].fTexDefaultX = RESULT_SIZE_X;
	g_TexR[0].fTexDefaultY = RESULT_SIZE_Y;
	g_TexR[0].fPolyPosX = 8;
	g_TexR[0].fPolyPosY = 5;

	g_TexR[1].fPosXLeft    = 0.0f;
	g_TexR[1].fPosXRight   = 1.0f;
	g_TexR[1].fPosYUp	  = 0.0f;
	g_TexR[1].fPosYDown    = 1.0f;
	g_TexR[1].fPolySizeX   = ARROW_SIZE_X;
	g_TexR[1].fPolySizeY   = ARROW_SIZE_Y;
	g_TexR[1].fTexDefaultX = ARROW_SIZE_X;
	g_TexR[1].fTexDefaultY = ARROW_SIZE_Y;
	g_TexR[1].fPolyPosX = 240;
	g_TexR[1].fPolyPosY = 200;

	g_TexR[2].fPosXLeft    = (float)RETRY_SIZE_Y / RETRY_DEFAULT_SIZE_Y;
	g_TexR[2].fPosXRight   = 1.0f;
	g_TexR[2].fPosYUp	  = 0.0f;
	g_TexR[2].fPosYDown    = (float)RETRY_SIZE_Y / RETRY_DEFAULT_SIZE_Y;
	g_TexR[2].fPolySizeX   = RETRY_SIZE_X;
	g_TexR[2].fPolySizeY   = RETRY_SIZE_Y;
	g_TexR[2].fTexDefaultX = RETRY_DEFAULT_SIZE_X;
	g_TexR[2].fTexDefaultY = RETRY_DEFAULT_SIZE_Y;
	g_TexR[2].fPolyPosX = 30;
	g_TexR[2].fPolyPosY = 400;

	g_TexR[3].fPosXLeft    = 0.0f;
	g_TexR[3].fPosXRight   = (float)GOTITLE_SIZE_X / GOTITLE_DEFAULT_SIZE_X;
	g_TexR[3].fPosYUp	  = 0.0f;
	g_TexR[3].fPosYDown    = (float)GOTITLE_SIZE_Y / GOTITLE_DEFAULT_SIZE_Y;
	g_TexR[3].fPolySizeX   = GOTITLE_SIZE_X;
	g_TexR[3].fPolySizeY   = GOTITLE_SIZE_Y;
	g_TexR[3].fTexDefaultX = GOTITLE_DEFAULT_SIZE_X;
	g_TexR[3].fTexDefaultY = GOTITLE_DEFAULT_SIZE_Y;
	g_TexR[3].fPolyPosX = 260;
	g_TexR[3].fPolyPosY = 400;

	MakeVertexResult(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEX_RESULT,					// ファイル名
								&g_TexR[0].pTextureFile);	// 読み込み

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEX_ARROW,					// ファイル名
								&g_TexR[1].pTextureFile);	// 読み込み

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEX_RETRY,					// ファイル名
								&g_TexR[2].pTextureFile);	// 読み込み
	
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEX_GOTITLE,					// ファイル名
								&g_TexR[3].pTextureFile);	// 読み込み
}

//====================================================
// 終了処理
//====================================================
void UninitResult(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		if(g_TexR[i].pTextureFile != NULL)
		{
			//----- テクスチャの解放 -----
			g_TexR[i].pTextureFile->Release();
			g_TexR[i].pTextureFile = NULL;

			//----- 頂点バッファの解放 -----
			g_TexR[i].pVtxBuffPolygon->Release();
			g_TexR[i].pVtxBuffPolygon = NULL;
		}
	}
}

//====================================================
// 更新処理
//====================================================
void UpdateResult(void)
{
	if(!g_nRorG)
	{
		g_TexR[2].fPosXLeft    = (float)RETRY_SIZE_X / RETRY_DEFAULT_SIZE_X;
		g_TexR[2].fPosXRight   = (float)(RETRY_SIZE_X + RETRY_SIZE_X) / RETRY_DEFAULT_SIZE_X;
		g_TexR[2].fPosYUp	  = 0.0f;
		g_TexR[2].fPosYDown    = (float)RETRY_SIZE_Y / RETRY_DEFAULT_SIZE_Y;
		g_TexR[3].fPosXLeft    = 0.0f;
		g_TexR[3].fPosXRight   = (float)RETRY_SIZE_X / RETRY_DEFAULT_SIZE_X;
		g_TexR[3].fPosYUp	  = 0.0f;
		g_TexR[3].fPosYDown    = (float)RETRY_SIZE_Y / RETRY_DEFAULT_SIZE_Y;
	}

	if(g_nRorG)
	{
		g_TexR[2].fPosXLeft    = 0.0f;
		g_TexR[2].fPosXRight   = (float)GOTITLE_SIZE_X / GOTITLE_DEFAULT_SIZE_X;
		g_TexR[2].fPosYUp	  = 0.0f;
		g_TexR[2].fPosYDown    = (float)GOTITLE_SIZE_Y / GOTITLE_DEFAULT_SIZE_Y;
		g_TexR[3].fPosXLeft    = (float)GOTITLE_SIZE_X / GOTITLE_DEFAULT_SIZE_X;
		g_TexR[3].fPosXRight   = (float)(GOTITLE_SIZE_X + GOTITLE_SIZE_X) / GOTITLE_DEFAULT_SIZE_X;
		g_TexR[3].fPosYUp	  = 0.0f;
		g_TexR[3].fPosYDown    = (float)GOTITLE_SIZE_Y / GOTITLE_DEFAULT_SIZE_Y;
	}
	RorGSetTexture();
}

//====================================================
// 描画処理
//====================================================
void DrawResult(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_TexR[i].pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		pDevice->SetTexture(0, g_TexR[i].pTextureFile);		// テクスチャ設定

		//----- ポリゴン描画 -----
		//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,	// 三角形ストリップを指定
		//							 NUM_POLYGON,			// 2つの三角形で四角形を作る
		//							 &g_Vertex[0],			// 頂点情報の開始アドレス
		//							 sizeof(VERTEX_2D));	// 頂点情報一つ分のサイズ
	
		//----- ポリゴン描画 -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//====================================================
// 頂点の作成
//====================================================

HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
											  D3DUSAGE_WRITEONLY,			  // バッファ使用法
											  FVF_VERTEX_2D,				  // 頂点フォーマット
											  D3DPOOL_MANAGED,				  // 保持するメモリクラス
											  &g_TexR[i].pVtxBuffPolygon,	  // 頂点バッファインターフェース
											  NULL)))
		{
			return E_FAIL;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
			g_TexR[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX, g_TexR[i].fPolyPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX + g_TexR[i].fPolySizeX, g_TexR[i].fPolyPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX, g_TexR[i].fPolyPosY + g_TexR[i].fPolySizeY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX + g_TexR[i].fPolySizeX, g_TexR[i].fPolyPosY + g_TexR[i].fPolySizeY, 0.0f);

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
			g_TexR[i].pVtxBuffPolygon->Unlock();

		}
	/*
		// (X, Y, Z)
		g_Vertex[0].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_Vertex[1].pos = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y, 0.0f);
		g_Vertex[2].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);
		g_Vertex[3].pos = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);

		//----- rhw設定 -----
		g_Vertex[0].rhw = 
		g_Vertex[1].rhw = 
		g_Vertex[2].rhw = 
		g_Vertex[3].rhw = 1.0f;

		//----- 頂点カラー設定 -----
		g_Vertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_Vertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_Vertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_Vertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//----- テクスチャ座標の設定 -----
		g_Vertex[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_Vertex[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_Vertex[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_Vertex[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	*/
	}
	return S_OK;
}

//====================================================
// テクスチャ座標の設定
//====================================================
void RorGSetTexture(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			g_TexR[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX, g_TexR[i].fPolyPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX + g_TexR[i].fPolySizeX, g_TexR[i].fPolyPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX, g_TexR[i].fPolyPosY + g_TexR[i].fPolySizeY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX + g_TexR[i].fPolySizeX, g_TexR[i].fPolyPosY + g_TexR[i].fPolySizeY, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_TexR[i].fPosXLeft , g_TexR[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_TexR[i].fPosXRight, g_TexR[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_TexR[i].fPosXLeft , g_TexR[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_TexR[i].fPosXRight, g_TexR[i].fPosYDown);

			//----- 頂点データのアンロック -----
			g_TexR[i].pVtxBuffPolygon->Unlock();

		}
	}
}

//====================================================
// 切り替え共有
//====================================================
int GetRorG (void)
{
	return g_nRorG;
}

//====================================================
// 切り替えセット
//====================================================
void SetRorG (int nType)
{
	g_nRorG = nType;
}