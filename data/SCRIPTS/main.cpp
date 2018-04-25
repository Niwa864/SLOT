//====================================================
//
// ���C������[main.cpp]
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "main.h"
#include "polygon.h" //�|���S�������p�̃w�b�_
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
// �}�N���E�萔��`
//****************************************************
#define CLASS_NAME	"AppClass"					// �E�C���h�E�̃N���X��
#define WINDOW_NAME	"SLOT"					// �E�C���h�E�̃L���v�V������

#define _DEBUG_									//�f�o�b�O�X�C�b�`:�R�����g�A�E�g��FPS��\��

//****************************************************
// �\���̒�`
//****************************************************

//****************************************************
// �v���g�^�C�v�錾
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
// �O���[�o���ϐ�
//****************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)

MODE g_mode = MODE_TITLE;	//�Q�[�����[�h

#ifdef	_DEBUG_
LPD3DXFONT			g_pD3DXFont = NULL;		// �t�H���g�ւ̃|�C���^
int					g_nCountFPS = 0;		// FPS�J�E���^
#endif

//====================================================
// ���C���֐�
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

	DWORD	dwExecLastTime;	// �O��̎��s����
	DWORD	dwFPSLastTime;	// �O���FPS�v������
	DWORD	dwCurrentTime;	// ���݂̎���
	DWORD	dwFrameCount;	// �t���[���J�E���^

	//----- �E�C���h�E�N���X�̓o�^ -----
	RegisterClassEx(&wcex);

	//----- �E�C���h�E�̍쐬 -----
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

	//----- ����������(�E�C���h�E���쐬���Ă���s��) -----
	if(FAILED(Init(hInstance, hWnd, TRUE))) //����FALSE�Ńt���X�N���[�����[�h
	{
		return -1;
	}

	//----- �t���[���J�E���g������ -----
	timeBeginPeriod(1);								//����\�ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime(); //�V�X�e���������~���b�Ŏ擾

	dwCurrentTime = dwFrameCount = 0;				//�J�E���^���[���N���A

	//----- �E�C���h�E�̕\��(�����������̌�ɍs��) -----
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//----- ���b�Z�[�W���[�v -----
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// ���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
		{ 
			//----- Windows�̏��� -----
			if(msg.message == WM_QUIT)	// PostQuitMessage()���Ă΂�āAWM_QUIT���b�Z�[�W�������烋�[�v�I��
			{
				break;
			}
			else
			{
				//----- ���b�Z�[�W�̖|��Ƒ��o -----
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//----- DirectX�̏��� -----

			dwCurrentTime = timeGetTime(); // �V�X�e�������擾
			
			if((dwCurrentTime - dwFPSLastTime) >= 500) // 0.5�b���Ƃ�FPS�\�������s
			{
#ifdef	_DEBUG_
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime); // FPS�v��
#endif				
				dwFPSLastTime = dwCurrentTime; // FPS�v��������ۑ�

				dwFrameCount = 0; //�J�E���g�N���A
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000/60)) // 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ����������ۑ�

				//----- �X�V���� -----
				Update();

				//----- �`�揈�� -----
				Draw();

				dwFrameCount ++; // �J�E���g�A�b�v
			}
		}
	}

	//----- �E�C���h�E�N���X�̓o�^������ -----
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//----- �I������ -----
	Uninit();

	timeEndPeriod(1); //����\�����Ƃɖ߂�

	return (int)msg.wParam;
}

//====================================================
//�E�C���h�E�v���V�[�W��
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
			nID = MessageBox(hWnd, "�I�����܂����H", "�I��", MB_YESNO);
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
				if(!GetAllStop()) // ���[�����O�S���~�܂��Ă��Ȃ���΂͂���
				{
					break;
				}
				if(GetDirectionFlg()) // ���o���͂͂���
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
		nID = MessageBox(hWnd, "�I�����܂����H", "�I��", MB_YESNO);
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
// ����������
// hInstance : �C���X�^���X�̃n���h��
// hWnd		 : �E�C���h�E�̃n���h��
// bWindow   : �E�C���h�E���[�h�ɂ��邩�ǂ���
//====================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	//----- Direct3D�I�u�W�F�N�g�̐��� -----
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//----- ���݂̃f�B�X�v���C���[�h���擾 -----
	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//----- �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ� -----
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferWidth			= (UINT)SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight			= (UINT)SCREEN_HEIGHT;			// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.BackBufferCount			= 0;						// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// �f���M���ɓ������ăt���b�v����
	d3dpp.Windowed					= bWindow;					// �E�C���h�E���[�h
	d3dpp.EnableAutoDepthStencil	= TRUE;						// �f�v�X�o�b�t�@(Z�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// �f�v�X�o�b�t�@�Ƃ���16bit���g��

	if(bWindow)
	{// �E�C���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g(�w��ł��Ȃ��̂�0�Œ�)
		d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��(VSync��҂����ɕ`��)
	}
	else
	{// �t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g(���݂̑��x�ɍ��킹��)
		d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��(VSync��҂��ĕ`��)
	}

	//----- �f�o�C�X�I�u�W�F�N�g�̐��� -----
	// [�f�o�C�X��������]<�`��>��<���_����>���n�[�h�E�F�A�ōs��
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,						// �f�B�X�v���C�A�_�v�^
									D3DDEVTYPE_HAL,							// �f�o�C�X�^�C�v
									hWnd,									// �t�H�[�J�X����E�C���h�E�ւ̃n���h��
									D3DCREATE_HARDWARE_VERTEXPROCESSING,	// �f�o�C�X�쐬����̑g�ݍ��킹
									&d3dpp,									// �f�o�C�X�̃v���[���e�[�V�����p�����[�^
									&g_pD3DDevice)))						// �f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		// ��L�̐ݒ肪���s������
		// [�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs��
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,	
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// [�f�o�C�X�쐬����]<�`��>��<���_����>��CPU�ōs��
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_REF,	
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}
	//----- �����_�[�X�e�[�g�p�����[�^�̐ݒ� -----
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); //���ʂ�ݒ�

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	//���u�����h���s��

	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); //���\�[�X�J���[�̎w��

	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); //���f�X�e�B�l�[�V�����J���[�̎w��

	//----- �T���v���[�X�e�[�g�p�����[�^�̐ݒ� -----
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP); //�e�N�X�`��U�l�̌J��Ԃ��ݒ�

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP); //�e�N�X�`��V�l�̌J��Ԃ��ݒ�

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //�e�N�X�`���g�厞�̕⊮�ݒ�

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); //�e�N�X�`���k�����̕⊮�ݒ�

#ifdef	_DEBUG_
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,FALSE, SHIFTJIS_CHARSET,
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH, "Terminal", &g_pD3DXFont);
#endif
	// ���\���p�t�H���g�̂�ݒ�
	// pDevice		   :�f�o�C�X�ւ̃|�C���^
	// Height		   :�����̍���
	// Width		   :�����̕�
	// Weight	  	   :�t�H���g�̑���
	// MipLevels	   :�~�b�v�}�b�v���x����
	// Italic		   :�C�^���b�N�t�H���g
	// CharSet		   :�t�H���g�̕����Z�b�g
	// OutPutPrecision :�t�H���g�����̈�v���@���w��
	// Quality		   :�i��
	// PitchAndFamily  :�s�b�`�ƃt�@�~���C���f�b�N�X
	// pFacename	   :�t�H���g�̖��O��ێ����镶����
	// ppFont		   :�t�H���g�ւ̃|�C���^

	//----- �T�E���h�̏����� -----
	InitSound(hWnd);

	//----- BG�̏����� -----
	InitBG();

	//----- �|���S���̏����� -----
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
// �I������
//====================================================
void Uninit(void)
{
#ifdef	_DEBUG_
	if(g_pD3DXFont != NULL)
	{// �t�H���g�̉��
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
#endif

	if(g_pD3DDevice != NULL)
	{// �f�o�C�X�̉��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// �w�i�̏I������
	UninitBG();

	// �|���S���I������
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
// �X�V����
//====================================================
void Update(void)
{
	int nMedal = GetMedal();

	//----- �e�X�V -----
	switch(g_mode)
	{
		case MODE_TITLE:	 UpdateBG();					// �w�i�`��
							 UpdateTitle();					// �^�C�g���`��
							 break;
		case MODE_PLAY:		 UpdateBG();					// �w�i�X�V
							 UpdateValuePolygon(nMedal);	// ���l�|���X�V
							 UpdatePolygon(true, 3);		// ���[���|��
							 UpdateReelLight();
							 UpdateDirection();				// ���o�|���X�V
							 UpdateOutMedal();
							 break;
		case MODE_DIRECTION: UpdateDirection();				// ���o�|���X�V
							 UpdateReelLight();
							 break;
		case MODE_BIG:		 UpdateValuePolygon(nMedal);	// ���l�|���X�V
							 UpdatePolygon(true, 3);		// ���[���|��
							 UpdateReelLight();
							 UpdateBig();
							 UpdateOutMedal();
							 break;
		case MODE_PAUSE:	 UpdatePause();
							 break;
		case MODE_RESULT:	 UpdateBG();
							 UpdateResult();				// ���U���g�X�V
							 UpdateScore(nMedal);
							 break;
	}
}
//====================================================
// �`�揈��
//====================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	// Count   : pRects�z��ɂ����`�̐�
	// pRects  : �N���A�����`�̔z��(NULL�w��Ńr���[�|�[�g��`�S�̂��N���A)
	// Flags   :�N���A����T�[�t�F�X�������t���O(���Ȃ��Ƃ��P���g�p���Ȃ���΂Ȃ�Ȃ�)
	//		   :[D3DCLEAR_TARGET - �����_�����O�^�[�Q�b�g���N���A����Color�p�����[�^�̒l�ɂ���]
	//		   :[D3DCLEAR_ZBUFFER - �[�x(Z)�o�b�t�@���N���A����Z�p�����[�^�̒l�ɂ���]
	//		   :[D3DCLEAR_STENCIL - �X�e���V���o�b�t�@���N���A����Stencil�p�����[�^�̒l�ɂ���]
	// Color   : �T�[�t�F�X���N���A����F
	// Z 	   : �f�v�X�o�b�t�@�ɕۑ�����l
	// Stencil : �X�e���V���o�b�t�@�ɕۑ�����l(����)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//----- �e�`�� -----
		switch(g_mode)
		{
			case MODE_TITLE:	 DrawBG();				// �w�i�`��
							 	 DrawTitle();			// �^�C�g���`��
								 break;
			case MODE_PLAY:		 DrawBG();				// �w�i�`��
							 	 DrawValuePolygon();	// ���l�|���`��
								 DrawPolygon();			// ���[���|���`��
								 DrawReelLight();
								 DrawDirection();		// ���o�|���`��
								 DrawOutMedal();
								 break;
			case MODE_DIRECTION: DrawBG();				// �w�i�`��
							 	 DrawValuePolygon();	// ���l�|���`��
								 DrawPolygon();			// ���[���|���`��
								 DrawReelLight();
								 DrawDirection();		// ���o�|���`��
								 DrawOutMedal();
								 break;
			case MODE_BIG:		 DrawBG();				// �w�i�`��
								 DrawValuePolygon();	// ���l�|���X�V
								 DrawPolygon();			// ���[���|��
								 DrawReelLight();
								 DrawBig();
								 DrawOutMedal();
								 break;
			case MODE_PAUSE:	 DrawBG();				// �w�i�`��
								 DrawValuePolygon();	// ���l�|���X�V
								 DrawPolygon();			// ���[���|��
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
								 DrawResult();			// ���U���g�X�V
								 DrawScore();
								 break;
		}

		// FPS�\��
#ifdef	_DEBUG_
		DrawFPS();
#endif

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	// pSourceRect		   : �]������`
	// pDestRect	   	   : �]�����`
	// hDestWindowOverride : �]����E�C���h�E�ւ̃|�C���^
	// pDirtyRegion		   : �ʏ��NULL��
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//====================================================
// �f�o�C�X�̎擾
//====================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

#ifdef	_DEBUG_
//====================================================
// FPS�\������
//====================================================
void DrawFPS(void)
{
	RECT	rect = { 0, 0, (long)SCREEN_WIDTH, (long)SCREEN_HEIGHT};	// �\���̋�`�̈�
	char	aStr[256];													// �\��������̃o�b�t�@
	
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);			//��ʂ�FPS����`��

	g_pD3DXFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA( 0x00, 0x00, 0x00, 0xff));
		// hDC		: �f�o�C�X�R���e�L�X�g�ւ̃n���h��
		// lpString : �`�敶����ւ̃|�C���^
		// nCount	: ������
		// lpRect	: �\���p�̒����`�̈�
		// uFormat	: �e�L�X�g�̐��`���@(��ɐF)
}
#endif

//====================================================
// ���[�h�Z�b�g
//====================================================
void SetMode(MODE mode)
{
	g_mode = mode;
}

//====================================================
// ���[�h���L
//====================================================
MODE GetMode(void)
{
	return g_mode;
}