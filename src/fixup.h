#pragma once

struct Fixup { 
	u32 rva, target;
	u16 type, level;
	
	enum { REL8, REL32, DIR32, DIR64 };
	
	static int compar(const Fixup& a, const Fixup& b) 
		{ return a.rva - b.rva; }
};

struct FixupList
{
	xArray<Fixup> list;
	
	void create(u32 rva, u32 target, u16 type, u16 level) {
		list.push_back(rva, target, type, level);
	}
	
	void sort() {
		qsort(list, Fixup::compar); }
	void dump(u64 base, int minLevel) {
		for(auto& x : list) {
			if(x.level < minLevel) continue;
			printf("fixup: %8I64X, %8I64X, %d, %d\n", 
				base+x.rva, base+x.target, x.type, x.level);	
		}
	}

};
