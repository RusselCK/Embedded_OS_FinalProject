/*
 * state.h
 *
 *  Created on: 2021年6月10日
 *      Author: HSNL
 */

#ifndef INCLUDE_TREADMILL_STATE_H_
#define INCLUDE_TREADMILL_STATE_H_

#include <treadmill/gc.h>

typedef struct state_s {
  TmStateHeader gc;
  Tm_DArray *rootset;
} State;

//State* State_new();
void State_destroy(State *state);

#endif /* INCLUDE_TREADMILL_STATE_H_ */
