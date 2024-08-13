/**
 * @file vistmachine_private.h
 * @author Ilia Voronin (www.linkedin.com/in/ilia-voronin-7a169122a)
 * @brief Private header file of state machine driver
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

#ifndef VISTMACH_PRIVATE_H
#define VISTMACH_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vistmachine.h"
#include "string.h"

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