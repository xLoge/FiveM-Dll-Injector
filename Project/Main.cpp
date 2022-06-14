#include "Common.h"

int main() 
{
	const char* file;
		SelectFile(file);
	if (file)
	{
		Injector(file);
		return 0;
	}
	else
	{
		return 1;
	}
}