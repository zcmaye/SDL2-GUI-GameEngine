#include"SGameApp.h"
#include"SCore/SSignal.h"

int main(int argc, char* argv[])
{
	SGameApp a(argc, argv);
	a.init("Hello World", 640, 480);

	return a.exec();
}
