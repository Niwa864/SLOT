//====================================================
//
// タイトル処理[title.cpp]
//
// ～更新履歴～
// 2016/08/03 制作開始
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "title.h"	//ポリゴン処理用のヘッダファイル
#include <stdio.h>
#include "sound.h"

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define TEX_START	"data/TEXTURE/start_400x400.png"
#define TEX_END		"data/TEXTURE/end_200x200.png"

#define START_SIZE_X	(400 * 0.8)	// ポリゴンサイズ
#define START_SIZE_Y	(400 * 0.8)

#define END_SIZE_X		(200 * 0.8)	// ポリゴンサイズ
#define END_SIZE_Y		(200 * 0.8)

#define START_DEFAULT_SIZE_X	(800 * 0.8)	// テクスチャのもともとの大きさ
#define START_DEFAULT_SIZE_Y	(400 * 0.8)	

#define END_DEFAULT_SIZE_X		(400 * 0.8)	// テクスチャのもともとの大きさ
#define END_DEFAULT_SIZE_Y		(200 * 0.8)	

#define TEXTURE_MAX	(2)


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
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);

void SorESetTexture(void);

//****************************************************
// グローバル変数
//****************************************************
tex g_TexT[TEXTURE_MAX];

int g_nSorE;

//====================================================
// 初期化処理
//====================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	//----- テクスチャの初期化 -----
	g_TexT[0].fPosXLeft    = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
	g_TexT[0].fPosXRight   = (float)(START_SIZE_X + START_SIZE_X) / START_DEFAULT_SIZE_X;
	g_TexT[0].fPosYUp	  = 0.0f;
	g_TexT[0].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
	g_TexT[0].fPolySizeX   = START_SIZE_X;
	g_TexT[0].fPolySizeY   = START_SIZE_Y;
	g_TexT[0].fTexDefaultX = START_DEFAULT_SIZE_X;
	g_TexT[0].fTexDefaultY = START_DEFAULT_SIZE_Y;
	g_TexT[0].fPolyPosX = 30;
	g_TexT[0].fPolyPosY = 250;

	g_TexT[1].fPosXLeft    = 0.0f;
	g_TexT[1].fPosXRight   = (float)END_SIZE_X / END_DEFAULT_SIZE_X;
	g_TexT[1].fPosYUp	  = 0.0f;
	g_TexT[1].fPosYDown    = (float)END_SIZE_Y / END_DEFAULT_SIZE_Y;
	g_TexT[1].fPolySizeX   = END_SIZE_X;
	g_TexT[1].fPolySizeY   = END_SIZE_Y;
	g_TexT[1].fTexDefaultX = END_DEFAULT_SIZE_X;
	g_TexT[1].fTexDefaultY = END_DEFAULT_SIZE_Y;
	g_TexT[1].fPolyPosX = 340;
	g_TexT[1].fPolyPosY = 450;

	MakeVertexTitle(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEX_START,					// ファイル名
								&g_TexT[0].pTextureFile);	// 読み込み

	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
								TEX_END,					// ファイル名
								&g_TexT[1].pTextureFile);	// 読み込み

	PlaySound(SOUND_LABEL_TITLE);
}

//====================================================
// 終了処理
//====================================================
void UninitTitle(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		if(g_TexT[i].pTextureFile != NULL)
		{
			//----- テクスチャの解放 -----
			g_TexT[i].pTextureFile->Release();
			g_TexT[i].pTextureFile = NULL;

			//----- 頂点バッファの解放 -----
			g_TexT[i].pVtxBuffPolygon->Release();
			g_TexT[i].pVtxBuffPolygon = NULL;
		}
	}
}

//====================================================
// 更新処理
//====================================================
void UpdateTitle(void)
{
	if(!g_nSorE)
	{
		g_TexT[0].fPosXLeft    = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
		g_TexT[0].fPosXRight   = (float)(START_SIZE_X + START_SIZE_X) / START_DEFAULT_SIZE_X;
		g_TexT[0].fPosYUp	  = 0.0f;
		g_TexT[0].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
		g_TexT[1].fPosXLeft    = 0.0f;
		g_TexT[1].fPosXRight   = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
		g_TexT[1].fPosYUp	  = 0.0f;
		g_TexT[1].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
	}

	if(g_nSorE)
	{
		g_TexT[0].fPosXLeft    = 0.0f;
		g_TexT[0].fPosXRight   = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
		g_TexT[0].fPosYUp	  = 0.0f;
		g_TexT[0].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
		g_TexT[1].fPosXLeft    = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
		g_TexT[1].fPosXRight   = (float)(START_SIZE_X + START_SIZE_X) / START_DEFAULT_SIZE_X;
		g_TexT[1].fPosYUp	  = 0.0f;
		g_TexT[1].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
	}
	SorESetTexture();
}

//====================================================
// 描画処理
//====================================================
void DrawTitle(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_TexT[i].pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		pDevice->SetTexture(0, g_TexT[i].pTextureFile);		// テクスチャ設定

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
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
											  D3DUSAGE_WRITEONLY,			  // バッファ使用法
											  FVF_VERTEX_2D,				  // 頂点フォーマット
											  D3DPOOL_MANAGED,				  // 保持するメモリクラス
											  &g_TexT[i].pVtxBuffPolygon,	  // 頂点バッファインターフェース
											  NULL)))
		{
			return E_FAIL;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
			g_TexT[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX, g_TexT[i].fPolyPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX + g_TexT[i].fPolySizeX, g_TexT[i].fPolyPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX, g_TexT[i].fPolyPosY + g_TexT[i].fPolySizeY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX + g_TexT[i].fPolySizeX, g_TexT[i].fPolyPosY + g_TexT[i].fPolySizeY, 0.0f);

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
			g_TexT[i].pVtxBuffPolygon->Unlock();

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
void SorESetTexture(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			g_TexT[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX, g_TexT[i].fPolyPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX + g_TexT[i].fPolySizeX, g_TexT[i].fPolyPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX, g_TexT[i].fPolyPosY + g_TexT[i].fPolySizeY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX + g_TexT[i].fPolySizeX, g_TexT[i].fPolyPosY + g_TexT[i].fPolySizeY, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_TexT[i].fPosXLeft , g_TexT[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_TexT[i].fPosXRight, g_TexT[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_TexT[i].fPosXLeft , g_TexT[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_TexT[i].fPosXRight, g_TexT[i].fPosYDown);

			//----- 頂点データのアンロック -----
			g_TexT[i].pVtxBuffPolygon->Unlock();

		}
	}
}

//====================================================
// 切り替え共有
//====================================================
int GetSorE (void)
{
	return g_nSorE;
}

//====================================================
// 切り替えセット
//====================================================
void SetSorE (int nType)
{
	g_nSorE = nType;
}