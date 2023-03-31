/*****************************************************************************

Author(s) or Contributor(s): Version 0.0  ~<NAME> 2023 

File: bin_srch_main.c

Description: Main is here

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

/* Main Starts Here: ********************************************************/

int main(int argc, char *argv[])
{
  
  int array[] = {1,4,3,7,8,10,12,13,15,20};

  int result = bin_search(array, 4, 0, 10);

  printf("\nIn Main... Result is: %d\n\n", result);

  return(0);
}

// End of Main
/****************************************************************************/

/****************************************************************************/
// End bin_srch_main.c