#include "Application.hpp"

#ifdef WIN32

#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return RaportPKUP::main(__argc, __argv);
#else

int main(int argc, char *argv[])
{

	return RaportPKUP::main(arc, argv);
#endif
}
