//====================================================
//
// ���U���g����[result.cpp]
//
// �`�X�V�����`
// 2016/08/08 ����J�n
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "result.h"	//�|���S�������p�̃w�b�_�t�@�C��
#include <stdio.h>

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEX_RESULT	"data/TEXTURE/result_610x480.png"
#define TEX_ARROW	"data/TEXTURE/allow_30x40.png"
#define TEX_RETRY	"data/TEXTURE/retry_300x260.png"
#define TEX_GOTITLE	"data/TEXTURE/title_300x260.png"

#define RESULT_SIZE_X	(610 * 0.8)	// �|���S���T�C�Y
#define RESULT_SIZE_Y	(480 * 0.8)
#define ARROW_SIZE_X	(30 * 0.8)	// �|���S���T�C�Y
#define ARROW_SIZE_Y	(40 * 0.8)
#define RETRY_SIZE_X		(300 * 0.8)	// �|���S���T�C�Y
#define RETRY_SIZE_Y		(260 * 0.8)
#define GOTITLE_SIZE_X		(300 * 0.8)	// �|���S���T�C�Y
#define GOTITLE_SIZE_Y		(260 * 0.8)

#define RETRY_DEFAULT_SIZE_X	(600 * 0.8)	// �e�N�X�`���̂��Ƃ��Ƃ̑傫��
#define RETRY_DEFAULT_SIZE_Y	(260 * 0.8)	
#define GOTITLE_DEFAULT_SIZE_X		(600 * 0.8)	// �e�N�X�`���̂��Ƃ��Ƃ̑傫��
#define GOTITLE_DEFAULT_SIZE_Y		(260 * 0.8)	

#define TEXTURE_MAX	(4)


//****************************************************
// �\���̒�`
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffPolygon; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 pTextureFile;	// �e�N�X�`���ւ̃|�C���^
	float fPolyPosX, fPolyPosY;
	float fPolySizeX, fPolySizeY;
	float fTexDefaultX, fTexDefaultY;
	float fPosXLeft, fPosXRight;		// U���W(0.0f ~ 1.0f)
	float fPosYUp, fPosYDown;			// V���W(0.0f ~ 1.0f)
}tex;

//****************************************************
// �񋓌^
//****************************************************

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice);

void RorGSetTexture(void);

//****************************************************
// �O���[�o���ϐ�
//****************************************************
tex g_TexR[TEXTURE_MAX];

int g_nRorG;

//====================================================
// ����������
//====================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	//----- �e�N�X�`���̏����� -----
	g_TexR[0].fPosXLeft    = 0.0f;
	g_TexR[0].fPosXRight   = 1.0f;
	g_TexR[0].fPosYUp	  = 0.0f;
	g_TexR[0].fPosYDown    = 1.0f;
	g_TexR[0].fPolySizeX   = RESULT_SIZE_X;
	g_TexR[0].fPolySizeY   = RESULT_SIZE_Y;
	g_TexR[0].fTexDefaultX = RESULT_SIZE_X;
	g_TexR[0].fTexDefaultY = RESULT_SIZE_Y;
	g_TexR[0].fPolyPosX = 8;
	g_TexR[0].fPolyPosY = 5;

	g_TexR[1].fPosXLeft    = 0.0f;
	g_TexR[1].fPosXRight   = 1.0f;
	g_TexR[1].fPosYUp	  = 0.0f;
	g_TexR[1].fPosYDown    = 1.0f;
	g_TexR[1].fPolySizeX   = ARROW_SIZE_X;
	g_TexR[1].fPolySizeY   = ARROW_SIZE_Y;
	g_TexR[1].fTexDefaultX = ARROW_SIZE_X;
	g_TexR[1].fTexDefaultY = ARROW_SIZE_Y;
	g_TexR[1].fPolyPosX = 240;
	g_TexR[1].fPolyPosY = 200;

	g_TexR[2].fPosXLeft    = (float)RETRY_SIZE_Y / RETRY_DEFAULT_SIZE_Y;
	g_TexR[2].fPosXRight   = 1.0f;
	g_TexR[2].fPosYUp	  = 0.0f;
	g_TexR[2].fPosYDown    = (float)RETRY_SIZE_Y / RETRY_DEFAULT_SIZE_Y;
	g_TexR[2].fPolySizeX   = RETRY_SIZE_X;
	g_TexR[2].fPolySizeY   = RETRY_SIZE_Y;
	g_TexR[2].fTexDefaultX = RETRY_DEFAULT_SIZE_X;
	g_TexR[2].fTexDefaultY = RETRY_DEFAULT_SIZE_Y;
	g_TexR[2].fPolyPosX = 30;
	g_TexR[2].fPolyPosY = 400;

	g_TexR[3].fPosXLeft    = 0.0f;
	g_TexR[3].fPosXRight   = (float)GOTITLE_SIZE_X / GOTITLE_DEFAULT_SIZE_X;
	g_TexR[3].fPosYUp	  = 0.0f;
	g_TexR[3].fPosYDown    = (float)GOTITLE_SIZE_Y / GOTITLE_DEFAULT_SIZE_Y;
	g_TexR[3].fPolySizeX   = GOTITLE_SIZE_X;
	g_TexR[3].fPolySizeY   = GOTITLE_SIZE_Y;
	g_TexR[3].fTexDefaultX = GOTITLE_DEFAULT_SIZE_X;
	g_TexR[3].fTexDefaultY = GOTITLE_DEFAULT_SIZE_Y;
	g_TexR[3].fPolyPosX = 260;
	g_TexR[3].fPolyPosY = 400;

	MakeVertexResult(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
								TEX_RESULT,					// �t�@�C����
								&g_TexR[0].pTextureFile);	// �ǂݍ���

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
								TEX_ARROW,					// �t�@�C����
								&g_TexR[1].pTextureFile);	// �ǂݍ���

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
								TEX_RETRY,					// �t�@�C����
								&g_TexR[2].pTextureFile);	// �ǂݍ���
	
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
								TEX_GOTITLE,					// �t�@�C����
								&g_TexR[3].pTextureFile);	// �ǂݍ���
}

//====================================================
// �I������
//====================================================
void UninitResult(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		if(g_TexR[i].pTextureFile != NULL)
		{
			//----- �e�N�X�`���̉�� -----
			g_TexR[i].pTextureFile->Release();
			g_TexR[i].pTextureFile = NULL;

			//----- ���_�o�b�t�@�̉�� -----
			g_TexR[i].pVtxBuffPolygon->Release();
			g_TexR[i].pVtxBuffPolygon = NULL;
		}
	}
}

//====================================================
// �X�V����
//====================================================
void UpdateResult(void)
{
	if(!g_nRorG)
	{
		g_TexR[2].fPosXLeft    = (float)RETRY_SIZE_X / RETRY_DEFAULT_SIZE_X;
		g_TexR[2].fPosXRight   = (float)(RETRY_SIZE_X + RETRY_SIZE_X) / RETRY_DEFAULT_SIZE_X;
		g_TexR[2].fPosYUp	  = 0.0f;
		g_TexR[2].fPosYDown    = (float)RETRY_SIZE_Y / RETRY_DEFAULT_SIZE_Y;
		g_TexR[3].fPosXLeft    = 0.0f;
		g_TexR[3].fPosXRight   = (float)RETRY_SIZE_X / RETRY_DEFAULT_SIZE_X;
		g_TexR[3].fPosYUp	  = 0.0f;
		g_TexR[3].fPosYDown    = (float)RETRY_SIZE_Y / RETRY_DEFAULT_SIZE_Y;
	}

	if(g_nRorG)
	{
		g_TexR[2].fPosXLeft    = 0.0f;
		g_TexR[2].fPosXRight   = (float)GOTITLE_SIZE_X / GOTITLE_DEFAULT_SIZE_X;
		g_TexR[2].fPosYUp	  = 0.0f;
		g_TexR[2].fPosYDown    = (float)GOTITLE_SIZE_Y / GOTITLE_DEFAULT_SIZE_Y;
		g_TexR[3].fPosXLeft    = (float)GOTITLE_SIZE_X / GOTITLE_DEFAULT_SIZE_X;
		g_TexR[3].fPosXRight   = (float)(GOTITLE_SIZE_X + GOTITLE_SIZE_X) / GOTITLE_DEFAULT_SIZE_X;
		g_TexR[3].fPosYUp	  = 0.0f;
		g_TexR[3].fPosYDown    = (float)GOTITLE_SIZE_Y / GOTITLE_DEFAULT_SIZE_Y;
	}
	RorGSetTexture();
}

//====================================================
// �`�揈��
//====================================================
void DrawResult(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_TexR[i].pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		pDevice->SetTexture(0, g_TexR[i].pTextureFile);		// �e�N�X�`���ݒ�

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

HRESULT MakeVertexResult(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
											  D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
											  FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
											  D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
											  &g_TexR[i].pVtxBuffPolygon,	  // ���_�o�b�t�@�C���^�[�t�F�[�X
											  NULL)))
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_TexR[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX, g_TexR[i].fPolyPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX + g_TexR[i].fPolySizeX, g_TexR[i].fPolyPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX, g_TexR[i].fPolyPosY + g_TexR[i].fPolySizeY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX + g_TexR[i].fPolySizeX, g_TexR[i].fPolyPosY + g_TexR[i].fPolySizeY, 0.0f);

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
			g_TexR[i].pVtxBuffPolygon->Unlock();

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
void RorGSetTexture(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			g_TexR[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX, g_TexR[i].fPolyPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX + g_TexR[i].fPolySizeX, g_TexR[i].fPolyPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX, g_TexR[i].fPolyPosY + g_TexR[i].fPolySizeY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TexR[i].fPolyPosX + g_TexR[i].fPolySizeX, g_TexR[i].fPolyPosY + g_TexR[i].fPolySizeY, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_TexR[i].fPosXLeft , g_TexR[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_TexR[i].fPosXRight, g_TexR[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_TexR[i].fPosXLeft , g_TexR[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_TexR[i].fPosXRight, g_TexR[i].fPosYDown);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_TexR[i].pVtxBuffPolygon->Unlock();

		}
	}
}

//====================================================
// �؂�ւ����L
//====================================================
int GetRorG (void)
{
	return g_nRorG;
}

//====================================================
// �؂�ւ��Z�b�g
//====================================================
void SetRorG (int nType)
{
	g_nRorG = nType;
}