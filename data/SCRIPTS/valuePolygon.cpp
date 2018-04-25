//====================================================
//
// �����|���S������[valuePolygon.cpp]
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "valuePolygon.h"	//�|���S�������p�̃w�b�_�t�@�C��
#include "big.h"

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define POLYGON_TEXTURE	"data/TEXTURE/value_190x26.png"

#define POLYGON_SIZE_X	(19)
#define POLYGON_SIZE_Y	(26)

#define TEX_DEFAULT_X	(190)
#define TEX_DEFAULT_Y	(26)

#define CREDIT_DIGIT_MAX (2)
#define COUNT_DIGIT_MAX	 (2)

#define TENS_POS_X	(82)
#define TENS_POS_Y	(503)

#define TENS_COUNT_POS_X	(214)
#define TENS_COUNT_POS_Y	(503)

//#define INTERVAL_CHANGE_PATTERN	(5)	//�t���[���P��

//****************************************************
// �\���̒�`
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffVPolygon; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	float fPosX, fPosY;
	float fPosXLeft, fPosXRight; // U���W
	float fPosYUp,   fPosYDown;	 // V���W
}value;

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexVPolygon(LPDIRECT3DDEVICE9 pDevice);

void SetTextureVPolygon();

//****************************************************
// �O���[�o���ϐ�
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureVPolygon = NULL; // �e�N�X�`���ւ̃|�C���^

// VERTEX_2D		   g_Vertex[NUM_VERTEX]; // ���_���i�[���[�N

//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVPolygon = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

value g_Credit[2];
value g_BigCount[2];

//====================================================
// ����������
//====================================================
void InitValuePolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	//----- ������ -----
	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		g_Credit[i].fPosXLeft = 0.0f;
		g_Credit[i].fPosXRight = g_Credit[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X;
		g_Credit[i].fPosYUp = 0.0f;
		g_Credit[i].fPosYDown = g_Credit[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y;

		g_BigCount[i].fPosXLeft = 0.0f;
		g_BigCount[i].fPosXRight = g_BigCount[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X;
		g_BigCount[i].fPosYUp = 0.0f;
		g_BigCount[i].fPosYDown = g_BigCount[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y;
	}

	MakeVertexVPolygon(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
							  POLYGON_TEXTURE,		// �t�@�C����
							  &g_pTextureVPolygon);	// �ǂݍ���
}

//====================================================
// �I������
//====================================================
void UninitValuePolygon(void)
{
	if(g_pTextureVPolygon != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTextureVPolygon->Release();
		g_pTextureVPolygon = NULL;
	}

	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{	
		if(g_Credit[i].pVtxBuffVPolygon != NULL)
		{
			//----- ���_�o�b�t�@�̉�� -----
			g_Credit[i].pVtxBuffVPolygon->Release();
			g_Credit[i].pVtxBuffVPolygon = NULL;
		}

		if(g_BigCount[i].pVtxBuffVPolygon != NULL)
		{
			//----- ���_�o�b�t�@�̉�� -----
			g_BigCount[i].pVtxBuffVPolygon->Release();
			g_BigCount[i].pVtxBuffVPolygon = NULL;
		}
	}
}

//====================================================
// �X�V����
//====================================================
void UpdateValuePolygon(int nMedal)
{
	int nTens, nOnes;
	int nBigTens, nBigOnes;

	//----- �e�N�X�`�����W�̎Z�o -----						
	nTens = nMedal / 10;
	nOnes = nMedal % 10;

	nBigTens = GetBigCount() / 10;
	nBigOnes = GetBigCount() % 10;

	g_Credit[1].fPosXLeft = (float)(nTens * POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_Credit[1].fPosXRight = (float)(nTens * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_Credit[1].fPosX = TENS_POS_X;	//�\�̈ʂ̍��W
	g_Credit[1].fPosY = TENS_POS_Y;
	g_BigCount[1].fPosXLeft = (float)(nBigTens * POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_BigCount[1].fPosXRight = (float)(nBigTens * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_BigCount[1].fPosX = TENS_COUNT_POS_X;	//�\�̈ʂ̍��W
	g_BigCount[1].fPosY = TENS_COUNT_POS_Y;

	g_Credit[0].fPosXLeft = (float)(nOnes * POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_Credit[0].fPosXRight = (float)(nOnes * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_Credit[0].fPosX = g_Credit[1].fPosX + POLYGON_SIZE_X;	//��̈ʂ̍��W
	g_Credit[0].fPosY = TENS_POS_Y;
	g_BigCount[0].fPosXLeft = (float)(nBigOnes * POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_BigCount[0].fPosXRight = (float)(nBigOnes * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X;
	g_BigCount[0].fPosX = g_BigCount[1].fPosX + POLYGON_SIZE_X;	//��̈ʂ̍��W
	g_BigCount[0].fPosY = TENS_COUNT_POS_Y;

	SetTextureVPolygon();
}

//====================================================
// �`�揈��
//====================================================
void DrawValuePolygon(void)
{
	for (int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_Credit[i].pVtxBuffVPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		pDevice->SetTexture(0, g_pTextureVPolygon);		// �e�N�X�`���ݒ�

		//----- �|���S���`�� -----
	//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,	// �O�p�`�X�g���b�v���w��
	//							 NUM_POLYGON,			// 2�̎O�p�`�Ŏl�p�`�����
	//							 &g_Vertex[0],			// ���_���̊J�n�A�h���X
	//							 sizeof(VERTEX_2D));	// ���_������̃T�C�Y
	
		//----- �|���S���`�� -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	for (int i = 0; i < COUNT_DIGIT_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_BigCount[i].pVtxBuffVPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		pDevice->SetTexture(0, g_pTextureVPolygon);		// �e�N�X�`���ݒ�

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

HRESULT MakeVertexVPolygon(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
												D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
												FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
												D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
												&g_Credit[i].pVtxBuffVPolygon,			  // ���_�o�b�t�@�C���^�[�t�F�[�X
												NULL)))
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_Credit[i].pVtxBuffVPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_Credit[i].fPosX, g_Credit[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Credit[i].fPosX + POLYGON_SIZE_X, g_Credit[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Credit[i].fPosX, g_Credit[i].fPosY + POLYGON_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Credit[i].fPosX + POLYGON_SIZE_X, g_Credit[i].fPosY + POLYGON_SIZE_Y, 0.0f);

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
			g_Credit[i].pVtxBuffVPolygon->Unlock();
		}
	}

	for(int i = 0; i < COUNT_DIGIT_MAX; i ++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
												D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
												FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
												D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
												&g_BigCount[i].pVtxBuffVPolygon,  // ���_�o�b�t�@�C���^�[�t�F�[�X
												NULL)))
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_BigCount[i].pVtxBuffVPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_BigCount[i].fPosX, g_BigCount[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_BigCount[i].fPosX + POLYGON_SIZE_X, g_BigCount[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_BigCount[i].fPosX, g_BigCount[i].fPosY + POLYGON_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_BigCount[i].fPosX + POLYGON_SIZE_X, g_BigCount[i].fPosY + POLYGON_SIZE_Y, 0.0f);

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
			g_BigCount[i].pVtxBuffVPolygon->Unlock();
		}
	}

	return S_OK;
}

//====================================================
// �e�N�X�`�����W�̐ݒ�
//====================================================
void SetTextureVPolygon(void)
{
	for(int i = 0; i < CREDIT_DIGIT_MAX; i ++)
	{
		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			g_Credit[i].pVtxBuffVPolygon->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_Credit[i].fPosX, g_Credit[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Credit[i].fPosX + POLYGON_SIZE_X, g_Credit[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Credit[i].fPosX, g_Credit[i].fPosY+ POLYGON_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Credit[i].fPosX + POLYGON_SIZE_X, g_Credit[i].fPosY + POLYGON_SIZE_Y, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_Credit[i].fPosXLeft, g_Credit[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_Credit[i].fPosXRight, g_Credit[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_Credit[i].fPosXLeft, g_Credit[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_Credit[i].fPosXRight, g_Credit[i].fPosYDown);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_Credit[i].pVtxBuffVPolygon->Unlock();

		}
	}

	for(int i = 0; i < COUNT_DIGIT_MAX; i ++)
	{
		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			g_BigCount[i].pVtxBuffVPolygon->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_BigCount[i].fPosX, g_BigCount[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_BigCount[i].fPosX + POLYGON_SIZE_X, g_BigCount[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_BigCount[i].fPosX, g_BigCount[i].fPosY+ POLYGON_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_BigCount[i].fPosX + POLYGON_SIZE_X, g_BigCount[i].fPosY + POLYGON_SIZE_Y, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_BigCount[i].fPosXLeft,  g_BigCount[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_BigCount[i].fPosXRight, g_BigCount[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_BigCount[i].fPosXLeft,  g_BigCount[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_BigCount[i].fPosXRight, g_BigCount[i].fPosYDown);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_BigCount[i].pVtxBuffVPolygon->Unlock();

		}
	}
}
