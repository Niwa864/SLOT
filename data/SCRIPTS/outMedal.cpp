//====================================================
//
// �r�o���_������[outMedal.cpp]
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "outMedal.h"	//�|���S�������p�̃w�b�_�t�@�C��

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define POLYGON_TEXTURE	"data/TEXTURE/resultV_35x42.png"
#define BAR_TEXTURE		"data/TEXTURE/outMedal_168x35.png"

#define POLYGON_SIZE_X	(35 * 0.8 * 0.5)
#define POLYGON_SIZE_Y	(42 * 0.8 * 0.5)

#define BAR_SIZE_X	(168 * 0.8)
#define BAR_SIZE_Y	(35 * 0.8)

#define TEX_DEFAULT_X	(350 * 0.8 * 0.5)
#define TEX_DEFAULT_Y	(42 * 0.8 * 0.5)

#define DIGIT_MAX (7)

#define VALUE_POS_X	(400)
#define VALUE_POS_Y	(12)

#define BAR_POS_X	(370)
#define BAR_POS_Y	(5)

//#define INTERVAL_CHANGE_PATTERN	(5)	//�t���[���P��

//****************************************************
// �\���̒�`
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffOutMedal; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	float fPosX, fPosY;
	float fPosXLeft, fPosXRight; // U���W
	float fPosYUp,   fPosYDown;	 // V���W
}value;

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexOutMedal(LPDIRECT3DDEVICE9 pDevice);

void SetTextureOutMedal();

//****************************************************
// �O���[�o���ϐ�
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureOutMedal = NULL; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBar = NULL; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBar = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

value g_OutMedal[DIGIT_MAX];

int g_nOutMedal;

//====================================================
// ����������
//====================================================
void InitOutMedal(void)
{
	g_nOutMedal = 0;

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	//----- ������ -----
	for (int i = 0; i < DIGIT_MAX; i ++)
	{
		g_OutMedal[i].fPosX = 0.0f;
		g_OutMedal[i].fPosY = 0.0f;
		g_OutMedal[i].fPosXLeft = 0.0f;
		g_OutMedal[i].fPosXRight = (float)(g_OutMedal[i].fPosXLeft + POLYGON_SIZE_X / TEX_DEFAULT_X);
		g_OutMedal[i].fPosYUp = 0.0f;
		g_OutMedal[i].fPosYDown = (float)(g_OutMedal[i].fPosYUp + POLYGON_SIZE_Y / TEX_DEFAULT_Y);
	}

	MakeVertexOutMedal(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
							  POLYGON_TEXTURE,		// �t�@�C����
							  &g_pTextureOutMedal);	// �ǂݍ���

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
							  BAR_TEXTURE,			// �t�@�C����
							  &g_pTextureBar);		// �ǂݍ���
}

//====================================================
// �I������
//====================================================
void UninitOutMedal(void)
{
	if(g_pTextureOutMedal != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTextureOutMedal->Release();
		g_pTextureOutMedal = NULL;
	}

	if(g_pTextureBar != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTextureBar->Release();
		g_pTextureBar = NULL;
	}

	for (int i = 0; i < DIGIT_MAX; i ++)
	{	
		if(g_OutMedal[i].pVtxBuffOutMedal != NULL)
		{
			//----- ���_�o�b�t�@�̉�� -----
			g_OutMedal[i].pVtxBuffOutMedal->Release();
			g_OutMedal[i].pVtxBuffOutMedal = NULL;
		}
	}

	if(g_pVtxBuffBar != NULL)
	{
		//----- ���_�o�b�t�@�̉�� -----
		g_pVtxBuffBar->Release();
		g_pVtxBuffBar = NULL;
	}
}

//====================================================
// �X�V����
//====================================================
void UpdateOutMedal(void)
{
	int nNo = 0;

	//----- �e�N�X�`�����W�̎Z�o -----
	for(int i = 0; i < DIGIT_MAX; i ++)
	{
		switch(i)
		{
			case 6:
				nNo = g_nOutMedal % 10;
				break;
			case 5:
				nNo = g_nOutMedal / 10;
				break;
			case 4:
				nNo = g_nOutMedal / 100;
				break;
			case 3:
				nNo = g_nOutMedal / 1000;
				break;
			case 2:
				nNo = g_nOutMedal / 10000;
				break;
			case 1:
				nNo = g_nOutMedal / 100000;
				break;
			case 0:
				nNo = g_nOutMedal / 1000000;
				break;
		}
		if(i == 6 || nNo != 0)
		{
			g_OutMedal[i].fPosXLeft  = (float)((nNo * POLYGON_SIZE_X) / TEX_DEFAULT_X);
			g_OutMedal[i].fPosXRight = (float)((nNo * POLYGON_SIZE_X + POLYGON_SIZE_X) / TEX_DEFAULT_X);
			g_OutMedal[i].fPosX		 = (float)(VALUE_POS_X + (POLYGON_SIZE_X * i));
			g_OutMedal[i].fPosY		 = (float)VALUE_POS_Y;
		}
	}
	SetTextureOutMedal();
}

//====================================================
// �`�揈��
//====================================================
void DrawOutMedal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffBar, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
	pDevice->SetTexture(0, g_pTextureBar);		// �e�N�X�`���ݒ�
	
	//----- �|���S���`�� -----
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	for (int i = 0; i < DIGIT_MAX; i ++)
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_OutMedal[i].pVtxBuffOutMedal, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		pDevice->SetTexture(0, g_pTextureOutMedal);		// �e�N�X�`���ݒ�

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

HRESULT MakeVertexOutMedal(LPDIRECT3DDEVICE9 pDevice)
{

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
										  D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
										  FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
										  D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
										  &g_pVtxBuffBar,			  // ���_�o�b�t�@�C���^�[�t�F�[�X
										  NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̓��e�𖄂߂�
	{
		VERTEX_2D	*pVtx;

		//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);
		
		//----- ���_���W�̐ݒ� -----
		pVtx[0].pos = D3DXVECTOR3((float)(BAR_POS_X), (float)(BAR_POS_Y), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(BAR_POS_X + BAR_SIZE_X), (float)(BAR_POS_Y), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(BAR_POS_X), (float)(BAR_POS_Y + BAR_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(BAR_POS_X + BAR_SIZE_X), (float)(BAR_POS_Y + BAR_SIZE_Y), 0.0f);

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
		g_pVtxBuffBar->Unlock();
	}

	for(int i = 0; i < DIGIT_MAX; i ++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
												D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
												FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
												D3DPOOL_MANAGED,			  // �ێ����郁�����N���X
												&g_OutMedal[i].pVtxBuffOutMedal,	  // ���_�o�b�t�@�C���^�[�t�F�[�X
												NULL)))
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_OutMedal[i].pVtxBuffOutMedal->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX), (float)(g_OutMedal[i].fPosY), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX + POLYGON_SIZE_X), (float)(g_OutMedal[i].fPosY), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX), (float)(g_OutMedal[i].fPosY + POLYGON_SIZE_Y), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX + POLYGON_SIZE_X), (float)(g_OutMedal[i].fPosY + POLYGON_SIZE_Y), 0.0f);

			//----- rhw�ݒ� -----
			pVtx[0].rhw = 
			pVtx[1].rhw = 
			pVtx[2].rhw = 
			pVtx[3].rhw = 1.0f;

			//----- ���_�J���[�ݒ� -----
			pVtx[0].col = D3DCOLOR_RGBA(68, 68, 68, 255);
			pVtx[1].col = D3DCOLOR_RGBA(68, 68, 68, 255);
			pVtx[2].col = D3DCOLOR_RGBA(68, 68, 68, 255);
			pVtx[3].col = D3DCOLOR_RGBA(68, 68, 68, 255);

			//----- �e�N�X�`�����W�̐ݒ� -----
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_OutMedal[i].pVtxBuffOutMedal->Unlock();
		}
	}
	return S_OK;
}

//====================================================
// �e�N�X�`�����W�̐ݒ�
//====================================================
void SetTextureOutMedal(void)
{
	// ���_�o�b�t�@�̓��e�𖄂߂�
	VERTEX_2D	*pVtx;

	g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
	//----- ���_���W�̐ݒ� -----
	pVtx[0].pos = D3DXVECTOR3((float)(BAR_POS_X), (float)(BAR_POS_Y), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)(BAR_POS_X + BAR_SIZE_X), (float)(BAR_POS_Y), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((float)(BAR_POS_X), (float)(BAR_POS_Y + BAR_SIZE_Y), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)(BAR_POS_X + BAR_SIZE_X), (float)(BAR_POS_Y + BAR_SIZE_Y), 0.0f);

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
	g_pVtxBuffBar->Unlock();

	for(int i = 0; i < DIGIT_MAX; i ++)
	{
		// ���_�o�b�t�@�̓��e�𖄂߂�
		VERTEX_2D	*pVtx;

		g_OutMedal[i].pVtxBuffOutMedal->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
		//----- ���_���W�̐ݒ� -----
		pVtx[0].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX), (float)(g_OutMedal[i].fPosY), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX + POLYGON_SIZE_X), (float)(g_OutMedal[i].fPosY), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX), (float)(g_OutMedal[i].fPosY+ POLYGON_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(g_OutMedal[i].fPosX + POLYGON_SIZE_X), (float)(g_OutMedal[i].fPosY + POLYGON_SIZE_Y), 0.0f);

		//----- rhw�ݒ� -----
		pVtx[0].rhw = 
		pVtx[1].rhw = 
		pVtx[2].rhw = 
		pVtx[3].rhw = 1.0f;

		//----- ���_�J���[�ݒ� -----
		pVtx[0].col = D3DCOLOR_RGBA(68, 68, 68, 255);
		pVtx[1].col = D3DCOLOR_RGBA(68, 68, 68, 255);
		pVtx[2].col = D3DCOLOR_RGBA(68, 68, 68, 255);
		pVtx[3].col = D3DCOLOR_RGBA(68, 68, 68, 255);

		//----- �e�N�X�`�����W�̐ݒ� -----
		pVtx[0].tex = D3DXVECTOR2(g_OutMedal[i].fPosXLeft,  g_OutMedal[i].fPosYUp);
		pVtx[1].tex = D3DXVECTOR2(g_OutMedal[i].fPosXRight, g_OutMedal[i].fPosYUp);
		pVtx[2].tex = D3DXVECTOR2(g_OutMedal[i].fPosXLeft,  g_OutMedal[i].fPosYDown);
		pVtx[3].tex = D3DXVECTOR2(g_OutMedal[i].fPosXRight, g_OutMedal[i].fPosYDown);

		//----- ���_�f�[�^�̃A�����b�N -----
		g_OutMedal[i].pVtxBuffOutMedal->Unlock();
	}
}

//====================================================
// �r�o�����_���̋��L
//====================================================
int	GetOutMedal(void)
{
	return g_nOutMedal;
}

//====================================================
// �r�o�����_���̃Z�b�g
//====================================================
void SetOutMedal(int nOutMedal)
{
	g_nOutMedal += nOutMedal;
}