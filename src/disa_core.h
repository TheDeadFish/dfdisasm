#pragma once
#include "basicBlock.h"
#include "fixup.h"
#include "disa.h"
#include <peFile/peMapp.h>

struct DisaCore : public Diasm
{
	BasicBlockList blocks;
	FixupList fixups;

	int fixup_create(u32 rva, u32 target, u16 type, u16 level);
	int function_mark(u32 rva);
	int indirect_mark(u32 rva);
	BasicBlock* block_create(u32 rva);
};

