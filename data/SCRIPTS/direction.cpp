//====================================================
//
// ���o����[direction.cpp]
//
// �`�X�V�����`
// 2016/06/14 ����J�n
// 2016/07/19 ���o������ǉ�
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "direction.h"
#include "polygon.h"

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define TEX_LOGO		"data/TEXTURE/Logo_390x291.png"
#define TEX_SPYKE_ANIM	"data/TEXTURE/SpykeAnim_500x291.png"
#define TEX_CALLIE01	"data/TEXTURE/callie_1200x291.png"
#define TEX_MARIE01		"data/TEXTURE/marie_1200x291.png"

#define POLYGON_POS_X	(95)
#define POLYGON_POS_Y	(11)

#define POLYGON_SIZE_X	(390 * 0.8)	// �|���S���T�C�Y
#define POLYGON_SIZE_Y	(291 * 0.8)

#define SPYKE_PATTERN_DIVIDE_X	(3)	// �e�N�X�`����������(X)
#define SPYKE_PATTERN_DIVIDE_Y	(1) // �e�N�X�`����������(Y)

#define SPYKE_PATTERN_SIZE_X	(1.0f / SPYKE_PATTERN_DIVIDE_X)	//1�p�^�[�����̃e�N�X�`���T�C�Y���Z�o
#define SPYKE_PATTERN_SIZE_Y	(1.0f / SPYKE_PATTERN_DIVIDE_Y)

#define SPYKE_PATTERN_MAX		(SPYKE_PATTERN_DIVIDE_X * SPYKE_PATTERN_DIVIDE_Y)	//�A�j���[�V�����̑��p�^�[����

#define LOGO_DEFAULT_SIZE_X	(390 * 0.8)	// �e�N�X�`���̂��Ƃ��Ƃ̑傫��
#define LOGO_DEFAULT_SIZE_Y	(291 * 0.8)	
#define SPYKE_DEFAULT_SIZE_X	(1500 * 0.8)
#define SPYKE_DEFAULT_SIZE_Y	(291 * 0.8)	
#define CALLIE_DEFAULT_SIZE_X	(1200 * 0.8)
#define CALLIE_DEFAULT_SIZE_Y	(291 * 0.8)	
#define MARIE_DEFAULT_SIZE_X	(1200 * 0.8)
#define MARIE_DEFAULT_SIZE_Y	(291 * 0.8)	

#define INTERVAL_CHANGE_PATTERN	(15)	//�t���[���P��
#define INTERVAL_FRAME_SEAO		(2)

//****************************************************
// �\���̒�`
//****************************************************

//****************************************************
// �񋓌^
//****************************************************

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexPolygonD(LPDIRECT3DDEVICE9 pDevice);

void SetTextureAnim(void);

//****************************************************
// �O���[�o���ϐ�
//****************************************************
LPDIRECT3DDEVICE9	g_pD3DDeviceD = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

int g_nCounterAnim;	// �A�j���[�V�����J�E���^�[
int g_nPatternAnim;	// �A�j���p�^�[��No

float g_fPosXLeft, g_fPosXRight;
float g_fPosYUp,   g_fPosYDown;

float g_fPosXLeftSeaO, g_fPosXRightSeaO;

bool g_bAnimFlg;
bool g_bDirectionFlg; // ���o�����ǂ���(1~)

//====================================================
// ����������
//====================================================
void InitDirection(void)
{
	int nNo = GetDirectionNo();

	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	//----- �J�E���^�[�̏����� -----
	g_nCounterAnim = 0;
	g_nPatternAnim = 0;

	g_fPosXLeftSeaO = 0.0f;
	g_fPosXRightSeaO = 0.0f;

	g_bAnimFlg = true;
	g_bDirectionFlg = false;

	MakeVertexPolygonD(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	switch(nNo)
	{
	case 0 : D3DXCreateTextureFromFile(pDevice, TEX_LOGO, &g_pTexturePolygon);
			 g_fPosXLeft  = 0.0f;
			 g_fPosXRight = (float)(POLYGON_SIZE_X / LOGO_DEFAULT_SIZE_X);
			 g_fPosYUp    = 0.0f;
			 g_fPosYDown  = (float)(POLYGON_SIZE_Y / LOGO_DEFAULT_SIZE_Y);
			 break;
	case 1 : D3DXCreateTextureFromFile(pDevice, TEX_SPYKE_ANIM, &g_pTexturePolygon);
			 g_fPosXLeft  = (float)(g_nPatternAnim % SPYKE_PATTERN_DIVIDE_X) * SPYKE_PATTERN_SIZE_X;
			 g_fPosXRight = (float)(g_nPatternAnim % SPYKE_PATTERN_DIVIDE_X) * SPYKE_PATTERN_SIZE_X + SPYKE_PATTERN_SIZE_X;
			 g_fPosYUp	  = (float)(g_nPatternAnim / (SPYKE_PATTERN_MAX / SPYKE_PATTERN_DIVIDE_Y)) * SPYKE_PATTERN_SIZE_Y;
			 g_fPosYDown  = (float)(g_nPatternAnim / (SPYKE_PATTERN_MAX / SPYKE_PATTERN_DIVIDE_Y)) * SPYKE_PATTERN_SIZE_Y + SPYKE_PATTERN_SIZE_Y;
			 break;
	case 2 : D3DXCreateTextureFromFile(pDevice, TEX_CALLIE01, &g_pTexturePolygon);
			 g_fPosXLeftSeaO  = 1.0f - (float)(POLYGON_SIZE_X / MARIE_DEFAULT_SIZE_X);
			 g_fPosXRightSeaO = 1.0f;
			 g_fPosYUp		  = 0.0f;
			 g_fPosYDown	  = (float)(POLYGON_SIZE_Y / MARIE_DEFAULT_SIZE_Y);
			 SetTextureAnim();
			 break;
	case 3 : D3DXCreateTextureFromFile(pDevice, TEX_MARIE01, &g_pTexturePolygon);
			 g_fPosXLeftSeaO  = 0.0f;
			 g_fPosXRightSeaO = (float)(POLYGON_SIZE_X / CALLIE_DEFAULT_SIZE_X);
			 g_fPosYUp		  = 0.0f;
			 g_fPosYDown	  = (float)(POLYGON_SIZE_Y / CALLIE_DEFAULT_SIZE_Y);
			 SetTextureAnim();
			 break;
	}
}

//====================================================
// �I������
//====================================================
void UninitDirection(void)
{

	if(g_pTexturePolygon != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;

		//----- ���_�o�b�t�@�̉�� -----
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//====================================================
// �X�V����
//====================================================
void UpdateDirection(void)
{
	int nNo = GetDirectionNo();

	//if(nNo == 0)		// 0�͒e��
	//{
	//	return;
	//}
	if(!g_bAnimFlg)		// �A�j������̋�������Ƃ��̂�
	{
		return;
	}

	g_nCounterAnim ++;	// �A�j���J�E���^�[�̃J�E���g�A�b�v

	switch(nNo)
	{
	case 0://if((g_nCounterAnim % INTERVAL_CHANGE_PATTERN) == 0)				// �摜�؂�ւ��^�C�~���O�̔��f
	   	   //{		
		   //	g_nPatternAnim = (g_nPatternAnim + 1) % SPYKE_PATTERN_MAX;	// �p�^�[���؂�ւ�
		   //}
		   SetTextureAnim();								// �p�^�[���ς���Ƃ��͈���g_PattaernAnim
		   break;

	case 1:if((g_nCounterAnim % INTERVAL_CHANGE_PATTERN) == 0)				// �摜�؂�ւ��^�C�~���O�̔��f
		   {		
			   g_nPatternAnim = (g_nPatternAnim + 1) % SPYKE_PATTERN_MAX;	// �p�^�[���؂�ւ�
		   }
		   SetTextureAnim();								// �p�^�[���ς���Ƃ��͈���g_PattaernAnim
		   break;

	case 2:if((g_nCounterAnim % INTERVAL_FRAME_SEAO) == 0)				// �摜�؂�ւ��^�C�~���O�̔��f
		   {		
			   	SetTextureAnim();								// �p�^�[���ς���Ƃ��͈���g_PattaernAnim

				//----- �e�N�X�`�����W�̎Z�o -----
				if(g_fPosXRightSeaO <= 0.55f)
				{
					g_fPosXLeftSeaO  -= 0.015f;
					g_fPosXRightSeaO -= 0.015f;
				}
				else if(g_fPosXRightSeaO <= 0.65f)
				{
					g_fPosXLeftSeaO  -= 0.005f;
					g_fPosXRightSeaO -= 0.005f;
				}
				else
				{
					g_fPosXLeftSeaO  -= 0.01f;
					g_fPosXRightSeaO -= 0.01f;
				}
		   }
		   break;

	case 3:if((g_nCounterAnim % INTERVAL_FRAME_SEAO) == 0)				// �摜�؂�ւ��^�C�~���O�̔��f
		   {		
			   	SetTextureAnim();								// �p�^�[���ς���Ƃ��͈���g_PattaernAnim

			   	//----- �e�N�X�`�����W�̎Z�o -----
				if(g_fPosXRightSeaO <= 0.55f)
				{
					g_fPosXLeftSeaO  += 0.01f;
					g_fPosXRightSeaO += 0.01f;
				}
				else if(g_fPosXRightSeaO <= 0.65f)
				{
					g_fPosXLeftSeaO  += 0.005f;
					g_fPosXRightSeaO += 0.005f;
				}
				else
				{
					g_fPosXLeftSeaO  += 0.015f;
					g_fPosXRightSeaO += 0.015f;
				}
		   }
		   break;
	}

	if(g_nPatternAnim == 2)
	{
		//while()
		//{

		//}
		UpdatePolygon(true, 0); // ��őS���񂷂̂ŉ��l
		SetMode(MODE_PLAY);
		//g_bDirectionFlg = true;
		SetDirectionNo(0);
		InitDirection();	//Init����g_bDirectionFlg��false��
	}

	//�l�v���X��
	if(g_fPosXRightSeaO < 0)
	{
		g_fPosXRightSeaO = -g_fPosXRightSeaO;
		g_fPosXLeftSeaO  = -g_fPosXLeftSeaO;
	}

	if(nNo == 2 && g_fPosXRightSeaO <= 0.3f) 
	{
		SetMode(MODE_PLAY);
		//g_bDirectionFlg = true;
		SetDirectionNo(0);
		InitDirection();	//Init����g_bDirectionFlg��false��
	}

	if(nNo == 3 && g_fPosXRightSeaO >= 1.05f) 
	{
		SetMode(MODE_PLAY);
		//g_bDirectionFlg = true;
		SetDirectionNo(0);
		InitDirection();
	}

	//----- �e�N�X�`�����W�̎Z�o -----
	//g_fPosXLeft	 += 0.01f;
	//g_fPosXRight += 0.01f;
}

//====================================================
// �`�揈��
//====================================================
void DrawDirection(void)
{
	int nNo = GetDirectionNo();

	//if(nNo == 0)		// 0�͒e��
	//{
	//	return;
	//}
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
	pDevice->SetTexture(0, g_pTexturePolygon);		// �e�N�X�`���ݒ�

	//----- �|���S���`�� -----
	//	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,	// �O�p�`�X�g���b�v���w��
	//							 NUM_POLYGON,			// 2�̎O�p�`�Ŏl�p�`�����
	//							 &g_Vertex[0],			// ���_���̊J�n�A�h���X
	//							 sizeof(VERTEX_2D));	// ���_������̃T�C�Y
	
	//----- �|���S���`�� -----
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}
//====================================================
// ���_�̍쐬
//====================================================

HRESULT MakeVertexPolygonD(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
											D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
											FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
											D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
											&g_pVtxBuffPolygon,	  // ���_�o�b�t�@�C���^�[�t�F�[�X
											NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̓��e�𖄂߂�
	{
		VERTEX_2D	*pVtx;

		//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
		
		//----- ���_���W�̐ݒ� -----
		pVtx[0].pos = D3DXVECTOR3((float)(POLYGON_POS_X), (float)(POLYGON_POS_Y), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(POLYGON_POS_X + POLYGON_SIZE_X), POLYGON_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(POLYGON_POS_X), (float)(POLYGON_POS_Y + POLYGON_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(POLYGON_POS_X + POLYGON_SIZE_X), (float)(POLYGON_POS_Y + POLYGON_SIZE_Y), 0.0f);

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
		g_pVtxBuffPolygon->Unlock();
	}
	return S_OK;
}

//====================================================
// �e�N�X�`�����W�̐ݒ�
//====================================================
void SetTextureAnim(void)
{
	int nNo = GetDirectionNo();

	// ���_�o�b�t�@�̓��e�𖄂߂�
	{
		VERTEX_2D	*pVtx;

		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0); //���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾

		//----- �e�N�X�`�����W�̎Z�o -----   
		switch(nNo)
		{
		case 0:g_fPosXLeft	= 0.0f;
			   g_fPosXRight = (float)(POLYGON_SIZE_X / LOGO_DEFAULT_SIZE_X);
			   g_fPosYUp	= 0.0f;
			   g_fPosYDown	= (float)(POLYGON_SIZE_Y / LOGO_DEFAULT_SIZE_Y);
			   break;

		case 1:g_fPosXLeft	= (float)((g_nPatternAnim % SPYKE_PATTERN_DIVIDE_X) * SPYKE_PATTERN_SIZE_X);
			   g_fPosXRight = (float)((g_nPatternAnim % SPYKE_PATTERN_DIVIDE_X) * SPYKE_PATTERN_SIZE_X + SPYKE_PATTERN_SIZE_X);
			   g_fPosYUp	= (float)((g_nPatternAnim / (SPYKE_PATTERN_MAX / SPYKE_PATTERN_DIVIDE_Y)) * SPYKE_PATTERN_SIZE_Y);
			   g_fPosYDown	= (float)((g_nPatternAnim / (SPYKE_PATTERN_MAX / SPYKE_PATTERN_DIVIDE_Y)) * SPYKE_PATTERN_SIZE_Y + SPYKE_PATTERN_SIZE_Y);
			   break;

		case 2:break;

		case 3:break;
		}

		//----- ���_���W�̐ݒ� -----
		pVtx[0].pos = D3DXVECTOR3((float)(POLYGON_POS_X), (float)(POLYGON_POS_Y), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(POLYGON_POS_X + POLYGON_SIZE_X), (float)(POLYGON_POS_Y), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(POLYGON_POS_X), (float)(POLYGON_POS_Y + POLYGON_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(POLYGON_POS_X + POLYGON_SIZE_X), (float)(POLYGON_POS_Y + POLYGON_SIZE_Y), 0.0f);

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
		switch(nNo)
			{
			case 0:pVtx[0].tex = D3DXVECTOR2(g_fPosXLeft, g_fPosYUp);
				   pVtx[1].tex = D3DXVECTOR2(g_fPosXRight, g_fPosYUp);
				   pVtx[2].tex = D3DXVECTOR2(g_fPosXLeft, g_fPosYDown);
				   pVtx[3].tex = D3DXVECTOR2(g_fPosXRight, g_fPosYDown);
				   break;

			case 1:pVtx[0].tex = D3DXVECTOR2(g_fPosXLeft, g_fPosYUp);
				   pVtx[1].tex = D3DXVECTOR2(g_fPosXRight, g_fPosYUp);
				   pVtx[2].tex = D3DXVECTOR2(g_fPosXLeft, g_fPosYDown);
				   pVtx[3].tex = D3DXVECTOR2(g_fPosXRight, g_fPosYDown);
				   break;

			case 2:pVtx[0].tex = D3DXVECTOR2(g_fPosXLeftSeaO, g_fPosYUp);
				   pVtx[1].tex = D3DXVECTOR2(g_fPosXRightSeaO, g_fPosYUp);
				   pVtx[2].tex = D3DXVECTOR2(g_fPosXLeftSeaO, g_fPosYDown);
				   pVtx[3].tex = D3DXVECTOR2(g_fPosXRightSeaO, g_fPosYDown);
				   break;

			case 3:pVtx[0].tex = D3DXVECTOR2(g_fPosXLeftSeaO, g_fPosYUp);
				   pVtx[1].tex = D3DXVECTOR2(g_fPosXRightSeaO, g_fPosYUp);
				   pVtx[2].tex = D3DXVECTOR2(g_fPosXLeftSeaO, g_fPosYDown);
				   pVtx[3].tex = D3DXVECTOR2(g_fPosXRightSeaO, g_fPosYDown);
				   break;

			}

		//----- ���_�f�[�^�̃A�����b�N -----
		g_pVtxBuffPolygon->Unlock();

	}
}

//====================================================
// ���o�I���t���O�̋��L
//====================================================
bool GetDirectionFlg (void)
{
	return g_bDirectionFlg;
}

//====================================================
// ���o�I���t���O�Z�b�g
//====================================================
void SetDirectionFlg (bool bFlg)
{
	g_bDirectionFlg = bFlg;
}

//====================================================
// ���o�I���t���O�̋��L
//====================================================
int GetPatternAnim (void)
{
	return g_nPatternAnim;
}