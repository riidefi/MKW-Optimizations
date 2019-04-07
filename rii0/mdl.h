#pragma once


#include <lib/debug/debug.hpp>
#include <lib/rii0/format.h>


#ifdef __cplusplus
extern "C" {
#endif


#define GetStaticModelManager() *(void**)0x809C2F44

// Get the visual model manager for course_model.brres
void* RMModelGetCourseModelManager();

// C version of nw4r::g3d::ResFile::GetResMdl(const(char const *))
extern void* BRRESHandle_GetMDL0HandleByName(void* handle, const char* name);
// C Version of nw4r::g3d::ResMdl::GetResShp(const(int))
extern void* MDL0Handle_GetObjectByIndex(void* handle, int idx);

void MDL0Object_SetVisibility(void* obj, BOOL stat);


//! @brief Show a MDL0 bone.
//!
//! @param[in] pBone Pointer to the binary bone structure (not a handle to it!)
//!
void showBone(void* pBone);

//! @brief Hide a MDL0 bone.
//!
//! @param[in] pBone Pointer to the binary bone structure (not a handle to it!)
//!
void hideBone(void* pBone);


#ifdef __cplusplus
}
#endif
