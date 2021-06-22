/*
 * value_type.h
 *
 *  Created on: Jun 8, 2021
 *      Author: HSNL
 */

#ifndef INC_VALUE_TYPE_H_
#define INC_VALUE_TYPE_H_

typedef enum {
  NumberType = 0,
  ObjectType,
  StringType,
  VectorType,
  MapType,
  ClosureType,
  TrueType,
  FalseType,
  NilType,
} ValueType;

#endif /* INC_VALUE_TYPE_H_ */
