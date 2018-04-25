//====================================================
//
// 排出メダル処理[outMedal.cpp]
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "outMedal.h"	//ポリゴン処理用のヘッダファイル

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define POLYGON_TEXTURE	"data/TEXTURE/resultV_35x42.png"
#define BAR_TEXTURE		"data/TEXTURE/outMedal_168x35.png"

#define POLYGON_SIZE_X	(35 * 0.8 * 0.5)
#define POLYGON_SIZE_Y	(42 * 0.8 * 0.5)

#define BAR_SIZE_X	(168 * 0.8)
#define BAR_SIZE_Y	(35 * 0.8)

#define TEX_DEFAULT_X	(350 * 0.8 * 0.5)
#define TEX_DEFAULT_Y	(42 * 0.8 * 0.5)

#define DIGIT_MAX (7)

#define VALUE_POS_X	(400)
#define VALUE_POS_Y	(12)

#define BAR_POS_X	(370)
#define BAR_POS_Y	(5)

//#define INTERVAL_CHANGE_PATTERN	(5)	//フレーム単位

//****************************************************
// 構造体定義
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffOutMedal; // 頂点バッファインターフェースへのポインタ
	float fPosX, fPosY;
	float fPosXLeft, fPosXRight; // U座標
	float fPosYUp,   fPosYDown;	 // V座標
}value;

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexOutMedal(LPDIRECT3DDEVICE9 pDevice);

void SetTextureOutMedal();

//****************************************************
// グローバル変数
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureOutMedal = NULL; // テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBar = NULL; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBar = NULL; // 頂点バッファインターフェースへのポインタ

value g_OutMedal[DIGIT_MAX];

int g_nOutMedal;

//====================================================
// 初期化処理
//====================================================
void InitOutMedal(void)
{
	g_nOutMedal = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	//----- 初期化 -----
	for (int i = 0; i < DIGIT_MAX; i ++)
	{
		g_OutMedal[i].fPosX = 0.0f;
		g_OutMedal[i].fPosY = 0.0f;
		g_OutMedal[i].fPosXLeft = 0.0f;
		g_OutMedal[i].fPosXRight = (float)(g_OutMedal[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X);
		g_OutMedal[i].fPosYUp = 0.0f;
		g_OutMedal[i].fPosYDown = (float)(g_OutMedal[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y);
	}

	MakeVertexOutMedal(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
							  POLYGON_TEXTURE,		// ファイル名
							  &g_pTextureOutMedal);	// 読み込み

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
							  BAR_TEXTURE,			// ファイル名
							  &g_pTextureBar);		// 読み込み
}

//====================================================
// 終了処理
//====================================================
void UninitOutMedal(void)
{
	if(g_pTextureOutMedal != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTextureOutMedal->Release();
		g_pTextureOutMedal = NULL;
	}

	if(g_pTextureBar != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTextureBar->Release();
		g_pTextureBar = NULL;
	}

	for (int i = 0; i < DIGIT_MAX; i ++)
	{	
		if(g_OutMedal[i].pVtxBuffOutMedal != NULL)
		{
			//----- 頂点バッファの解放 -----
			g_OutMedal[i].pVtxBuffOutMedal->Release();
			g_OutMedal[i].pVtxBuffOutMedal = NULL;
		}
	}

	if(g_pVtxBuffBar != NULL)
	{
		//----- 頂点バッファの解放 -----
		g_pVtxBuffBar->Release();
		g_pVtxBuffBar = NULL;
	}
}

//====================================================
// 更新処理
//====================================================
void UpdateOutMedal(void)
{
	int nNo = 0;

	//----- テクスチャ座標の算出 -----
	for(int i = 0; i < DIGIT_MAX; i ++)
	{
		switch(i)
		{
			case 6:
				nNo = g_nOutMedal % 10;
				break;
			case 5:
				nNo = g_nOutMedal / 10;
				break;
			case 4:
				nNo = g_nOutMedal / 100;
				break;
			case 3:
				nNo = g_nOutMedal / 1000;
				break;
			case 2:
				nNo = g_nOutMedal / 10000;
				break;
			case 1:
				nNo = g_nOutMedal / 100000;
				break;
			case 0:
				nNo = g_nOutMedal / 1000000;
				break;
		}
		if(i == 6 || nNo != 0)
		{
			g_OutMedal[i].fPosXLeft  = (float)((nNo * POLYGON_SIZE_X) / TEX_DEFAULT_X);
			g_OutMedal[i].fPosXRight = (float)((nNo * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X);
			g_OutMedal[i].fPosX		 = (float)(VALUE_POS_X + (POLYGON_SIZE_X * i));
			g_OutMedal[i].fPosY		 = (float)VALUE_POS_Y;
		}
	}
	SetTextureOutMedal();
}

//====================================================
// 描画処理
//====================================================
void DrawOutMedal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBar, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
	pDevice->SetTexture(0, g_pTextureBar);		// テクスチャ設定
	
	//----- ポリゴン描画 -----
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	for (int i = 0; i < DIGIT_MAX; i ++)
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_OutMedal[i].pVtxBuffOutMedal, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		pDevice->SetTexture(0, g_pTextureOutMedal);		// テクスチャ設定

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

HRESULT MakeVertexOutMedal(LPDIRECT3DDEVICE9 pDevice)
{

	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
										  D3DUSAGE_WRITEONLY,			  // バッファ使用法
										  FVF_VERTEX_2D,				  // 頂点フォーマット
										  D3DPOOL_MANAGED,				  // 保持するメモリクラス
										  &g_pVtxBuffBar,			  // 頂点バッファインターフェース
										  NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファの内容を埋める
	{
		VERTEX_2D	*pVtx;

		//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);
		
		//----- 頂点座標の設定 -----
		pVtx[0].pos = D3DXVECTOR3((float)(BAR_POS_X), (float)(BAR_POS_Y), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(BAR_POS_X + BAR_SIZE_X), (float)(BAR_POS_Y), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(BAR_POS_X), (float)(BAR_POS_Y + BAR_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(BAR_POS_X + BAR_SIZE_X), (float)(BAR_POS_Y + BAR_SIZE_Y), 0.0f);

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
		g_pVtxBuffBar->Unlock();
	}

	for(int i = 0; i < DIGIT_MAX; i ++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
												D3DUSAGE_WRITEONLY,			  // バッファ使用法
												FVF_VERTEX_2D,				  // 頂点フォーマット
												D3DPOOL_MANAGED,			  // 保持するメモリクラス
												&g_OutMedal[i].pVtxBuffOutMedal,	  // 頂点バッファインターフェース
												NULL)))
		{
			return E_FAIL;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
			g_OutMedal[i].pVtxBuffOutMedal->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX), (float)(g_OutMedal[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX + POLYGON_SIZE_X), (float)(g_OutMedal[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX), (float)(g_OutMedal[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX + POLYGON_SIZE_X), (float)(g_OutMedal[i].fPosY + POLYGON_SIZE_Y), 0.0f);

			//----- rhw設定 -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- 頂点カラー設定 -----
			pVtx[0].col = D3DCOLOR_RGBA(68, 68, 68, 255);
			pVtx[1].col = D3DCOLOR_RGBA(68, 68, 68, 255);
			pVtx[2].col = D3DCOLOR_RGBA(68, 68, 68, 255);
			pVtx[3].col = D3DCOLOR_RGBA(68, 68, 68, 255);

			//----- テクスチャ座標の設定 -----
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//----- 頂点データのアンロック -----
			g_OutMedal[i].pVtxBuffOutMedal->Unlock();
		}
	}
	return S_OK;
}

//====================================================
// テクスチャ座標の設定
//====================================================
void SetTextureOutMedal(void)
{
	// 頂点バッファの内容を埋める
	VERTEX_2D	*pVtx;

	g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
	//----- 頂点座標の設定 -----
	pVtx[0].pos = D3DXVECTOR3((float)(BAR_POS_X), (float)(BAR_POS_Y), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)(BAR_POS_X + BAR_SIZE_X), (float)(BAR_POS_Y), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((float)(BAR_POS_X), (float)(BAR_POS_Y + BAR_SIZE_Y), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)(BAR_POS_X + BAR_SIZE_X), (float)(BAR_POS_Y + BAR_SIZE_Y), 0.0f);

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
	g_pVtxBuffBar->Unlock();

	for(int i = 0; i < DIGIT_MAX; i ++)
	{
		// 頂点バッファの内容を埋める
		VERTEX_2D	*pVtx;

		g_OutMedal[i].pVtxBuffOutMedal->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
		//----- 頂点座標の設定 -----
		pVtx[0].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX), (float)(g_OutMedal[i].fPosY), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX + POLYGON_SIZE_X), (float)(g_OutMedal[i].fPosY), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX), (float)(g_OutMedal[i].fPosY+ POLYGON_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX + POLYGON_SIZE_X), (float)(g_OutMedal[i].fPosY + POLYGON_SIZE_Y), 0.0f);

		//----- rhw設定 -----
		pVtx[0].rhw = 
		pVtx[1].rhw = 
		pVtx[2].rhw = 
		pVtx[3].rhw = 1.0f;

		//----- 頂点カラー設定 -----
		pVtx[0].col = D3DCOLOR_RGBA(68, 68, 68, 255);
		pVtx[1].col = D3DCOLOR_RGBA(68, 68, 68, 255);
		pVtx[2].col = D3DCOLOR_RGBA(68, 68, 68, 255);
		pVtx[3].col = D3DCOLOR_RGBA(68, 68, 68, 255);

		//----- テクスチャ座標の設定 -----
		pVtx[0].tex = D3DXVECTOR2(g_OutMedal[i].fPosXLeft,  g_OutMedal[i].fPosYUp);
		pVtx[1].tex = D3DXVECTOR2(g_OutMedal[i].fPosXRight, g_OutMedal[i].fPosYUp);
		pVtx[2].tex = D3DXVECTOR2(g_OutMedal[i].fPosXLeft,  g_OutMedal[i].fPosYDown);
		pVtx[3].tex = D3DXVECTOR2(g_OutMedal[i].fPosXRight, g_OutMedal[i].fPosYDown);

		//----- 頂点データのアンロック -----
		g_OutMedal[i].pVtxBuffOutMedal->Unlock();
	}
}

//====================================================
// 排出総メダルの共有
//====================================================
int	GetOutMedal(void)
{
	return g_nOutMedal;
}

//====================================================
// 排出総メダルのセット
//====================================================
void SetOutMedal(int nOutMedal)
{
	g_nOutMedal += nOutMedal;
}