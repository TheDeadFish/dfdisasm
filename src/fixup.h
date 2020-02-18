#pragma once

struct Fixup { 
	u32 rva, target;
	u16 type, level;
	
	enum { REL8, REL32, DIR32, DIR64 };
};

struct FixupList
{
	xArray<Fixup> list;
	
	
	
	



};
