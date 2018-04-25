//====================================================
//
// �|���S������[polygon.h]
//
//====================================================
#ifndef ___POLYGON_H___
#define ___POLYGON_H___

//****************************************************
// �C���N���[�h��
//****************************************************
#include "main.h"

//****************************************************
// �}�N���E�萔��`
//****************************************************

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
	int nAnimFrame;
	int nCounterAnim;					// �A�j���[�V�����J�E���^�[
	int nNo;							// case���W�␳�Ƒ���p
	bool bAnimFlg;						// �A�j���[�V���������邩�ǂ���
	bool bReviceFlg;
}reel;

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
void InitPolygon(void);		// �|���S��������
void UninitPolygon(void);	// �|���S���I��
void UpdatePolygon(bool, int);	// �|���S���X�V
void DrawPolygon(void);		// �|���S���`��

void ChangeMedal(int ,bool);

int GetMedal (void);	// ���_�������L
int	GetDirectionNo(void);
void SetDirectionNo(int);
bool GetAllStop(void);

//****************************************************
// �񋓌^
//****************************************************
enum
{
	 SYMBOL_CALLIE7,
	 SYMBOL_MARIE7,
	 SYMBOL_BAR,
	 SYMBOL_BELL,
	 SYMBOL_WMELON,
	 SYMBOL_CHERRY,
	 SYMBOL_REPLAY,
	 SYMBOL_MAX
};

#endif