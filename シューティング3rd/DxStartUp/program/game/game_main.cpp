#include "DxLib.h"
#include "game_main.h"
#include "../support/Support.h"
#include <time.h>
#include <math.h>

class A {
public:
	virtual void func() {
		DrawStringEx(100, 200, 0xffffffff, "A::func()");

	}
};

class B : public A {
public:

	void func() {
		DrawStringEx(100, 200, 0xffffffff, "B::func()");
	}
};



B bx;
A ax;

//=============================================================================
// name... game_main
// work... �Q�[���̃��C�����[�v
// arg.... none
// ret.... [ ����I�� : 0 ]
//=============================================================================
int game_main()
{
	bx.A::func();
	DrawStringEx(100, 100, 0xffffffff, "hellow world") ;

	return 0 ;

}

