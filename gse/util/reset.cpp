/*
 * Tyler Anderson Tue Dec 20 13:02:23 EST 2011
 * Simple program to test the speed at which we can
 * clear the master trigger
 */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "HTCD2.h"
#include "tcd2_defs.h"
#include "tcd2_err.h"
#include "error_resolve.h"
#include "command_resolve.h"

using namespace std;

int main()
{
  
  HTCD2 tcdinstr;
  if( error_resolve(tcdinstr.open(0x04b4,0x0098))) 
    return -1;

  unsigned int cmd = GET, node = ROSTACK, param = MODE;
  unsigned short value = 0;
  
  printf("Resetting and reloading the firmware\n");
  error_resolve(tcdinstr.tcd2_fx2lp_reset());
  printf("Firmware reset and reloaded\n");
  
  // Wed Jan  4 15:11:43 EST 2012
  usleep(10000000); // 50 ms

  printf("Sending command: GET ROSTACK MODE \n");
  error_resolve(tcdinstr.tcd2_execute(cmd,node,param,&value));
  printf("Response value: ");
  if( value & 0x8000 )
    printf("ERROR ");
  printf("%d\n", value & 0x0FFF);

  printf("Done\n");
  if( error_resolve(tcdinstr.close()) )
    return -1;
    
  return 0;
}
