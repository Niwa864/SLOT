//====================================================
//
// メイン処理[main.cpp]
//
//====================================================

//****************************************************
// インクルード部
//****************************************************
#include "main.h"
#include "polygon.h" //ポリゴン処理用のヘッダ
#include "bg.h"
#include "valuePolygon.h"
#include "direction.h"
#include "title.h"
#include "result.h"
#include "score.h"
#include "outMedal.h"
#include "big.h"
#include "sound.h"
#include "pause.h"
#include "reelLight.h"

//****************************************************
// マクロ・定数定義
//****************************************************
#define CLASS_NAME	"AppClass"					// ウインドウのクラス名
#define WINDOW_NAME	"SLOT"					// ウインドウのキャプション名

#define _DEBUG_									//デバッグスイッチ:コメントアウトでFPS非表示

//****************************************************
// 構造体定義
//****************************************************

//****************************************************
// プロトタイプ宣言
//****************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

#ifdef	_DEBUG_
void DrawFPS(void);
#endif

//****************************************************
// グローバル変数
//****************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)

MODE g_mode = MODE_TITLE;	//ゲームモード

#ifdef	_DEBUG_
LPD3DXFONT			g_pD3DXFont = NULL;		// フォントへのポインタ
int					g_nCountFPS = 0;		// FPSカウンタ
#endif

//====================================================
// メイン関数
//====================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCMdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;

	DWORD	dwExecLastTime;	// 前回の実行時刻
	DWORD	dwFPSLastTime;	// 前回のFPS計測時刻
	DWORD	dwCurrentTime;	// 現在の時刻
	DWORD	dwFrameCount;	// フレームカウンタ

	//----- ウインドウクラスの登録 -----
	RegisterClassEx(&wcex);

	//----- ウインドウの作成 -----
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(int)(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(int)(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL);

	//----- 初期化処理(ウインドウを作成してから行う) -----
	if(FAILED(Init(hInstance, hWnd, TRUE))) //ここFALSEでフルスクリーンモード
	{
		return -1;
	}

	//----- フレームカウント初期化 -----
	timeBeginPeriod(1);								//分解能設定
	dwExecLastTime = dwFPSLastTime = timeGetTime(); //システム時刻をミリ秒で取得

	dwCurrentTime = dwFrameCount = 0;				//カウンタをゼロクリア

	//----- ウインドウの表示(初期化処理の後に行う) -----
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//----- メッセージループ -----
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// メッセージを取得しなかった場合"0"を返す
		{ 
			//----- Windowsの処理 -----
			if(msg.message == WM_QUIT)	// PostQuitMessage()が呼ばれて、WM_QUITメッセージが来たらループ終了
			{
				break;
			}
			else
			{
				//----- メッセージの翻訳と送出 -----
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//----- DirectXの処理 -----

			dwCurrentTime = timeGetTime(); // システム時刻取得
			
			if((dwCurrentTime - dwFPSLastTime) >= 500) // 0.5秒ごとにFPS表示を実行
			{
#ifdef	_DEBUG_
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime); // FPS計測
#endif				
				dwFPSLastTime = dwCurrentTime; // FPS計測時刻を保存

				dwFrameCount = 0; //カウントクリア
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000/60)) // 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理時刻を保存

				//----- 更新処理 -----
				Update();

				//----- 描画処理 -----
				Draw();

				dwFrameCount ++; // カウントアップ
			}
		}
	}

	//----- ウインドウクラスの登録を解除 -----
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//----- 終了処理 -----
	Uninit();

	timeEndPeriod(1); //分解能をもとに戻す

	return (int)msg.wParam;
}

//====================================================
//ウインドウプロシージャ
//====================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了", MB_YESNO);
			if(nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		case 0x5A:	// Zkey
			if(GetAllStop() == false && (GetMode() == MODE_PLAY ||
										 GetMode() == MODE_BIG))
			{
				UpdatePolygon(false, 0);
				DrawPolygon();
				PlaySound(SOUND_LABEL_SE_STOP);
			}
			break;
		case 0x58:	// Xkey
			if(GetAllStop() == false && (GetMode() == MODE_PLAY ||
										 GetMode() == MODE_BIG))
			{
				UpdatePolygon(false, 1);
				DrawPolygon();
				PlaySound(SOUND_LABEL_SE_STOP);
			}
			break;
		case 0x43:	// Ckey
			if(GetAllStop() == false && (GetMode() == MODE_PLAY ||
										 GetMode() == MODE_BIG))
			{
				UpdatePolygon(false, 2);
				DrawPolygon();
				PlaySound(SOUND_LABEL_SE_STOP);
			}
			break;
		case VK_LEFT:
			if(GetMode() == MODE_TITLE)
			{
				SetSorE(0);
			}
			if(GetMode() == MODE_RESULT)
			{
				SetRorG(0);
			}
			break;
		case VK_SPACE:
			if(GetMode() == MODE_PLAY ||
			   GetMode() == MODE_BIG  ||
			   GetMode() == MODE_PAUSE)
			{
				SetMode(MODE_RESULT);
				InitBG();
				InitResult();
				InitScore();

				StopSound(SOUND_LABEL_PLAY);
				StopSound(SOUND_LABEL_BIG);

				if(60 < GetMedal() + GetOutMedal())
				{
					PlaySound(SOUND_LABEL_WIN);
				}
				else
				{
					PlaySound(SOUND_LABEL_LOSE);
				}
			}
			break;
		case VK_RIGHT:
			if(GetMode() == MODE_TITLE)
			{
				SetSorE(1);
			}
			if(GetMode() == MODE_RESULT)
			{
				SetRorG(1);
			}
			break;
		case VK_SHIFT:
			if(GetMode() == MODE_PLAY || 
			   GetMode() == MODE_DIRECTION ||
			   GetMode() == MODE_BIG)
			{
				SaveMode(GetMode());
				SetMode(MODE_PAUSE);
			}
			else if(GetMode() == MODE_PAUSE)
			{
				SetMode(LoadMode());
			}
			break;
		case VK_RETURN:	//Enterkey
			if(GetMode() == MODE_TITLE)
			{
				if(0 == GetSorE())
				{
					SetMode(MODE_PLAY);
					InitBG();
					InitPolygon();
					InitDirection();
					InitBig();
					InitBigValue();
					InitValuePolygon();
					InitOutMedal();

					PlaySound(SOUND_LABEL_PLAY);
				}

				if(1 == GetSorE())
				{
					DestroyWindow(hWnd);
				}

				StopSound(SOUND_LABEL_TITLE);
			}
			if(GetMode() == MODE_RESULT)
			{
				if(0 == GetRorG())
				{
					SetMode(MODE_PLAY);
					InitBG();
					InitPolygon();
					InitDirection();
					InitBig();
					InitBigValue();
					InitValuePolygon();
					InitOutMedal();

					PlaySound(SOUND_LABEL_PLAY);
				}

				if(1 == GetRorG())
				{
					SetMode(MODE_TITLE);
					InitBG();
					InitTitle();
				}
				StopSound(SOUND_LABEL_LOSE);
				StopSound(SOUND_LABEL_WIN);
			}

			if(GetMode() == MODE_PLAY ||
			   GetMode() == MODE_BIG)
			{
				if(!GetAllStop()) // リールが三つ全部止まっていなければはじく
				{
					break;
				}
				if(GetDirectionFlg()) // 演出中ははじく
				{
					break;
				}
			
				for(int i = 0; i < 3; i ++)
				{
					UpdatePolygon(true, i);
				}
				ChangeMedal(3, false);
				SetReelLightOut();
				PlaySound(SOUND_LABEL_SE_REEL);
			}
		}
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了", MB_YESNO);
		if(nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//====================================================
// 初期化処理
// hInstance : インスタンスのハンドル
// hWnd		 : ウインドウのハンドル
// bWindow   : ウインドウモードにするかどうか
//====================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	//----- Direct3Dオブジェクトの生成 -----
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//----- 現在のディスプレイモードを取得 -----
	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//----- デバイスのプレゼンテーションパラメータの設定 -----
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferWidth			= (UINT)SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= (UINT)SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定
	d3dpp.BackBufferCount			= 0;						// バックバッファの数
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウインドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ(Zバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウインドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート(指定できないので0固定)
		d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル(VSyncを待たずに描画)
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート(現在の速度に合わせる)
		d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル(VSyncを待って描画)
	}

	//----- デバイスオブジェクトの生成 -----
	// [デバイス生成制御]<描画>と<頂点処理>をハードウェアで行う
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,						// ディスプレイアダプタ
									D3DDEVTYPE_HAL,							// デバイスタイプ
									hWnd,									// フォーカスするウインドウへのハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,	// デバイス作成制御の組み合わせ
									&d3dpp,									// デバイスのプレゼンテーションパラメータ
									&g_pD3DDevice)))						// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行う
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,	
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行う
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_REF,	
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}
	//----- レンダーステートパラメータの設定 -----
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); //裏面を設定

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//αブレンドを行う

	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); //αソースカラーの指定

	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //αデスティネーションカラーの指定

	//----- サンプラーステートパラメータの設定 -----
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP); //テクスチャU値の繰り返し設定

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP); //テクスチャV値の繰り返し設定

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //テクスチャ拡大時の補完設定

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); //テクスチャ縮小時の補完設定

#ifdef	_DEBUG_
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH, "Terminal", &g_pD3DXFont);
#endif
	// 情報表示用フォントのを設定
	// pDevice		   :デバイスへのポインタ
	// Height		   :文字の高さ
	// Width		   :文字の幅
	// Weight	  	   :フォントの太さ
	// MipLevels	   :ミップマップレベル数
	// Italic		   :イタリックフォント
	// CharSet		   :フォントの文字セット
	// OutPutPrecision :フォント特性の一致方法を指定
	// Quality		   :品質
	// PitchAndFamily  :ピッチとファミリインデックス
	// pFacename	   :フォントの名前を保持する文字列
	// ppFont		   :フォントへのポインタ

	//----- サウンドの初期化 -----
	InitSound(hWnd);

	//----- BGの初期化 -----
	InitBG();

	//----- ポリゴンの初期化 -----
	InitTitle();
	InitPolygon();
	InitReelLight();
	InitValuePolygon();
	InitDirection();
	InitBig();
	InitBigValue();
	InitOutMedal();
	InitPause();
	InitResult();
	InitScore();

	return S_OK;
}

//====================================================
// 終了処理
//====================================================
void Uninit(void)
{
#ifdef	_DEBUG_
	if(g_pD3DXFont != NULL)
	{// フォントの解放
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
#endif

	if(g_pD3DDevice != NULL)
	{// デバイスの解放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// 背景の終了処理
	UninitBG();

	// ポリゴン終了処理
	UninitTitle();
	UninitPolygon();
	UninitReelLight();
	UninitValuePolygon();
	UninitDirection();
	UninitBig();
	UninitOutMedal();
	UninitPause();
	UninitResult();
	UninitScore();
	UninitSound();
}

//====================================================
// 更新処理
//====================================================
void Update(void)
{
	int nMedal = GetMedal();

	//----- 各更新 -----
	switch(g_mode)
	{
		case MODE_TITLE:	 UpdateBG();					// 背景描画
							 UpdateTitle();					// タイトル描画
							 break;
		case MODE_PLAY:		 UpdateBG();					// 背景更新
							 UpdateValuePolygon(nMedal);	// 数値ポリ更新
							 UpdatePolygon(true, 3);		// リールポリ
							 UpdateReelLight();
							 UpdateDirection();				// 演出ポリ更新
							 UpdateOutMedal();
							 break;
		case MODE_DIRECTION: UpdateDirection();				// 演出ポリ更新
							 UpdateReelLight();
							 break;
		case MODE_BIG:		 UpdateValuePolygon(nMedal);	// 数値ポリ更新
							 UpdatePolygon(true, 3);		// リールポリ
							 UpdateReelLight();
							 UpdateBig();
							 UpdateOutMedal();
							 break;
		case MODE_PAUSE:	 UpdatePause();
							 break;
		case MODE_RESULT:	 UpdateBG();
							 UpdateResult();				// リザルト更新
							 UpdateScore(nMedal);
							 break;
	}
}
//====================================================
// 描画処理
//====================================================
void Draw(void)
{
	// バックバッファ＆Zバッファのクリア
	// Count   : pRects配列にある矩形の数
	// pRects  : クリアする矩形の配列(NULL指定でビューポート矩形全体をクリア)
	// Flags   :クリアするサーフェスを示すフラグ(少なくとも１つを使用しなければならない)
	//		   :[D3DCLEAR_TARGET - レンダリングターゲットをクリアしてColorパラメータの値にする]
	//		   :[D3DCLEAR_ZBUFFER - 深度(Z)バッファをクリアしてZパラメータの値にする]
	//		   :[D3DCLEAR_STENCIL - ステンシルバッファをクリアしてStencilパラメータの値にする]
	// Color   : サーフェスをクリアする色
	// Z 	   : デプスバッファに保存する値
	// Stencil : ステンシルバッファに保存する値(整数)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//----- 各描画 -----
		switch(g_mode)
		{
			case MODE_TITLE:	 DrawBG();				// 背景描画
							 	 DrawTitle();			// タイトル描画
								 break;
			case MODE_PLAY:		 DrawBG();				// 背景描画
							 	 DrawValuePolygon();	// 数値ポリ描画
								 DrawPolygon();			// リールポリ描画
								 DrawReelLight();
								 DrawDirection();		// 演出ポリ描画
								 DrawOutMedal();
								 break;
			case MODE_DIRECTION: DrawBG();				// 背景描画
							 	 DrawValuePolygon();	// 数値ポリ描画
								 DrawPolygon();			// リールポリ描画
								 DrawReelLight();
								 DrawDirection();		// 演出ポリ描画
								 DrawOutMedal();
								 break;
			case MODE_BIG:		 DrawBG();				// 背景描画
								 DrawValuePolygon();	// 数値ポリ更新
								 DrawPolygon();			// リールポリ
								 DrawReelLight();
								 DrawBig();
								 DrawOutMedal();
								 break;
			case MODE_PAUSE:	 DrawBG();				// 背景描画
								 DrawValuePolygon();	// 数値ポリ更新
								 DrawPolygon();			// リールポリ
								 DrawReelLight();
								 if(LoadMode() == MODE_BIG)
								 {
									 DrawBig();
								 }
								 else
								 {
									DrawDirection();
								 }
								 DrawOutMedal();
								 DrawPause();
								 break;
			case MODE_RESULT:	 DrawBG();
								 DrawResult();			// リザルト更新
								 DrawScore();
								 break;
		}

		// FPS表示
#ifdef	_DEBUG_
		DrawFPS();
#endif

		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	// pSourceRect		   : 転送元矩形
	// pDestRect	   	   : 転送先矩形
	// hDestWindowOverride : 転送先ウインドウへのポインタ
	// pDirtyRegion		   : 通常はNULLで
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//====================================================
// デバイスの取得
//====================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

#ifdef	_DEBUG_
//====================================================
// FPS表示処理
//====================================================
void DrawFPS(void)
{
	RECT	rect = { 0, 0, (long)SCREEN_WIDTH, (long)SCREEN_HEIGHT};	// 表示の矩形領域
	char	aStr[256];													// 表示文字列のバッファ
	
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);			//画面にFPS情報を描画

	g_pD3DXFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA( 0x00, 0x00, 0x00, 0xff));
		// hDC		: デバイスコンテキストへのハンドル
		// lpString : 描画文字列へのポインタ
		// nCount	: 文字数
		// lpRect	: 表示用の長方形領域
		// uFormat	: テキストの整形方法(主に色)
}
#endif

//====================================================
// モードセット
//====================================================
void SetMode(MODE mode)
{
	g_mode = mode;
}

//====================================================
// モード共有
//====================================================
MODE GetMode(void)
{
	return g_mode;
}