//====================================================
//
// �w�i����[bg.cpp]
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "bg.h"	//�|���S�������p�̃w�b�_�t�@�C��

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define BG_TITLE	"data/TEXTURE/title_631x792.png"
#define BG_TEXTURE	"data/TEXTURE/bg_631x792.png"
#define BG_RESULT	"data/TEXTURE/resultBG_631x792.png"

#define BG_POS_X	(0)	// �\�����W
#define BG_POS_Y	(0)

#define BG_SIZE_X	(631 * 0.8)	// �|���S���T�C�Y
#define BG_SIZE_Y	(792 * 0.8)

//****************************************************
// �\���̒�`
//****************************************************

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexBG(LPDIRECT3DDEVICE9 pDevice);

//****************************************************
// �O���[�o���ϐ�
//****************************************************

LPDIRECT3DTEXTURE9 g_pTextureBG = NULL; // �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

//====================================================
// ����������
//====================================================
HRESULT InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	MakeVertexBG(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
							  BG_TEXTURE,		// �t�@�C����
							  &g_pTextureBG);	// �ǂݍ���

	if(MODE_TITLE == GetMode())
	{
		//----- �e�N�X�`���̓ǂݍ��� -----
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
								  BG_TITLE,			// �t�@�C����
								  &g_pTextureBG);	// �ǂݍ���
	}

	if(MODE_RESULT == GetMode())
	{
		//----- �e�N�X�`���̓ǂݍ��� -----
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
								  BG_RESULT,		// �t�@�C����
								  &g_pTextureBG);	// �ǂݍ���
	}

	return S_OK;
}

//====================================================
// �I������
//====================================================
void UninitBG(void)
{
	if(g_pTextureBG != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTextureBG->Release();
		g_pTextureBG = NULL;
	}
	
	if(g_pVtxBuffBG != NULL)
	{
		//----- ���_�o�b�t�@�̉�� -----
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG= NULL;
	}
}

//====================================================
// �X�V����
//====================================================
void UpdateBG(void)
{
	//
}

//====================================================
// �`�揈��
//====================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
	pDevice->SetTexture(0, g_pTextureBG);		// �e�N�X�`���ݒ�
	
	//----- �|���S���`�� -----
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}

//====================================================
// ���_�̍쐬
//====================================================

HRESULT MakeVertexBG(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
										  D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
										  FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
										  D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
										  &g_pVtxBuffBG,			  // ���_�o�b�t�@�C���^�[�t�F�[�X
										  NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̓��e�𖄂߂�
	{
		VERTEX_2D	*pVtx;

		//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
		
		//----- ���_���W�̐ݒ� -----
		pVtx[0].pos = D3DXVECTOR3((float)(BG_POS_X),  (float)(BG_POS_Y),  0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(BG_POS_X + BG_SIZE_X), (float)(BG_POS_Y),  0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(BG_POS_X),  (float)(BG_POS_Y + BG_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(BG_POS_X + BG_SIZE_X), (float)(BG_POS_Y + BG_SIZE_Y), 0.0f);

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
		g_pVtxBuffBG->Unlock();

	}
	return S_OK;
}