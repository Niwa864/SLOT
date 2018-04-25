//====================================================
//
// 数字ポリゴン処理[valuePolygon.cpp]
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "valuePolygon.h"	//ポリゴン処理用のヘッダファイル
#include "big.h"

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define POLYGON_TEXTURE	"data/TEXTURE/value_190x26.png"

#define POLYGON_SIZE_X	(19)
#define POLYGON_SIZE_Y	(26)

#define TEX_DEFAULT_X	(190)
#define TEX_DEFAULT_Y	(26)

#define CREDIT_DIGIT_MAX (2)
#define COUNT_DIGIT_MAX	 (2)

#define TENS_POS_X	(82)
#define TENS_POS_Y	(503)

#define TENS_COUNT_POS_X	(214)
#define TENS_COUNT_POS_Y	(503)

//#define INTERVAL_CHANGE_PATTERN	(5)	//フレーム単位

//****************************************************
// 構造体定義
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffVPolygon; // 頂点バッファインターフェースへのポインタ
	float fPosX, fPosY;
	float fPosXLeft, fPosXRight; // U座標
	float fPosYUp,   fPosYDown;	 // V座標
}value;

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexVPolygon(LPDIRECT3DDEVICE9 pDevice);

void SetTextureVPolygon();

//****************************************************
// グローバル変数
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureVPolygon = NULL; // テクスチャへのポインタ

// VERTEX_2D		   g_Vertex[NUM_VERTEX]; // 頂点情報格納ワーク

//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVPolygon = NULL; // 頂点バッファインターフェースへのポインタ

value g_Credit[2];
value g_BigCount[2];

//====================================================
// 初期化処理
//====================================================
void InitValuePolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	//----- 初期化 -----
	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		g_Credit[i].fPosXLeft = 0.0f;
		g_Credit[i].fPosXRight = g_Credit[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X;
		g_Credit[i].fPosYUp = 0.0f;
		g_Credit[i].fPosYDown = g_Credit[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y;

		g_BigCount[i].fPosXLeft = 0.0f;
		g_BigCount[i].fPosXRight = g_BigCount[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X;
		g_BigCount[i].fPosYUp = 0.0f;
		g_BigCount[i].fPosYDown = g_BigCount[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y;
	}

	MakeVertexVPolygon(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
							  POLYGON_TEXTURE,		// ファイル名
							  &g_pTextureVPolygon);	// 読み込み
}

//====================================================
// 終了処理
//====================================================
void UninitValuePolygon(void)
{
	if(g_pTextureVPolygon != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTextureVPolygon->Release();
		g_pTextureVPolygon = NULL;
	}

	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{	
		if(g_Credit[i].pVtxBuffVPolygon != NULL)
		{
			//----- 頂点バッファの解放 -----
			g_Credit[i].pVtxBuffVPolygon->Release();
			g_Credit[i].pVtxBuffVPolygon = NULL;
		}

		if(g_BigCount[i].pVtxBuffVPolygon != NULL)
		{
			//----- 頂点バッファの解放 -----
			g_BigCount[i].pVtxBuffVPolygon->Release();
			g_BigCount[i].pVtxBuffVPolygon = NULL;
		}
	}
}

//====================================================
// 更新処理
//====================================================
void UpdateValuePolygon(int nMedal)
{
	int nTens, nOnes;
	int nBigTens, nBigOnes;

	//----- テクスチャ座標の算出 -----						
	nTens = nMedal / 10;
	nOnes = nMedal % 10;

	nBigTens = GetBigCount() / 10;
	nBigOnes = GetBigCount() % 10;

	g_Credit[1].fPosXLeft = (float)(nTens * POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_Credit[1].fPosXRight = (float)(nTens * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_Credit[1].fPosX = TENS_POS_X;	//十の位の座標
	g_Credit[1].fPosY = TENS_POS_Y;
	g_BigCount[1].fPosXLeft = (float)(nBigTens * POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_BigCount[1].fPosXRight = (float)(nBigTens * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_BigCount[1].fPosX = TENS_COUNT_POS_X;	//十の位の座標
	g_BigCount[1].fPosY = TENS_COUNT_POS_Y;

	g_Credit[0].fPosXLeft = (float)(nOnes * POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_Credit[0].fPosXRight = (float)(nOnes * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_Credit[0].fPosX = g_Credit[1].fPosX + POLYGON_SIZE_X;	//一の位の座標
	g_Credit[0].fPosY = TENS_POS_Y;
	g_BigCount[0].fPosXLeft = (float)(nBigOnes * POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_BigCount[0].fPosXRight = (float)(nBigOnes * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_BigCount[0].fPosX = g_BigCount[1].fPosX + POLYGON_SIZE_X;	//一の位の座標
	g_BigCount[0].fPosY = TENS_COUNT_POS_Y;

	SetTextureVPolygon();
}

//====================================================
// 描画処理
//====================================================
void DrawValuePolygon(void)
{
	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_Credit[i].pVtxBuffVPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		pDevice->SetTexture(0, g_pTextureVPolygon);		// テクスチャ設定

		//----- ポリゴン描画 -----
	//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,	// 三角形ストリップを指定
	//							 NUM_POLYGON,			// 2つの三角形で四角形を作る
	//							 &g_Vertex[0],			// 頂点情報の開始アドレス
	//							 sizeof(VERTEX_2D));	// 頂点情報一つ分のサイズ
	
		//----- ポリゴン描画 -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	for (int i = 0; i < COUNT_DIGIT_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_BigCount[i].pVtxBuffVPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		pDevice->SetTexture(0, g_pTextureVPolygon);		// テクスチャ設定

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

HRESULT MakeVertexVPolygon(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
												D3DUSAGE_WRITEONLY,			  // バッファ使用法
												FVF_VERTEX_2D,				  // 頂点フォーマット
												D3DPOOL_MANAGED,				  // 保持するメモリクラス
												&g_Credit[i].pVtxBuffVPolygon,			  // 頂点バッファインターフェース
												NULL)))
		{
			return E_FAIL;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
			g_Credit[i].pVtxBuffVPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_Credit[i].fPosX, g_Credit[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Credit[i].fPosX + POLYGON_SIZE_X, g_Credit[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Credit[i].fPosX, g_Credit[i].fPosY + POLYGON_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Credit[i].fPosX + POLYGON_SIZE_X, g_Credit[i].fPosY + POLYGON_SIZE_Y, 0.0f);

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
			g_Credit[i].pVtxBuffVPolygon->Unlock();
		}
	}

	for(int i = 0; i < COUNT_DIGIT_MAX; i ++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
												D3DUSAGE_WRITEONLY,			  // バッファ使用法
												FVF_VERTEX_2D,				  // 頂点フォーマット
												D3DPOOL_MANAGED,				  // 保持するメモリクラス
												&g_BigCount[i].pVtxBuffVPolygon,  // 頂点バッファインターフェース
												NULL)))
		{
			return E_FAIL;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
			g_BigCount[i].pVtxBuffVPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_BigCount[i].fPosX, g_BigCount[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_BigCount[i].fPosX + POLYGON_SIZE_X, g_BigCount[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_BigCount[i].fPosX, g_BigCount[i].fPosY + POLYGON_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_BigCount[i].fPosX + POLYGON_SIZE_X, g_BigCount[i].fPosY + POLYGON_SIZE_Y, 0.0f);

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
			g_BigCount[i].pVtxBuffVPolygon->Unlock();
		}
	}

	return S_OK;
}

//====================================================
// テクスチャ座標の設定
//====================================================
void SetTextureVPolygon(void)
{
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			g_Credit[i].pVtxBuffVPolygon->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_Credit[i].fPosX, g_Credit[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Credit[i].fPosX + POLYGON_SIZE_X, g_Credit[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Credit[i].fPosX, g_Credit[i].fPosY+ POLYGON_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Credit[i].fPosX + POLYGON_SIZE_X, g_Credit[i].fPosY + POLYGON_SIZE_Y, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_Credit[i].fPosXLeft, g_Credit[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_Credit[i].fPosXRight, g_Credit[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_Credit[i].fPosXLeft, g_Credit[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_Credit[i].fPosXRight, g_Credit[i].fPosYDown);

			//----- 頂点データのアンロック -----
			g_Credit[i].pVtxBuffVPolygon->Unlock();

		}
	}

	for(int i = 0; i < COUNT_DIGIT_MAX; i ++)
	{
		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			g_BigCount[i].pVtxBuffVPolygon->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_BigCount[i].fPosX, g_BigCount[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_BigCount[i].fPosX + POLYGON_SIZE_X, g_BigCount[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_BigCount[i].fPosX, g_BigCount[i].fPosY+ POLYGON_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_BigCount[i].fPosX + POLYGON_SIZE_X, g_BigCount[i].fPosY + POLYGON_SIZE_Y, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_BigCount[i].fPosXLeft,  g_BigCount[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_BigCount[i].fPosXRight, g_BigCount[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_BigCount[i].fPosXLeft,  g_BigCount[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_BigCount[i].fPosXRight, g_BigCount[i].fPosYDown);

			//----- 頂点データのアンロック -----
			g_BigCount[i].pVtxBuffVPolygon->Unlock();

		}
	}
}
