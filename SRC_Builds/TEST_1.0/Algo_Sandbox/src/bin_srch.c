/*****************************************************************************

Author(s) or Contributor(s): Version 0.0  ~<NAME> 2023 

File: bin_srch.c

Description: Sandbox                                              

*****************************************************************************/


/* Includes: ****************************************************************/

#include "../inc/bin_srch.h"

/****************************************************************************/

/* Defines: *****************************************************************/
/****************************************************************************/

/* Typedefs: ****************************************************************/
/****************************************************************************/

/* Globals: *****************************************************************/
/****************************************************************************/

/* Function Definitions: ****************************************************/

/*****************************************************************************

Name:	bin_search()                                         
Purpose: Binary Search
Parameters: Sandbox                                          
Returns: Sandbox                                        

*****************************************************************************/

int bin_search(int *array, int index, int left, int right)
{

  printf("\nTESTING INSIDE FUNCTION CALL\n");

  if (left > right)
    return(-1);

  int mid = left + (right - 1) / 2;

  if (*(array + mid) == index)
    return mid;
  else if (*(array + mid) > index)
    return bin_search(array, index, left, mid - 1);      
  else if (*(array + mid) < index)
    return bin_search(array, index, mid + 1, right);
}

// End bin_search() 
/****************************************************************************/

/****************************************************************************/
// End bin_srch.c