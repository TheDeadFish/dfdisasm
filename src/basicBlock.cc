#include <stdshit.h>
#include "basicBlock.h"

int BasicBlock::compar(
	const BasicBlock& a, const BasicBlock& b) 
{ 
	return a.rva - b.rva;
}
	

BasicBlock* BasicBlockList::fast_find(u32 rva)
{
	// todo -- binary search
	return find(rva);
}

BasicBlock* BasicBlockList::find(u32 rva)
{
	for(auto& b : blocks) {
		if(inRng1(rva, b.rva, b.end))
			return &b; }
	return NULL;
}

BasicBlock* BasicBlockList::create(u32 rva, int flags)
{
	BasicBlock* found = find(rva);
	if(found) {
		if(found->rva != rva) 
			split(found, rva, flags);
		return NULL;
	}

	return alloc(rva, flags);
}

BasicBlock* BasicBlockList::alloc(u32 rva, int flags)
{
	auto& b = blocks.xnxcalloc();
	b.rva = rva; b.end = rva;
	if(flags) b.flags = b.FLAG_FUNC;
	return &b;
}

BasicBlock* BasicBlockList::split(BasicBlock* block, u32 rva, int flags)
{	
	assert((rva > block->rva)&&(rva < block->end));
	
	// create the new block
	BasicBlock* newBlk = alloc(); 
	*newBlk = *block; newBlk->rva = rva;
	
	// adjust the old block
	block->end = rva; 
	block->target = 0; block->flags = 0;
	block->type = block->TYPE_SPLIT;
	block->flags = BasicBlock::FLAG_CONT;
	
	// new block is function
	if(flags){ block->flags = 0;
		newBlk->flags |= BasicBlock::FLAG_FUNC; }
	
	return newBlk;
};

extern u64 base_addr;


void BasicBlockList::process(void)
{
	sort();
	
#if 0
	BasicBlock* curBlock = blocks;
	BasicBlock* endBlock = blocks.end();
	
	while(curBlock < endBlock) 
	{
		// locate end of block
		BasicBlock* first = curBlock;
		while((++curBlock < endBlock)
			&&(curBlock[-1].cont()));
		BasicBlock* last = curBlock-1;
		
		// 
		
		
		//if((last->type == last->TYPE_JMP)
		//&&(inRng1(last->target, first->rva, last->end)))
		//	last->flags = last->FLAG_JMP_CONT;
		
		//	printf("special %I64X\n", base_addr+last->target);
			
			
	
		
		chunks.push_back(first->rva, last->end, 0);
	}
	
#endif


	for(auto& bb : blocks) {
	
		
	
	
	
		
	
	
	
	
	
		printf("block: %8I64X, %8I64X, %8I64X, %d, %d\n", 
			base_addr+bb.rva, base_addr+bb.end, 
				bb.target?base_addr+bb.target:0, 
				bb.type, bb.flags);
	}
	
	
	
	
	

}

BasicChunk* BasicBlockList::chunk_find(u32 rva)
{
	for(auto& c : chunks)
		if(c.rva == rva) return &c;
	return NULL;
}

/*
void BasicBlockList::chunk_add(
	BasicBlock* first, BasicBlock* last);
{





	chunks.push_back(first->rva, last->end, 0);
}*/
