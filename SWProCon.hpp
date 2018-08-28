// C++でNintendo SwitchのPro Controllerの信号を読み取る
#pragma once

#include <fstream>
#include <thread>

namespace SWPC{
	enum ButtonsNum {B, A, Y, X, L, R, ZL, ZR, MINUS, PLUS, LEFT_STICK, RIGHT_STICK, HOME, SC, UP, RIGHT, DOWN, LEFT}
	const int NumButtons = 18;
	enum SticksNum {LEFT_X, LEFT_Y, RIGHT_X, RIGHT_Y};
	const int NumSticks = 4;
	class ProController {
	public:
		ProController();
		ProController(int timeout = 0);
		ProController(const char*, int timeout = 0);
		void init(const char*, int);
		bool connectedCheck();
		virtual ~ProController();
	private:
		std::fstream JoyStick;
		std::thread readThread;
		bool connectedFlag;
		bool loopFlag;
		bool yReverseFlag;
		bool readButtonData[NumButtons];
		bool buttonData[NumButtons];
		int readStickData[NumSticks];
		int stickData[NumSticks];
	}
}