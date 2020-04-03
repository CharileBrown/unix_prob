#include<memory>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

int main()
{
	shared_ptr<int> p1=make_shared<int>();
	shared_ptr<int> p3;
	shared_ptr<string> p2=make_shared<string>(10,'9');
	cout<<*p2<<endl;
	int a=10,b=20,c=30;
	int *p = &b;
	p = p1.get();
	cout<<*p<<endl;
	if(p1&&*p1==0)
	{
			cout<<"I am in"<<endl;
			cout<<a<<endl;
			*p1 = a;
	}
	//cout<<(*p1)<<endl;
	return 0;
}

