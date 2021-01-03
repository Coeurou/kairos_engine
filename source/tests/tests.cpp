#if defined(WIN32)

#include <windows.h>

extern int main(int argc, char* argv[]);

////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) { return main(__argc, __argv); }

#endif // WIN32

#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
    std::cout << "Hello World!\n";
}
