/*!
 * @file
 * @brief Implementations for RII0 events.
 */

#include <lib/rii0/event.h>
#include <lib/rii0/manager.h>
#include <lib/debug/debug.hpp>



#ifdef __cplusplus
extern "C" {
#endif

void RMEventProcessModelVisUpdate(RiiEvent* pEvent)
{
	void* courseModelHandle;
	void* mdl0;
	void* obj;
	ModelVisibilityParam* pParam = (ModelVisibilityParam*)((char*)pEvent + sizeof(RiiEvent));

	RMVerboseReport("Vis: %u; Res: %u; Idx: %u\n", pParam->bVisible, pParam->mResourceType, pParam->mModelIndex);

	courseModelHandle = (void*)RMModelGetCourseModelManager();
	DebugAssert(courseModelHandle);

	mdl0 = (void*)BRRESHandle_GetMDL0HandleByName(courseModelHandle, "course");
	DebugAssert(mdl0);

	obj = (void*)MDL0Handle_GetObjectByIndex(&mdl0, pParam->mModelIndex);
	DebugAssert(obj);

	
	MDL0Object_SetVisibility(obj, pParam->bVisible);
}

void RMEventProcess(u16 idx)
{
	char* pEventParam;
	int i;
	RiiEvent* pEvent = RMManagerGetEvent(idx);
		
	RMVerboseReport("RMEventProcess was called (idx: %u)!\n", idx);

	RMVerboseReport("Resolved event @ %p\n", pEvent);

	if (!pEvent)
		return;

	pEventParam = (char*)pEvent + sizeof(RiiEvent);

	switch (pEvent->type)
	{
	case RiiEventType_ModelVisUpdate:
		RMEventProcessModelVisUpdate(pEvent);
		break;
	case RiiEventType_Chain:
	{
		for (i = 0; i < ((EventChainParam*)pEventParam)->mNumEvents; i++)
		{
			int curChild = ((u8*)pEventParam + sizeof(EventChainParam))[i];

			// Prevent infinite recursion
			if (curChild == i) continue;

			RMEventProcess(curChild);
		}
		break;
	}
	}
}



#if 0
	void RM_RegisterEvent(RiiEvent* in, u32 id)
	{

		// Nintendo-style execution announcement
		DebugReport("RM_RegisterEvent was called!\n");

		switch (in->type)
		{
		case RiiEventType_ModelVisUpdate:
			// TODO
			DebugReport("ModelVisUpdate!\n");
			break;
		case RiiEventType_Chain:
			// TODO
			DebugReport("Chain!\n");
			break;
		default:
			// todo
			break;
		}
	}
#endif
	void RMEventRegisterEvents(RiiEntry* ent)
	{
		int j;
		RiiEvent* iter;
		BOOL result;

		// Nintendo-style execution announcement
		DebugReport("RM_RegisterEvents was called!\n");

		iter = (RiiEvent*)&ent[1];

		// Allocate event table: TODO check for exist
		if (!RMManagerCreateEventTable(ent->nEntry))
		{
			DebugReport("RMEventRegisterEvents: Failed to create event table!\n");
			return;
		}


		for (j = 0; j < ent->nEntry; j++)
		{
			// OSReport("EVENT #%u\n", j+1);
			result = RMManagerRegisterEvent(j+1, iter);
			DebugAssert(result);
			RMVerboseReport("Event type: %u and struct size: %u\n", iter->type, iter->structSize);
			iter = (RiiEvent*)((u32)iter + iter->structSize);
		}
	}

	void RMEventCheckpointEnter(u8 id)
	{
		CheckpointTrigger* trig;
		u16 enterEvent;
		DebugReport("Enter CKPT: %u\n", id);

		// Check for registed CKPT
		trig = RMManagerGetCheckpointTrigger(id);
		if ( !trig || !*(u32*)trig )
		{
			DebugReport("No registed trigger!\n");
			return;
		}
		enterEvent = trig->onEnterEvent;
		RMVerboseReport("ev: %u\n", enterEvent);
		if (enterEvent)
		{
			RMEventProcess(enterEvent);
		}
	}
	void RMEventCheckpointExit(u8 id)
	{
		CheckpointTrigger* trig;
		u16 exitEvent;
		DebugReport("Exit CKPT: %u\n", id);

		// Check for registed CKPT
		trig = RMManagerGetCheckpointTrigger(id);
		if (!trig || !*(u32*)trig)
		{
			DebugReport("No registed trigger!\n");
			return;
		}
		exitEvent = trig->onExitEvent;
		RMVerboseReport("ex: %u\n", exitEvent);
		if (exitEvent)
		{
			RMEventProcess(exitEvent);
		}
	}

#ifdef __cplusplus
}
#endif