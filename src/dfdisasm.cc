#include <stdshit.h>
#include "dfdisasm.h"

u64 base_addr;

int DfDisAsm::load(cch* file)
{
	// load pe file
	IFRET(peMap.load(file));
	dis.init(peMap.data.data, peMap.data.len,
		peMap.imageBase(), peMap.PE64());
		
	base_addr = peMap.imageBase();
		
	// autoanalysis
	dis.exec(peMap.ioh()->AddressOfEntryPoint);
	
	
	
	dis.fixups.sort();
	dis.fixups.dump(dis.getBase(), 2);
	
	dis.blocks.process();
	
	return 0;	
}
