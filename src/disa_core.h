#pragma once
#include "basicBlock.h"
#include "fixup.h"
#include "disa.h"

struct DisaCore : public Diasm
{
	BasicBlockList blocks;
	FixupList fixups;

	int fixup_create(u32 rva, u32 target, u16 type, u16 level);
	int function_mark(u32 rva);
	int indirect_mark(u32 rva);
	BasicBlockList::create_t block_create(u32 rva, bool func);
	
	
	
	
	
	
	// public interface 
	void add_reloc(u32 rva, bool dir64);
	
	
	
	
	
	
	
	
	
	
	
	
};

