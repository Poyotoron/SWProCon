#include <chrono>
#include <cstring>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include "SWProCon.hpp"

using namespace std;
using namespace SWPC;

ProController::ProController() {
	init("/dev/input/js0", false, 0);
}

ProController::ProController(bool precision, int timeout) {
	init("/dev/input/js0", precision, timeout);
}

ProController::ProController(const char* fileName, bool precision, int timeout) {
	init(fileName, precision, timeout);
}

void ProController::init(const char* fileName, bool precision, int timeout) {
	precisionFlag = precision;
	loopFlag = false;
	yReverseFlag = false;
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
		// コントローラーが接続されたかの確認、されてない場合はループ続行
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
	if (!connectedFlag) {
		cout << "Timeout." << "\n";
	} else {
		loopFlag = true;
		readThread = thread([&]{ readLoop(); });
	}
}

// 接続状態を返す
bool ProController::connectedCheck() {
	return connectedFlag;
}

void ProController::precisionMode(bool precision) {
	precisionFlag = precision;
}

// データを読む
/*
===Pro Controllerから送信されるデータについて===
16bit*4でデータが送られてくる。
・1つ目の16bitは何かの時間をミリ秒で管理している。
・2つ目の16bitは最初の16bitのオーバーフローをカウントしている。
・3つ目の16bitは値を示している。
	ボタンの場合は0か1
	アナログスティック、十字ボタンの場合は-32767(0x8001)~32767(0x7FFF)
・4つ目の16bitは識別を表している。
	上8bitは操作箇所の識別
	下8bitはボタンかアナログスティックかの識別
*/
void ProController::read() {
	vector<char> data;
	// charは8bit入る。
	char c;
	// 64bit分集めるまで待機
	while (data.size() != 8) {
		c = JoyStick.get();
		data.push_back(c);
	}
	if (data[6] == 0x01) {
		if (0 <= data[7] && data[7] < NumButtons - 4) {
			int index = data[7];
			if (data[4] == 0x00) {
				readButtonData[index] = false;
			} else if (data[4] == 0x01) {
				readButtonData[index] = true;
			}
		}
	} else if (data[6] == 0x02) {
		if (0 <= data[7] && data[7] < NumSticks) {
			int index = data[7];
			readStickData[index] = data[5];
			if (precisionFlag) {
				readStickData[index] *= 0x100;
				readStickData[index] += data[4];
				if (readStickData[index] >= 32768) {
					readStickData[index] -= -65536;
				}
			} else {
				if (readStickData[index] >= 128) {
					readStickData[index] -= 256;
				}
			}
			/*
			UP:14
			RIGHT:15
			DOWN:16
			LEFT:17
			*/
			if (index == 4) {
				if (readStickData[index] > 0) {
					readButtonData[15] = true;
					readButtonData[17] = false;
				} else if (readStickData[index] < 0) {
					readButtonData[15] = false;
					readButtonData[17] = true;
				} else {
					readButtonData[15] = false;
					readButtonData[17] = false;
				}
			} else if (index == 5) {
				if (readStickData[index] > 0) {
					readButtonData[14] = false;
					readButtonData[16] = true;
				} else if (readStickData[index] < 0) {
					readButtonData[14] = true;
					readButtonData[16] = false;
				} else {
					readButtonData[14] = false;
					readButtonData[16] = false;
				}
			}
		}
	}
}

// データ読みのループ
void ProController::readLoop() {
	while (loopFlag) {
		read();
	}
}

// コントローラーから受け取ったデータの更新
void ProController::update() {
	memcpy(beforeButtonData, buttonData, sizeof(beforeButtonData));
	memcpy(buttonData, readButtonData, sizeof(buttonData));
	memcpy(stickData, readStickData, sizeof(stickData));
}

// y方向を逆にするかどうか(デフォルトでは下が正)
void ProController::yReverseSet(bool setVar) {
	yReverseFlag = setVar;
}

// 指定されたボタンが押されているか返す 第2引数がtrueだとそのボタンだけが押されている場合のみtrueを返す
bool ProController::button(ButtonsNum Button, bool onlyFlag) {
	if (onlyFlag) {
		for (int i = 0; i < NumButtons; ++i){
			if (buttonData[i]) {
				if (i != Button) {
					return false;
				}
			}
		}
	}
	return buttonData[Button];
}

bool ProController::press(ButtonsNum Button) {
	return !beforeButtonData[Button] && buttonData[Button];
}

bool ProController::release(ButtonsNum Button) {
	return beforeButtonData[Button] && !buttonData[Button];
}

// 指定されたスティックの状態を返す
int ProController::stick(SticksNum Stick) {
	if (yReverseFlag && (Stick == LEFT_Y || Stick == RIGHT_Y || Stick == CROSS_Y)) {
		return -stickData[Stick];
	}
	return stickData[Stick];
}

// 仮想関数わからない
ProController::~ProController() {
	loopFlag = false;
	readThread.join();
	JoyStick.close();
}