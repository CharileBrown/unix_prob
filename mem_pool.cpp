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
		MemoryPool(ushort unitsize,ushort initsize=1024, ushort growsize=256);
		~MemoryPool();
	void* Alloc();
	void  Free( void *p );	
}mem_pool;
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
}
MemoryPool::MemoryPool(ushort _unitsize,ushort _initsize=1024,ushort _growsize=256)
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
	while(pMyblock && !pMyBlock->nfree)
        pMyBlock = pMyBlock -> pNext;
    if( pMyBlock )
    {
        char *pfree = pMyBlock->aData + pMyBlock->first*unitsize;
        pMyBlock->fist = *((ushort*)pfree)?*((ushort*)pfree):pMyBlock->first + 1;
        pMyblock->nfree--;
        return pfree;
    }
    else
    {
        pMyBlock = new(growsize,unitsize)MemoryPool();
        if(pMyBlock == NULL)
        {
            fprintf(stderr,"Fail to get memory.\n");
            return NULL;
        }
        memset(pMyBlock,0,sizeof(pMyBlock)+growsize*unitsize+3);
        pMyBlock->size = growsize;
        pMyBlock->nfree = growsize;
        pMyBlock->aData
    }
    
}

