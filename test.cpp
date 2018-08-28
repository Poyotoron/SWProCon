#include <iostream>
#include "SWProCon.hpp"

using namespace std;
using namespace SWPC;

int main(void) {
	cout << "Start" << "\n";
	ProController Controller;
	Controller.connectedCheck();
	return 0;
}