#include <stdshit.h>
#include "disa_core.h"
const char progName[] = "test";

DisaCore dis;

int main()
{
	HMODULE hMod = GetModuleHandle(NULL);
	dis.init(hMod, 1024*1024, (uint64_t)hMod);
	dis.exec(dis.getRva((void*)&main));
}
