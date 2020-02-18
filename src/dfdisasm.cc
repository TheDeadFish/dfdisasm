#include <stdshit.h>
#include "dfdisasm.h"


int DfDisAsm::load(cch* file)
{
	// load pe file
	IFRET(peMap.load(file));
	dis.init(peMap.data.data, peMap.data.len,
		peMap.imageBase(), peMap.PE64());
		
	// autoanalysis
	dis.exec(peMap.ioh()->AddressOfEntryPoint);
	
	return 0;	
}
