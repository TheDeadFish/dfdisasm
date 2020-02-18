#include <stdshit.h>
#include "basicBlock.h"

BasicBlock* BasicBlockList::find(u32 rva)
{
	for(auto& b : blocks) {
		if(inRng1(rva, b.rva, b.end))
			return &b; }
	return NULL;
}

BasicBlock* BasicBlockList::create(u32 rva)
{
	BasicBlock* found = find(rva);
	if(found) {
		if(found->rva != rva) 
			split(found, rva);
		return NULL;
	}

	return alloc(rva);
}

BasicBlock* BasicBlockList::alloc(u32 rva)
{
	auto& b = blocks.xnxcalloc();
	b.rva = rva; b.end = rva;
	return &b;
}

BasicBlock* BasicBlockList::split(BasicBlock* block, u32 rva)
{	
	assert((rva > block->rva)&&(rva < block->end));
	
	// create the new block
	BasicBlock* newBlk = alloc(); 
	*newBlk = *block; newBlk->rva = rva;
	
	// adjust the old block
	block->end = rva; 
	block->target = 0; block->type = 0;
	block->flags = BasicBlock::FLAG_DONE;
	return newBlk;
};
