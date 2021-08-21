/*****************************************************************************
Author(s) or Contributor(s): Version .0  ~<NAME> 2021                                                                 

File: ServerMain.c              

Description: Server-side Main                                              

*****************************************************************************/


/* Includes: ****************************************************************/

#include  "../inc/CS_Common.h"

/****************************************************************************/

/* Main Starts Here: ********************************************************/
//
int main(int argc, char *argv[])
{
  // Initialize Local Variables
  int16_t sSrvSOK = SokInit_Handlr();
  // Winsock
  #ifndef LIN
    
    WSADATA wsaData;
    
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
      fprintf(stderr,"[-]WSAStartup() = FAIL");
      exit(1);
    }
    
  #endif
  // Error Handling
  if (sSrvSOK == -1)
  {
    printf("\n[-]Creation of SOCKET = FAIL\n");
    return EXIT_FAILURE;
  }
  
  SLEEP
  
  printf("\n[+]Creation of SOCKET = OK\n\n");

  SrvConnection_Hndlr(sSrvSOK, MAX_CONN, REM_SRV_PORT);
  
  return(0);

}

/****************************************************************************/

/****************************************************************************/
// End ServerMain.c
