/*!
 * @file
 * @brief Functions for dealing with models
 */


#include <lib/rii0/mdl.h>
#include <revolution/os.h>
#include <lib/debug/debug.hpp>

#ifdef __cplusplus
extern "C" {
#endif


void* RMModelGetCourseModelManager()
{
	void* courseModelHandle;
	// Acquire the static model manager. Controls course_model.brres, vrcorn_model.brres and course.kcl aka KDGndCol
	void* pStaticModelManager = GetStaticModelManager();
	DebugAssert(pStaticModelManager);

	// We only care about course_model.brres for now
	courseModelHandle = (void*)((char*)pStaticModelManager + 8);
	DebugAssert(courseModelHandle && **(u32**)courseModelHandle == 'bres');

	return courseModelHandle;
}
void MDL0Object_SetVisibility(void* obj, BOOL stat)
{
	if (!obj) return;

	if (stat)
		*(u32*)((char*)obj + 0x34) &= ~2;
	else
		*(u32*)((char*)obj + 0x34) |= 2;
}
#define BONE_FLAG_VISIBLE 0x00000100

void showBone(void* pBone)
{
	*(u32*)((char*)pBone + 0x14) |= BONE_FLAG_VISIBLE;
}

void hideBone(void* pBone)
{
	*(u32*)((char*)pBone + 0x14) &= ~BONE_FLAG_VISIBLE;
}


#ifdef __cplusplus
}
#endif