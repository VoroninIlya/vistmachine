#ifndef VISTMACH_PRIVATE_H
#define VISTMACH_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vistmachine.h"

typedef enum {
  entry = 0,
  pending,
  exit
}substate_t;

static int32_t findStateByName(const VISTMACH_states_t* stMachineInst, const char* name);

#ifdef __cplusplus
}
#endif

#endif // VISTMACH