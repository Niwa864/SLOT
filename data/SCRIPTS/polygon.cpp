//====================================================
//
// ポリゴン処理[polygon.cpp]
//
// ～更新履歴～
// 2016/06/09 疑似的に回す処理に値を変更
// 2016/06/12 ちゃんと回す処理に改善
// 2016/06/13 シンボル設定
// 2016/06/14 停止時のリール補正追加
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "polygon.h"	//ポリゴン処理用のヘッダファイル
#include <stdio.h>
#include "valuePolygon.h"
#include "direction.h"
#include "bg.h"
#include "outMedal.h"
#include "big.h"
#include "sound.h"
#include "reelLight.h"

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define TEX_REEL01	"data/TEXTURE/Reel01_160x208.png"
#define TEX_REEL02	"data/TEXTURE/Reel02_160x208.png"
#define TEX_REEL03	"data/TEXTURE/Reel03_160x208.png"

#define POLYGON_SIZE_X	(160 * 0.8)	// ポリゴンサイズ
#define POLYGON_SIZE_Y	(208 * 0.8)

#define TEX_DEFAULT_SIZE_X	(160 * 0.8)	// テクスチャのもともとの大きさ
#define TEX_DEFAULT_SIZE_Y	(1393 * 0.8)	

#define SYMBOL_SIZE_Y	(55.7)	//シンボル一個分のサイズ

#define INTERVAL_CHANGE_PATTERN	(1)	//フレーム単位

#define REEL_MAX	(3)
#define REEL_PATTERN_MAX	(3)



//****************************************************
// 構造体定義
//****************************************************

//****************************************************
// 列挙型
//****************************************************

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice);

void ReelAnimSetTexture(void);
void CheckReel(void);

//****************************************************
// グローバル変数
//****************************************************

// VERTEX_2D		   g_Vertex[NUM_VERTEX]; // 頂点情報格納ワーク

reel g_Reel[REEL_MAX];

int g_nDisPattern[3][3]; //   0 1 2
						 // 0 0 3 6
						 // 1 1 4 7
					 	 // 2 2 5 8

//----- シンボルパターンの初期化 -----
int	g_nPattern[3][20] = {{SYMBOL_WMELON, SYMBOL_BELL, SYMBOL_REPLAY, SYMBOL_CALLIE7,
						  SYMBOL_CHERRY, SYMBOL_CALLIE7, SYMBOL_BELL, SYMBOL_REPLAY,
						  SYMBOL_WMELON, SYMBOL_BELL, SYMBOL_MARIE7, SYMBOL_BELL,
						  SYMBOL_REPLAY, SYMBOL_WMELON, SYMBOL_CHERRY, SYMBOL_WMELON,
						  SYMBOL_BELL, SYMBOL_REPLAY, SYMBOL_BAR, SYMBOL_CHERRY},

						  {SYMBOL_REPLAY, SYMBOL_WMELON, SYMBOL_BELL, SYMBOL_CHERRY,
						  SYMBOL_CALLIE7, SYMBOL_REPLAY, SYMBOL_CHERRY, SYMBOL_WMELON,
						  SYMBOL_REPLAY, SYMBOL_BELL, SYMBOL_WMELON, SYMBOL_BAR,
						  SYMBOL_BELL, SYMBOL_CHERRY, SYMBOL_MARIE7, SYMBOL_REPLAY,
						  SYMBOL_BELL, SYMBOL_WMELON, SYMBOL_MARIE7, SYMBOL_CHERRY},

						  {SYMBOL_CHERRY, SYMBOL_REPLAY, SYMBOL_BELL, SYMBOL_CALLIE7,
						  SYMBOL_WMELON, SYMBOL_CHERRY, SYMBOL_BELL, SYMBOL_CHERRY,
						  SYMBOL_REPLAY, SYMBOL_WMELON, SYMBOL_CHERRY, SYMBOL_REPLAY,
						  SYMBOL_BELL, SYMBOL_BAR, SYMBOL_WMELON, SYMBOL_MARIE7,
						  SYMBOL_REPLAY, SYMBOL_BELL, SYMBOL_MARIE7, SYMBOL_WMELON},

};
bool g_bAllStop;

int g_nMedal;
int g_nSaveMedal;

int g_nDirectionNo;

bool g_bCheckReelFlg;

// これ以上変数作るとオーバーフローする

//====================================================
// 初期化処理
//====================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	//----- リールの初期化 -----
	for(int i = 0; i < REEL_MAX; i ++)
	{
		g_Reel[i].fPolySizeX   = (float)POLYGON_SIZE_X;
		g_Reel[i].fPolySizeY   = (float)POLYGON_SIZE_Y;
		g_Reel[i].fPosXLeft    = 0.0f;
		g_Reel[i].fPosXRight   = (float)(POLYGON_SIZE_X / TEX_DEFAULT_SIZE_X);
		g_Reel[i].fPosYUp		= 0.0f;
		g_Reel[i].fPosYDown    = (float)(POLYGON_SIZE_Y / TEX_DEFAULT_SIZE_Y);
		g_Reel[i].fTexDefaultX = (float)TEX_DEFAULT_SIZE_X;
		g_Reel[i].fTexDefaultY = (float)TEX_DEFAULT_SIZE_Y;
		g_Reel[i].nAnimFrame	= 2;
		g_Reel[i].bAnimFlg		= true;
		g_Reel[i].bReviceFlg	= false;
	}
	g_Reel[0].fPolyPosX = 43;
	g_Reel[0].fPolyPosY = 317;
	g_Reel[1].fPolyPosX = 189;
	g_Reel[1].fPolyPosY = 317;
	g_Reel[2].fPolyPosX = 334;
	g_Reel[2].fPolyPosY = 317;

	g_bAllStop = false;
	g_nMedal = 47;
	int g_nDirectionNo = 0;
	g_bCheckReelFlg = false;

	MakeVertexPolygon(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,						// デバイスへのポインタ
							  TEX_REEL01,					// ファイル名
							  &g_Reel[0].pTextureFile);		// 読み込み

	D3DXCreateTextureFromFile(pDevice,
							  TEX_REEL02,
							  &g_Reel[1].pTextureFile);

	D3DXCreateTextureFromFile(pDevice,
							  TEX_REEL03,
							  &g_Reel[2].pTextureFile);
}

//====================================================
// 終了処理
//====================================================
void UninitPolygon(void)
{
	for(int i = 0; i < REEL_MAX; i ++)
	{
		if(g_Reel[i].pTextureFile != NULL)
		{
			//----- テクスチャの解放 -----
			g_Reel[i].pTextureFile->Release();
			g_Reel[i].pTextureFile = NULL;

			//----- 頂点バッファの解放 -----
			g_Reel[i].pVtxBuffPolygon->Release();
			g_Reel[i].pVtxBuffPolygon = NULL;
		}
	}
}

//====================================================
// 更新処理
//====================================================
void UpdatePolygon(bool bGetFlg, int nReelNo)
{
	int nReviseReel;
	bool bDirectionFlg = GetDirectionFlg();
	int g_nPatternAnim = GetPatternAnim();

	//if(nReelNo < REEL_MAX)
		g_Reel[nReelNo].bAnimFlg = bGetFlg;

	//----- リザルト移行 -----
	if(g_nMedal < 3)
	{
		SetMode(MODE_RESULT);
		InitBG();

		StopSound(SOUND_LABEL_PLAY);
		PlaySound(SOUND_LABEL_LOSE);
	}

	//----- リプレイ動作 -----
	if(g_nPatternAnim == 2 && g_nDirectionNo == 1) //演出が終わったら使用
	{
		SetReelLightOut();
		g_nDirectionNo = 0;
		for(int i = 0; i < REEL_MAX; i ++)
		{
			UpdatePolygon(true, i);			
		}
		SetDirectionFlg(false);
	}

	if(g_Reel[0].bAnimFlg == true &&	// 全てのリールが動いた時
	   g_Reel[1].bAnimFlg == true && 
	   g_Reel[2].bAnimFlg == true )
	{
		g_bCheckReelFlg = false;

		for(int i = 0; i < REEL_MAX; i ++)
		{
			g_Reel[i].bReviceFlg = false;
		}
	}
		
	//----- アニメーション処理 -----
	for(int i = 0; i < REEL_MAX + 1; i ++) // 対応要素の次値で補正がかかる (例: 0→1)
	{
		if(g_Reel[i].nCounterAnim >= 40)
		{
			g_Reel[i].nCounterAnim = 0;
		}

		if(g_Reel[i].bAnimFlg)		//アニメ制御の許可があるときのみ
		{
			g_Reel[i].nCounterAnim ++;		// アニメカウンターのカウントアップ
	
			if((g_Reel[i].nCounterAnim % INTERVAL_CHANGE_PATTERN) == 0)	// 画像切り替えタイミングの判断
			{
				ReelAnimSetTexture();									// テクスチャ座標設定

				//if(i == 1 && g_Reel[i].nCounterAnim < 20)
				//{
				//	continue;
				//}
				//else if(i == 2 && g_Reel[i].nCounterAnim < 40)
				//{
				//	continue;
				//}

			//----- メダル数の保存 -----
			g_nSaveMedal = g_nMedal;

			//----- テクスチャ座標の算出 -----
			g_Reel[i].fPosYUp	-= 0.02f;
			g_Reel[i].fPosYDown -= 0.02f;

			g_nMedal = g_nSaveMedal; //fPosYUpでオーバーフローしてるのでここで入れ直し

				if(g_Reel[i].fPosYUp < -1.0f)
				{
					g_Reel[i].fPosYUp	= 0.0f;
					g_Reel[i].fPosYDown = (float)(POLYGON_SIZE_Y / TEX_DEFAULT_SIZE_Y);
				}
			}
		}
		
		else if(!g_Reel[i].bAnimFlg && g_bCheckReelFlg == false)	// Z X Ckeyを押したとき 
		{
			if(g_Reel[i].bReviceFlg)
			{
				continue;
			}

			g_Reel[i].nCounterAnim = 0;

			//----- 値補正 -----
			g_Reel[i].nNo = (int)((g_Reel[i].fPosYUp * TEX_DEFAULT_SIZE_Y) / SYMBOL_SIZE_Y); //0 ~ 19

			if(g_Reel[i].nNo < 0)
			{
				g_Reel[i].nNo += 20;
			}

			if(GetMode() == MODE_BIG)
			{
				switch(i)
				{
					case 0: if(g_Reel[i].nNo <= 7)
							{
								g_Reel[i].nNo = 7;
							}
							else if(g_Reel[i].nNo <= 12)
							{
								g_Reel[i].nNo = 12;
							}
							else if(g_Reel[i].nNo <= 14)
							{
								g_Reel[i].nNo = 14;
							}
							else if(g_Reel[i].nNo <= 19)
							{
								g_Reel[i].nNo = 19;
							}
							break;
					case 1: if(g_Reel[i].nNo <= 0 || g_Reel[i].nNo > 16)
							{
								g_Reel[i].nNo = 0;
							}
							else if(g_Reel[i].nNo <= 6)
							{
								g_Reel[i].nNo = 6;
							}
							else if(g_Reel[i].nNo <= 9)
							{
								g_Reel[i].nNo = 9;
							}
							else if(g_Reel[i].nNo <= 16)
							{
								g_Reel[i].nNo = 16;
							}
							break;
					case 2: if(g_Reel[i].nNo <= 3 || g_Reel[i].nNo > 18)
							{
								g_Reel[i].nNo = 3;
							}
							else if(g_Reel[i].nNo <= 8)
							{
								g_Reel[i].nNo = 8;
							}
							else if(g_Reel[i].nNo <= 13)
							{
								g_Reel[i].nNo = 13;
							}
							else if(g_Reel[i].nNo <= 18)
							{
								g_Reel[i].nNo = 18;
							}
							break;
				}
			}

			for(int j = 0; j < REEL_PATTERN_MAX; j ++)
			{
				nReviseReel = g_Reel[i].nNo + j; //プラスに直して値補正
				if(nReviseReel > 19)
				{
					nReviseReel -= 20;
				}
				//----- 値代入 -----
				g_nDisPattern[i][j] = g_nPattern[i][nReviseReel];
			}

			//----- 座標補正 -----
			g_Reel[i].fPosYUp = (float)((g_Reel[i].nNo * SYMBOL_SIZE_Y) / TEX_DEFAULT_SIZE_Y);
			g_Reel[i].fPosYDown = (float)(POLYGON_SIZE_Y / TEX_DEFAULT_SIZE_Y) + (g_Reel[i].fPosYUp);

			g_Reel[i].bReviceFlg = true;
		}
	}

	if(g_Reel[0].bAnimFlg == false &&	// リールがすべて止まっていてチェックをしていない時
	   g_Reel[1].bAnimFlg == false && 
	   g_Reel[2].bAnimFlg == false)
	{
		if(!g_bCheckReelFlg)
		{
			CheckReel();
			g_bCheckReelFlg = true;
			
			if(GetMode() == MODE_BIG)
			{
				ChangeBigCounter();
				InitBig();
			}
		}
		g_bAllStop = true;
	}
	else
	{
		g_bAllStop = false;
	}
}

//====================================================
// 描画処理
//====================================================
void DrawPolygon(void)
{
	for(int i = 0; i < REEL_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_Reel[i].pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
		pDevice->SetTexture(0, g_Reel[i].pTextureFile);		// テクスチャ設定

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

HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < REEL_MAX; i ++)
	{
		// オブジェクトの頂点バッファを生成
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
											  D3DUSAGE_WRITEONLY,			  // バッファ使用法
											  FVF_VERTEX_2D,				  // 頂点フォーマット
											  D3DPOOL_MANAGED,				  // 保持するメモリクラス
											  &g_Reel[i].pVtxBuffPolygon,	  // 頂点バッファインターフェース
											  NULL)))
		{
			return E_FAIL;
		}

		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
			g_Reel[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX), (float)(g_Reel[i].fPolyPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX + POLYGON_SIZE_X), (float)(g_Reel[i].fPolyPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX), (float)(g_Reel[i].fPolyPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX + POLYGON_SIZE_X), (float)(g_Reel[i].fPolyPosY + POLYGON_SIZE_Y), 0.0f);

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
			g_Reel[i].pVtxBuffPolygon->Unlock();

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
void ReelAnimSetTexture(void)
{
	for(int i = 0; i < REEL_MAX; i ++)
	{
		// 頂点バッファの内容を埋める
		{
			VERTEX_2D	*pVtx;

			g_Reel[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		
			//----- 頂点座標の設定 -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX), (float)(g_Reel[i].fPolyPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX + POLYGON_SIZE_X), (float)(g_Reel[i].fPolyPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX), (float)(g_Reel[i].fPolyPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX + POLYGON_SIZE_X), (float)(g_Reel[i].fPolyPosY + POLYGON_SIZE_Y), 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_Reel[i].fPosXLeft, g_Reel[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_Reel[i].fPosXRight, g_Reel[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_Reel[i].fPosXLeft, g_Reel[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_Reel[i].fPosXRight, g_Reel[i].fPosYDown);

			//----- 頂点データのアンロック -----
			g_Reel[i].pVtxBuffPolygon->Unlock();

		}
	}
}

//====================================================
// 役判定
//====================================================
void CheckReel(void)
{
	//----- リプレイ -----
	if(g_nDisPattern[0][0] == SYMBOL_REPLAY && g_nDisPattern[1][0] == SYMBOL_REPLAY && g_nDisPattern[2][0] == SYMBOL_REPLAY ||
	   g_nDisPattern[0][1] == SYMBOL_REPLAY && g_nDisPattern[1][1] == SYMBOL_REPLAY && g_nDisPattern[2][1] == SYMBOL_REPLAY ||
	   g_nDisPattern[0][2] == SYMBOL_REPLAY && g_nDisPattern[1][2] == SYMBOL_REPLAY && g_nDisPattern[2][2] == SYMBOL_REPLAY ||
	   g_nDisPattern[0][0] == SYMBOL_REPLAY && g_nDisPattern[1][1] == SYMBOL_REPLAY && g_nDisPattern[2][2] == SYMBOL_REPLAY ||
	   g_nDisPattern[0][2] == SYMBOL_REPLAY && g_nDisPattern[1][1] == SYMBOL_REPLAY && g_nDisPattern[2][0] == SYMBOL_REPLAY )
	{
		if(GetMode() != MODE_BIG)
		{
			//----- シンボル点灯 -----
			for(int j = 0; j < REEL_PATTERN_MAX; j ++)
			{
				if(g_nDisPattern[0][j] == SYMBOL_REPLAY && g_nDisPattern[1][j] == SYMBOL_REPLAY)
				{
					for(int k = 0; k < REEL_MAX; k ++)
					{
						SetReelLight(SYMBOL_REPLAY, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + (j * SYMBOL_SIZE_Y));
					}
					break;
				}
				else if(g_nDisPattern[0][0] == SYMBOL_REPLAY && g_nDisPattern[1][1] == SYMBOL_REPLAY)
				{
					for(int k = 0; k < REEL_MAX; k ++)
					{
						SetReelLight(SYMBOL_REPLAY, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j + k) * SYMBOL_SIZE_Y));
					}
					break;
				}
				else if(g_nDisPattern[0][2] == SYMBOL_REPLAY && g_nDisPattern[1][1] == SYMBOL_REPLAY && j == 2)
				{
					for(int k = 0; k < REEL_MAX; k ++)
					{
						SetReelLight(SYMBOL_REPLAY, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j - k) * SYMBOL_SIZE_Y));
					}
					break;
				}
			}
		SetMode(MODE_DIRECTION);
		g_nDirectionNo = 1;

		InitDirection();
		SetDirectionFlg(true);
		PlaySound(SOUND_LABEL_SE_REEL);
		}
	}

	//----- チェリー -----
	if(g_nDisPattern[0][0] == SYMBOL_CHERRY ||
	   g_nDisPattern[0][1] == SYMBOL_CHERRY ||
	   g_nDisPattern[0][2] == SYMBOL_CHERRY)
	{
		//～～～～～3枚～～～～～

		//----- シンボル点灯 -----
		for(int j = 0; j < REEL_PATTERN_MAX; j ++)
		{
			if(g_nDisPattern[0][j] == SYMBOL_CHERRY)
			{
				SetReelLight(SYMBOL_CHERRY, g_Reel[0].fPolyPosX, g_Reel[0].fPolyPosY + (j * SYMBOL_SIZE_Y));
			}
		}

		ChangeMedal(3, true);
		PlaySound(SOUND_LABEL_SE_WIN);
	}

	//----- ベル -----
	if(g_nDisPattern[0][0] == SYMBOL_BELL && g_nDisPattern[1][0] == SYMBOL_BELL && g_nDisPattern[2][0] == SYMBOL_BELL ||
	   g_nDisPattern[0][1] == SYMBOL_BELL && g_nDisPattern[1][1] == SYMBOL_BELL && g_nDisPattern[2][1] == SYMBOL_BELL ||
	   g_nDisPattern[0][2] == SYMBOL_BELL && g_nDisPattern[1][2] == SYMBOL_BELL && g_nDisPattern[2][2] == SYMBOL_BELL ||
	   g_nDisPattern[0][0] == SYMBOL_BELL && g_nDisPattern[1][1] == SYMBOL_BELL && g_nDisPattern[2][2] == SYMBOL_BELL ||
	   g_nDisPattern[0][2] == SYMBOL_BELL && g_nDisPattern[1][1] == SYMBOL_BELL && g_nDisPattern[2][0] == SYMBOL_BELL )
	{
		//～～～～～8枚～～～～～

		//----- シンボル点灯 -----
		for(int j = 0; j < REEL_PATTERN_MAX; j ++)
		{
			if(g_nDisPattern[0][j] == SYMBOL_BELL && g_nDisPattern[1][j] == SYMBOL_BELL)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_BELL, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + (j * SYMBOL_SIZE_Y));
				}
				break;
			}
			else if(g_nDisPattern[0][0] == SYMBOL_BELL && g_nDisPattern[1][1] == SYMBOL_BELL)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_BELL, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j + k) * SYMBOL_SIZE_Y));
				}
				break;
			}
			else if(g_nDisPattern[0][2] == SYMBOL_BELL && g_nDisPattern[1][1] == SYMBOL_BELL && j == 2)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_BELL, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j - k) * SYMBOL_SIZE_Y));
				}
				break;
			}
		}

		ChangeMedal(8, true);
		PlaySound(SOUND_LABEL_SE_WIN);
	}

	//----- スイカ -----
	if(g_nDisPattern[0][0] == SYMBOL_WMELON && g_nDisPattern[1][0] == SYMBOL_WMELON && g_nDisPattern[2][0] == SYMBOL_WMELON ||
	   g_nDisPattern[0][1] == SYMBOL_WMELON && g_nDisPattern[1][1] == SYMBOL_WMELON && g_nDisPattern[2][1] == SYMBOL_WMELON ||
	   g_nDisPattern[0][2] == SYMBOL_WMELON && g_nDisPattern[1][2] == SYMBOL_WMELON && g_nDisPattern[2][2] == SYMBOL_WMELON ||
	   g_nDisPattern[0][0] == SYMBOL_WMELON && g_nDisPattern[1][1] == SYMBOL_WMELON && g_nDisPattern[2][2] == SYMBOL_WMELON ||
	   g_nDisPattern[0][2] == SYMBOL_WMELON && g_nDisPattern[1][1] == SYMBOL_WMELON && g_nDisPattern[2][0] == SYMBOL_WMELON )
	{
		//～～～～～15枚～～～～～

		//----- シンボル点灯 -----
		for(int j = 0; j < REEL_PATTERN_MAX; j ++)
		{
			if(g_nDisPattern[0][j] == SYMBOL_WMELON && g_nDisPattern[1][j] == SYMBOL_WMELON)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_WMELON, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + (j * SYMBOL_SIZE_Y));
				}
				break;
			}
			else if(g_nDisPattern[0][0] == SYMBOL_WMELON && g_nDisPattern[1][1] == SYMBOL_WMELON)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_WMELON, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j + k) * SYMBOL_SIZE_Y));
				}
				break;
			}
			else if(g_nDisPattern[0][2] == SYMBOL_WMELON && g_nDisPattern[1][1] == SYMBOL_WMELON && j == 2)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_WMELON, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j - k) * SYMBOL_SIZE_Y));
				}
				break;
			}
		}

		ChangeMedal(15, true);
		PlaySound(SOUND_LABEL_SE_WIN);
	}

	//----- BIG -----
	if(g_nDisPattern[0][0] == SYMBOL_BAR && g_nDisPattern[1][0] == SYMBOL_BAR && g_nDisPattern[2][0] == SYMBOL_BAR ||
		g_nDisPattern[0][1] == SYMBOL_BAR && g_nDisPattern[1][1] == SYMBOL_BAR && g_nDisPattern[2][1] == SYMBOL_BAR ||
		g_nDisPattern[0][2] == SYMBOL_BAR && g_nDisPattern[1][2] == SYMBOL_BAR && g_nDisPattern[2][2] == SYMBOL_BAR ||
		g_nDisPattern[0][0] == SYMBOL_BAR && g_nDisPattern[1][1] == SYMBOL_BAR && g_nDisPattern[2][2] == SYMBOL_BAR ||
		g_nDisPattern[0][2] == SYMBOL_BAR && g_nDisPattern[1][1] == SYMBOL_BAR && g_nDisPattern[2][0] == SYMBOL_BAR )
	{
		//～～～～～300枚(15 * 20)～～～～～

		//----- シンボル点灯 -----
		for(int i = 0; i < REEL_MAX; i ++)
		{
			for(int j = 0; j < REEL_PATTERN_MAX; j ++)
			{
				if(g_nDisPattern[i][j] == SYMBOL_BAR)
				{
					SetReelLight(SYMBOL_BAR, g_Reel[i].fPolyPosX, g_Reel[i].fPolyPosY + (j * SYMBOL_SIZE_Y));
				}
			}
		}

		if(GetMode() != MODE_BIG)
		{
			SetMode(MODE_BIG);
			//塩辛マーク/シオカラ節演出

			StopSound(SOUND_LABEL_PLAY);
			PlaySound(SOUND_LABEL_BIG);
		}
	}

	//----- REG -----
	if(g_nDisPattern[0][0] == SYMBOL_CALLIE7 && g_nDisPattern[1][0] == SYMBOL_CALLIE7 && g_nDisPattern[2][0] == SYMBOL_CALLIE7 ||
	   g_nDisPattern[0][1] == SYMBOL_CALLIE7 && g_nDisPattern[1][1] == SYMBOL_CALLIE7 && g_nDisPattern[2][1] == SYMBOL_CALLIE7 ||
	   g_nDisPattern[0][2] == SYMBOL_CALLIE7 && g_nDisPattern[1][2] == SYMBOL_CALLIE7 && g_nDisPattern[2][2] == SYMBOL_CALLIE7 ||
	   g_nDisPattern[0][0] == SYMBOL_CALLIE7 && g_nDisPattern[1][1] == SYMBOL_CALLIE7 && g_nDisPattern[2][2] == SYMBOL_CALLIE7 ||
	   g_nDisPattern[0][2] == SYMBOL_CALLIE7 && g_nDisPattern[1][1] == SYMBOL_CALLIE7 && g_nDisPattern[2][0] == SYMBOL_CALLIE7 )
	{
		//～～～～～?枚～～～～～

		//----- シンボル点灯 -----
		for(int j = 0; j < REEL_PATTERN_MAX; j ++)
		{
			if(g_nDisPattern[0][j] == SYMBOL_CALLIE7 && g_nDisPattern[1][j] == SYMBOL_CALLIE7)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_CALLIE7, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + (j * SYMBOL_SIZE_Y));
				}
				break;
			}
			else if(g_nDisPattern[0][0] == SYMBOL_CALLIE7 && g_nDisPattern[1][1] == SYMBOL_CALLIE7)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_CALLIE7, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j + k) * SYMBOL_SIZE_Y));
				}
				break;
			}
			else if(g_nDisPattern[0][2] == SYMBOL_CALLIE7 && g_nDisPattern[1][1] == SYMBOL_CALLIE7 && j == 2)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_CALLIE7, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j - k) * SYMBOL_SIZE_Y));
				}
				break;
			}
		}

		ChangeMedal(100, true);

		SetMode(MODE_DIRECTION);
		g_nDirectionNo = 2;

		InitDirection();
		SetDirectionFlg(true);

		PlaySound(SOUND_LABEL_SE_CALLIE);
	}

	if(g_nDisPattern[0][0] == SYMBOL_MARIE7 && g_nDisPattern[1][0] == SYMBOL_MARIE7 && g_nDisPattern[2][0] == SYMBOL_MARIE7 ||
	   g_nDisPattern[0][1] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && g_nDisPattern[2][1] == SYMBOL_MARIE7 ||
	   g_nDisPattern[0][2] == SYMBOL_MARIE7 && g_nDisPattern[1][2] == SYMBOL_MARIE7 && g_nDisPattern[2][2] == SYMBOL_MARIE7 ||
	   g_nDisPattern[0][0] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && g_nDisPattern[2][2] == SYMBOL_MARIE7 ||
	   g_nDisPattern[0][2] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && g_nDisPattern[2][0] == SYMBOL_MARIE7 )
	{
		//～～～～～?枚～～～～～

		//----- シンボル点灯 -----
		for(int j = 0; j < REEL_PATTERN_MAX; j ++)
		{
			if(g_nDisPattern[0][j] == SYMBOL_MARIE7 && g_nDisPattern[1][j] == SYMBOL_MARIE7)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_MARIE7, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + (j * SYMBOL_SIZE_Y));
				}
				break;
			}
			else if(g_nDisPattern[0][0] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_MARIE7, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j + k) * SYMBOL_SIZE_Y));
				}
				break;
			}
			else if(g_nDisPattern[0][2] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && j == 2)
			{
				for(int k = 0; k < REEL_MAX; k ++)
				{
					SetReelLight(SYMBOL_MARIE7, g_Reel[0 + k].fPolyPosX, g_Reel[0 + k].fPolyPosY + ((j - k) * SYMBOL_SIZE_Y));
				}
				break;
			}
		}

		ChangeMedal(100, true);
		
		SetMode(MODE_DIRECTION);
		g_nDirectionNo = 3;

		InitDirection();
		SetDirectionFlg(true);

		PlaySound(SOUND_LABEL_SE_MARIE);
	}

	////----- MINI -----
	//if(g_nDisPattern[0][0] == SYMBOL_CALLIE7 && g_nDisPattern[1][0] == SYMBOL_CALLIE7 && g_nDisPattern[2][0] == SYMBOL_BAR ||
	//   g_nDisPattern[0][0] == SYMBOL_CALLIE7 && g_nDisPattern[1][1] == SYMBOL_CALLIE7 && g_nDisPattern[2][2] == SYMBOL_BAR ||
	//   g_nDisPattern[0][1] == SYMBOL_CALLIE7 && g_nDisPattern[1][1] == SYMBOL_CALLIE7 && g_nDisPattern[2][1] == SYMBOL_BAR ||
	//   g_nDisPattern[0][2] == SYMBOL_CALLIE7 && g_nDisPattern[1][2] == SYMBOL_CALLIE7 && g_nDisPattern[2][2] == SYMBOL_BAR ||
	//   g_nDisPattern[0][2] == SYMBOL_CALLIE7 && g_nDisPattern[1][1] == SYMBOL_CALLIE7 && g_nDisPattern[2][0] == SYMBOL_BAR)
	//{
	//	//～～～～～?枚～～～～～
	//	ChangeMedal(50, true);
	//	//アオリ7/ソロ　軽い演出
	//}
	//if(g_nDisPattern[0][0] == SYMBOL_MARIE7 && g_nDisPattern[1][0] == SYMBOL_MARIE7 && g_nDisPattern[2][0] == SYMBOL_BAR ||
	//   g_nDisPattern[0][0] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && g_nDisPattern[2][2] == SYMBOL_BAR ||
	//   g_nDisPattern[0][1] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && g_nDisPattern[2][1] == SYMBOL_BAR ||
	//   g_nDisPattern[0][2] == SYMBOL_MARIE7 && g_nDisPattern[1][2] == SYMBOL_MARIE7 && g_nDisPattern[2][2] == SYMBOL_BAR ||
	//   g_nDisPattern[0][2] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && g_nDisPattern[2][0] == SYMBOL_BAR)
	//{
	//	//～～～～～?枚～～～～～
	//	ChangeMedal(50, true);
	//	//ホタル7/ソロ　軽い演出
	//}
}

//====================================================
// メダル加減関数
//====================================================
void ChangeMedal(int nMedal,bool bPorM)
{
	if(g_nMedal < 0)
	{
		g_nMedal = 0;
		return;
	}

	if(bPorM)
	{
		g_nMedal += nMedal;
	}
	else
	{
		g_nMedal -= nMedal;
	}

	if(g_nMedal > 50)
	{
		SetOutMedal(g_nMedal - 50);
		g_nMedal = 50;
	}
}

//====================================================
// メダル数情報共有
//====================================================
int GetMedal (void)
{
	return g_nMedal;
}

//====================================================
// 演出割り当ての共有
//====================================================
int	GetDirectionNo(void)
{
	return g_nDirectionNo;
}

//====================================================
// 演出割り当てのセット
//====================================================
void SetDirectionNo(int nNo)
{
	g_nDirectionNo = nNo;
}

//====================================================
// リール全停止かどうかの共有
//====================================================
bool GetAllStop(void)
{
	return g_bAllStop;
}