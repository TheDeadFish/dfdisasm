#include <stdshit.h>
#include "disa.h"
#include "fixup.h"

#define FOR_REV(var, rng, ...) { auto && __range = rng; \
	auto __begin = std::end(__range); auto __end = std::begin(__range); \
	while(__begin != __end) { var = *--__begin; __VA_ARGS__; }}
	

void Diasm::push(u32 rva)
{
	if(rva) stack.push_back(rva);
}


u32 Diasm::pop(void)
{
	if(!stack.dataSize) return 0;
	stack.pop_back();
	return *stack.end();
}


void Diasm::setRva(u32 rva) { 
	u.inp_buf_index = rva; 
 	u.pc = base+rva;
}

void Diasm::init(void* data, size_t size, uint64_t base, bool x64)
{
	this->base = base; ud_init(&u);
	ud_set_mode(&u, x64 ? 64 : 32);
	ud_set_syntax(&u, UD_SYN_INTEL); 
	ud_set_input_buffer(&u, (unsigned char*)data, size);
}

int Diasm::exec(BasicBlock& bb)
{
	while(ud_disassemble(&u))
	{
		u32 rva = u.inp_buf_index;

		FOR_REV(auto& op, u.operand,
			switch(op.type) {
			case UD_OP_IMM:
				rva -= op.size/8;
				if(op.size >= 32) {
					if(op.size > 32) fixup_create(rva, getRva(op.lval.uqword), Fixup::DIR64, 0);
					else { fixup_create(rva, getRva(op.lval.udword), Fixup::DIR32, 0); }
				} break;
			
			case UD_OP_MEM:
				rva -= op.offset/8;
				if((op.offset >= 32)
				&&(op.base !=  UD_R_ESP)&&(op.base != UD_R_RSP)) {
					u16 level = (u.mnemonic == UD_Ilea) ? 0 :
						((op.base || op.index) ? 1 : 2);
					fixup_create(rva, getRva(op.lval.udword), Fixup::DIR32, level);
				} break;
					
			case UD_OP_JIMM:
				u32 target = rva; rva -= op.size/8; 
				if(op.size >= 32) { target += op.lval.udword;
					fixup_create(rva, target, Fixup::REL32, 2);
					if(u.mnemonic == UD_Icall) function_mark(target);
				} else { target += op.lval.sbyte;
					fixup_create(rva, target, Fixup::REL8, 2); }
					
				push(target);
					
				if(u.mnemonic != UD_Icall) {
					bb.target = target;
					if(u.mnemonic != UD_Ijmp) {
						bb.type = bb.TYPE_COND; 
						bb.end = getRva(); return 1;
					} else {
						bb.type = bb.TYPE_JMP; 
						bb.end = getRva(); return 0;
					}
				}

				break;
			}
		);
		
		switch(u.mnemonic) {
		case UD_Ijmp:
			indirect_mark(getRva()-u.inp_ctr);
			bb.type = bb.TYPE_IND; 
			bb.end = getRva(); return 0;
			
		case UD_Iret: case UD_Iretf:
		case UD_Iiretw: case UD_Iiretd:
		case UD_Iiretq:
			bb.type = bb.TYPE_RET;
			bb.flags = bb.FLAG_END;
			bb.end = getRva(); return 0;
		}

	}

	return -1;
}

int Diasm::exec(u32 rva)
{
	while(rva) {
		setRva(rva);
		while(auto* block = block_create(getRva())) {
			int ec = exec(*block); if(ec < 0) return ec; 
			
			printf("block: %8I64X, %8I64X, %8I64X, %d, %d\n", 
				getAddr(block->rva), getAddr(block->end), 
				block->target ? getAddr(block->target) : 0, 
				block->type, block->flags);
				

			
			
			
			
			
			
			
			if(ec == 0) break; }
		rva = pop();
	}
	
	return 0;
}