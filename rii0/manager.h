/*!
 * @file
 * @brief Headers for the RII0 manager.
 */

#pragma once

#include <lib/rii0/format.h>

#ifdef __cplusplus
extern "C" {
#endif

// Currently assumes only one manager, and each function rederives it

//! @brief  Returns the start address of the RII0 section.
//!
//! @return Returns a pointer to the RII0 section if possible otherwise NULL.
//!
void* RMManagerGetStart();

//! @brief      Sets the start pointer of the manager.
//!
//! @param[in]  pStart     New pointer to the RII0 section.
//! @param[out] pLastValue Optional value to set to previous state.
//!
//! @return     If the operation succeeded.
//!
BOOL    RMManagerSetStartEx(void* pStart, void** pLastValue);
#define RMManagerSetStart(pStart) RMManagerSetStartEx(pStart, 0)

//! @brief     Get the checkpoint trigger at a specified index.
//!
//! @param[in] idx Checkpoint index.
//!
//! @return    Pointer to the checkpoint trigger if successful, otherwise NULL.
//!
CheckpointTrigger* RMManagerGetCheckpointTrigger(u8 idx);

//! @brief      Set the checkpoint trigger at the specified index to a certain value
//!
//! @param[in]  idx        Checkpoint index
//! @param[out] newTrig    New checkpoint trigger to copy.
//! @param[out] pLastValue Optional value to set to previous state of the checkpoint trigger.
//!
//! @return     Whether or not the operation succeeded.
//!
BOOL    RMManagerSetCheckpointTriggerEx(u8 idx, CheckpointTrigger* newTrig, CheckpointTrigger* lastVal);
#define RMManagerSetCheckpointTrigger(idx, newTrig) RMManagerSetCheckpointTriggerEx(idx, newTrig, 0)

BOOL RMManagerLoadCheckpointTriggers(CheckpointTrigger* pTrigTable);
void RMManagerReportCheckpointTriggers();

BOOL RMManagerCreateEventTable(u32 nEntry);
RiiEvent* RMManagerGetEvent(u16 idx);
BOOL RMManagerRegisterEvent(u16 idx, RiiEvent* pEvent);

// Use RMSetupKMP
void RMManagerCreate();
// Use RMDestroy
void RMManagerDestroy();

// C version of EGG::Heap::`static void* alloc(u32 size, s32 align, Heap* heap);`
extern void* RMHeapAlloc(u32 size, s32 align, void* heap);
// C version of EGG::Heap::`static void free(void* memBlock, Heap* heap);`
extern void RMHeapFree(void* block, void* heap);


#ifdef __cplusplus
}
#endif