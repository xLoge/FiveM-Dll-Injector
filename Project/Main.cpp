#include "Common.h"

int main() 
{
	const char* file;
		SelectFile(file);
	if (file)
		Injector(file);
	else
		return 1;
}
