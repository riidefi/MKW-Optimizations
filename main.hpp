#pragma once

#include <lib/rii0/rii0.h>

#define Get_RaceData2()		*(u32*)0x809bd730
#define Get_JmpResCourse()	*(u32*)0x809bd6e8

#define Get_LapCheckerEx(RD2)	!RD2 ? 0 : (*(u32*)((char*)RD2 + 0xAC))
#define Get_LapChecker()		(Get_LapCheckerEx(Get_RaceData2()))

#define LapChecker_GetCurCKPT(LC)  !LC ? 0 : (*(u16*)((char*)LC + 0xA))

//! @brief Handles a checkpoint sector load to determine the appropriate RII0 calls needed.
//!
//! @details
//! - Implements RM_EVENT_CKPT_Exit and RM_EVENT_CKPT_Enter
//!
void CKPT_Load_Handler();

//! @brief Sets up the environment
//!
//! @details
//! - Implements RMSetupKMP
//!
void CKPT_Setup();

//! @brief Cleans up the environment
//!
//! @details
//! - Implements RMDestroy
//!
void CKPT_Close();
