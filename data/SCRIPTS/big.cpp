//====================================================
//
// ビッグ演出処理[big.cpp]
//
// ～更新履歴～
// 2016/08/19 制作開始
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "big.h"
#include "sound.h"

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define TEX_SEAO		"data/TEXTURE/SeaO_390x291.png"
#define TEX_PLUS_PINK	"data/TEXTURE/15p_200x110.png"
#define TEX_PLUS_GREEN	"data/TEXTURE/15g_200x110.png"

#define SEAO_POS_X		(95)
#define SEAO_POS_Y		(11)
#define PLUS_POS_X		(175)
#define PLUS_POS_Y		(110)

#define SEAO_SIZE_X	(390 * 0.8)	// ポリゴンサイズ
#define SEAO_SIZE_Y	(291 * 0.8)
#define PLUS_SIZE_X (200 * 0.8)
#define PLUS_SIZE_Y (110 * 0.8)

#define SEAO_PATTERN_DIVIDE_X	(4)	// テクスチャ内分割数(X)
#define SEAO_PATTERN_DIVIDE_Y	(1) // テクスチャ内分割数(Y)

#define SEAO_PATTERN_SIZE_X	(1.0f / SEAO_PATTERN_DIVIDE_X)	//1パターン分のテクスチャサイズを算出
#define SEAO_PATTERN_SIZE_Y	(1.0f / SEAO_PATTERN_DIVIDE_Y)

#define SEAO_PATTERN_MAX		(SEAO_PATTERN_DIVIDE_X * SEAO_PATTERN_DIVIDE_Y)	//アニメーションの総パターン数

#define SEAO_DEFAULT_SIZE_X		(1560 * 0.8)	// テクスチャのもともとの大きさ
#define SEAO_DEFAULT_SIZE_Y		(291 * 0.8)		

#define INTERVAL_CHANGE_PATTERN	(12)	//フレーム単位

#define PLUS_TEX_MAX			(2)
#define POLYGON_MAX				(2)
//#define INTERVAL_FRAME_SEAO		(2)

#define COUNT_MAX				(20)

//****************************************************
// 構造体定義
//****************************************************
typedef struct
{
	float fPosXLeft, fPosXRight; // U座標
	float fPosYUp,   fPosYDown;	 // V座標
}pos;

//****************************************************
// 列挙型
//****************************************************

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexBig(LPDIRECT3DDEVICE9 pDevice);

void SetTextureBig(void);

//****************************************************
// グローバル変数
//****************************************************
LPDIRECT3DTEXTURE9 g_pTextureBig = NULL; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBig = NULL; // 頂点バッファインターフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlus;	  // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlus; // 頂点バッファインターフェースへのポインタ

int g_nCounterBig;	// アニメーションカウンター
int g_nPatternBig;	// アニメパターンNo

pos g_PolyPos[POLYGON_MAX];

int g_nBigCount;

MODE g_SaveMode;

int g_nAlpha;

//====================================================
// 初期化処理
//====================================================
void InitBig(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	//int nSaveCount = g_nBigCount;

	//----- カウンターの初期化 -----
	//g_nCounterBig = 0;
	//g_nPatternBig = 0;

	//g_nBigCount = 0;

	//g_bAnimFlg = true;

	MakeVertexBig(pDevice);				 // 頂点情報の作成

	int nColorPG = g_nBigCount % 2;

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
							  TEX_SEAO,				// ファイル名
							  &g_pTextureBig);		// 読み込み

	switch(nColorPG)
	{
		case 0: //----- テクスチャの読み込み -----
				D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
										  TEX_PLUS_PINK,		// ファイル名
										  &g_pTexturePlus);		// 読み込み---Plus同じ場所使う
				break;

		case 1: //----- テクスチャの読み込み -----
				D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
										  TEX_PLUS_GREEN,		// ファイル名
										  &g_pTexturePlus);		// 読み込み
				break;
	}
}

//====================================================
// 終了処理
//====================================================
void UninitBig(void)
{

	if(g_pTextureBig != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTextureBig->Release();
		g_pTextureBig = NULL;

		//----- 頂点バッファの解放 -----
		g_pVtxBuffBig->Release();
		g_pVtxBuffBig = NULL;
	}

	if(g_pTexturePlus != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTexturePlus->Release();
		g_pTexturePlus = NULL;

		//----- 頂点バッファの解放 -----
		g_pVtxBuffPlus->Release();
		g_pVtxBuffPlus = NULL;
	}
}

//====================================================
// 更新処理
//====================================================
void UpdateBig(void)
{

	//if(!g_bAnimFlg)		// アニメ制御の許可があるときのみ
	//{
	//	return;
	//}

	g_nCounterBig ++;	// アニメカウンターのカウントアップ

	if((g_nCounterBig % INTERVAL_CHANGE_PATTERN) == 0)				// 画像切り替えタイミングの判断
	{		
		g_nPatternBig = (g_nPatternBig + 1) % SEAO_PATTERN_MAX;		// パターン切り替え
	}
	SetTextureBig();
		 
	if(g_nBigCount > COUNT_MAX)
	{
		SetMode(MODE_PLAY);
		InitBigValue();

		StopSound(SOUND_LABEL_BIG);
		PlaySound(SOUND_LABEL_PLAY);
	}
}

//====================================================
// 描画処理
//====================================================
void DrawBig(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int i = 0; i < POLYGON_MAX; i ++)
	{

		switch(i)
		{
			case 0: // 頂点バッファをデバイスのデータストリームにバインド
					pDevice->SetStreamSource(0, g_pVtxBuffBig, 0, sizeof(VERTEX_2D));
					break;
			case 1: // 頂点バッファをデバイスのデータストリームにバインド
					pDevice->SetStreamSource(0, g_pVtxBuffPlus, 0, sizeof(VERTEX_2D));
					break;
		}

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		switch(i)
		{
			case 0: pDevice->SetTexture(0, g_pTextureBig);		// テクスチャ設定
					break;
			case 1: pDevice->SetTexture(0, g_pTexturePlus);		// テクスチャ設定
					break;
		}

		//----- ポリゴン描画 -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

}
//====================================================
// 頂点の作成
//====================================================

HRESULT MakeVertexBig(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < POLYGON_MAX; i ++)
	{
		switch(i)
		{
			case 0: // オブジェクトの頂点バッファを生成
					if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
															D3DUSAGE_WRITEONLY,			  // バッファ使用法
															FVF_VERTEX_2D,				  // 頂点フォーマット
															D3DPOOL_MANAGED,			  // 保持するメモリクラス
															&g_pVtxBuffBig,				  // 頂点バッファインターフェース
															NULL)))
					{
						return E_FAIL;
					}
					break;
			case 1: // オブジェクトの頂点バッファを生成
					if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
															D3DUSAGE_WRITEONLY,			  // バッファ使用法
															FVF_VERTEX_2D,				  // 頂点フォーマット
															D3DPOOL_MANAGED,			  // 保持するメモリクラス
															&g_pVtxBuffPlus,			  // 頂点バッファインターフェース
															NULL)))
					{
						return E_FAIL;
					}
					break;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			switch(i)
			{
				case 0: //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
						g_pVtxBuffBig->Lock(0, 0, (void**)&pVtx, 0);

						//----- 頂点座標の設定 -----
						pVtx[0].pos = D3DXVECTOR3((float)(SEAO_POS_X), (float)(SEAO_POS_Y), 0.0f);
						pVtx[1].pos = D3DXVECTOR3((float)(SEAO_POS_X + SEAO_SIZE_X), (float)(SEAO_POS_Y), 0.0f);
						pVtx[2].pos = D3DXVECTOR3((float)(SEAO_POS_X), (float)(SEAO_POS_Y + SEAO_SIZE_Y), 0.0f);
						pVtx[3].pos = D3DXVECTOR3((float)(SEAO_POS_X + SEAO_SIZE_X), (float)(SEAO_POS_Y + SEAO_SIZE_Y), 0.0f);
						break;
				case 1: //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
						g_pVtxBuffPlus->Lock(0, 0, (void**)&pVtx, 0);

						//----- 頂点座標の設定 -----
						pVtx[0].pos = D3DXVECTOR3((float)(PLUS_POS_X), (float)(PLUS_POS_Y), 0.0f);
						pVtx[1].pos = D3DXVECTOR3((float)(PLUS_POS_X + PLUS_SIZE_X), (float)(PLUS_POS_Y), 0.0f);
						pVtx[2].pos = D3DXVECTOR3((float)(PLUS_POS_X), (float)(PLUS_POS_Y + PLUS_SIZE_Y), 0.0f);
						pVtx[3].pos = D3DXVECTOR3((float)(PLUS_POS_X + PLUS_SIZE_X), (float)(PLUS_POS_Y + PLUS_SIZE_Y), 0.0f);
						break;
			}

			//----- rhw設定 -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- 頂点カラー設定 -----
			switch(i)
			{
			case 0: pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					break;

			case 1: pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_nAlpha);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_nAlpha);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_nAlpha);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_nAlpha);
					break;
			}

			g_nAlpha --;

			//----- テクスチャ座標の設定 -----
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			switch(i)
			{
				case 0: //----- 頂点データのアンロック -----
						g_pVtxBuffBig->Unlock();
						break;
				case 1: //----- 頂点データのアンロック -----
						g_pVtxBuffBig->Unlock();
						break;
			}
		}
	}
	return S_OK;
}

//====================================================
// テクスチャ座標の設定
//====================================================
void SetTextureBig(void)
{

	// 頂点バッファの内容を埋める
	{
		VERTEX_2D	*pVtx;

		for(int i = 0; i < POLYGON_MAX; i ++)
		{
			switch(i)
			{
			case 0: g_pVtxBuffBig->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得

					//----- テクスチャ座標の算出 -----   
					g_PolyPos[i].fPosXLeft	= (float)(g_nPatternBig % SEAO_PATTERN_DIVIDE_X) * SEAO_PATTERN_SIZE_X;
					g_PolyPos[i].fPosXRight = (float)(g_nPatternBig % SEAO_PATTERN_DIVIDE_X) * SEAO_PATTERN_SIZE_X + SEAO_PATTERN_SIZE_X;
					g_PolyPos[i].fPosYUp	= (float)(g_nPatternBig / (SEAO_PATTERN_MAX / SEAO_PATTERN_DIVIDE_Y)) * SEAO_PATTERN_SIZE_Y;
					g_PolyPos[i].fPosYDown	= (float)(g_nPatternBig / (SEAO_PATTERN_MAX / SEAO_PATTERN_DIVIDE_Y)) * SEAO_PATTERN_SIZE_Y + SEAO_PATTERN_SIZE_Y;

					//----- 頂点座標の設定 -----
					pVtx[0].pos = D3DXVECTOR3((float)(SEAO_POS_X), (float)(SEAO_POS_Y), 0.0f);
					pVtx[1].pos = D3DXVECTOR3((float)(SEAO_POS_X + SEAO_SIZE_X), (float)(SEAO_POS_Y), 0.0f);
					pVtx[2].pos = D3DXVECTOR3((float)(SEAO_POS_X), (float)(SEAO_POS_Y + SEAO_SIZE_Y), 0.0f);
					pVtx[3].pos = D3DXVECTOR3((float)(SEAO_POS_X + SEAO_SIZE_X), (float)(SEAO_POS_Y + SEAO_SIZE_Y), 0.0f);
					break;

			case 1: g_pVtxBuffPlus->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得

					//----- テクスチャ座標の算出 -----   
					g_PolyPos[i].fPosXLeft	= 0.0f;
					g_PolyPos[i].fPosXRight = (float)PLUS_SIZE_X / PLUS_SIZE_X;
					g_PolyPos[i].fPosYUp	= 0.0f;
					g_PolyPos[i].fPosYDown	= (float)PLUS_SIZE_Y / PLUS_SIZE_Y;

					//----- 頂点座標の設定 -----
					pVtx[0].pos = D3DXVECTOR3(PLUS_POS_X, PLUS_POS_Y, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(PLUS_POS_X + PLUS_SIZE_X, PLUS_POS_Y, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(PLUS_POS_X, PLUS_POS_Y + PLUS_SIZE_Y, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(PLUS_POS_X + PLUS_SIZE_X, PLUS_POS_Y + PLUS_SIZE_Y, 0.0f);
					break;
			}


			//----- rhw設定 -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- 頂点カラー設定 -----
			switch(i)
			{
			case 0: pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					break;

			case 1: pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_nAlpha);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_nAlpha);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_nAlpha);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_nAlpha);
					break;
			}

			g_nAlpha --;

			//----- テクスチャ座標の設定 -----
			pVtx[0].tex = D3DXVECTOR2(g_PolyPos[i].fPosXLeft,  g_PolyPos[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_PolyPos[i].fPosXRight, g_PolyPos[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_PolyPos[i].fPosXLeft,  g_PolyPos[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_PolyPos[i].fPosXRight, g_PolyPos[i].fPosYDown);

			//----- 頂点データのアンロック -----
			g_pVtxBuffBig->Unlock();

		}

	}
}

//====================================================
// Bigカウンター加算関数
//====================================================
void ChangeBigCounter(void)
{
	g_nBigCount ++;
}

//====================================================
// Bigカウンター共有
//====================================================
int GetBigCount(void)
{
	return g_nBigCount;
}


//====================================================
// モードセーブ(ポーズ回帰用)
//====================================================
void SaveMode (MODE Mode)
{
	g_SaveMode = Mode;
}

//====================================================
// モードロード
//====================================================
MODE LoadMode (void)
{
	return g_SaveMode;
}

//====================================================
// 値初期化用
//====================================================
void InitBigValue (void)
{
	g_nBigCount = 0;
	g_nCounterBig = 0;
	g_nPatternBig = 0;
	g_nAlpha = 255;
}

