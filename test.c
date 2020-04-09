#include "unix.h"
#include <cstdio>
#include <iostream>

struct test
{
	void *(*function )(void *);
	void *arg;
};

void *fun(void *arg)
{
	int a = (long long)arg;
	printf("%d\n",a);
	return NULL;
}

template<class T> T add( T a,T b )
{
	return a+b;
}

template<class T1,class T2> class A
{
	public:
	T1 a;
	T2 b;
	A( T1 a,T2 b ){
		this->a = a;
		this->b =b;
	}
	void pp(){
		printf("sum of A == %f\n",this->a+this->b);
	}
};


int main()
{
	int a = 128;
	struct test tt;
	tt.function = fun;
	long long tmp = (long long)a;
	tt.arg = (void *)tmp;
	tt.function(tt.arg);
}
