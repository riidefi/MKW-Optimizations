#pragma once


#ifdef __cplusplus
extern "C" {
#endif


#include <RKtypes.h>


typedef struct
{
	u32 mMagic;
	u16 nEntry;
	u16 mUserData;
} CommonSectionHeader;
typedef struct
{
	u16 onEnterEvent;
	u16 onExitEvent;
} CheckpointTrigger;

/*
RII0 Format:

CommonSectionHeader
offsets [nEntries] // relative to start of RII0 section

RSections [nEntries]

*/

enum RiiSecType
{
	RiiSecType_PAD,
	RiiSecType_CheckpointTrigger,
	RiiSecType_Event
};

#define RiiSecType_t u32 // RiiSecType

typedef struct
{
	RiiSecType_t	mType;
	u32				nEntry;
} RiiEntry;
enum RiiEventType
{
	RiiEventType_PAD,
	RiiEventType_ModelVisUpdate, // 1
	//! @brief [2] (Not in tool yet!) Chains events
	//!
	RiiEventType_Chain,
};

#define RiiEventType_t u32 // RiiEvent::EventType
typedef struct RiiEvent
{
	RiiEventType_t	type; // 001 - model visibility change
	u32				structSize;
} RiiEvent;



typedef struct {
	u8  bVisible; //
	u8  mResourceType; // only 0 so far; 0 - course_model.brres -> model #0 -> polygon
	u16 mModelIndex; // acts as polygon index in mode 0

} ModelVisibilityParam;

typedef struct
{
	// Only 255 events. Though another chain may be called.
	u8 mNumEvents;
	u8 _[3]; // Padding for now.
	// mNumEvent bytes of events to call down will be placed here.
} EventChainParam;




#ifdef __cplusplus
}
#endif