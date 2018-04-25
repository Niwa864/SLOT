//====================================================
//
// �|�[�Y����[pause.cpp]
//
//====================================================

//****************************************************
// �C���N���[�h��
//****************************************************
#include "pause.h"

//****************************************************
// �}�N���E�萔��`
//****************************************************

// �ǂݍ��ރe�N�X�`���t�@�C��
#define PAUSE_TEXTURE	"data/TEXTURE/pause_631x792.png"

#define PAUSE_POS_X	(0)	// �\�����W
#define PAUSE_POS_Y	(0)

#define PAUSE_SIZE_X	(631 * 0.8)	// �|���S���T�C�Y
#define PAUSE_SIZE_Y	(792 * 0.8)

//****************************************************
// �\���̒�`
//****************************************************

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);

//****************************************************
// �O���[�o���ϐ�
//****************************************************

LPDIRECT3DTEXTURE9 g_pTexturePause = NULL; // �e�N�X�`���ւ̃|�C���^

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL; // ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

//====================================================
// ����������
//====================================================
HRESULT InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); // �f�o�C�X�擾

	MakeVertexPause(pDevice);				 // ���_���̍쐬

	//----- �e�N�X�`���̓ǂݍ��� -----
	D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
							  PAUSE_TEXTURE,		// �t�@�C����
							  &g_pTexturePause);	// �ǂݍ���

	return S_OK;
}

//====================================================
// �I������
//====================================================
void UninitPause(void)
{
	if(g_pTexturePause != NULL)
	{
		//----- �e�N�X�`���̉�� -----
		g_pTexturePause->Release();
		g_pTexturePause = NULL;
	}
	
	if(g_pVtxBuffPause != NULL)
	{
		//----- ���_�o�b�t�@�̉�� -----
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause= NULL;
	}
}

//====================================================
// �X�V����
//====================================================
void UpdatePause(void)
{
	//
}

//====================================================
// �`�揈��
//====================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);					// ���_�t�H�[�}�b�g�ݒ�
	
	pDevice->SetTexture(0, g_pTexturePause);		// �e�N�X�`���ݒ�
	
	//----- �|���S���`�� -----
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}

//====================================================
// ���_�̍쐬
//====================================================

HRESULT MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, // �m�ۂ���o�b�t�@�T�C�Y
										  D3DUSAGE_WRITEONLY,			  // �o�b�t�@�g�p�@
										  FVF_VERTEX_2D,				  // ���_�t�H�[�}�b�g
										  D3DPOOL_MANAGED,				  // �ێ����郁�����N���X
										  &g_pVtxBuffPause,			  // ���_�o�b�t�@�C���^�[�t�F�[�X
										  NULL)))
	{
		return E_FAIL;
	}

	// ���_�o�b�t�@�̓��e�𖄂߂�
	{
		VERTEX_2D	*pVtx;

		//���_�o�b�t�@�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
		
		//----- ���_���W�̐ݒ� -----
		pVtx[0].pos = D3DXVECTOR3((float)(PAUSE_POS_X), (float)(PAUSE_POS_Y), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(PAUSE_POS_X + PAUSE_SIZE_X), (float)(PAUSE_POS_Y), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(PAUSE_POS_X), (float)(PAUSE_POS_Y + PAUSE_SIZE_Y), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(PAUSE_POS_X + PAUSE_SIZE_X), (float)(PAUSE_POS_Y + PAUSE_SIZE_Y), 0.0f);

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
		g_pVtxBuffPause->Unlock();

	}
	return S_OK;
}