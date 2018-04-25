//====================================================
//
// ���C������ [main.h]
//
//====================================================
#ifndef ___MAIN_H___
#define ___MAIN_H___

#define USE_SOUND

//****************************************************
// �C���N���[�h��
//****************************************************
#include <Windows.h>
#include "d3dx9.h"			// �`�揈���ɕK�v

#include "xaudio2.h"

//****************************************************
// ���C�u�����̃����N
//****************************************************
#if 1 // [������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J" -> "����" -> "�ǉ��̈ˑ��t�@�C��"�ɑΏۃ��C�u������ݒ肷��]
#pragma comment (lib, "d3d9.lib")		//�`�揈���ɕK�v
#pragma comment (lib, "d3dx9.lib")		//[d3d9.lib]�̊g�����C�u����
#pragma comment (lib, "dxguid.lib")		//DirectX�R���|�[�l���g
#pragma comment (lib, "winmm.lib")		//�����v���p�Ƀ}���`���f�B�A�^�C�}�[
#endif

//****************************************************
// �}�N���E�萔��`
//****************************************************
#define SCREEN_WIDTH	(631 * 0.8)		// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(792 * 0.8)		// �E�C���h�E�̍���

#define SCREEN_CENTER_X (SCREEN_WIDTH / 2)	// �E�C���h�E�̒��SX���W
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��SY���W

#define NUM_VERTEX		(4)			// ���_��
#define	NUM_POLYGON		(2)			// �|���S����

#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���_�t�H�[�}�b�g(��ʏ���W | ���_�J���[ | �e�N�X�`�����W)

// ��L�̒��_���t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float		rhw;	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_2D;

//****************************************************
// �\���̒�`
//****************************************************

//****************************************************
// �񋓌^
//****************************************************
typedef enum
{
	MODE_TITLE,			//0:�^�C�g��
	MODE_PLAY,			//1:�Q�[���v���C
	MODE_DIRECTION,		//2:���o
	MODE_BIG,			//3:�r�b�N���o
	MODE_PAUSE,			//4:�|�[�Y
	MODE_RESULT,		//5:���U���g
	MODE_MAX
} MODE;

//****************************************************
// �v���g�^�C�v�錾
//****************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE);
MODE GetMode(void);

//****************************************************
// �O���[�o���ϐ�
//****************************************************
#endif