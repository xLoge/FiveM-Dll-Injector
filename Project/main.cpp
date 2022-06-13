#include "main.h"

int main() 
{
	const char* file = SelectFile();
	if (file)
	{
		Inject(file);
		return 0;
	}
	return 1;
}
