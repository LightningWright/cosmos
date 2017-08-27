#include <Windows.h>

void initConsole() {
	FILE* stream;
	AllocConsole();
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	return;
}

void hideConsole(int state) {
	//	Hides Console
	//	(int state)	:	SW_HIDE		=	0
	//				:	SW_RESTORE	=	9
	ShowWindow(GetConsoleWindow(), state);

}