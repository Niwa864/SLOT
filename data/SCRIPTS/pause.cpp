//====================================================
//
// ポーズ処理[pause.cpp]
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "pause.h"

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define PAUSE_TEXTURE	"data/TEXTURE/pause_631x792.png"

#define PAUSE_POS_X	(0)	// 表示座標
#define PAUSE_POS_Y	(0)

#define PAUSE_SIZE_X	(631 * 0.8)	// ポリゴンサイズ
#define PAUSE_SIZE_Y	(792 * 0.8)

//****************************************************
// 構造体定義
//****************************************************

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);

//****************************************************
// グローバル変数
//****************************************************

LPDIRECT3DTEXTURE9 g_pTexturePause = NULL; // テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL; // 頂点バッファインターフェースへのポインタ

//====================================================
// 初期化処理
//====================================================
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	MakeVertexPause(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
							  PAUSE_TEXTURE,		// ファイル名
							  &g_pTexturePause);	// 読み込み

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void UninitPause(void)
{
	if(g_pTexturePause != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTexturePause->Release();
		g_pTexturePause = NULL;
	}
	
	if(g_pVtxBuffPause != NULL)
	{
		//----- 頂点バッファの解放 -----
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause= NULL;
	}
}

//====================================================
// 更新処理
//====================================================
void UpdatePause(void)
{
	//
}

//====================================================
// 描画処理
//====================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
	pDevice->SetTexture(0, g_pTexturePause);		// テクスチャ設定
	
	//----- ポリゴン描画 -----
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}

//====================================================
// 頂点の作成
//====================================================

HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
										  D3DUSAGE_WRITEONLY,			  // バッファ使用法
										  FVF_VERTEX_2D,				  // 頂点フォーマット
										  D3DPOOL_MANAGED,				  // 保持するメモリクラス
										  &g_pVtxBuffPause,			  // 頂点バッファインターフェース
										  NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファの内容を埋める
	{
		VERTEX_2D	*pVtx;

		//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
		
		//----- 頂点座標の設定 -----
		pVtx[0].pos = D3DXVECTOR3((float)(PAUSE_POS_X), (float)(PAUSE_POS_Y), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(PAUSE_POS_X + PAUSE_SIZE_X), (float)(PAUSE_POS_Y), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(PAUSE_POS_X), (float)(PAUSE_POS_Y + PAUSE_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(PAUSE_POS_X + PAUSE_SIZE_X), (float)(PAUSE_POS_Y + PAUSE_SIZE_Y), 0.0f);

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
		g_pVtxBuffPause->Unlock();

	}
	return S_OK;
}