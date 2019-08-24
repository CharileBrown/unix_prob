#include<iostream>
#include<cstdio>

using namespace std;

class Test
{
	public:
	 int a;
	 int b;
	 char c;
	 Test():a(0){printf("this is no-arg create\n");}
	 Test(int id)
	 {
		printf("this is create\n");
		a = id; 
	 }
	 static void *operator new(size_t sz,int len,int num)
	 {
		 if( sz == sizeof(Test) )
		 	printf("this is operator new %d %d\n",len,num);
		 return  ::operator new(sz + len*num);
	 }
	 static void *operator new[](size_t sz)
	 {
	 	printf("sz == %lu\n",sz);
		return ::operator new(sz);
	 }

};


int main()
{
	Test *test = new(4,100) Test(4);
	Test *tt = new Test[100];
	test->a = 10;
	test->b = 20;
	test->c = 's';
	printf("%d %d %c\n",test->a,test->b,test->c);
}
