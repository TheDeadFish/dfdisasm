#pragma once

struct BasicBlock {
	u32 rva, end, target;
	u8 type, flags; u16 jmpLen;
	
	enum { TYPE_SPLIT, TYPE_RET, 
		TYPE_COND, TYPE_JMP, TYPE_IND };
	enum { FLAG_END = 1, FLAG_DONE = 2 };
};

struct BasicBlockList
{
	xArray<BasicBlock> blocks;
	
	
	BasicBlock* find(u32 rva);
	BasicBlock* create(u32 rva);

	// helper functions
	BasicBlock* alloc(u32 rva);
	BasicBlock* alloc() {
		return &blocks.xnxalloc(); }
	BasicBlock* split(BasicBlock* block, u32 rva);
};
