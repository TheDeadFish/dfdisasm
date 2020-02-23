#pragma once

struct BasicBlock {
	u32 rva, end, target;
	s8 type; u8 flags; u16 jmpLen;
	
	enum { TYPE_RET = -1, TYPE_SPLIT, 
		TYPE_CALL, TYPE_COND, TYPE_JMP, TYPE_IND };
		
	enum { FLAG_JMP_OUT=1, FLAG_JMP_CONT=2,
		FLAG_CONT=4, FLAG_FUNC=8};
	
	bool cont() { return flags & FLAG_CONT; }
	
	bool call() { return type == TYPE_CALL; }
	
	bool func() { return flags & FLAG_FUNC; }
	
	
	
	
	
	
	
	
	u32 getInd(int index);
	
	static int compar(const BasicBlock& a, const BasicBlock& b); 
	
	
	
	
	
	
	
	// manipulation
	void doSplit(u32 end, bool cont) {
		u8 flg = (flags & FLAG_FUNC) | (cont ? FLAG_CONT : 0);
		*this = {rva, end, 0, TYPE_SPLIT, flg, 0}; }
		
};

struct BasicChunk
{
	u32 rva, end, next;
};

struct BasicBlockList
{
	xArray<BasicBlock> blocks;
	xArray<BasicChunk> chunks;
	
	
	DEF_RETPAIR(find_t, BasicBlock*, bb, int, type)
	
	
	
	
	find_t find(u32 rva);
	
	
	DEF_RETPAIR(create_t, BasicBlock*, block, u32, limit);
	
	
	
	
	
	create_t create(u32 rva, int flags);	
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
