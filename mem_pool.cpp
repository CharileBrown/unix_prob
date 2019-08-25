#include <iostream>
#include <cstdio>
#include <cstring>
#include <unistd.h>

using namespace std;
const int MINSIZE = 64;
struct MemoryBlock;
typedef unsigned short ushort;
class MemoryPool
{
	private:	
		MemoryBlock *pBlock;
		ushort initsize;
		ushort unitsize;
		ushort growsize;
	public:
		MemoryPool(ushort _unitsize,ushort _initsize=1024, ushort _growsize=256);
		~MemoryPool();
	void* Alloc();
	void  Free( void *p ,int flag);	
};
struct MemoryBlock
{
	ushort size;
	ushort nfree;
	ushort first;
	MemoryBlock *pNext;
	char aData[1];
	static void *operator new( size_t sz,ushort len,ushort num )
	{
		return ::operator new(sizeof(MemoryBlock) + len * num + 3);
	}
	static void operator delete(void *p,size_t)
	{
		::operator delete(p);
	}
};
MemoryPool::MemoryPool(ushort _unitsize,ushort _initsize,ushort _growsize)
{
	unitsize = _unitsize + MINSIZE - ( _unitsize%MINSIZE );
	initsize = _initsize;
	growsize = _growsize;
}
void *MemoryPool::Alloc()
{
	if( !pBlock )
	{
		pBlock = new(initsize,unitsize)MemoryBlock();
        	memset(pBlock,0,sizeof(pBlock)+initsize*unitsize+3);
		pBlock->size = initsize;
		pBlock->nfree = initsize;
		pBlock->first = 0;
		pBlock->pNext = NULL;
	}
	MemoryBlock *pMyBlock = pBlock;
	while(pMyBlock && !pMyBlock->nfree)
        pMyBlock = pMyBlock -> pNext;
    if( pMyBlock )
    {
        char *pfree = pMyBlock->aData + pMyBlock->first*unitsize;
        pMyBlock->first = *((ushort*)pfree)?*((ushort*)pfree):pMyBlock->first + 1;
        pMyBlock->nfree--;
        return (void *)pfree;
    }
    else
    {
        pMyBlock = new(growsize,unitsize)MemoryBlock();
        if(pMyBlock == NULL)
        {
            fprintf(stderr,"Fail to get memory.\n");
            return NULL;
        }
        memset(pMyBlock,0,sizeof(pMyBlock)+growsize*unitsize+3);
        pMyBlock->size = growsize;
        pMyBlock->nfree = growsize-1;
        pMyBlock->first = 1;
	pMyBlock->pNext = pBlock;
	pBlock = pMyBlock;
	return (void *)(pMyBlock->aData);
    }
    
}
void MemoryPool::Free( void *p ,int flag)
{
	MemoryBlock *pMyBlock = pBlock;
	MemoryBlock *pPre;
	while( (p<pMyBlock->aData||(p>pMyBlock->aData)+(pMyBlock->size)*unitsize)&&pMyBlock )
	{
		pPre = pMyBlock;	
		pMyBlock = pMyBlock->pNext;
	}
	if( pMyBlock==NULL ){
		fprintf(stderr,"Fail to recover memory\n");
		return;
	}
	ushort tmp = pMyBlock->first;
	pMyBlock->first = (ushort)((long long)p-(long long)(pMyBlock->aData))/unitsize;
	*((ushort *)p) = tmp;
	pMyBlock->nfree++;	
	if( flag&&pMyBlock->size==pMyBlock->nfree )
	{
		pPre->pNext = pMyBlock->pNext;
		delete(pMyBlock);
		return;
	}
}

