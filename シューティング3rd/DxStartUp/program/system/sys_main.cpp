#include "DxLib.h"
#include "../game/game_setup.h"
#include "../game/game_main.h"
#include "../support/IsKeyDownTrigger.h"
#include "../support/FrameRateController.h"


// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{

	// �E�B���h�E���[�h�ŋN��
	ChangeWindowMode( true ) ;

	// �E�B���h�E�T�C�Y�ݒ�
	SetGraphMode( 1024, 768, 32 ) ;

	// �t���[�����[�g�ݒ�
	FpsSetting( 60, 800 ) ;

	// �c�w���C�u��������������
	if( DxLib_Init() == -1 )		
	{
		// �G���[���N�����璼���ɏI��
		return -1 ;				
	}

	// �Q�[���p�̏�����
	game_setup() ;

	// ���b�Z�[�W���[�v
	while( 1 )
	{

		if( ProcessMessage() == -1 ){
			break ;
		}

		// ��ʂ��N���A
		ClearDrawScreen() ;

		// �L�[���͏�����
		KeyTriggerInitialize() ;

		// �Q�[���p ���C�����[�v
		game_main() ;

		// �t���[�����[�g�R���g���[��
		FpsControll() ;

		// �o�b�N�o�b�t�@���t���b�v
		ScreenFlip() ;
		//SetWaitVSyncFlag(FALSE);

	}

	// �c�w���C�u�����g�p�̏I������
	DxLib_End() ;			

	// �\�t�g�̏I��
	return 0 ;				 
}
