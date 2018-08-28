#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#include "SWProCon.hpp"

using namespace std;
using namespace SWPC;

ProController::ProController() {
	init("/dev/input/js0", 0);
}

ProController::ProController(int timeout) {
	init("/dev/input/js0", timeout);
}

ProController::ProController(const char* fileName, int timeout) {
	init(fileName, timeout);
}

void ProController::init(const char* fileName, int timeout) {
	loopFlag = false;
	for (int i = 0; i < NumButtons; ++i) {
		readButtonData[i] = false;
		buttonData[i] = false;
	}
	for (int i = 0; i < NumSticks; ++i) {
		readStickData[i] = false;
		stickData[i] = false;
	}
	auto startTime = chrono::system_clock::now();
	connectedFlag = false;
	cout << "Connect ProController." << "\n";
	while(true) {
		if (timeout) {
			// 現在時間と比較して、開始時間＋タイムアウト秒の方が小さければ終了
			if (chrono::time_point<chrono::system_clock>(startTime + chrono::seconds(timeout)) < chrono::system_clock::now()) {
				break;
			}
		}
		try {
			JoyStick.open(fileName);
			if (JoyStick.is_open()) {
				cout << "Connected." << "\n";
				connectedFlag = true;
				break;
			}
		}
		catch (ifstream::failure error) {
			continue;
		}
	}
	yReverseFlag = false;
	if (!connectedFlag) {
		cout << "Timeout." << "\n";
	} else {
		loopFlag = true;
		readThread = thread([&]{ readLoop(); });
	}
}

bool ProController::connectedCheck() {
	return connectedFlag;
}