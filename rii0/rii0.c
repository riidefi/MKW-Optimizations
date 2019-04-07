#include <lib/rii0/rii0.h>
#include <lib/rii0/format.h>
#include <lib/debug/debug.hpp>
#include <lib/rii0/kmp.h>

#include <lib/rii0/manager.h>

#ifdef __cplusplus
extern "C" {
#endif

// Internal functions
void RMiParse();
void RMiSetup(void* pRii0);

void RMiParse()
{
	char* RII0;
	u32* offsets;
	int i, j;
	RiiEntry* curEnt;


	// Nintendo-style execution announcement
	DebugReport("RMiParse was called!\n");

#if 0
	// Clear the system
	for (j = 0; j < 256; j++)
	{
		spRiiManager->mEventTable[j] = 0;
	}
#endif
	RII0 = (char*)RMManagerGetStart();
	DebugAssert(RII0);

	// Let's first go through all sections

	offsets = (u32*)(RII0 + sizeof(CommonSectionHeader));

	RMVerboseReport("#Rii0 entries: %u\n", ((CommonSectionHeader*)RII0)->nEntry);

	for (i = 0; i < ((CommonSectionHeader*)RII0)->nEntry; ++i)
	{
		curEnt = (RiiEntry*)(RII0 + offsets[i]);

		switch (curEnt->mType)
		{
		case RiiSecType_CheckpointTrigger:
			RMVerboseReport("RMiParse: CKPT Trig section!\n");
			RMManagerLoadCheckpointTriggers((CheckpointTrigger*)&curEnt[1]);
			break;
		case RiiSecType_Event:
			RMVerboseReport("RMiParse: Event section!\n");
			RMEventRegisterEvents(curEnt);
			break;
		default:
			// TODO
			break;
		}
	}
}

void RMSetupKMP(void* kmpManager)
{
	KMPHandle_t* pHandle;
	CommonSectionHeader* pRii0;

	// Nintendo-style execution announcement
	DebugReport("RMSetupKMP was called!\n");

	if (!kmpManager)
	{
		DebugReport("Null KMP manager!\n");
		return;
	}

	// Ensure r3 isn't junk when called.
	DebugAssert(
		((u32)kmpManager & 0x80000000) ||
		((u32)kmpManager & 0x90000000)
	);

	// Acquire the KMP file handle from the manager (first member past VT)
	pHandle = *(KMPHandle_t**)((char*)kmpManager + 4);
	DebugAssert(pHandle);

	// Silly sanity check to ensure that searchKmp is working properly
	DebugAssert(RMSearchKMP(pHandle, 'CKPH'));

	// Search for the RII0 KMP section. This method is an updated function that Nintendo uses in both MKW (compiler inlined) and in MK7.
	pRii0 = (CommonSectionHeader*)RMSearchKMP(pHandle, 'RII0');

	if (!pRii0)
	{
		DebugReport("RMSetupKMP: KMP does not have Rii0 section!\n");
		return;
	}

	DebugAssert(pRii0 && *(u32*)pRii0 == 'RII0');

	RMiSetup(pRii0);
}
void RMiSetup(void* pRii0)
{
	DebugReport("RMiSetup was called with Rii0 Section @ %p\n", pRii0);

	// Create our manager. there's no check atm to stop double allocation, so be careful to call this function only once!
	RMManagerCreate();

	RMManagerSetStart(pRii0);
	// Parse
	RMiParse();
	// Dump information
	// RMiDump();

	DebugReport("RMiSetup: Successfully set up RII0 section!\n");
}
void RMDestroy()
{
	// For now we delegate to the manager
	RMManagerDestroy();
}

#ifdef __cplusplus
}
#endif