/*
 * state.c
 *
 *  Created on: 2021年6月10日
 *      Author: HSNL
 */
#include <treadmill/state.h>

void
State_destroy(State *state)
{
  Tm_DArray_destroy(state->rootset);
  free(state);
}
