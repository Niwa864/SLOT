//====================================================
//
// ���[���_������[reelLight.cpp]
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "reelLight.h"

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define REELLIGHT_TEXTURE	"data/TEXTURE/reelLight_160x80.png"

#define REELLIGHT_SIZE_X	(160 * 0.8)	// �|���S���T�C�Y
#define REELLIGHT_SIZE_Y	(80 * 0.8)

#define REELLIGHT_DEFAULT_X	(160 * 0.8)
#define REELLIGHT_DEFAULT_Y	(560 * 0.8)

#define REELLIGHT_MAX		(5) //4�ȏ�Ŏ���

//****************************************************
// �\���̒�`
//****************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffRLPolygon; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	float fPosX, fPosY;
	float fPosXLeft, fPosXRight; // U���W
	float fPosYUp,   fPosYDown;	 // V���W
	bool bExist;
}light;

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexReelLight(LPDIRECT3DDEVICE9 pDevice);

void SetTextureRLPolygon();

//****************************************************
// �O���[�o���ϐ�
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureReelLight = NULL; // �e�N�X�`���ւ̃|�C���^

light g_light[REELLIGHT_MAX];

//====================================================
// ����������
//====================================================
void InitReelLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		g_light[i].fPosYUp = 0.0f;
		g_light[i].fPosYDown = g_light[i].fPosYUp + REELLIGHT_SIZE_Y / REELLIGHT_DEFAULT_Y;
		g_light[i].fPosXLeft = 0.0f;
		g_light[i].fPosXRight = g_light[i].fPosXLeft + REELLIGHT_SIZE_X / REELLIGHT_DEFAULT_X;
		g_light[i].fPosX = 0.0f;
		g_light[i].fPosY = 0.0f;
		g_light[i].bExist = false;
	}

	MakeVertexReelLight(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
							  REELLIGHT_TEXTURE,		// �t�@�C����
							  &g_pTextureReelLight);	// �ǂݍ���
}

//====================================================
// �I������
//====================================================
void UninitReelLight(void)
{
	if(g_pTextureReelLight != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTextureReelLight->Release();
		g_pTextureReelLight = NULL;
	}
	
	for(int i = 0; i < REELLIGHT_MAX; i ++)
	{
		if(g_light[i].pVtxBuffRLPolygon != NULL)
		{
			//----- ���_�o�b�t�@�̉�� -----
			g_light[i].pVtxBuffRLPolygon->Release();
			g_light[i].pVtxBuffRLPolygon= NULL;
		}
	}
}

//====================================================
// �X�V����
//====================================================
void UpdateReelLight(void)
{
	//
	SetTextureRLPolygon();
}

//====================================================
// �`�揈��
//====================================================
void DrawReelLight(void)
{
	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		if(!g_light[i].bExist)
		{
			continue;
		}

		g_light[0].fPosX = 43;

		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_light[i].pVtxBuffRLPolygon, 0, sizeof(VERTEX_2D));

		pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
		pDevice->SetTexture(0, g_pTextureReelLight);		// �e�N�X�`���ݒ�
	
		//----- �|���S���`�� -----
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//====================================================
// ���_�̍쐬
//====================================================
HRESULT MakeVertexReelLight(LPDIRECT3DDEVICE9 pDevice)
{
	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
											  D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
											  FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
											  D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
											  &g_light[i].pVtxBuffRLPolygon,  // ���_�o�b�t�@�C���^�[�t�F�[�X
											  NULL)))
		{
			return E_FAIL;
		}

		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_light[i].pVtxBuffRLPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_light[i].fPosX, g_light[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_light[i].fPosX + REELLIGHT_SIZE_X, g_light[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_light[i].fPosX, g_light[i].fPosY + REELLIGHT_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_light[i].fPosX + REELLIGHT_SIZE_X, g_light[i].fPosY + REELLIGHT_SIZE_Y, 0.0f);

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
			g_light[i].pVtxBuffRLPolygon->Unlock();

		}
	}
	return S_OK;
}

//====================================================
// �e�N�X�`�����W�̐ݒ�
//====================================================
void SetTextureRLPolygon(void)
{
	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		// ���_�o�b�t�@�̓��e�𖄂߂�
		{
			VERTEX_2D	*pVtx;

			g_light[i].pVtxBuffRLPolygon->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		
			//----- ���_���W�̐ݒ� -----
			pVtx[0].pos = D3DXVECTOR3(g_light[i].fPosX, g_light[i].fPosY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_light[i].fPosX + REELLIGHT_SIZE_X, g_light[i].fPosY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_light[i].fPosX, g_light[i].fPosY+ REELLIGHT_SIZE_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_light[i].fPosX + REELLIGHT_SIZE_X, g_light[i].fPosY + REELLIGHT_SIZE_Y, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(g_light[i].fPosXLeft,  g_light[i].fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(g_light[i].fPosXRight, g_light[i].fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(g_light[i].fPosXLeft,  g_light[i].fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(g_light[i].fPosXRight, g_light[i].fPosYDown);

			//----- ���_�f�[�^�̃A�����b�N -----
			g_light[i].pVtxBuffRLPolygon->Unlock();

		}
	}
}


//====================================================
// ���[���_���Z�b�g
//====================================================
void SetReelLight(int nNo, float fPosX, float fPosY)
{
	for(int i = 1; i < REELLIGHT_MAX; i ++)
	{
		if(g_light[i].bExist)
		{
			continue;
		}
		g_light[i].fPosYUp = (REELLIGHT_SIZE_Y * nNo) / REELLIGHT_DEFAULT_Y;
		g_light[i].fPosYDown = (REELLIGHT_SIZE_Y / REELLIGHT_DEFAULT_Y) + g_light[i].fPosYUp;
		g_light[i].fPosX = fPosX;
		g_light[i].fPosY = fPosY;
		g_light[i].bExist = true;
		break;
	}
}

//====================================================
// ���[�������Z�b�g
//====================================================
void SetReelLightOut(void)
{
	for(int i = 0; i < REELLIGHT_MAX; i ++)
	{
		g_light[i].bExist = false;
	}
}