#include "main.hpp"
#include <kamek.h>
#include <lib/debug/debug.hpp>
#include <assert.h>

#include <lib/rii0/mdl.h>
#include <lib/rii0/event.h>


// Hook the end of the race SceneEx setup vfunc.
kmBranch(0x80554728, CKPT_Setup);

// hook end of jmpResCourse static destructor
kmBranch(0x80512768, CKPT_Close);

// Hook the end of the checkpoint processing. This will be called twice, oh well
kmBranch(0x80535208, CKPT_Load_Handler);

// Will be called for last checkpoint box on start. We need to reset this value on start for the first comparison.
// Although internally a u16, the format does not allow more than 255 checkpoints
u8 lastNum;


void CKPT_Load_Handler()
{
	// We need this to determine the current checkpoint sector of the primary player.
	u16 ckptNum = LapChecker_GetCurCKPT(Get_LapChecker());
	
	// This check is important as our hook will be activated for enemy checkpoint loads, too.
	if (ckptNum != lastNum)
	{
		// These are separate as one checkpaths may feed into multiple checkpaths.
		RMEventCheckpointEnter(lastNum);
		RMEventCheckpointExit(ckptNum);

		lastNum = ckptNum;
	}
}



void CKPT_Setup()
{

	RMSetupKMP((void*)Get_JmpResCourse()); // Mount RII0
	lastNum = 0; // reset last num

}

void CKPT_Close()
{
	
	RMDestroy();
}


