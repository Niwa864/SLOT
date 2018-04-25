//====================================================
//
// 背景処理[bg.cpp]
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "bg.h"	//ポリゴン処理用のヘッダファイル

//****************************************************
// マクロ・定数定義
//****************************************************

// 読み込むテクスチャファイル
#define BG_TITLE	"data/TEXTURE/title_631x792.png"
#define BG_TEXTURE	"data/TEXTURE/bg_631x792.png"
#define BG_RESULT	"data/TEXTURE/resultBG_631x792.png"

#define BG_POS_X	(0)	// 表示座標
#define BG_POS_Y	(0)

#define BG_SIZE_X	(631 * 0.8)	// ポリゴンサイズ
#define BG_SIZE_Y	(792 * 0.8)

//****************************************************
// 構造体定義
//****************************************************

//****************************************************
// プロトタイプ宣言
//****************************************************
HRESULT MakeVertexBG(LPDIRECT3DDEVICE9 pDevice);

//****************************************************
// グローバル変数
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureBG = NULL; // テクスチャへのポインタ

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL; // 頂点バッファインターフェースへのポインタ

//====================================================
// 初期化処理
//====================================================
HRESULT InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // デバイス取得

	MakeVertexBG(pDevice);				 // 頂点情報の作成

	//----- テクスチャの読み込み -----
	D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
							  BG_TEXTURE,		// ファイル名
							  &g_pTextureBG);	// 読み込み

	if(MODE_TITLE == GetMode())
	{
		//----- テクスチャの読み込み -----
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
								  BG_TITLE,			// ファイル名
								  &g_pTextureBG);	// 読み込み
	}

	if(MODE_RESULT == GetMode())
	{
		//----- テクスチャの読み込み -----
		D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
								  BG_RESULT,		// ファイル名
								  &g_pTextureBG);	// 読み込み
	}

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void UninitBG(void)
{
	if(g_pTextureBG != NULL)
	{
		//----- テクスチャの解放 -----
		g_pTextureBG->Release();
		g_pTextureBG = NULL;
	}
	
	if(g_pVtxBuffBG != NULL)
	{
		//----- 頂点バッファの解放 -----
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG= NULL;
	}
}

//====================================================
// 更新処理
//====================================================
void UpdateBG(void)
{
	//
}

//====================================================
// 描画処理
//====================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);					// 頂点フォーマット設定
	
	pDevice->SetTexture(0, g_pTextureBG);		// テクスチャ設定
	
	//----- ポリゴン描画 -----
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}

//====================================================
// 頂点の作成
//====================================================

HRESULT MakeVertexBG(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // 確保するバッファサイズ
										  D3DUSAGE_WRITEONLY,			  // バッファ使用法
										  FVF_VERTEX_2D,				  // 頂点フォーマット
										  D3DPOOL_MANAGED,				  // 保持するメモリクラス
										  &g_pVtxBuffBG,			  // 頂点バッファインターフェース
										  NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファの内容を埋める
	{
		VERTEX_2D	*pVtx;

		//頂点バッファの範囲をロックし、頂点バッファへのポインタを取得
		g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
		
		//----- 頂点座標の設定 -----
		pVtx[0].pos = D3DXVECTOR3((float)(BG_POS_X),  (float)(BG_POS_Y),  0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(BG_POS_X + BG_SIZE_X), (float)(BG_POS_Y),  0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(BG_POS_X),  (float)(BG_POS_Y + BG_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(BG_POS_X + BG_SIZE_X), (float)(BG_POS_Y + BG_SIZE_Y), 0.0f);

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
		g_pVtxBuffBG->Unlock();

	}
	return S_OK;
}