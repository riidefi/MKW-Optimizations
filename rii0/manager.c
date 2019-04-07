#include <lib/rii0/manager.h>
#include <lib/debug/debug.hpp>
#ifdef __cplusplus
extern "C" {
#endif

	enum flag
	{
		RM_FLAG_LOADED_SECTION = 1, // check for pStart
		RM_FLAG_LOADED_CKPT_TRIGS = 2, // check for checkpoint trigger table
		RM_FLAG_LOADED_EVENT_TABLE = 4 // check for event table
	};
typedef struct
{

	void* pStart; // start addr of RII0 section
	CheckpointTrigger* mCkptTrigs; // checkpoint trigger table
	RiiEvent** mEventTable;
	u16 numEvents;
	u8 mFlag;

} RiiManager;


// #define BSS_MANAGER


#ifdef BSS_MANAGER
// bss will be placed right after code
RiiManager sRiiManager;
#define spRiiManager ((RiiManager*)&sRiiManager)
#else
RiiManager* spRiiManager;
#endif



void* RMManagerGetStart()
{
	if (spRiiManager && spRiiManager->mFlag & RM_FLAG_LOADED_SECTION)
		return spRiiManager->pStart;

	DebugReport("RMManagerGetStart: Fail!\n");
	return 0;
}
BOOL RMManagerSetStartEx(void* pStart, void** pLastValue)
{
	if (!spRiiManager)
		return FALSE;

	// Will still write last value even if not already loaded!
	if (pLastValue)
		*pLastValue = spRiiManager->pStart;

	spRiiManager->pStart = pStart;
	spRiiManager->mFlag |= RM_FLAG_LOADED_SECTION;

	return TRUE;
}

CheckpointTrigger* RMManagerGetCheckpointTrigger(u8 idx)
{
	if (spRiiManager && spRiiManager->mCkptTrigs && (spRiiManager->mFlag & RM_FLAG_LOADED_CKPT_TRIGS))
		return &spRiiManager->mCkptTrigs[idx];
	
	DebugReport("RMManagerGetCheckpointTrigger: Fail!\n");
	return 0;
}
BOOL RMManagerSetCheckpointTriggerEx(u8 idx, CheckpointTrigger* newTrig, CheckpointTrigger* lastVal)
{
	if (!spRiiManager || !spRiiManager->mCkptTrigs)
	{
		DebugReport("RMManagerSetCheckpointTriggerEx: Fail!\n");
		return FALSE;
	}

	if (lastVal)
		*lastVal = spRiiManager->mCkptTrigs[idx];

	spRiiManager->mCkptTrigs[idx] = *newTrig;

	return TRUE;
}
BOOL RMManagerLoadCheckpointTriggers(CheckpointTrigger* pTrigTable)
{
	DebugReport("RMManagerLoadCheckpointTriggers was called!\n");

	if (!spRiiManager)
	{
		DebugReport("RMManagerLoadCheckpointTriggers: Fail -- Manager has not been loaded!\n");
		return FALSE;
	}

	if (spRiiManager->mFlag & RM_FLAG_LOADED_CKPT_TRIGS)
		DebugReport("RMManagerLoadCheckpointTriggers: Overwriting checkpoint triggers!\n");
	else
		spRiiManager->mFlag |= RM_FLAG_LOADED_CKPT_TRIGS;

	spRiiManager->mCkptTrigs = pTrigTable;

	return TRUE;
}
void RMManagerReportCheckpointTriggers()
{
#ifdef DEBUG
	OSReport("TODO\n");
#endif
}
BOOL RMManagerCreateEventTable(u32 nEntry)
{
	if (!spRiiManager)
	{
		DebugReport("RMManagerCreateEventTable: Manager is not initialized!\n");
		return FALSE;
	}
	if (spRiiManager->mFlag & RM_FLAG_LOADED_EVENT_TABLE)
	{
		DebugReport("RMManagerCreateEventTable: Event table already loaded. Freeing!\n");
		if (spRiiManager->mEventTable)
			RMHeapFree(spRiiManager->mEventTable, 0);
		// spRiiManager->mFlag &= ~RM_FLAG_LOADED_EVENT_TABLE;
	}
	spRiiManager->numEvents = nEntry;
	spRiiManager->mEventTable = RMHeapAlloc(4 * nEntry, 4, 0);
	DebugAssert(spRiiManager->mEventTable);
	spRiiManager->mFlag |= RM_FLAG_LOADED_EVENT_TABLE;
	return TRUE;
}
RiiEvent* RMManagerGetEvent(u16 idx)
{
	idx--; // 1 indexed
	if (!spRiiManager || !spRiiManager->mEventTable || !(spRiiManager->mFlag & RM_FLAG_LOADED_EVENT_TABLE))
	{
		DebugReport("RMManagerGetEvent: Fail -- Event table not ready!\n");
		return 0;
	}
	if (idx >= spRiiManager->numEvents)
	{
		DebugReport("RMManagerGetEvent: Fail -- Event idx %u out of bounds (%u max)\n", idx, spRiiManager->numEvents);
		return 0;
	}
	return spRiiManager->mEventTable[idx];
}
BOOL RMManagerRegisterEvent(u16 idx, RiiEvent* pEvent)
{
	idx--; // 1 indexed
	if (!spRiiManager || !spRiiManager->mEventTable || !(spRiiManager->mFlag & RM_FLAG_LOADED_EVENT_TABLE))
	{
		DebugReport("RMManagerRegisterEvent: Fail -- Event table not ready!\n");
		return FALSE;
	}
	if (idx >= spRiiManager->numEvents)
	{
		DebugReport("RMManagerRegisterEvent: Fail -- Event idx %u out of bounds (%u max)\n", idx, spRiiManager->numEvents);
		return FALSE;
	}
	RMVerboseReport("RMManagerRegisterEvent: %u=%p\n", idx, pEvent);
	spRiiManager->mEventTable[idx] = pEvent;
	return TRUE;
}

void RMManagerCreate()
{
#ifndef BSS_MANAGER
	DebugReport("RMManagerCreate: Heap allocating manager!\n");
	spRiiManager = RMHeapAlloc(sizeof(RiiManager), 4, 0);
	DebugAssert(spRiiManager);
#else
	DebugReport("RMManagerCreate: BSS Manager mode!\n");
#endif


	spRiiManager->mEventTable = 0;
}
void RMManagerDestroy()
{
#ifndef BSS_MANAGER
	DebugReport("RMManagerCreate: Heap freeing manager!\n");
	RMHeapFree(spRiiManager, 0);
#endif
	DebugReport("Destrying manager!\n");
	// ignores flag
	if (spRiiManager->mEventTable)
	{
		DebugReport("Freeing event table!\n");
		RMHeapFree(spRiiManager->mEventTable, 0);
	}
}

#ifdef __cplusplus
}
#endif