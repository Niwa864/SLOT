//====================================================
//
// ポリゴン処理[polygon.h]
//
//====================================================
#ifndef ___POLYGON_H___
#define ___POLYGON_H___

//****************************************************
// インクルード部
//****************************************************
#include "main.h"

//****************************************************
// マクロ・定数定義
//****************************************************

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
	int nAnimFrame;
	int nCounterAnim;					// アニメーションカウンター
	int nNo;							// case座標補正と代入用
	bool bAnimFlg;						// アニメーション許可するかどうか
	bool bReviceFlg;
}reel;

//****************************************************
// プロトタイプ宣言
//****************************************************
void InitPolygon(void);		// ポリゴン初期化
void UninitPolygon(void);	// ポリゴン終了
void UpdatePolygon(bool, int);	// ポリゴン更新
void DrawPolygon(void);		// ポリゴン描画

void ChangeMedal(int ,bool);

int GetMedal (void);	// メダル数共有
int	GetDirectionNo(void);
void SetDirectionNo(int);
bool GetAllStop(void);

//****************************************************
// 列挙型
//****************************************************
enum
{
	 SYMBOL_CALLIE7,
	 SYMBOL_MARIE7,
	 SYMBOL_BAR,
	 SYMBOL_BELL,
	 SYMBOL_WMELON,
	 SYMBOL_CHERRY,
	 SYMBOL_REPLAY,
	 SYMBOL_MAX
};

#endif