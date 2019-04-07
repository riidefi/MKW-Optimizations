/*!
 * @file
 * @brief Headers for RII0 events.
 */

#pragma once

#include <revolution/os.h>

#include <lib/rii0/format.h>

#ifdef __cplusplus
extern "C" {
#endif

// Functions
void RMEventProcessModelVisUpdate(RiiEvent* pEvent);

void RMEventRegisterEvents(RiiEntry* ent);

// Implementation calls these when appropriate

void RMEventCheckpointEnter(u8 id);
void RMEventCheckpointExit(u8 id);


#ifdef __cplusplus
}
#endif