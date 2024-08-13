/**
 * @file vistmachine.h
 * @author Ilia Voronin (www.linkedin.com/in/ilia-voronin-7a169122a)
 * @brief Header file of state machine driver
 *
 * @copyright Copyright (c) 2024 Ilia Voronin
 * 
 * This software is licensed under GNU GENERAL PUBLIC LICENSE 
 * The terms can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS,
 * Without warranty of any kind, express or implied, 
 * including but not limited to the warranties of merchantability, 
 * fitness for a particular purpose and noninfringement. 
 * In no event shall the authors or copyright holders be liable for any claim, 
 * damages or other liability, whether in an action of contract, tort or otherwise, 
 * arising from, out of or in connection with the software 
 * or the use or other dealings in the software.
 * 
 */

#ifndef VISTMACH_H
#define VISTMACH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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
