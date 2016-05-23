/*
 * Tyler Anderson Tue Dec 20 13:02:23 EST 2011
 * Simple program for sending commands
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HTCD2.h"
#include "tcd2_defs.h"
#include "tcd2_err.h"
#include "command_resolve.h"
#include "error_resolve.h"

using namespace std;

int main()
{
  
  HTCD2 tcdinstr;
  int retval = 0;
  if( error_resolve(tcdinstr.open(0x04b4,0x0098))) 
    return -1;

  char sendbuf[100];
  unsigned int cmd = 0, node = 0, param = 0;
  unsigned short value = 0;
  
  printf("\nEnter commands in pneumonic form, and the "
	 "\nresponse will be shown, along with any "
	 "\nerror messages\n");

  while(1)
    {
      printf("------------------------------------------------\n");
      printf("Send command: \n");
      memset(sendbuf,'\0',100);
      cin.getline(sendbuf,100);
      if(sendbuf[strlen(sendbuf)-1] != ' ') // put space as last thing
	sendbuf[strlen(sendbuf)] = ' ';

      printf("\n");

      if (command_resolve(sendbuf,cmd,node,param,value) )
	continue;
	      
      error_resolve(tcdinstr.tcd2_execute(cmd,node,param,&value));
      printf("Response value: ");
      if( value & 0x8000 )
	printf("ERROR ");
      printf("%d\n", value & 0x0FFF);
    }
  
  if( error_resolve(tcdinstr.close()) )
    return -1;
    
  return 0;
}
