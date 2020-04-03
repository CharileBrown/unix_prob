#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <typeinfo>

using namespace std;

int main(){
	int list[1000]={0};
	char a = 'a';
	int b = static_cast<int>(a);
	printf("%d\n",list[b]);
	return 0;
}
