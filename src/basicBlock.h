#pragma once

struct BasicBlock {
	u32 rva, end, target;
	s8 type; u8 flags; u16 jmpLen;
	
	enum { TYPE_RET = -1, TYPE_SPLIT, 
		TYPE_CALL, TYPE_COND, TYPE_JMP, TYPE_IND };
		
	enum { FLAG_JMP_OUT=1, FLAG_JMP_CONT=2,
		FLAG_CONT=4, FLAG_FUNC=8};
		
	//enum { FLAG_END = 1, FLAG_DONE = 2, 
	//	FLAG_CONT = 4 };
	
	//bool funcEnd() { return flags & FLAG_END; }
	//bool isDone() { return flags & FLAG_DONE; }
	
	
	bool cont() { return flags & FLAG_CONT; }
	
	bool call() { return type == TYPE_CALL; }
	
	
	
	
	
	
	
	
	
	
	u32 getInd(int index);
	
	static int compar(const BasicBlock& a, const BasicBlock& b); 
	
};

struct BasicChunk
{
	u32 rva, end, next;
};

struct BasicBlockList
{
	xArray<BasicBlock> blocks;
	xArray<BasicChunk> chunks;
	
	
	BasicBlock* find(u32 rva);
	BasicBlock* create(u32 rva, int flags);	
	BasicBlock* fast_find(u32 rva);
	
	

	// helper functions
	BasicBlock* alloc(u32 rva, int flags);
	BasicBlock* alloc() {
		return &blocks.xnxalloc(); }
	BasicBlock* split(BasicBlock* block, u32 rva, int flags);
	
	
	
	void sort() { qsort(blocks, BasicBlock::compar); }
	void process(void);
	
	
	
	// chunk api
	BasicChunk* chunk_find(u32 rva);
	
private:
	//void chunk_add(BasicBlock* first, BasicBlock* last);
};
