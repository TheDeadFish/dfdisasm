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
	setRva(bb.rva);

	while(ud_disassemble(&u))
	{
		u32 rva = u.inp_buf_index;

		FOR_REV(auto& op, u.operand,
			switch(op.type) {
			case UD_OP_IMM:
				rva -= op.size/8;
				if(op.size >= 32) {
					if(op.size > 32) fixup_create(rva, getRva(op.lval.uqword), Fixup::DIR64, 0);
					else { 
						u16 level = 3-is_one_of(u.mnemonic, UD_Imov, UD_Iadd, UD_Icmp, UD_Isub, UD_Ipush);
						fixup_create(rva, getRva(op.lval.udword), Fixup::DIR32, level); 
					}
				} break;
			
			case UD_OP_MEM:
				rva -= op.offset/8;
				if((op.offset >= 32)
				&&(op.base !=  UD_R_ESP)&&(op.base != UD_R_RSP)) {
					u16 level = 1;
					if((u.mnemonic != UD_Ilea)&&(!op.base)) {
						if(!op.index) level = 2;
						ei(op.scale >= 2) level = 1; }
					fixup_create(rva, getRva(op.lval.udword), Fixup::DIR32, level);
				} break;
					
			case UD_OP_JIMM:
				u32 target = rva; rva -= op.size/8; 
				if(op.size >= 32) { target += op.lval.udword;
					fixup_create(rva, target, Fixup::REL32, 0);
				} else { target += op.lval.sbyte;
					fixup_create(rva, target, Fixup::REL8, 0); }
				
				bb.target = target;
				bb.end = getRva();
				push(getRva());
				
				if(u.mnemonic == UD_Icall){ bb.type = bb.TYPE_CALL;
					bb.flags = bb.FLAG_JMP_OUT | bb.FLAG_CONT;
				} ei(u.mnemonic == UD_Ijmp){ bb.type = bb.TYPE_JMP;
				} else { bb.type = bb.TYPE_COND; 
					bb.flags = bb.FLAG_CONT; }

				return 1;
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
			bb.end = getRva(); return 0;
		}

	}

	return -1;
}

int Diasm::exec(u32 rva)
{
	while(rva) { 
		auto* block = block_create(rva, 0);
		while(block) { int ec = exec(*block);
			if(ec <= 0) { if(ec < 0) return ec; break; }
			block = block_create(block->target,	block->call());
		}
		rva = pop();
	}
	
	return 0;
}

Void Diasm::getPtr(u32 rva, u32 size)
{
	if(!chkRva(rva, size)) return NULL;
	return getData(rva);
}


bool Diasm::add_reloc(u32 rva, bool dir64)
{
	u64 addr;
	if(dir64) {
		u32* val = get32(rva); 
		if(!val) return false; addr = *val;
	} else {
		u64* val = get64(rva); 
		if(!val) return false; addr = *val;
	}
	
	fixup_create(rva, getRva(addr), dir64 
		? Fixup::DIR64 : Fixup::DIR32, 1);
	return true;
}
