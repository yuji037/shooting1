#include "DxLib.h"
#include "../game/game_setup.h"
#include "../game/game_main.h"
#include "../support/IsKeyDownTrigger.h"
#include "../support/FrameRateController.h"


// プログラムは WinMain から始まります
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{

	// ウィンドウモードで起動
	ChangeWindowMode( true ) ;

	// ウィンドウサイズ設定
	SetGraphMode( 1024, 768, 32 ) ;

	// フレームレート設定
	FpsSetting( 60, 800 ) ;

	// ＤＸライブラリ初期化処理
	if( DxLib_Init() == -1 )		
	{
		// エラーが起きたら直ちに終了
		return -1 ;				
	}

	// ゲーム用の初期化
	game_setup() ;

	// メッセージループ
	while( 1 )
	{

		if( ProcessMessage() == -1 ){
			break ;
		}

		// 画面をクリア
		ClearDrawScreen() ;

		// キー入力初期化
		KeyTriggerInitialize() ;

		// ゲーム用 メインループ
		game_main() ;

		// フレームレートコントロール
		FpsControll() ;

		// バックバッファをフリップ
		ScreenFlip() ;
		//SetWaitVSyncFlag(FALSE);

	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End() ;			

	// ソフトの終了
	return 0 ;				 
}
