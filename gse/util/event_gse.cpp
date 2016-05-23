/*
 * Tyler Anderson Sat Jan  7 11:18:15 EST 2012
 * Print all the data, and write it into a rootTree
 * (I'll freely admit this program is kinda crufty. 
 *  What do you expect for 20 minutes of quick
 *  coding? The Mona Lisa? It's only for diagnositc
 *  purposes anyway.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "HTCD2.h"
#include "tcd2_defs.h"
#include "tcd2_err.h"
#include "error_resolve.h"
#include "event_resolve.h"

using namespace std;

int main()
{
  HTCD2 tcdinstr;
  FILE * fp;
      
  ////////////////////////////////////////////////////////////
  // Open the TCD USB connection
  ////////////////////////////////////////////////////////////
  if( error_resolve(tcdinstr.open()) )
    return -1;

  ////////////////////////////////////////////////////////////
  // Put the TCD in event mode, and set CLEV to 1
  ////////////////////////////////////////////////////////////
  unsigned short value = EVT_MODE;
  if( error_resolve(tcdinstr.tcd2_execute(SET,ROSTACK,MODE,&value)))
    return -1;

  value = 1;
  if( error_resolve(tcdinstr.tcd2_execute(SET,ROSTACK,CLEV,&value)))
    return -1;

  ////////////////////////////////////////////////////////////
  // Readout and print the event data
  ///////////////////////////////////////////////////////////
  unsigned char buf[512];
  int total = 0;

  ////////////////////////////////////////////////////////////
  // Open a file for writing
  // TODO add a timestamp here
  ///////////////////////////////////////////////////////////
  fp = fopen("data.txt","w");

  error_resolve(tcdinstr.clear_event());
  while( 1/*total < 1000*/ )
    {
      total++;
      
      memset(buf,0,512);
      
      printf("--------------------------------------------------------------------\n");
      printf("Loop %d\n",total);

      // Read out the data
      error_resolve(tcdinstr.tcd2_readout_gse(buf));
      FillData(buf,fp);
      PrintAll();
      error_resolve(tcdinstr.clear_event());
      // TBA debug
      // value = 1;
      // error_resolve(tcdinstr.tcd2_execute(SET,ROSTACK,CLEV,&value));
      // printf("value = %d\n",value);

    }
  
  ////////////////////////////////////////////////////////////
  // Close down the TCD USB connection
  ////////////////////////////////////////////////////////////
  if( error_resolve(tcdinstr.close()) )
    return -1;  
  
  return 0;
}

