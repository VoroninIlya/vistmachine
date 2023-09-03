#include "vistmachine_private.h"

void VISTMACH_InitState(VISTMACH_pStates dest, const char* name, 
  VISTMACH_cbState_t entryFunc, VISTMACH_cbState_t pendingFunc,
  VISTMACH_cbState_t exitFunc) {
  
  if(dest == NULL)
    return;

  if(name == NULL)
    memset(dest->name, 0, VISTMACH_STATE_NAME_LEN);
  else
    strncpy(dest->name, name, VISTMACH_STATE_NAME_LEN);

  dest->entryFunc.func = entryFunc.func;
  dest->entryFunc.context = entryFunc.context;
  dest->pendingFunc.func = pendingFunc.func;
  dest->pendingFunc.context = pendingFunc.context;
  dest->exitFunc.func = exitFunc.func;
  dest->exitFunc.context = exitFunc.context;
}

void VISTMACH_CpyState(VISTMACH_pStates dest, VISTMACH_state_t src) {

  strncpy(dest->name, src.name, VISTMACH_STATE_NAME_LEN);
  dest->entryFunc.func = src.entryFunc.func;
  dest->entryFunc.context = src.entryFunc.context;
  dest->pendingFunc.func = src.pendingFunc.func;
  dest->pendingFunc.context = src.pendingFunc.context;
  dest->exitFunc.func = src.exitFunc.func;
  dest->exitFunc.context = src.exitFunc.context;
}

bool VISTMACH_Init(VISTMACH_states_t* stMachineInst, VISTMACH_state_t* state, 
  const uint32_t length) {
  
  if(stMachineInst == NULL)
    return false;

  if(state == NULL)
    return false;

  if(length == 0)
    return false;

  stMachineInst->state = state;
  stMachineInst->currentSubstate = 0;
  stMachineInst->length = length;
  stMachineInst->activeState = -1;
  stMachineInst->lastInitializedState = 0;

  return true;
}

bool VISTMACH_AddState(VISTMACH_states_t* stMachineInst, 
  const VISTMACH_state_t state) {
    
  if(NULL == stMachineInst)
    return false;
        
  if(0 == strlen(state.name))
    return false;

  if(stMachineInst->lastInitializedState >= stMachineInst->length)
    return false;
  // the name of state schould be unique
  for(uint32_t i = 0; i < stMachineInst->lastInitializedState; i++) {
    if(0 == strncmp(stMachineInst->state[i].name, state.name, VISTMACH_STATE_NAME_LEN))
      return false;
  }

  VISTMACH_CpyState(&stMachineInst->state[stMachineInst->lastInitializedState], state);
  stMachineInst->lastInitializedState++;

  return true;
}

void VISTMACH_Start(VISTMACH_states_t* stMachineInst) {
  stMachineInst->activeState = 0;
  stMachineInst->nextState = 0;
  stMachineInst->currentSubstate = entry;
}

void VISTMACH_SwitchToStateByIndex(VISTMACH_states_t* stMachineInst, uint32_t index) {
  if(0 < index && index < stMachineInst->lastInitializedState &&
     pending == stMachineInst->currentSubstate) {
    stMachineInst->nextState = index;
    stMachineInst->currentSubstate = exit; //
  }
}

void VISTMACH_SwitchToStateByName(VISTMACH_states_t* stMachineInst, const char* name) {
  int32_t stateIndex = findStateByName(stMachineInst, name);
  VISTMACH_SwitchToStateByIndex(stMachineInst, stateIndex);
}

void VISTMACH_Stop(VISTMACH_states_t* stMachineInst) {
  stMachineInst->nextState = -1;
  stMachineInst->currentSubstate = exit; //
}

bool VISTMACH_Runtime(VISTMACH_states_t* stMachineInst) {
  bool result = true;
  if(0 <= stMachineInst->activeState && 
     stMachineInst->activeState < (int32_t)stMachineInst->lastInitializedState) {
    switch ((substate_t)stMachineInst->currentSubstate) {

      // ==============================================================================
      case entry: {
        if(NULL != stMachineInst->state[stMachineInst->activeState].entryFunc.func) {
          stMachineInst->state[stMachineInst->activeState].entryFunc.func(
            stMachineInst, 
            stMachineInst->state[stMachineInst->activeState].entryFunc.context
          );
        }
        stMachineInst->currentSubstate = pending;
        break;
      }

      // ==============================================================================
      case pending: {
        if(NULL != stMachineInst->state[stMachineInst->activeState].pendingFunc.func) {
          stMachineInst->state[stMachineInst->activeState].pendingFunc.func(
            stMachineInst,
            stMachineInst->state[stMachineInst->activeState].pendingFunc.context
          );
        }
        break;
      }

      // ==============================================================================
      case exit: {
        if(NULL != stMachineInst->state[stMachineInst->activeState].exitFunc.func) {
          stMachineInst->state[stMachineInst->activeState].exitFunc.func(
            stMachineInst,
            stMachineInst->state[stMachineInst->activeState].exitFunc.context
          );
        }
        stMachineInst->activeState = stMachineInst->nextState;
        stMachineInst->currentSubstate = entry;
        break;
      }
    }
  }
  else{
    result = false;
  }
  return result;
}

static int32_t findStateByName(const VISTMACH_states_t* stMachineInst, const char* name) {
  int32_t result = -1;
  for(uint32_t i = 0; i < stMachineInst->lastInitializedState; i++) {
    if(0 == strncmp(stMachineInst->state[i].name, name, VISTMACH_STATE_NAME_LEN)) {
      result = i;
    }
  }
  return result;
}
