#include "unix.h"


int main()
{
	int key_t key;
	if( (key = ftok(".",'z')) < 0 )
	{
		perror("ftok error");
		exit(1);
	}
	
}
