//====================================================
//
// �r�b�O���o����[big.cpp]
//
// �`�X�V�����`
// 2016/08/19 ����J�n
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "big.h"
#include "sound.h"

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEX_SEAO		"data/TEXTURE/SeaO_390x291.png"
#define TEX_PLUS_PINK	"data/TEXTURE/15p_200x110.png"
#define TEX_PLUS_GREEN	"data/TEXTURE/15g_200x110.png"

#define SEAO_POS_X		(95)
#define SEAO_POS_Y		(11)
#define PLUS_POS_X		(175)
#define PLUS_POS_Y		(110)

#define SEAO_SIZE_X	(390 * 0.8)	// �|���S���T�C�Y
#define SEAO_SIZE_Y	(291 * 0.8)
#define PLUS_SIZE_X (200 * 0.8)
#define PLUS_SIZE_Y (110 * 0.8)

#define SEAO_PATTERN_DIVIDE_X	(4)	// �e�N�X�`����������(X)
#define SEAO_PATTERN_DIVIDE_Y	(1) // �e�N�X�`����������(Y)

#define SEAO_PATTERN_SIZE_X	(1.0f / SEAO_PATTERN_DIVIDE_X)	//1�p�^�[�����̃e�N�X�`���T�C�Y���Z�o
#define SEAO_PATTERN_SIZE_Y	(1.0f / SEAO_PATTERN_DIVIDE_Y)

#define SEAO_PATTERN_MAX		(SEAO_PATTERN_DIVIDE_X * SEAO_PATTERN_DIVIDE_Y)	//�A�j���[�V�����̑��p�^�[����

#define SEAO_DEFAULT_SIZE_X		(1560 * 0.8)	// �e�N�X�`���̂��Ƃ��Ƃ̑傫��
#define SEAO_DEFAULT_SIZE_Y		(291 * 0.8)		

#define INTERVAL_CHANGE_PATTERN	(12)	//�t���[���P��

#define PLUS_TEX_MAX			(2)
#define POLYGON_MAX				(2)
//#define INTERVAL_FRAME_SEAO		(2)

#define COUNT_MAX				(20)

//****************************************************
// �\���̒�`
//****************************************************
typedef struct
{
	float fPosXLeft, fPosXRight; // U���W
	float fPosYUp,   fPosYDown;	 // V���W
}pos;

//****************************************************
// �񋓌^
//****************************************************

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexBig(LPDIRECT3DDEVICE9 pDevice);

void SetTextureBig(void);

//****************************************************
// �O���[�o���ϐ�
//****************************************************
LPDIRECT3DTEXTURE9 g_pTextureBig = NULL; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBig = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlus;	  // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlus; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

int g_nCounterBig;	// �A�j���[�V�����J�E���^�[
int g_nPatternBig;	// �A�j���p�^�[��No

pos g_PolyPos[POLYGON_MAX];

int g_nBigCount;

MODE g_SaveMode;

int g_nAlpha;

//====================================================
// ����������
//====================================================
void InitBig(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	//int nSaveCount = g_nBigCount;

	//----- �J�E���^�[�̏����� -----
	//g_nCounterBig = 0;
	//g_nPatternBig = 0;

	//g_nBigCount = 0;

	//g_bAnimFlg = true;

	MakeVertexBig(pDevice);				 // ���_���̍쐬

	int nColorPG = g_nBigCount % 2;

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
							  TEX_SEAO,				// �t�@�C����
							  &g_pTextureBig);		// �ǂݍ���

	switch(nColorPG)
	{
		case 0: //----- �e�N�X�`���̓ǂݍ��� -----
				D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
										  TEX_PLUS_PINK,		// �t�@�C����
										  &g_pTexturePlus);		// �ǂݍ���---Plus�����ꏊ�g��
				break;

		case 1: //----- �e�N�X�`���̓ǂݍ��� -----
				D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
										  TEX_PLUS_GREEN,		// �t�@�C����
										  &g_pTexturePlus);		// �ǂݍ���
				break;
	}
}

//====================================================
// �I������
//====================================================
void UninitBig(void)
{

	if(g_pTextureBig != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTextureBig->Release();
		g_pTextureBig = NULL;

		//----- ���_�o�b�t�@�̉�� -----
		g_pVtxBuffBig->Release();
		g_pVtxBuffBig = NULL;
	}

	if(g_pTexturePlus != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTexturePlus->Release();
		g_pTexturePlus = NULL;

		//----- ���_�o�b�t�@�̉�� -----
		g_pVtxBuffPlus->Release();
		g_pVtxBuffPlus = NULL;
	}
}

//====================================================
// �X�V����
//====================================================
void UpdateBig(void)
{

	//if(!g_bAnimFlg)		// �A�j������̋�������Ƃ��̂�
	//{
	//	return;
	//}

	g_nCounterBig ++;	// �A�j���J�E���^�[�̃J�E���g�A�b�v

	if((g_nCounterBig % INTERVAL_CHANGE_PATTERN) == 0)				// �摜�؂�ւ��^�C�~���O�̔��f
	{		
		g_nPatternBig = (g_nPatternBig + 1) % SEAO_PATTERN_MAX;		// �p�^�[���؂�ւ�
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
// �`�揈��
//====================================================
void DrawBig(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int i = 0; i < POLYGON_MAX; i ++)
	{

		switch(i)
		{
			case 0: // ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
					pDevice->SetStreamSource(0, g_pVtxBuffBig, 0, sizeof(VERTEX_2D));
					break;
			case 1: // ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
					pDevice->SetStreamSource(0, g_pVtxBuffPlus, 0, sizeof(VERTEX_2D));
					break;
		}

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		switch(i)
		{
			case 0: pDevice->SetTexture(0, g_pTextureBig);		// �e�N�X�`���ݒ�
					break;
			case 1: pDevice->SetTexture(0, g_pTexturePlus);		// �e�N�X�`���ݒ�
					break;
		}

		//----- �|���S���`�� -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

}
//====================================================
// ���_�̍쐬
//====================================================

HRESULT MakeVertexBig(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < POLYGON_MAX; i ++)
	{
		switch(i)
		{
			case 0: // �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
					if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
															D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
															FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
															D3DPOOL_MANAGED,			  // �ێ����郁�����N���X
															&g_pVtxBuffBig,				  // ���_�o�b�t�@�C���^�[�t�F�[�X
															NULL)))
					{
						return E_FAIL;
					}
					break;
			case 1: // �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
					if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
															D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
															FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
															D3DPOOL_MANAGED,			  // �ێ����郁�����N���X
															&g_pVtxBuffPlus,			  // ���_�o�b�t�@�C���^�[�t�F�[�X
															NULL)))
					{
						return E_FAIL;
					}
					break;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			switch(i)
			{
				case 0: //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
						g_pVtxBuffBig->Lock(0, 0, (void**)&pVtx, 0);

						//----- ���_���W�̐ݒ� -----
						pVtx[0].pos = D3DXVECTOR3((float)(SEAO_POS_X), (float)(SEAO_POS_Y), 0.0f);
						pVtx[1].pos = D3DXVECTOR3((float)(SEAO_POS_X + SEAO_SIZE_X), (float)(SEAO_POS_Y), 0.0f);
						pVtx[2].pos = D3DXVECTOR3((float)(SEAO_POS_X), (float)(SEAO_POS_Y + SEAO_SIZE_Y), 0.0f);
						pVtx[3].pos = D3DXVECTOR3((float)(SEAO_POS_X + SEAO_SIZE_X), (float)(SEAO_POS_Y + SEAO_SIZE_Y), 0.0f);
						break;
				case 1: //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
						g_pVtxBuffPlus->Lock(0, 0, (void**)&pVtx, 0);

						//----- ���_���W�̐ݒ� -----
						pVtx[0].pos = D3DXVECTOR3((float)(PLUS_POS_X), (float)(PLUS_POS_Y), 0.0f);
						pVtx[1].pos = D3DXVECTOR3((float)(PLUS_POS_X + PLUS_SIZE_X), (float)(PLUS_POS_Y), 0.0f);
						pVtx[2].pos = D3DXVECTOR3((float)(PLUS_POS_X), (float)(PLUS_POS_Y + PLUS_SIZE_Y), 0.0f);
						pVtx[3].pos = D3DXVECTOR3((float)(PLUS_POS_X + PLUS_SIZE_X), (float)(PLUS_POS_Y + PLUS_SIZE_Y), 0.0f);
						break;
			}

			//----- rhw�ݒ� -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- ���_�J���[�ݒ� -----
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

			//----- �e�N�X�`�����W�̐ݒ� -----
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			switch(i)
			{
				case 0: //----- ���_�f�[�^�̃A�����b�N -----
						g_pVtxBuffBig->Unlock();
						break;
				case 1: //----- ���_�f�[�^�̃A�����b�N -----
						g_pVtxBuffBig->Unlock();
						break;
			}
		}
	}
	return S_OK;
}

//====================================================
// �e�N�X�`�����W�̐ݒ�
//====================================================
void SetTextureBig(void)
{

	// ���_�o�b�t�@�̓��e�𖄂߂�
	{
		VERTEX_2D	*pVtx;

		for(int i = 0; i < POLYGON_MAX; i ++)
		{
			switch(i)
			{
			case 0: g_pVtxBuffBig->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾

					//----- �e�N�X�`�����W�̎Z�o -----   
					g_PolyPos[i].fPosXLeft	= (float)(g_nPatternBig % SEAO_PATTERN_DIVIDE_X) * SEAO_PATTERN_SIZE_X;
					g_PolyPos[i].fPosXRight = (float)(g_nPatternBig % SEAO_PATTERN_DIVIDE_X) * SEAO_PATTERN_SIZE_X + SEAO_PATTERN_SIZE_X;
					g_PolyPos[i].fPosYUp	= (float)(g_nPatternBig / (SEAO_PATTERN_MAX / SEAO_PATTERN_DIVIDE_Y)) * SEAO_PATTERN_SIZE_Y;
					g_PolyPos[i].fPosYDown	= (float)(g_nPatternBig / (SEAO_PATTERN_MAX / SEAO_PATTERN_DIVIDE_Y)) * SEAO_PATTERN_SIZE_Y + SEAO_PATTERN_SIZE_Y;

					//----- ���_���W�̐ݒ� -----
					pVtx[0].pos = D3DXVECTOR3((float)(SEAO_POS_X), (float)(SEAO_POS_Y), 0.0f);
					pVtx[1].pos = D3DXVECTOR3((float)(SEAO_POS_X + SEAO_SIZE_X), (float)(SEAO_POS_Y), 0.0f);
					pVtx[2].pos = D3DXVECTOR3((float)(SEAO_POS_X), (float)(SEAO_POS_Y + SEAO_SIZE_Y), 0.0f);
					pVtx[3].pos = D3DXVECTOR3((float)(SEAO_POS_X + SEAO_SIZE_X), (float)(SEAO_POS_Y + SEAO_SIZE_Y), 0.0f);
					break;

			case 1: g_pVtxBuffPlus->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾

					//----- �e�N�X�`�����W�̎Z�o -----   
					g_PolyPos[i].fPosXLeft	= 0.0f;
					g_PolyPos[i].fPosXRight = (float)PLUS_SIZE_X / PLUS_SIZE_X;
					g_PolyPos[i].fPosYUp	= 0.0f;
					g_PolyPos[i].fPosYDown	= (float)PLUS_SIZE_Y / PLUS_SIZE_Y;

					//----- ���_���W�̐ݒ� -----
					pVtx[0].pos = D3DXVECTOR3(PLUS_POS_X, PLUS_POS_Y, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(PLUS_POS_X + PLUS_SIZE_X, PLUS_POS_Y, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(PLUS_POS_X, PLUS_POS_Y + PLUS_SIZE_Y, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(PLUS_POS_X + PLUS_SIZE_X, PLUS_POS_Y + PLUS_SIZE_Y, 0.0f);
					break;
			}


			//----- rhw�ݒ� -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- ���_�J���[�ݒ� -----
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

			//----- �e�N�X�`�����W�̐ݒ� -----
			pVtx[0].tex = D3DXVECTOR2(g_PolyPos[i].fPosXLeft,  g_PolyPos[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_PolyPos[i].fPosXRight, g_PolyPos[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_PolyPos[i].fPosXLeft,  g_PolyPos[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_PolyPos[i].fPosXRight, g_PolyPos[i].fPosYDown);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_pVtxBuffBig->Unlock();

		}

	}
}

//====================================================
// Big�J�E���^�[���Z�֐�
//====================================================
void ChangeBigCounter(void)
{
	g_nBigCount ++;
}

//====================================================
// Big�J�E���^�[���L
//====================================================
int GetBigCount(void)
{
	return g_nBigCount;
}


//====================================================
// ���[�h�Z�[�u(�|�[�Y��A�p)
//====================================================
void SaveMode (MODE Mode)
{
	g_SaveMode = Mode;
}

//====================================================
// ���[�h���[�h
//====================================================
MODE LoadMode (void)
{
	return g_SaveMode;
}

//====================================================
// �l�������p
//====================================================
void InitBigValue (void)
{
	g_nBigCount = 0;
	g_nCounterBig = 0;
	g_nPatternBig = 0;
	g_nAlpha = 255;
}

