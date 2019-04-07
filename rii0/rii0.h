#pragma once


#include <revolution/os.h>



#ifdef __cplusplus
extern "C" {
#endif


//! @brief		Load the maanger from the kmp. Your implementation must call this. (Recommended branch off end of CourseLoadScene::Setup)
//!
//! @param[in]	kmpManager Pointer to the KMP Manager. (Field::JmpResourceCourse::spInstance)
//!
void RMSetupKMP(void* kmpManager);

// Todo: separate rmcreate? currently setupKmp creates manager

//! @brief Destroy the manager and free all used memory. Call this on race exit.
//!
void RMDestroy();


#ifdef __cplusplus
}
#endif