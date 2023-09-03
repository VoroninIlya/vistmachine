#ifndef VISTMACH_H
#define VISTMACH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "string.h"

#define VISTMACH_STATE_NAME_LEN 10

typedef struct {
  void (*func)(void* this, void* context);
  void* context;
}VISTMACH_cbState_t;

typedef struct 
{
  char name[VISTMACH_STATE_NAME_LEN];
  VISTMACH_cbState_t entryFunc;
  VISTMACH_cbState_t pendingFunc;
  VISTMACH_cbState_t exitFunc;
}VISTMACH_state_t, *VISTMACH_pStates;

typedef struct 
{
  VISTMACH_pStates state;
  uint32_t currentSubstate;
  int32_t activeState;
  int32_t nextState;
  uint32_t lastInitializedState;
  uint32_t length;
}VISTMACH_states_t;

void VISTMACH_InitState(VISTMACH_pStates dest, const char* name, 
  VISTMACH_cbState_t entryFunc, VISTMACH_cbState_t pendingFunc,
  VISTMACH_cbState_t exitFunc);

void VISTMACH_CpyState(VISTMACH_pStates dest, VISTMACH_state_t src);

bool VISTMACH_Init(VISTMACH_states_t* stMachineInst, VISTMACH_state_t* state, 
  const uint32_t length);

bool VISTMACH_AddState(VISTMACH_states_t* stMachineInst, 
  const VISTMACH_state_t state);

void VISTMACH_Start(VISTMACH_states_t* stMachineInst);

void VISTMACH_SwitchToStateByIndex(VISTMACH_states_t* stMachineInst, uint32_t index);

void VISTMACH_SwitchToStateByName(VISTMACH_states_t* stMachineInst, const char* name);

void VISTMACH_Stop(VISTMACH_states_t* stMachineInst); 

bool VISTMACH_Runtime(VISTMACH_states_t* stMachineInst);

#ifdef __cplusplus
}
#endif

#endif // VISTMACH
