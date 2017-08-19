
#include "DxLib.h"


static int frame_time;	//1フレームの時間(ミリ秒)
static int wait_time;	//待ち時間
static int last_time ;
static int now_time ;	//最後に取得した時間と，今の時間
static float count;		//フレームのカウント
static float fps;		//表示するしたFPS値
static int update_time; //表示値を更新する時間
static int last_update; //最後に表示値を更新した時間
static int disp_x, disp_y; //描画関係の変数
static DWORD disp_color;

//初期化
void FpsSetting(float RefreshRate, int UpdateTime){
    frame_time = (int)( 1000.0f/RefreshRate ); //1フレームの時間の計算
    update_time = UpdateTime;
    wait_time = 0;
    last_time = now_time = 0;
    count = 0.0f;
    fps = 0.0f;
    last_update = 0;
    //描画関係
    disp_x = 0;
    disp_y = 0;
    disp_color = GetColor( 255 , 255 , 255 );
}

//待ち時間の計算
void Wait(){
    now_time = GetNowCount();
    wait_time = frame_time - ( now_time - last_time );
    if( wait_time > 0 ){ //待ち時間があれば
        WaitTimer( wait_time ); //指定時間待つ
    }
	last_time = GetNowCount();
}
//FPS値の計算
float Get(){
    count += 1.0f;
    if( update_time < ( last_time - last_update ) ){ //アップデート時間になっていれば
        fps = count / (float)( last_time - last_update ) * 1000.0f ; //FPS値の計算
        last_update = last_time;
        count = 0.0f;
    }
	return ( fps );
}

//処理をまとめたもの
void FpsControll(){
    Get();
    Wait();
}

//描画設定の変更
void SetDisp(int x , int y , DWORD color ){
    disp_x = x;
    disp_y = y;
    disp_color = color;
}

// フレームレートの取得
float GetFps(){
	return fps ;
}

