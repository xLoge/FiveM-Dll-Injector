#include "Common.h"

int main(int argc, char** argv)
{
	char* file;
		SelectFile(file);
	if (file)
	{
		Injector(file);
		rename(argv[0], (randStr() + ".exe").c_str());
		return 0;
	}
	else
	{
		rename(argv[0], (randStr() + ".exe").c_str());
		return 1;
	}
}
