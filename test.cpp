#include <iostream>
#include <unistd.h>
#include "SWProCon.hpp"

using namespace std;
using namespace SWPC;

int main(void) {
	cout << "Start" << "\n";
	ProController Controller;
	Controller.update();
	UPDATELOOP (Controller, !(Controller.button(UP) && Controller.button(Y))) {
		for (int j = 0; j < NumButtons; ++j) {
			if (Controller.button(ButtonsNum(j)))
				cout << j << " ";
		}
		cout << "\n";
		for (int j = 0; j < NumSticks; ++j) {
			cout << Controller.stick(SticksNum(j)) << " ";
		}
		cout << "\n";
		usleep(14000);
	}
	return 0;
}