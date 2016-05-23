/*
 * Tyler Anderson Wed Dec 21 00:55:54 EST 2011
 * Simple program to show that we are receiving events properly
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HTCD2.h"
#include "tcd2_defs.h"
#include "tcd2_err.h"
#include "error_resolve.h"

using namespace std;

int main()
{
  int count_tcddata = 0;
  unsigned char tcd_buf[1000];
  int count_cddata = 0;
  unsigned char cd_buf[1000];
  HTCD2 tcdinstr;

  const char concnames[10][5] = {"PXL","PXR","PYL","PYR","MXL","MXR","MYL","MYR","S3","CD"};

  const char dataaddresses[16][10] = {"TDC00","TDC01","TDC02","TDC03","TDC10","TDC11","TDC12","TDC13","PD00","PD01","PD02","PD03","PD10","PD11","PD12","PD13"};

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
  unsigned short * curP;
  int nvalid = 0;
  int valid_ids[9] = {0,0,0,0,0,0,0,0,0};
  int compression = 0;
  unsigned int num_good[9] = {0,0,0,0,0,0,0,0,0};
  unsigned short slip_mask;
  unsigned char address = 0;
  unsigned short data = 0;
  
  int total = 0;
  
  error_resolve(tcdinstr.clear_event());
  while( 1/*total < 1000*/ )
    {
      total++;
      // start of loop initializations
      nvalid = 0;
      for( int i = 0; i < 9; i++ )
	valid_ids[i] = 0;      
      compression = 0;
      for( int i = 0; i < 9; i++ )
	num_good[i] = 0;
      slip_mask = 0;
      address = 0;
      data = 0;
    
      memset(tcd_buf,0,1000);
      memset(cd_buf,0,1000);
      count_tcddata = 0;
      count_cddata = 0;
      
      printf("--------------------------------------------------------------------\n");
      printf("Loop %d\n",total);
      
      // Read out the data
      error_resolve(tcdinstr.tcd2_readout(&count_tcddata,tcd_buf,&count_cddata,cd_buf)) ;
      // error_resolve(tcdinstr.clear_event());
     
      printf("count_tcddata = %d \n",count_tcddata);
      
      // for( int i = 0; i < count_tcddata; i++ )
      // {
      // printf("%x ",tcd_buf[i]);
      // }
      // printf("\n");
      
      curP = (unsigned short*) tcd_buf;
      
      // Print the number of bytes
      printf("%d byte packet \n", *curP++);
      
      // Event number
      printf("EVENTNO = %d\n",(unsigned int)(*curP));
      
      curP++;
      curP++;
      
      // First we need to count the number of valids we expect
      
      for( int conc = 0; conc < 9; conc++ )
	if( (*curP) & (0x0001 << conc) )
	  valid_ids[nvalid++] = conc;
      
      // Figure out what type of compression we are doing
      
      if( ((*curP & 0x0600) >> 9 ) == 1 )
	compression = 1;
      else
	compression = 2;
      
      printf("compression = %d\n",compression);
      
      printf("%d concentrators with at least some valid data\n",nvalid);
      
      // Now read the number of valid items for each concentrator, as determined from 
      // the entries in valid_ids
      for( int nv = 0; nv < nvalid; nv++ )
	{
	  if( !((nv+3)%4) )
	    curP++;
	  
	  num_good[nv] = ((unsigned int)(*curP >> (((nv+3)%4)*4))) & 0x0000000F ;
	  num_good[nv]+=1;
	  printf("Shifting right %d positions, ",(((nv+3)%4)*4));  
	}
      curP++;
            
      if( compression == 2 )
	{
	  curP++; // timed_out mask, which doesn't mean too much here
	  slip_mask = *curP++;
	}

      for( int nv = 0; nv < nvalid; nv++ )
	{
	  printf("\n");
	  printf("%s: ",concnames[valid_ids[nv]]);
	  printf("# good items = %d, ",num_good[nv]);
	  for( unsigned int ng = 0; ng < num_good[nv]; ng++ )
	    {
	      // extract the data
	      address = 0;
	      data = 0;
	      address = (*curP & 0xF000 ) >> 12;
	      data = *curP & 0x0FFF;
	      curP++;
	      
	      // Now print the result:
	      printf("%s = %d, ",dataaddresses[address],data);
	    }
	  printf("\n");
	}
      printf("\n");
      printf("calc_crc16 = %d\n\n",*curP++);

    }  
  
  ////////////////////////////////////////////////////////////
  // Close down the TCD USB connection
  ////////////////////////////////////////////////////////////
  if( error_resolve(tcdinstr.close()) )
    return -1;  
  
  return 0;
}
  
