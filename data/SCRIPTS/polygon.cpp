//====================================================
//
// �|���S������[polygon.cpp]
//
// �`�X�V�����`
// 2016/06/09 �^���I�ɉ񂷏����ɒl��ύX
// 2016/06/12 �����Ɖ񂷏����ɉ��P
// 2016/06/13 �V���{���ݒ�
// 2016/06/14 ��~���̃��[���␳�ǉ�
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "polygon.h"	//�|���S�������p�̃w�b�_�t�@�C��
#include <stdio.h>
#include "valuePolygon.h"
#include "direction.h"
#include "bg.h"
#include "outMedal.h"
#include "big.h"
#include "sound.h"
#include "reelLight.h"

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEX_REEL01	"data/TEXTURE/Reel01_160x208.png"
#define TEX_REEL02	"data/TEXTURE/Reel02_160x208.png"
#define TEX_REEL03	"data/TEXTURE/Reel03_160x208.png"

#define POLYGON_SIZE_X	(160 * 0.8)	// �|���S���T�C�Y
#define POLYGON_SIZE_Y	(208 * 0.8)

#define TEX_DEFAULT_SIZE_X	(160 * 0.8)	// �e�N�X�`���̂��Ƃ��Ƃ̑傫��
#define TEX_DEFAULT_SIZE_Y	(1393 * 0.8)	

#define SYMBOL_SIZE_Y	(55.7)	//�V���{������̃T�C�Y

#define INTERVAL_CHANGE_PATTERN	(1)	//�t���[���P��

#define REEL_MAX	(3)
#define REEL_PATTERN_MAX	(3)



//****************************************************
// �\���̒�`
//****************************************************

//****************************************************
// �񋓌^
//****************************************************

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice);

void ReelAnimSetTexture(void);
void CheckReel(void);

//****************************************************
// �O���[�o���ϐ�
//****************************************************

// VERTEX_2D		   g_Vertex[NUM_VERTEX]; // ���_���i�[���[�N

reel g_Reel[REEL_MAX];

int g_nDisPattern[3][3]; //   0 1 2
						 // 0 0 3 6
						 // 1 1 4 7
					 	 // 2 2 5 8

//----- �V���{���p�^�[���̏����� -----
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

// ����ȏ�ϐ����ƃI�[�o�[�t���[����

//====================================================
// ����������
//====================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	//----- ���[���̏����� -----
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

	MakeVertexPolygon(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
							  TEX_REEL01,					// �t�@�C����
							  &g_Reel[0].pTextureFile);		// �ǂݍ���

	D3DXCreateTextureFromFile(pDevice,
							  TEX_REEL02,
							  &g_Reel[1].pTextureFile);

	D3DXCreateTextureFromFile(pDevice,
							  TEX_REEL03,
							  &g_Reel[2].pTextureFile);
}

//====================================================
// �I������
//====================================================
void UninitPolygon(void)
{
	for(int i = 0; i < REEL_MAX; i ++)
	{
		if(g_Reel[i].pTextureFile != NULL)
		{
			//----- �e�N�X�`���̉�� -----
			g_Reel[i].pTextureFile->Release();
			g_Reel[i].pTextureFile = NULL;

			//----- ���_�o�b�t�@�̉�� -----
			g_Reel[i].pVtxBuffPolygon->Release();
			g_Reel[i].pVtxBuffPolygon = NULL;
		}
	}
}

//====================================================
// �X�V����
//====================================================
void UpdatePolygon(bool bGetFlg, int nReelNo)
{
	int nReviseReel;
	bool bDirectionFlg = GetDirectionFlg();
	int g_nPatternAnim = GetPatternAnim();

	//if(nReelNo < REEL_MAX)
		g_Reel[nReelNo].bAnimFlg = bGetFlg;

	//----- ���U���g�ڍs -----
	if(g_nMedal < 3)
	{
		SetMode(MODE_RESULT);
		InitBG();

		StopSound(SOUND_LABEL_PLAY);
		PlaySound(SOUND_LABEL_LOSE);
	}

	//----- ���v���C���� -----
	if(g_nPatternAnim == 2 && g_nDirectionNo == 1) //���o���I�������g�p
	{
		SetReelLightOut();
		g_nDirectionNo = 0;
		for(int i = 0; i < REEL_MAX; i ++)
		{
			UpdatePolygon(true, i);			
		}
		SetDirectionFlg(false);
	}

	if(g_Reel[0].bAnimFlg == true &&	// �S�Ẵ��[������������
	   g_Reel[1].bAnimFlg == true && 
	   g_Reel[2].bAnimFlg == true )
	{
		g_bCheckReelFlg = false;

		for(int i = 0; i < REEL_MAX; i ++)
		{
			g_Reel[i].bReviceFlg = false;
		}
	}
		
	//----- �A�j���[�V�������� -----
	for(int i = 0; i < REEL_MAX + 1; i ++) // �Ή��v�f�̎��l�ŕ␳�������� (��: 0��1)
	{
		if(g_Reel[i].nCounterAnim >= 40)
		{
			g_Reel[i].nCounterAnim = 0;
		}

		if(g_Reel[i].bAnimFlg)		//�A�j������̋�������Ƃ��̂�
		{
			g_Reel[i].nCounterAnim ++;		// �A�j���J�E���^�[�̃J�E���g�A�b�v
	
			if((g_Reel[i].nCounterAnim % INTERVAL_CHANGE_PATTERN) == 0)	// �摜�؂�ւ��^�C�~���O�̔��f
			{
				ReelAnimSetTexture();									// �e�N�X�`�����W�ݒ�

				//if(i == 1 && g_Reel[i].nCounterAnim < 20)
				//{
				//	continue;
				//}
				//else if(i == 2 && g_Reel[i].nCounterAnim < 40)
				//{
				//	continue;
				//}

			//----- ���_�����̕ۑ� -----
			g_nSaveMedal = g_nMedal;

			//----- �e�N�X�`�����W�̎Z�o -----
			g_Reel[i].fPosYUp	-= 0.02f;
			g_Reel[i].fPosYDown -= 0.02f;

			g_nMedal = g_nSaveMedal; //fPosYUp�ŃI�[�o�[�t���[���Ă�̂ł����œ��꒼��

				if(g_Reel[i].fPosYUp < -1.0f)
				{
					g_Reel[i].fPosYUp	= 0.0f;
					g_Reel[i].fPosYDown = (float)(POLYGON_SIZE_Y / TEX_DEFAULT_SIZE_Y);
				}
			}
		}
		
		else if(!g_Reel[i].bAnimFlg && g_bCheckReelFlg == false)	// Z X Ckey���������Ƃ� 
		{
			if(g_Reel[i].bReviceFlg)
			{
				continue;
			}

			g_Reel[i].nCounterAnim = 0;

			//----- �l�␳ -----
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
				nReviseReel = g_Reel[i].nNo + j; //�v���X�ɒ����Ēl�␳
				if(nReviseReel > 19)
				{
					nReviseReel -= 20;
				}
				//----- �l��� -----
				g_nDisPattern[i][j] = g_nPattern[i][nReviseReel];
			}

			//----- ���W�␳ -----
			g_Reel[i].fPosYUp = (float)((g_Reel[i].nNo * SYMBOL_SIZE_Y) / TEX_DEFAULT_SIZE_Y);
			g_Reel[i].fPosYDown = (float)(POLYGON_SIZE_Y / TEX_DEFAULT_SIZE_Y) + (g_Reel[i].fPosYUp);

			g_Reel[i].bReviceFlg = true;
		}
	}

	if(g_Reel[0].bAnimFlg == false &&	// ���[�������ׂĎ~�܂��Ă��ă`�F�b�N�����Ă��Ȃ���
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
// �`�揈��
//====================================================
void DrawPolygon(void)
{
	for(int i = 0; i < REEL_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_Reel[i].pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		pDevice->SetTexture(0, g_Reel[i].pTextureFile);		// �e�N�X�`���ݒ�

		//----- �|���S���`�� -----
		//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,	// �O�p�`�X�g���b�v���w��
		//							 NUM_POLYGON,			// 2�̎O�p�`�Ŏl�p�`�����
		//							 &g_Vertex[0],			// ���_���̊J�n�A�h���X
		//							 sizeof(VERTEX_2D));	// ���_������̃T�C�Y
	
		//----- �|���S���`�� -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//====================================================
// ���_�̍쐬
//====================================================

HRESULT MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < REEL_MAX; i ++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
											  D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
											  FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
											  D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
											  &g_Reel[i].pVtxBuffPolygon,	  // ���_�o�b�t�@�C���^�[�t�F�[�X
											  NULL)))
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_Reel[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX), (float)(g_Reel[i].fPolyPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX + POLYGON_SIZE_X), (float)(g_Reel[i].fPolyPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX), (float)(g_Reel[i].fPolyPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX + POLYGON_SIZE_X), (float)(g_Reel[i].fPolyPosY + POLYGON_SIZE_Y), 0.0f);

			//----- rhw�ݒ� -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- ���_�J���[�ݒ� -----
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//----- �e�N�X�`�����W�̐ݒ� -----
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_Reel[i].pVtxBuffPolygon->Unlock();

		}
	/*
		// (X, Y, Z)
		g_Vertex[0].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_Vertex[1].pos = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y, 0.0f);
		g_Vertex[2].pos = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);
		g_Vertex[3].pos = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);

		//----- rhw�ݒ� -----
		g_Vertex[0].rhw = 
		g_Vertex[1].rhw = 
		g_Vertex[2].rhw = 
		g_Vertex[3].rhw = 1.0f;

		//----- ���_�J���[�ݒ� -----
		g_Vertex[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_Vertex[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_Vertex[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		g_Vertex[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//----- �e�N�X�`�����W�̐ݒ� -----
		g_Vertex[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_Vertex[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		g_Vertex[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		g_Vertex[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	*/
	}
	return S_OK;
}

//====================================================
// �e�N�X�`�����W�̐ݒ�
//====================================================
void ReelAnimSetTexture(void)
{
	for(int i = 0; i < REEL_MAX; i ++)
	{
		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			g_Reel[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX), (float)(g_Reel[i].fPolyPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX + POLYGON_SIZE_X), (float)(g_Reel[i].fPolyPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX), (float)(g_Reel[i].fPolyPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Reel[i].fPolyPosX + POLYGON_SIZE_X), (float)(g_Reel[i].fPolyPosY + POLYGON_SIZE_Y), 0.0f);

			//----- rhw�ݒ� -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- ���_�J���[�ݒ� -----
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//----- �e�N�X�`�����W�̐ݒ� -----
			pVtx[0].tex = D3DXVECTOR2(g_Reel[i].fPosXLeft, g_Reel[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_Reel[i].fPosXRight, g_Reel[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_Reel[i].fPosXLeft, g_Reel[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_Reel[i].fPosXRight, g_Reel[i].fPosYDown);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_Reel[i].pVtxBuffPolygon->Unlock();

		}
	}
}

//====================================================
// �𔻒�
//====================================================
void CheckReel(void)
{
	//----- ���v���C -----
	if(g_nDisPattern[0][0] == SYMBOL_REPLAY && g_nDisPattern[1][0] == SYMBOL_REPLAY && g_nDisPattern[2][0] == SYMBOL_REPLAY ||
	   g_nDisPattern[0][1] == SYMBOL_REPLAY && g_nDisPattern[1][1] == SYMBOL_REPLAY && g_nDisPattern[2][1] == SYMBOL_REPLAY ||
	   g_nDisPattern[0][2] == SYMBOL_REPLAY && g_nDisPattern[1][2] == SYMBOL_REPLAY && g_nDisPattern[2][2] == SYMBOL_REPLAY ||
	   g_nDisPattern[0][0] == SYMBOL_REPLAY && g_nDisPattern[1][1] == SYMBOL_REPLAY && g_nDisPattern[2][2] == SYMBOL_REPLAY ||
	   g_nDisPattern[0][2] == SYMBOL_REPLAY && g_nDisPattern[1][1] == SYMBOL_REPLAY && g_nDisPattern[2][0] == SYMBOL_REPLAY )
	{
		if(GetMode() != MODE_BIG)
		{
			//----- �V���{���_�� -----
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

	//----- �`�F���[ -----
	if(g_nDisPattern[0][0] == SYMBOL_CHERRY ||
	   g_nDisPattern[0][1] == SYMBOL_CHERRY ||
	   g_nDisPattern[0][2] == SYMBOL_CHERRY)
	{
		//�`�`�`�`�`3���`�`�`�`�`

		//----- �V���{���_�� -----
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

	//----- �x�� -----
	if(g_nDisPattern[0][0] == SYMBOL_BELL && g_nDisPattern[1][0] == SYMBOL_BELL && g_nDisPattern[2][0] == SYMBOL_BELL ||
	   g_nDisPattern[0][1] == SYMBOL_BELL && g_nDisPattern[1][1] == SYMBOL_BELL && g_nDisPattern[2][1] == SYMBOL_BELL ||
	   g_nDisPattern[0][2] == SYMBOL_BELL && g_nDisPattern[1][2] == SYMBOL_BELL && g_nDisPattern[2][2] == SYMBOL_BELL ||
	   g_nDisPattern[0][0] == SYMBOL_BELL && g_nDisPattern[1][1] == SYMBOL_BELL && g_nDisPattern[2][2] == SYMBOL_BELL ||
	   g_nDisPattern[0][2] == SYMBOL_BELL && g_nDisPattern[1][1] == SYMBOL_BELL && g_nDisPattern[2][0] == SYMBOL_BELL )
	{
		//�`�`�`�`�`8���`�`�`�`�`

		//----- �V���{���_�� -----
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

	//----- �X�C�J -----
	if(g_nDisPattern[0][0] == SYMBOL_WMELON && g_nDisPattern[1][0] == SYMBOL_WMELON && g_nDisPattern[2][0] == SYMBOL_WMELON ||
	   g_nDisPattern[0][1] == SYMBOL_WMELON && g_nDisPattern[1][1] == SYMBOL_WMELON && g_nDisPattern[2][1] == SYMBOL_WMELON ||
	   g_nDisPattern[0][2] == SYMBOL_WMELON && g_nDisPattern[1][2] == SYMBOL_WMELON && g_nDisPattern[2][2] == SYMBOL_WMELON ||
	   g_nDisPattern[0][0] == SYMBOL_WMELON && g_nDisPattern[1][1] == SYMBOL_WMELON && g_nDisPattern[2][2] == SYMBOL_WMELON ||
	   g_nDisPattern[0][2] == SYMBOL_WMELON && g_nDisPattern[1][1] == SYMBOL_WMELON && g_nDisPattern[2][0] == SYMBOL_WMELON )
	{
		//�`�`�`�`�`15���`�`�`�`�`

		//----- �V���{���_�� -----
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
		//�`�`�`�`�`300��(15 * 20)�`�`�`�`�`

		//----- �V���{���_�� -----
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
			//���h�}�[�N/�V�I�J���߉��o

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
		//�`�`�`�`�`?���`�`�`�`�`

		//----- �V���{���_�� -----
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
		//�`�`�`�`�`?���`�`�`�`�`

		//----- �V���{���_�� -----
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
	//	//�`�`�`�`�`?���`�`�`�`�`
	//	ChangeMedal(50, true);
	//	//�A�I��7/�\���@�y�����o
	//}
	//if(g_nDisPattern[0][0] == SYMBOL_MARIE7 && g_nDisPattern[1][0] == SYMBOL_MARIE7 && g_nDisPattern[2][0] == SYMBOL_BAR ||
	//   g_nDisPattern[0][0] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && g_nDisPattern[2][2] == SYMBOL_BAR ||
	//   g_nDisPattern[0][1] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && g_nDisPattern[2][1] == SYMBOL_BAR ||
	//   g_nDisPattern[0][2] == SYMBOL_MARIE7 && g_nDisPattern[1][2] == SYMBOL_MARIE7 && g_nDisPattern[2][2] == SYMBOL_BAR ||
	//   g_nDisPattern[0][2] == SYMBOL_MARIE7 && g_nDisPattern[1][1] == SYMBOL_MARIE7 && g_nDisPattern[2][0] == SYMBOL_BAR)
	//{
	//	//�`�`�`�`�`?���`�`�`�`�`
	//	ChangeMedal(50, true);
	//	//�z�^��7/�\���@�y�����o
	//}
}

//====================================================
// ���_�������֐�
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
// ���_������񋤗L
//====================================================
int GetMedal (void)
{
	return g_nMedal;
}

//====================================================
// ���o���蓖�Ă̋��L
//====================================================
int	GetDirectionNo(void)
{
	return g_nDirectionNo;
}

//====================================================
// ���o���蓖�ẴZ�b�g
//====================================================
void SetDirectionNo(int nNo)
{
	g_nDirectionNo = nNo;
}

//====================================================
// ���[���S��~���ǂ����̋��L
//====================================================
bool GetAllStop(void)
{
	return g_bAllStop;
}