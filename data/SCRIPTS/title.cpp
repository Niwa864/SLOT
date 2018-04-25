//====================================================
//
// �^�C�g������[title.cpp]
//
// �`�X�V�����`
// 2016/08/03 ����J�n
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "title.h"	//�|���S�������p�̃w�b�_�t�@�C��
#include <stdio.h>
#include "sound.h"

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEX_START	"data/TEXTURE/start_400x400.png"
#define TEX_END		"data/TEXTURE/end_200x200.png"

#define START_SIZE_X	(400 * 0.8)	// �|���S���T�C�Y
#define START_SIZE_Y	(400 * 0.8)

#define END_SIZE_X		(200 * 0.8)	// �|���S���T�C�Y
#define END_SIZE_Y		(200 * 0.8)

#define START_DEFAULT_SIZE_X	(800 * 0.8)	// �e�N�X�`���̂��Ƃ��Ƃ̑傫��
#define START_DEFAULT_SIZE_Y	(400 * 0.8)	

#define END_DEFAULT_SIZE_X		(400 * 0.8)	// �e�N�X�`���̂��Ƃ��Ƃ̑傫��
#define END_DEFAULT_SIZE_Y		(200 * 0.8)	

#define TEXTURE_MAX	(2)


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
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice);

void SorESetTexture(void);

//****************************************************
// �O���[�o���ϐ�
//****************************************************
tex g_TexT[TEXTURE_MAX];

int g_nSorE;

//====================================================
// ����������
//====================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	//----- �e�N�X�`���̏����� -----
	g_TexT[0].fPosXLeft    = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
	g_TexT[0].fPosXRight   = (float)(START_SIZE_X + START_SIZE_X) / START_DEFAULT_SIZE_X;
	g_TexT[0].fPosYUp	  = 0.0f;
	g_TexT[0].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
	g_TexT[0].fPolySizeX   = START_SIZE_X;
	g_TexT[0].fPolySizeY   = START_SIZE_Y;
	g_TexT[0].fTexDefaultX = START_DEFAULT_SIZE_X;
	g_TexT[0].fTexDefaultY = START_DEFAULT_SIZE_Y;
	g_TexT[0].fPolyPosX = 30;
	g_TexT[0].fPolyPosY = 250;

	g_TexT[1].fPosXLeft    = 0.0f;
	g_TexT[1].fPosXRight   = (float)END_SIZE_X / END_DEFAULT_SIZE_X;
	g_TexT[1].fPosYUp	  = 0.0f;
	g_TexT[1].fPosYDown    = (float)END_SIZE_Y / END_DEFAULT_SIZE_Y;
	g_TexT[1].fPolySizeX   = END_SIZE_X;
	g_TexT[1].fPolySizeY   = END_SIZE_Y;
	g_TexT[1].fTexDefaultX = END_DEFAULT_SIZE_X;
	g_TexT[1].fTexDefaultY = END_DEFAULT_SIZE_Y;
	g_TexT[1].fPolyPosX = 340;
	g_TexT[1].fPolyPosY = 450;

	MakeVertexTitle(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
								TEX_START,					// �t�@�C����
								&g_TexT[0].pTextureFile);	// �ǂݍ���

	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�ւ̃|�C���^
								TEX_END,					// �t�@�C����
								&g_TexT[1].pTextureFile);	// �ǂݍ���

	PlaySound(SOUND_LABEL_TITLE);
}

//====================================================
// �I������
//====================================================
void UninitTitle(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		if(g_TexT[i].pTextureFile != NULL)
		{
			//----- �e�N�X�`���̉�� -----
			g_TexT[i].pTextureFile->Release();
			g_TexT[i].pTextureFile = NULL;

			//----- ���_�o�b�t�@�̉�� -----
			g_TexT[i].pVtxBuffPolygon->Release();
			g_TexT[i].pVtxBuffPolygon = NULL;
		}
	}
}

//====================================================
// �X�V����
//====================================================
void UpdateTitle(void)
{
	if(!g_nSorE)
	{
		g_TexT[0].fPosXLeft    = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
		g_TexT[0].fPosXRight   = (float)(START_SIZE_X + START_SIZE_X) / START_DEFAULT_SIZE_X;
		g_TexT[0].fPosYUp	  = 0.0f;
		g_TexT[0].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
		g_TexT[1].fPosXLeft    = 0.0f;
		g_TexT[1].fPosXRight   = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
		g_TexT[1].fPosYUp	  = 0.0f;
		g_TexT[1].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
	}

	if(g_nSorE)
	{
		g_TexT[0].fPosXLeft    = 0.0f;
		g_TexT[0].fPosXRight   = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
		g_TexT[0].fPosYUp	  = 0.0f;
		g_TexT[0].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
		g_TexT[1].fPosXLeft    = (float)START_SIZE_X / START_DEFAULT_SIZE_X;
		g_TexT[1].fPosXRight   = (float)(START_SIZE_X + START_SIZE_X) / START_DEFAULT_SIZE_X;
		g_TexT[1].fPosYUp	  = 0.0f;
		g_TexT[1].fPosYDown    = (float)START_SIZE_Y / START_DEFAULT_SIZE_Y;
	}
	SorESetTexture();
}

//====================================================
// �`�揈��
//====================================================
void DrawTitle(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_TexT[i].pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		pDevice->SetTexture(0, g_TexT[i].pTextureFile);		// �e�N�X�`���ݒ�

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
HRESULT MakeVertexTitle(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
											  D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
											  FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
											  D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
											  &g_TexT[i].pVtxBuffPolygon,	  // ���_�o�b�t�@�C���^�[�t�F�[�X
											  NULL)))
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_TexT[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX, g_TexT[i].fPolyPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX + g_TexT[i].fPolySizeX, g_TexT[i].fPolyPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX, g_TexT[i].fPolyPosY + g_TexT[i].fPolySizeY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX + g_TexT[i].fPolySizeX, g_TexT[i].fPolyPosY + g_TexT[i].fPolySizeY, 0.0f);

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
			g_TexT[i].pVtxBuffPolygon->Unlock();

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
void SorESetTexture(void)
{
	for(int i = 0; i < TEXTURE_MAX; i ++)
	{
		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			g_TexT[i].pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX, g_TexT[i].fPolyPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX + g_TexT[i].fPolySizeX, g_TexT[i].fPolyPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX, g_TexT[i].fPolyPosY + g_TexT[i].fPolySizeY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_TexT[i].fPolyPosX + g_TexT[i].fPolySizeX, g_TexT[i].fPolyPosY + g_TexT[i].fPolySizeY, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_TexT[i].fPosXLeft , g_TexT[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_TexT[i].fPosXRight, g_TexT[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_TexT[i].fPosXLeft , g_TexT[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_TexT[i].fPosXRight, g_TexT[i].fPosYDown);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_TexT[i].pVtxBuffPolygon->Unlock();

		}
	}
}

//====================================================
// �؂�ւ����L
//====================================================
int GetSorE (void)
{
	return g_nSorE;
}

//====================================================
// �؂�ւ��Z�b�g
//====================================================
void SetSorE (int nType)
{
	g_nSorE = nType;
}