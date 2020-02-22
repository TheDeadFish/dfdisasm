#include <stdshit.h>
#include "disa_core.h"

int DisaCore::fixup_create(u32 rva, u32 target, u16 type, u16 level)
{
	fixups.create(rva, target, type, level);
	

	//printf("fixup: %8I64X, %8I64X, %d, %d\n", 
	//	getAddr(rva), getAddr(target), type, level);
	return 0;
}

int DisaCore::function_mark(u32 rva)
{
	//printf("function: %I64X\n", getAddr(rva));
	return 0;

}

int DisaCore::indirect_mark(u32 rva)
{
	printf("indirect: %I64X\n", getAddr(rva));
	return 0;


}

BasicBlock* DisaCore::block_create(u32 rva, bool func)
{
	return blocks.create(rva, func);
}
