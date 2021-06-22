/*
 * vector.c
 *
 *  Created on: 2021年6月10日
 *      Author: HSNL
 */
#include <treadmill/vector.h>

void Vector_each(Object* vector, Vector_iter iter)
{
  Tm_DArray *array = OBJ2ARY(vector);
  int count = Tm_DArray_count(array);

  for(int i=0; i<count; i++) {
    iter((Object* )Tm_DArray_at(array, i));
  }
}
