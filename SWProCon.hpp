// C++でNintendo SwitchのPro Controllerの信号を読み取る
#pragma once

#include <fstream>
#include <thread>

#define UPDATELOOP(c, x) for((c).update(); (x); (c).update()) // コントローラcの状態をupdateしながら条件xでループ

namespace SWPC{
	// ボタン(全部で14+4(十字ボタンはスティック扱い))
	enum ButtonsNum {B, A, Y, X, L, R, ZL, ZR, MINUS, PLUS, LEFT_STICK, RIGHT_STICK, HOME, SS, UP, RIGHT, DOWN, LEFT};
	const int NumButtons = 18;
	// スティック(左XYと右XYと十字ボタン)
	enum SticksNum {LEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y, CROSS_X, CROSS_Y};
	const int NumSticks = 6;
	class ProController {
	public:
		ProController();
		ProController(bool, int timeout = 0);
		ProController(const char*, bool precision = false, int timeout = 0);
		void init(const char*, bool, int);
		bool connectedCheck();
		void precisionMode(bool precision);
		void read();
		void update();
		void yReverseSet(bool setVar = true);
		bool button(ButtonsNum, bool onlyFlag = false);
		bool press(ButtonsNum);
		bool release(ButtonsNum);
		int stick(SticksNum);
		virtual ~ProController();
	private:
		void readLoop();
		std::fstream JoyStick;
		std::thread readThread;
		bool connectedFlag;
		bool loopFlag;
		bool precisionFlag;
		bool yReverseFlag;
		bool readButtonData[NumButtons];
		bool buttonData[NumButtons];
		bool beforeButtonData[NumButtons];
		int readStickData[NumSticks];
		int stickData[NumSticks];
	};
}
