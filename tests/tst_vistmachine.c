#include "unity.h"
#include "unity_fixture.h"
#include "vistmachine.h"

TEST_GROUP(TST_VISTMACHINE);

TEST_GROUP_RUNNER(TST_VISTMACHINE)
{
  RUN_TEST_CASE(TST_VISTMACHINE, VISTMACH_Init);
  RUN_TEST_CASE(TST_VISTMACHINE, VISTMACH_AddState);
  RUN_TEST_CASE(TST_VISTMACHINE, VISTMACH_Runtime);
}

#define STATES_NUMBER 3
static VISTMACH_state_t statesArray[STATES_NUMBER];
static VISTMACH_states_t stMachineInstatnce;

TEST_SETUP(TST_VISTMACHINE)
{
  TEST_ASSERT_TRUE(VISTMACH_Init(&stMachineInstatnce, statesArray, STATES_NUMBER));
}

TEST_TEAR_DOWN(TST_VISTMACHINE)
{
}

// ===================================================================================
// Test VISTMACHINE_Init =============================================================
TEST(TST_VISTMACHINE, VISTMACH_Init)
{
  TEST_ASSERT_FALSE(VISTMACH_Init(NULL, statesArray, STATES_NUMBER));
  TEST_ASSERT_FALSE(VISTMACH_Init(&stMachineInstatnce, NULL, STATES_NUMBER));
  TEST_ASSERT_FALSE(VISTMACH_Init(&stMachineInstatnce, statesArray, 0));
}

// ===================================================================================
// Test VISTMACHINE_AddState =========================================================
TEST(TST_VISTMACHINE, VISTMACH_AddState)
{
  VISTMACH_cbState_t cbSatate = {NULL, NULL};

  VISTMACH_state_t wrongState;
  VISTMACH_InitState(&wrongState, NULL, cbSatate, cbSatate, cbSatate);

  VISTMACH_state_t state1;
  VISTMACH_InitState(&state1, "state1", cbSatate, cbSatate, cbSatate);

  VISTMACH_state_t state2;
  VISTMACH_InitState(&state2, "state2", cbSatate, cbSatate, cbSatate);

  VISTMACH_state_t state2Rep;
  VISTMACH_InitState(&state2Rep, "state2", cbSatate, cbSatate, cbSatate);

  VISTMACH_state_t state3;
  VISTMACH_InitState(&state3, "state3", cbSatate, cbSatate, cbSatate);

  VISTMACH_state_t state4;
  VISTMACH_InitState(&state4, "state4", cbSatate, cbSatate, cbSatate);

  TEST_ASSERT_FALSE(VISTMACH_AddState(NULL, state1));
  TEST_ASSERT_FALSE(VISTMACH_AddState(&stMachineInstatnce, wrongState));
  TEST_ASSERT_TRUE(VISTMACH_AddState(&stMachineInstatnce, state1));
  TEST_ASSERT_TRUE(VISTMACH_AddState(&stMachineInstatnce, state2));
  TEST_ASSERT_FALSE(VISTMACH_AddState(&stMachineInstatnce, state2Rep));
  TEST_ASSERT_TRUE(VISTMACH_AddState(&stMachineInstatnce, state3));
  TEST_ASSERT_FALSE(VISTMACH_AddState(&stMachineInstatnce, state4));
}

// ==================================================================================
// Asset of test environment for VISTMACH_Runtime

typedef struct {
  int result;
  int a;
  int b;
}context1_t;

void entry1(void* this, void* context) {
  context1_t* context1 = (context1_t*)context;
  context1->result = 0;
  context1->a = 21;
  context1->b = 132;
}

void pending1(void* this, void* context) {
  context1_t* context1 = (context1_t*)context;
  context1->result = context1->b + context1->a;
  
  VISTMACH_states_t* thisMachine = (VISTMACH_states_t*)this;
  VISTMACH_SwitchToStateByIndex(thisMachine, thisMachine->activeState + 1);
}

void exit1(void* this, void* context)
{
  context1_t* context1 = (context1_t*)context;

  TEST_ASSERT_EQUAL_INT32(context1->result, context1->b + context1->a);
}

#define SUBSTATES_NUMBER 1
static VISTMACH_state_t subStatesArray[SUBSTATES_NUMBER];

static bool subFunc(void) {
  return true;
}

typedef struct 
{
  bool (*someFunc)(void);
}subContext_t;

void subPending(void* this, void* context) {

  TEST_ASSERT_EQUAL_INT64((int64_t)subStatesArray, (int64_t)(((VISTMACH_states_t*)this)->state));

  subContext_t* subContext = (subContext_t*)context;

  TEST_ASSERT_TRUE(subContext->someFunc());
  VISTMACH_Stop((VISTMACH_states_t*)this);
}

typedef struct 
{
  VISTMACH_states_t submachine;
}context2_t;

void entry2(void* this, void* context) {
  context2_t* context2 = (context2_t*)context;
  VISTMACH_Init(&context2->submachine, subStatesArray, SUBSTATES_NUMBER);

  subContext_t subContext;
  subContext.someFunc = subFunc;
  VISTMACH_cbState_t subEntryCb = {NULL, NULL};
  VISTMACH_cbState_t subPendingCb = {subPending, (void*)&subContext};
  VISTMACH_cbState_t subExitCb = {NULL, NULL};
  VISTMACH_state_t subState;
  VISTMACH_InitState(&subState, "subState", subEntryCb, subPendingCb, subExitCb);
  VISTMACH_AddState(&context2->submachine, subState);
  VISTMACH_Start(&context2->submachine);
}

void pending2(void* this, void* context) {
  context2_t* context2 = (context2_t*)context;

  if(false == VISTMACH_Runtime(&context2->submachine))
    VISTMACH_Stop((VISTMACH_states_t*)this);
}

void exit2(void* this, void* context) {
  TEST_ASSERT_EQUAL_INT64((int64_t)statesArray, (int64_t)(((VISTMACH_states_t*)this)->state));
}

// ===================================================================================
// Test VISTMACHINE_Runtime ==========================================================
TEST(TST_VISTMACHINE, VISTMACH_Runtime)
{
  context1_t context1 = {0, 0, 0};
  VISTMACH_cbState_t cbEntry1 = {entry1, (void*)&context1};
  VISTMACH_cbState_t cbPending1 = {pending1, (void*)&context1};
  VISTMACH_cbState_t cbexit1 = {exit1, (void*)&context1};
  VISTMACH_state_t state1;
  VISTMACH_InitState(&state1, "state1", cbEntry1, cbPending1, cbexit1);

  context2_t context2;
  VISTMACH_cbState_t cbEntry2 = {entry2, (void*)&context2};
  VISTMACH_cbState_t cbPending2 = {pending2, (void*)&context2};
  VISTMACH_cbState_t cbexit2 = {exit2, (void*)&context2};
  VISTMACH_state_t state2;
  VISTMACH_InitState(&state2, "state2", cbEntry2, cbPending2, cbexit2);

  VISTMACH_AddState(&stMachineInstatnce, state1);
  VISTMACH_AddState(&stMachineInstatnce, state2);

  VISTMACH_Start(&stMachineInstatnce);

  while(VISTMACH_Runtime(&stMachineInstatnce));

}