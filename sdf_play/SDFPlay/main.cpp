#pragma warning(disable:4996)
#include "SdfPlayApp.h"

int main(int argc, char *argv[])
{
	SdfPlayApp *app;

	app = new SdfPlayApp();
	app->init();
	app->run();

	return 0;
}

