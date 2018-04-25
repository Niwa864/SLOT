//====================================================
//
// �X�R�A����[score.cpp]
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "score.h"	//�|���S�������p�̃w�b�_�t�@�C��]
#include "outMedal.h"

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define POLYGON_TEXTURE	"data/TEXTURE/resultV_35x42.png"

#define POLYGON_SIZE_X	(35 * 0.8)
#define POLYGON_SIZE_Y	(42 * 0.8)

#define TEX_DEFAULT_X	(350 * 0.8)
#define TEX_DEFAULT_Y	(42 * 0.8)

#define CREDIT_DIGIT_MAX (7)
#define MONEY_DIGIT_MAX	 (7)

#define CREDIT_POS_X	(153)
#define CREDIT_POS_Y	(150)

#define MONEY_POS_X		(140)
#define MONEY_POS_Y		(252)

//#define INTERVAL_CHANGE_PATTERN	(5)	//�t���[���P��

//****************************************************
// �\���̒�`
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffScore; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	float fPosX, fPosY;
	float fPosXLeft, fPosXRight; // U���W
	float fPosYUp,   fPosYDown;	 // V���W
}value;

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

void SetTextureScore();

//****************************************************
// �O���[�o���ϐ�
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureScore = NULL; // �e�N�X�`���ւ̃|�C���^

// VERTEX_2D		   g_Vertex[NUM_VERTEX]; // ���_���i�[���[�N

//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVPolygon = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

value g_Score[CREDIT_DIGIT_MAX];
value g_Money[MONEY_DIGIT_MAX];

//====================================================
// ����������
//====================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	//----- ������ -----
	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		g_Score[i].fPosXLeft  = 0.0f;
		g_Score[i].fPosXRight = (float)(g_Score[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X);
		g_Score[i].fPosYUp    = 0.0f;
		g_Score[i].fPosYDown  = (float)(g_Score[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y);
	}

	for (int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		g_Money[i].fPosXLeft  = 0.0f;
		g_Money[i].fPosXRight = (float)(g_Money[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X);
		g_Money[i].fPosYUp    = 0.0f;
		g_Money[i].fPosYDown  = (float)(g_Money[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y);
	}

	MakeVertexScore(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
							  POLYGON_TEXTURE,		// �t�@�C����
							  &g_pTextureScore);	// �ǂݍ���
}

//====================================================
// �I������
//====================================================
void UninitScore(void)
{
	if(g_pTextureScore != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{	
		if(g_Score[i].pVtxBuffScore != NULL)
		{
			//----- ���_�o�b�t�@�̉�� -----
			g_Score[i].pVtxBuffScore->Release();
			g_Score[i].pVtxBuffScore = NULL;
		}
	}

	for (int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{	
		if(g_Money[i].pVtxBuffScore != NULL)
		{
			//----- ���_�o�b�t�@�̉�� -----
			g_Money[i].pVtxBuffScore->Release();
			g_Money[i].pVtxBuffScore = NULL;
		}
	}
}

//====================================================
// �X�V����
//====================================================
void UpdateScore(int nMedal)
{
	int nNo = 0;
	int nMoney = 0;
	int nAllMedal = 0;

	nAllMedal = nMedal + GetOutMedal();

	nMoney = (int)(nAllMedal * 0.17);
	nMoney *= 100;

	//----- �e�N�X�`�����W�̎Z�o -----
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		switch(i)
		{
			case 6:
				nNo = nAllMedal % 10;
				break;
			case 5:
				nNo = nAllMedal / 10;
				break;
			case 4:
				nNo = nAllMedal / 100;
				break;
			case 3:
				nNo = nAllMedal / 1000;
				break;
			case 2:
				nNo = nAllMedal / 10000;
				break;
			case 1:
				nNo = nAllMedal / 100000;
				break;
			case 0:
				nNo = nAllMedal / 1000000;
				break;
		}
		g_Score[i].fPosXLeft  = (float)((nNo * POLYGON_SIZE_X) / TEX_DEFAULT_X);
		g_Score[i].fPosXRight = (float)((nNo * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X);
		g_Score[i].fPosX	  = (float)(CREDIT_POS_X + (POLYGON_SIZE_X * i * 1.2));
		g_Score[i].fPosY	  = (float)(CREDIT_POS_Y);
	}

	//----- �e�N�X�`�����W�̎Z�o -----
	for(int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		switch(i)
		{
			case 6:
				nNo = nMoney % 10;
				break;
			case 5:
				nNo = nMoney / 10;
				break;
			case 4:
				nNo = nMoney / 100;
				break;
			case 3:
				nNo = nMoney / 1000;
				break;
			case 2:
				nNo = nMoney / 10000;
				break;
			case 1:
				nNo = nMoney / 100000;
				break;
			case 0:
				nNo = nMoney / 1000000;
				break;
		}
		g_Money[i].fPosXLeft  = (float)((nNo * POLYGON_SIZE_X) / TEX_DEFAULT_X);
		g_Money[i].fPosXRight = (float)((nNo * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X);
		g_Money[i].fPosX =		(float)(MONEY_POS_X + (POLYGON_SIZE_X * i * 1.2));
		g_Money[i].fPosY =		(float)(MONEY_POS_Y);

		if(i > 0)
		{
			g_Money[i].fPosX += 6;
		}
		if(i > 3)
		{
			g_Money[i].fPosX += 6;
		}
	}
	SetTextureScore();
}

//====================================================
// �`�揈��
//====================================================
void DrawScore(void)
{
	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_Score[i].pVtxBuffScore, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		pDevice->SetTexture(0, g_pTextureScore);		// �e�N�X�`���ݒ�

		//----- �|���S���`�� -----
	//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,	// �O�p�`�X�g���b�v���w��
	//							 NUM_POLYGON,			// 2�̎O�p�`�Ŏl�p�`�����
	//							 &g_Vertex[0],			// ���_���̊J�n�A�h���X
	//							 sizeof(VERTEX_2D));	// ���_������̃T�C�Y
	
		//----- �|���S���`�� -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	for (int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_Money[i].pVtxBuffScore, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		pDevice->SetTexture(0, g_pTextureScore);		// �e�N�X�`���ݒ�

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

HRESULT MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
												D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
												FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			  // �ێ����郁�����N���X
												&g_Score[i].pVtxBuffScore,	  // ���_�o�b�t�@�C���^�[�t�F�[�X
												NULL)))
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_Score[i].pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Score[i].fPosX),  (float)(g_Score[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Score[i].fPosX + POLYGON_SIZE_X), (float)(g_Score[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Score[i].fPosX),  (float)(g_Score[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Score[i].fPosX + POLYGON_SIZE_X), (float)(g_Score[i].fPosY + POLYGON_SIZE_Y), 0.0f);

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
			g_Score[i].pVtxBuffScore->Unlock();
		}
	}
	for(int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
												D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
												FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			  // �ێ����郁�����N���X
												&g_Money[i].pVtxBuffScore,	  // ���_�o�b�t�@�C���^�[�t�F�[�X
												NULL)))
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_Money[i].pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Money[i].fPosX), (float)(g_Money[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Money[i].fPosX + POLYGON_SIZE_X), (float)(g_Money[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Money[i].fPosX), (float)(g_Money[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Money[i].fPosX + POLYGON_SIZE_X), (float)(g_Money[i].fPosY + POLYGON_SIZE_Y), 0.0f);

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
			g_Money[i].pVtxBuffScore->Unlock();
		}
	}
	return S_OK;
}

//====================================================
// �e�N�X�`�����W�̐ݒ�
//====================================================
void SetTextureScore(void)
{
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			g_Score[i].pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Score[i].fPosX), (float)(g_Score[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Score[i].fPosX + POLYGON_SIZE_X), (float)(g_Score[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Score[i].fPosX), (float)(g_Score[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Score[i].fPosX + POLYGON_SIZE_X), (float)(g_Score[i].fPosY + POLYGON_SIZE_Y), 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_Score[i].fPosXLeft, g_Score[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_Score[i].fPosXRight, g_Score[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_Score[i].fPosXLeft, g_Score[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_Score[i].fPosXRight, g_Score[i].fPosYDown);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_Score[i].pVtxBuffScore->Unlock();

		}
	}

	for(int i = 0; i < MONEY_DIGIT_MAX; i ++)
	{
		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			g_Money[i].pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_Money[i].fPosX), (float)(g_Money[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_Money[i].fPosX + POLYGON_SIZE_X), (float)(g_Money[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_Money[i].fPosX), (float)(g_Money[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_Money[i].fPosX + POLYGON_SIZE_X), (float)(g_Money[i].fPosY + POLYGON_SIZE_Y), 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_Money[i].fPosXLeft,  g_Money[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_Money[i].fPosXRight, g_Money[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_Money[i].fPosXLeft,  g_Money[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_Money[i].fPosXRight, g_Money[i].fPosYDown);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_Money[i].pVtxBuffScore->Unlock();

		}
	}
}