/*
 * Tyler Anderson Tue Dec 20 13:02:23 EST 2011
 * Simple program to nop all the concentrators
 * once per minute and record the number of failures
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

int main(int argc, char * argv[])
{
  
  HTCD2 tcdinstr;
  if( error_resolve(tcdinstr.open(0x04b4,0x0098))) 
    return -1;

  unsigned int cmd = TCDNOP, param = THR00;
  unsigned short read_value = 0;
  unsigned short expected_value = 0;

  printf("Resetting and reloading the firmware\n");
  error_resolve(tcdinstr.tcd2_fx2lp_reset());
  printf("Firmware reset and reloaded\n");
  
  usleep(500000); // 1/2 second

  int readme[10] = {0,0,0,0,0,0,0,0,0,0};
  char names[10][10] = {"pxl","pxr","pyl","pyr","mxl","mxr","myl","myr","s3","cd"};
  
  // read them all if no argument given
  if(argc < 2)
    for( int i = 0; i < 10; i++)
      readme[i] = 1;
 
  for( int i = 0; i < argc-1; i++)
    {
      for( int j = 0; j < 10; j++)
	{
	  if( !strcmp(names[j],argv[i+1]))
	    {
	      readme[j] = 1;
	      printf("NOPing %s\n",names[j]);
	    }
	}
    }
   
  for(int i = 0; i < 10; i++)
    printf("readme[%d] = %d\n",i,readme[i]);

  int ngood[10] = {0,0,0,0,0,0,0,0,0,0};
  int nerror[10] = {0,0,0,0,0,0,0,0,0,0};
  int nbad[10] = {0,0,0,0,0,0,0,0,0,0};
  int nother[10] = {0,0,0,0,0,0,0,0,0,0};
  int inverted_value = 0;

  read_value = 0;
  expected_value = 0;

  while(1)
    {
      sleep(1);
      printf("---------------------------------------------------------\n");
      for( int i = 0; i < 10; i++)
	{
	  if( readme[i] == 0 )
	    continue;
	  
	  if( expected_value == 4096 )
	    expected_value = 0;
	  
	  inverted_value = 4095 - expected_value;
	  read_value = expected_value;
	  
	  // printf("%d: expected_value = %d, inverted_value = %d\n",i,expected_value,inverted_value);
	  error_resolve(tcdinstr.tcd2_execute(cmd,i,param,&read_value));
	  // printf("%d: read_value = %d, inverted_value = %d\n",i,read_value,inverted_value);
	  
	  if( read_value & 0x8000 )
	    {
	      nerror[i]++;
	    }
	  else if( (read_value & 0x0FFF) != inverted_value )
	    {
	      nbad[i]++;
	    }
	  else if( (read_value & 0x0FFF) == inverted_value )
	    {
	      ngood[i]++;
	    }
	  else 
	    nother[i]++;
	  
	  
	  printf("%s: ngood = %d, nerror = %d, nbad = %d, nother = %d\n",names[i],ngood[i],nerror[i],nbad[i],nother[i]);
	  // printf("\n");
	  expected_value++;
	}
    }
  
  printf("Done\n");
  if( error_resolve(tcdinstr.close()) )
    return -1;
  
  return 0;
}
