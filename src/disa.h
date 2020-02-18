#pragma once
#include "basicBlock.h"
#include <udis86.h>

class Diasm
{
public:
	// disasembly functions
	void init(void* data, size_t size, 
		uint64_t base, bool x64=0);
	
	
	
	u32 getRva(void* x) { return ((uint8_t*)x)-u.inp_buf; }
	
	
	
	u32 getRva(uint64_t x) { return x-base; }
	
	
	
	
	uint64_t getAddr(u32 rva) { return base+rva; }
	
	
	
	
	
	u32 getRva() { return u.inp_buf_index; }
	
	
	u32 getSize() { return u.inp_buf_size; }
	Void getData(u32 rva) { return u.inp_buf+rva; }
	
	u64 getBase() { return base; }
	
	
	
	
	
	int exec(u32 rva);
	
	// data access
	bool chkRva(u32 rva, u32 size) { return 
		!(ovf_add(rva, size)&&(getSize() <= rva)); }
	Void getPtr(u32 rva, u32 size);
	
	u32* get32(u32 rva) { return getPtr(rva, 4); }
	u64* get64(u32 rva) { return getPtr(rva, 8); }
	
	
	
	
	bool add_reloc(u32 rva, bool dir64);
		
	
	
	// callbacks
	virtual int fixup_create(u32 rva, u32 target, u16 type, u16 level)=0;
	virtual int function_mark(u32 rva)=0;
	virtual int indirect_mark(u32 rva)=0;
	virtual BasicBlock* block_create(u32 rva)=0;
	
private:
	ud_t u;
	uint64_t base;

	int exec(BasicBlock& bb);
	void setRva(u32 rva); 
	
	// disasembly stack
	xVector<u32> stack;
	void push(u32 rva);
	u32 pop(void);
};
