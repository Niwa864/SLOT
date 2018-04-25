//====================================================
//
// 演出処理[direction.cpp]
//
// ～更新履歴～
// 2016/06/14 制作開始
// 2016/07/19 演出処理を追加
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "direction.h"
#include "polygon.h"

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define TEX_LOGO		"data/TEXTURE/Logo_390x291.png"
#define TEX_SPYKE_ANIM	"data/TEXTURE/SpykeAnim_500x291.png"
#define TEX_CALLIE01	"data/TEXTURE/callie_1200x291.png"
#define TEX_MARIE01		"data/TEXTURE/marie_1200x291.png"

#define POLYGON_POS_X	(95)
#define POLYGON_POS_Y	(11)

#define POLYGON_SIZE_X	(390 * 0.8)	// ポリゴンサイズ
#define POLYGON_SIZE_Y	(291 * 0.8)

#define SPYKE_PATTERN_DIVIDE_X	(3)	// テクスチャ内分割数(X)
#define SPYKE_PATTERN_DIVIDE_Y	(1) // テクスチャ内分割数(Y)

#define SPYKE_PATTERN_SIZE_X	(1.0f / SPYKE_PATTERN_DIVIDE_X)	//1パターン分のテクスチャサイズを算出
#define SPYKE_PATTERN_SIZE_Y	(1.0f / SPYKE_PATTERN_DIVIDE_Y)

#define SPYKE_PATTERN_MAX		(SPYKE_PATTERN_DIVIDE_X * SPYKE_PATTERN_DIVIDE_Y)	//アニメーションの総パターン数

#define LOGO_DEFAULT_SIZE_X	(390 * 0.8)	// テクスチャのもともとの大きさ
#define LOGO_DEFAULT_SIZE_Y	(291 * 0.8)	
#define SPYKE_DEFAULT_SIZE_X	(1500 * 0.8)
#define SPYKE_DEFAULT_SIZE_Y	(291 * 0.8)	
#define CALLIE_DEFAULT_SIZE_X	(1200 * 0.8)
#define CALLIE_DEFAULT_SIZE_Y	(291 * 0.8)	
#define MARIE_DEFAULT_SIZE_X	(1200 * 0.8)
#define MARIE_DEFAULT_SIZE_Y	(291 * 0.8)	

#define INTERVAL_CHANGE_PATTERN	(15)	//フレーム単位
#define INTERVAL_FRAME_SEAO		(2)

//****************************************************
// 構造体定義
//****************************************************

//****************************************************
// 列挙型
//****************************************************

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexPolygonD(LPDIRECT3DDEVICE9 pDevice);

void SetTextureAnim(void);

//****************************************************
// グローバル変数
//****************************************************
LPDIRECT3DDEVICE9	g_pD3DDeviceD = NULL;	// Deviceオブジェクト(描画に必要)
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL; // 頂点バッファインターフェースへのポインタ

int g_nCounterAnim;	// アニメーションカウンター
int g_nPatternAnim;	// アニメパターンNo

float g_fPosXLeft, g_fPosXRight;
float g_fPosYUp,   g_fPosYDown;

float g_fPosXLeftSeaO, g_fPosXRightSeaO;

bool g_bAnimFlg;
bool g_bDirectionFlg; // 演出中かどうか(1~)

//====================================================
// 初期化処理
//====================================================
void InitDirection(void)
{
	int nNo = GetDirectionNo();

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	//----- カウンターの初期化 -----
	g_nCounterAnim = 0;
	g_nPatternAnim = 0;

	g_fPosXLeftSeaO = 0.0f;
	g_fPosXRightSeaO = 0.0f;

	g_bAnimFlg = true;
	g_bDirectionFlg = false;

	MakeVertexPolygonD(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	switch(nNo)
	{
	case 0 : D3DXCreateTextureFromFile(pDevice, TEX_LOGO, &g_pTexturePolygon);
			 g_fPosXLeft  = 0.0f;
			 g_fPosXRight = (float)(POLYGON_SIZE_X / LOGO_DEFAULT_SIZE_X);
			 g_fPosYUp    = 0.0f;
			 g_fPosYDown  = (float)(POLYGON_SIZE_Y / LOGO_DEFAULT_SIZE_Y);
			 break;
	case 1 : D3DXCreateTextureFromFile(pDevice, TEX_SPYKE_ANIM, &g_pTexturePolygon);
			 g_fPosXLeft  = (float)(g_nPatternAnim % SPYKE_PATTERN_DIVIDE_X) * SPYKE_PATTERN_SIZE_X;
			 g_fPosXRight = (float)(g_nPatternAnim % SPYKE_PATTERN_DIVIDE_X) * SPYKE_PATTERN_SIZE_X + SPYKE_PATTERN_SIZE_X;
			 g_fPosYUp	  = (float)(g_nPatternAnim / (SPYKE_PATTERN_MAX / SPYKE_PATTERN_DIVIDE_Y)) * SPYKE_PATTERN_SIZE_Y;
			 g_fPosYDown  = (float)(g_nPatternAnim / (SPYKE_PATTERN_MAX / SPYKE_PATTERN_DIVIDE_Y)) * SPYKE_PATTERN_SIZE_Y + SPYKE_PATTERN_SIZE_Y;
			 break;
	case 2 : D3DXCreateTextureFromFile(pDevice, TEX_CALLIE01, &g_pTexturePolygon);
			 g_fPosXLeftSeaO  = 1.0f - (float)(POLYGON_SIZE_X / MARIE_DEFAULT_SIZE_X);
			 g_fPosXRightSeaO = 1.0f;
			 g_fPosYUp		  = 0.0f;
			 g_fPosYDown	  = (float)(POLYGON_SIZE_Y / MARIE_DEFAULT_SIZE_Y);
			 SetTextureAnim();
			 break;
	case 3 : D3DXCreateTextureFromFile(pDevice, TEX_MARIE01, &g_pTexturePolygon);
			 g_fPosXLeftSeaO  = 0.0f;
			 g_fPosXRightSeaO = (float)(POLYGON_SIZE_X / CALLIE_DEFAULT_SIZE_X);
			 g_fPosYUp		  = 0.0f;
			 g_fPosYDown	  = (float)(POLYGON_SIZE_Y / CALLIE_DEFAULT_SIZE_Y);
			 SetTextureAnim();
			 break;
	}
}

//====================================================
// 終了処理
//====================================================
void UninitDirection(void)
{

	if(g_pTexturePolygon != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;

		//----- 頂点バッファの解放 -----
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//====================================================
// 更新処理
//====================================================
void UpdateDirection(void)
{
	int nNo = GetDirectionNo();

	//if(nNo == 0)		// 0は弾く
	//{
	//	return;
	//}
	if(!g_bAnimFlg)		// アニメ制御の許可があるときのみ
	{
		return;
	}

	g_nCounterAnim ++;	// アニメカウンターのカウントアップ

	switch(nNo)
	{
	case 0://if((g_nCounterAnim % INTERVAL_CHANGE_PATTERN) == 0)				// 画像切り替えタイミングの判断
	   	   //{		
		   //	g_nPatternAnim = (g_nPatternAnim + 1) % SPYKE_PATTERN_MAX;	// パターン切り替え
		   //}
		   SetTextureAnim();								// パターン変えるときは引数g_PattaernAnim
		   break;

	case 1:if((g_nCounterAnim % INTERVAL_CHANGE_PATTERN) == 0)				// 画像切り替えタイミングの判断
		   {		
			   g_nPatternAnim = (g_nPatternAnim + 1) % SPYKE_PATTERN_MAX;	// パターン切り替え
		   }
		   SetTextureAnim();								// パターン変えるときは引数g_PattaernAnim
		   break;

	case 2:if((g_nCounterAnim % INTERVAL_FRAME_SEAO) == 0)				// 画像切り替えタイミングの判断
		   {		
			   	SetTextureAnim();								// パターン変えるときは引数g_PattaernAnim

				//----- テクスチャ座標の算出 -----
				if(g_fPosXRightSeaO <= 0.55f)
				{
					g_fPosXLeftSeaO  -= 0.015f;
					g_fPosXRightSeaO -= 0.015f;
				}
				else if(g_fPosXRightSeaO <= 0.65f)
				{
					g_fPosXLeftSeaO  -= 0.005f;
					g_fPosXRightSeaO -= 0.005f;
				}
				else
				{
					g_fPosXLeftSeaO  -= 0.01f;
					g_fPosXRightSeaO -= 0.01f;
				}
		   }
		   break;

	case 3:if((g_nCounterAnim % INTERVAL_FRAME_SEAO) == 0)				// 画像切り替えタイミングの判断
		   {		
			   	SetTextureAnim();								// パターン変えるときは引数g_PattaernAnim

			   	//----- テクスチャ座標の算出 -----
				if(g_fPosXRightSeaO <= 0.55f)
				{
					g_fPosXLeftSeaO  += 0.01f;
					g_fPosXRightSeaO += 0.01f;
				}
				else if(g_fPosXRightSeaO <= 0.65f)
				{
					g_fPosXLeftSeaO  += 0.005f;
					g_fPosXRightSeaO += 0.005f;
				}
				else
				{
					g_fPosXLeftSeaO  += 0.015f;
					g_fPosXRightSeaO += 0.015f;
				}
		   }
		   break;
	}

	if(g_nPatternAnim == 2)
	{
		//while()
		//{

		//}
		UpdatePolygon(true, 0); // 後で全部回すので仮値
		SetMode(MODE_PLAY);
		//g_bDirectionFlg = true;
		SetDirectionNo(0);
		InitDirection();	//Init内でg_bDirectionFlgをfalseに
	}

	//値プラス化
	if(g_fPosXRightSeaO < 0)
	{
		g_fPosXRightSeaO = -g_fPosXRightSeaO;
		g_fPosXLeftSeaO  = -g_fPosXLeftSeaO;
	}

	if(nNo == 2 && g_fPosXRightSeaO <= 0.3f) 
	{
		SetMode(MODE_PLAY);
		//g_bDirectionFlg = true;
		SetDirectionNo(0);
		InitDirection();	//Init内でg_bDirectionFlgをfalseに
	}

	if(nNo == 3 && g_fPosXRightSeaO >= 1.05f) 
	{
		SetMode(MODE_PLAY);
		//g_bDirectionFlg = true;
		SetDirectionNo(0);
		InitDirection();
	}

	//----- テクスチャ座標の算出 -----
	//g_fPosXLeft	 += 0.01f;
	//g_fPosXRight += 0.01f;
}

//====================================================
// 描画処理
//====================================================
void DrawDirection(void)
{
	int nNo = GetDirectionNo();

	//if(nNo == 0)		// 0は弾く
	//{
	//	return;
	//}
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
	pDevice->SetTexture(0, g_pTexturePolygon);		// テクスチャ設定

	//----- ポリゴン描画 -----
	//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,	// 三角形ストリップを指定
	//							 NUM_POLYGON,			// 2つの三角形で四角形を作る
	//							 &g_Vertex[0],			// 頂点情報の開始アドレス
	//							 sizeof(VERTEX_2D));	// 頂点情報一つ分のサイズ
	
	//----- ポリゴン描画 -----
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}
//====================================================
// 頂点の作成
//====================================================

HRESULT MakeVertexPolygonD(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
											D3DUSAGE_WRITEONLY,			  // バッファ使用法
											FVF_VERTEX_2D,				  // 頂点フォーマット
											D3DPOOL_MANAGED,				  // 保持するメモリクラス
											&g_pVtxBuffPolygon,	  // 頂点バッファインターフェース
											NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファの内容を埋める
	{
		VERTEX_2D	*pVtx;

		//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
		//----- 頂点座標の設定 -----
		pVtx[0].pos = D3DXVECTOR3((float)(POLYGON_POS_X), (float)(POLYGON_POS_Y), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(POLYGON_POS_X + POLYGON_SIZE_X), POLYGON_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(POLYGON_POS_X), (float)(POLYGON_POS_Y + POLYGON_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(POLYGON_POS_X + POLYGON_SIZE_X), (float)(POLYGON_POS_Y + POLYGON_SIZE_Y), 0.0f);

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
		g_pVtxBuffPolygon->Unlock();
	}
	return S_OK;
}

//====================================================
// テクスチャ座標の設定
//====================================================
void SetTextureAnim(void)
{
	int nNo = GetDirectionNo();

	// 頂点バッファの内容を埋める
	{
		VERTEX_2D	*pVtx;

		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得

		//----- テクスチャ座標の算出 -----   
		switch(nNo)
		{
		case 0:g_fPosXLeft	= 0.0f;
			   g_fPosXRight = (float)(POLYGON_SIZE_X / LOGO_DEFAULT_SIZE_X);
			   g_fPosYUp	= 0.0f;
			   g_fPosYDown	= (float)(POLYGON_SIZE_Y / LOGO_DEFAULT_SIZE_Y);
			   break;

		case 1:g_fPosXLeft	= (float)((g_nPatternAnim % SPYKE_PATTERN_DIVIDE_X) * SPYKE_PATTERN_SIZE_X);
			   g_fPosXRight = (float)((g_nPatternAnim % SPYKE_PATTERN_DIVIDE_X) * SPYKE_PATTERN_SIZE_X + SPYKE_PATTERN_SIZE_X);
			   g_fPosYUp	= (float)((g_nPatternAnim / (SPYKE_PATTERN_MAX / SPYKE_PATTERN_DIVIDE_Y)) * SPYKE_PATTERN_SIZE_Y);
			   g_fPosYDown	= (float)((g_nPatternAnim / (SPYKE_PATTERN_MAX / SPYKE_PATTERN_DIVIDE_Y)) * SPYKE_PATTERN_SIZE_Y + SPYKE_PATTERN_SIZE_Y);
			   break;

		case 2:break;

		case 3:break;
		}

		//----- 頂点座標の設定 -----
		pVtx[0].pos = D3DXVECTOR3((float)(POLYGON_POS_X), (float)(POLYGON_POS_Y), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(POLYGON_POS_X + POLYGON_SIZE_X), (float)(POLYGON_POS_Y), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(POLYGON_POS_X), (float)(POLYGON_POS_Y + POLYGON_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(POLYGON_POS_X + POLYGON_SIZE_X), (float)(POLYGON_POS_Y + POLYGON_SIZE_Y), 0.0f);

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
		switch(nNo)
			{
			case 0:pVtx[0].tex = D3DXVECTOR2(g_fPosXLeft, g_fPosYUp);
				   pVtx[1].tex = D3DXVECTOR2(g_fPosXRight, g_fPosYUp);
				   pVtx[2].tex = D3DXVECTOR2(g_fPosXLeft, g_fPosYDown);
				   pVtx[3].tex = D3DXVECTOR2(g_fPosXRight, g_fPosYDown);
				   break;

			case 1:pVtx[0].tex = D3DXVECTOR2(g_fPosXLeft, g_fPosYUp);
				   pVtx[1].tex = D3DXVECTOR2(g_fPosXRight, g_fPosYUp);
				   pVtx[2].tex = D3DXVECTOR2(g_fPosXLeft, g_fPosYDown);
				   pVtx[3].tex = D3DXVECTOR2(g_fPosXRight, g_fPosYDown);
				   break;

			case 2:pVtx[0].tex = D3DXVECTOR2(g_fPosXLeftSeaO, g_fPosYUp);
				   pVtx[1].tex = D3DXVECTOR2(g_fPosXRightSeaO, g_fPosYUp);
				   pVtx[2].tex = D3DXVECTOR2(g_fPosXLeftSeaO, g_fPosYDown);
				   pVtx[3].tex = D3DXVECTOR2(g_fPosXRightSeaO, g_fPosYDown);
				   break;

			case 3:pVtx[0].tex = D3DXVECTOR2(g_fPosXLeftSeaO, g_fPosYUp);
				   pVtx[1].tex = D3DXVECTOR2(g_fPosXRightSeaO, g_fPosYUp);
				   pVtx[2].tex = D3DXVECTOR2(g_fPosXLeftSeaO, g_fPosYDown);
				   pVtx[3].tex = D3DXVECTOR2(g_fPosXRightSeaO, g_fPosYDown);
				   break;

			}

		//----- 頂点データのアンロック -----
		g_pVtxBuffPolygon->Unlock();

	}
}

//====================================================
// 演出終了フラグの共有
//====================================================
bool GetDirectionFlg (void)
{
	return g_bDirectionFlg;
}

//====================================================
// 演出終了フラグセット
//====================================================
void SetDirectionFlg (bool bFlg)
{
	g_bDirectionFlg = bFlg;
}

//====================================================
// 演出終了フラグの共有
//====================================================
int GetPatternAnim (void)
{
	return g_nPatternAnim;
}