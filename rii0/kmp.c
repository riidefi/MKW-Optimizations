/*!
 * @file
 * @brief Implementations necessary for extending the MKW KMP format.
*/

#include <lib/rii0/kmp.h>
#include <revolution/os.h>
#include <lib/debug/debug.hpp>

#ifdef __cplusplus
extern "C" {
#endif

// Revised decompiled inline function. (Originally C++ JmpResource::search)
void* RMSearchKMP(KMPHandle_t* pHandle, u32 key)
{
	void* found;
	KMPHeader_t* KMPHeader;
	void* start;
	u32 numSections;
	void* currentSectionOffset;
	int i;
	u32 curMagic;
	found = NULL;
	KMPHeader = (KMPHeader_t*)pHandle->mSource;


	numSections = KMPHeader->numSections;
	start = (char*)KMPHeader + KMPHeader->headerSize;

	for (i = 0; i < numSections; i++)
	{
		currentSectionOffset = (void*)(pHandle->mSectionOffsets[i]);

		curMagic = *(u32*)((u32)currentSectionOffset + (u32)start);

		if (curMagic == key)
			return (void*)((u32)currentSectionOffset + (u32)start);
	}
	DebugReport("Could not find '%c%c%c%c' section!\n", key);

	return NULL;
}

#ifdef __cplusplus
}
#endif