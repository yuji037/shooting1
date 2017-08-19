
#include "DxLib.h"


static int frame_time;	//1�t���[���̎���(�~���b)
static int wait_time;	//�҂�����
static int last_time ;
static int now_time ;	//�Ō�Ɏ擾�������ԂƁC���̎���
static float count;		//�t���[���̃J�E���g
static float fps;		//�\�����邵��FPS�l
static int update_time; //�\���l���X�V���鎞��
static int last_update; //�Ō�ɕ\���l���X�V��������
static int disp_x, disp_y; //�`��֌W�̕ϐ�
static DWORD disp_color;

//������
void FpsSetting(float RefreshRate, int UpdateTime){
    frame_time = (int)( 1000.0f/RefreshRate ); //1�t���[���̎��Ԃ̌v�Z
    update_time = UpdateTime;
    wait_time = 0;
    last_time = now_time = 0;
    count = 0.0f;
    fps = 0.0f;
    last_update = 0;
    //�`��֌W
    disp_x = 0;
    disp_y = 0;
    disp_color = GetColor( 255 , 255 , 255 );
}

//�҂����Ԃ̌v�Z
void Wait(){
    now_time = GetNowCount();
    wait_time = frame_time - ( now_time - last_time );
    if( wait_time > 0 ){ //�҂����Ԃ������
        WaitTimer( wait_time ); //�w�莞�ԑ҂�
    }
	last_time = GetNowCount();
}
//FPS�l�̌v�Z
float Get(){
    count += 1.0f;
    if( update_time < ( last_time - last_update ) ){ //�A�b�v�f�[�g���ԂɂȂ��Ă����
        fps = count / (float)( last_time - last_update ) * 1000.0f ; //FPS�l�̌v�Z
        last_update = last_time;
        count = 0.0f;
    }
	return ( fps );
}

//�������܂Ƃ߂�����
void FpsControll(){
    Get();
    Wait();
}

//�`��ݒ�̕ύX
void SetDisp(int x , int y , DWORD color ){
    disp_x = x;
    disp_y = y;
    disp_color = color;
}

// �t���[�����[�g�̎擾
float GetFps(){
	return fps ;
}

