/*
 * vector.h
 *
 *  Created on: 2021年6月10日
 *      Author: HSNL
 */

#ifndef INCLUDE_TREADMILL_VECTOR_H_
#define INCLUDE_TREADMILL_VECTOR_H_

#include <treadmill/object.h>

typedef void (*Vector_iter)(Object* );

void Vector_each(Object* obj, Vector_iter);

#endif /* INCLUDE_TREADMILL_VECTOR_H_ */
