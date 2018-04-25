//====================================================
//
// スコア処理[score.cpp]
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "score.h"	//ポリゴン処理用のヘッダファイル]
#include "outMedal.h"

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define POLYGON_TEXTURE	"data/TEXTURE/resultV_35x42.png"

#define POLYGON_SIZE_X	(35 * 0.8)
#define POLYGON_SIZE_Y	(42 * 0.8)

#define TEX_DEFAULT_X	(350 * 0.8)
#define TEX_DEFAULT_Y	(42 * 0.8)

#define CREDIT_DIGIT_MAX (7)
#define MONEY_DIGIT_MAX	 (7)

#define CREDIT_POS_X	(153)
#define CREDIT_POS_Y	(150)

#define MONEY_POS_X		(140)
#define MONEY_POS_Y		(252)

//#define INTERVAL_CHANGE_PATTERN	(5)	//フレーム単位

//****************************************************
// 構造体定義
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffScore; // 頂点バッファインターフェースへのポインタ
	float fPosX, fPosY;
	float fPosXLeft, fPosXRight; // U座標
	float fPosYUp,   fPosYDown;	 // V座標
}value;

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

void SetTextureScore();

//****************************************************
// グローバル変数
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureScore = NULL; // テクスチャへのポインタ

// VERTEX_2D		   g_Vertex[NUM_VERTEX]; // 頂点情報格納ワーク

//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVPolygon = NULL; // 頂点バッファインターフェースへのポインタ

value g_Score[CREDIT_DIGIT_MAX];
value g_Money[MONEY_DIGIT_MAX];

//====================================================
// 初期化処理
//====================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	//----- 初期化 -----
	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		g_Score[i].fPosXLeft  = 0.0f;
		g_Score[i].fPosXRight = (float)(g_Score[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X);
		g_Score[i].fPosYUp    = 0.0f;
		g_Score[i].fPosYDown  = (float)(g_Score[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y);
	}

	for (int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		g_Money[i].fPosXLeft  = 0.0f;
		g_Money[i].fPosXRight = (float)(g_Money[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X);
		g_Money[i].fPosYUp    = 0.0f;
		g_Money[i].fPosYDown  = (float)(g_Money[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y);
	}

	MakeVertexScore(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
							  POLYGON_TEXTURE,		// ファイル名
							  &g_pTextureScore);	// 読み込み
}

//====================================================
// 終了処理
//====================================================
void UninitScore(void)
{
	if(g_pTextureScore != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{	
		if(g_Score[i].pVtxBuffScore != NULL)
		{
			//----- 頂点バッファの解放 -----
			g_Score[i].pVtxBuffScore->Release();
			g_Score[i].pVtxBuffScore = NULL;
		}
	}

	for (int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{	
		if(g_Money[i].pVtxBuffScore != NULL)
		{
			//----- 頂点バッファの解放 -----
			g_Money[i].pVtxBuffScore->Release();
			g_Money[i].pVtxBuffScore = NULL;
		}
	}
}

//====================================================
// 更新処理
//====================================================
void UpdateScore(int nMedal)
{
	int nNo = 0;
	int nMoney = 0;
	int nAllMedal = 0;

	nAllMedal = nMedal + GetOutMedal();

	nMoney = (int)(nAllMedal * 0.17);
	nMoney *= 100;

	//----- テクスチャ座標の算出 -----
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		switch(i)
		{
			case 6:
				nNo = nAllMedal % 10;
				break;
			case 5:
				nNo = nAllMedal / 10;
				break;
			case 4:
				nNo = nAllMedal / 100;
				break;
			case 3:
				nNo = nAllMedal / 1000;
				break;
			case 2:
				nNo = nAllMedal / 10000;
				break;
			case 1:
				nNo = nAllMedal / 100000;
				break;
			case 0:
				nNo = nAllMedal / 1000000;
				break;
		}
		g_Score[i].fPosXLeft  = (float)((nNo * POLYGON_SIZE_X) / TEX_DEFAULT_X);
		g_Score[i].fPosXRight = (float)((nNo * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X);
		g_Score[i].fPosX	  = (float)(CREDIT_POS_X + (POLYGON_SIZE_X * i * 1.2));
		g_Score[i].fPosY	  = (float)(CREDIT_POS_Y);
	}

	//----- テクスチャ座標の算出 -----
	for(int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		switch(i)
		{
			case 6:
				nNo = nMoney % 10;
				break;
			case 5:
				nNo = nMoney / 10;
				break;
			case 4:
				nNo = nMoney / 100;
				break;
			case 3:
				nNo = nMoney / 1000;
				break;
			case 2:
				nNo = nMoney / 10000;
				break;
			case 1:
				nNo = nMoney / 100000;
				break;
			case 0:
				nNo = nMoney / 1000000;
				break;
		}
		g_Money[i].fPosXLeft  = (float)((nNo * POLYGON_SIZE_X) / TEX_DEFAULT_X);
		g_Money[i].fPosXRight = (float)((nNo * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X);
		g_Money[i].fPosX =		(float)(MONEY_POS_X + (POLYGON_SIZE_X * i * 1.2));
		g_Money[i].fPosY =		(float)(MONEY_POS_Y);

		if(i > 0)
		{
			g_Money[i].fPosX += 6;
		}
		if(i > 3)
		{
			g_Money[i].fPosX += 6;
		}
	}
	SetTextureScore();
}

//====================================================
// 描画処理
//====================================================
void DrawScore(void)
{
	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_Score[i].pVtxBuffScore, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		pDevice->SetTexture(0, g_pTextureScore);		// テクスチャ設定

		//----- ポリゴン描画 -----
	//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,	// 三角形ストリップを指定
	//							 NUM_POLYGON,			// 2つの三角形で四角形を作る
	//							 &g_Vertex[0],			// 頂点情報の開始アドレス
	//							 sizeof(VERTEX_2D));	// 頂点情報一つ分のサイズ
	
		//----- ポリゴン描画 -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	for (int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_Money[i].pVtxBuffScore, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		pDevice->SetTexture(0, g_pTextureScore);		// テクスチャ設定

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

HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
												D3DUSAGE_WRITEONLY,			  // バッファ使用法
												FVF_VERTEX_2D,				  // 頂点フォーマット
												D3DPOOL_MANAGED,			  // 保持するメモリクラス
												&g_Score[i].pVtxBuffScore,	  // 頂点バッファインターフェース
												NULL)))
		{
			return E_FAIL;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
			g_Score[i].pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Score[i].fPosX),  (float)(g_Score[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Score[i].fPosX + POLYGON_SIZE_X), (float)(g_Score[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Score[i].fPosX),  (float)(g_Score[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Score[i].fPosX + POLYGON_SIZE_X), (float)(g_Score[i].fPosY + POLYGON_SIZE_Y), 0.0f);

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
			g_Score[i].pVtxBuffScore->Unlock();
		}
	}
	for(int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
												D3DUSAGE_WRITEONLY,			  // バッファ使用法
												FVF_VERTEX_2D,				  // 頂点フォーマット
												D3DPOOL_MANAGED,			  // 保持するメモリクラス
												&g_Money[i].pVtxBuffScore,	  // 頂点バッファインターフェース
												NULL)))
		{
			return E_FAIL;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
			g_Money[i].pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Money[i].fPosX), (float)(g_Money[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Money[i].fPosX + POLYGON_SIZE_X), (float)(g_Money[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Money[i].fPosX), (float)(g_Money[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Money[i].fPosX + POLYGON_SIZE_X), (float)(g_Money[i].fPosY + POLYGON_SIZE_Y), 0.0f);

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
			g_Money[i].pVtxBuffScore->Unlock();
		}
	}
	return S_OK;
}

//====================================================
// テクスチャ座標の設定
//====================================================
void SetTextureScore(void)
{
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			g_Score[i].pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Score[i].fPosX), (float)(g_Score[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Score[i].fPosX + POLYGON_SIZE_X), (float)(g_Score[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Score[i].fPosX), (float)(g_Score[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Score[i].fPosX + POLYGON_SIZE_X), (float)(g_Score[i].fPosY + POLYGON_SIZE_Y), 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_Score[i].fPosXLeft, g_Score[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_Score[i].fPosXRight, g_Score[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_Score[i].fPosXLeft, g_Score[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_Score[i].fPosXRight, g_Score[i].fPosYDown);

			//----- 頂点データのアンロック -----
			g_Score[i].pVtxBuffScore->Unlock();

		}
	}

	for(int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			g_Money[i].pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Money[i].fPosX), (float)(g_Money[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Money[i].fPosX + POLYGON_SIZE_X), (float)(g_Money[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Money[i].fPosX), (float)(g_Money[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Money[i].fPosX + POLYGON_SIZE_X), (float)(g_Money[i].fPosY + POLYGON_SIZE_Y), 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_Money[i].fPosXLeft,  g_Money[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_Money[i].fPosXRight, g_Money[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_Money[i].fPosXLeft,  g_Money[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_Money[i].fPosXRight, g_Money[i].fPosYDown);

			//----- 頂点データのアンロック -----
			g_Money[i].pVtxBuffScore->Unlock();

		}
	}
}