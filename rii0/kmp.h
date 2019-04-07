/*!
 * @file
 * @brief Headers for extending KMP.
 */

#pragma once

#include <RKtypes.h>

typedef struct
{
	void* mSource; // 00
	int*  mSectionOffsets; // 04 array of size numSections
	int   mRevision; // 08
	int   mSectionsStart; // 0c
} KMPHandle_t;

typedef struct
{
	u32 magic; // 00
	u32 fileSize; // 04
	u16 numSections; // 08
	u16 headerSize; // 0A
	u32 revision; // 0C
	// 10 Section offsets start 
} KMPHeader_t;

void* RMSearchKMP(KMPHandle_t* pHandle, u32 key);