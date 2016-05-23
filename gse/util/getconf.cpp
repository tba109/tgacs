//////////////////////////////////////////////////////
// Tyler Anderson Tue Dec 20 16:58:54 EST 2011
// Get the current configuration and print it
//////////////////////////////////////////////////////
#include <stdio.h>
#include "HTCD2.h"
#include "tcd2_defs.h"
#include "error_resolve.h"

int main()
{
  tcf_data tcf_for_tcd;
  tcf_data tcf_for_cd;

  HTCD2 tcdinstr;

  printf("main: Opening TCD2 USB device\n");
  if( error_resolve(tcdinstr.open(0x04b4,0x0098)) )
    return -1;
    
  if( error_resolve(tcdinstr.tcd2_getconf(tcf_for_tcd,tcf_for_cd)) )
    return -1;
    
  // Print the data read for testing purposes
  printf("tcf_for_tcd.h = %x\n",tcf_for_tcd.h);  
  printf("tcf_for_cd.h = %x\n",tcf_for_cd.h);
  printf("\n\n");
  
  printf("       THR00  THR01  THR02  THR03  THR10  THR11  THR12  THR13 LEVEL0 LEVEL1  DISC0  DISC1  DISC2  DISC3  HV00   HV01   HV10   HV11   ");
  
  const char concnames[10][5] = {"PXL","PXR","PYL","PYR","MXL","MXR","MYL","MYR","S3","CD"};

  for( int i = 0; i < 162; i++ )
    {
      if( !(i%18) )
	{
	  printf("\n");
	  printf("%3s:   ",concnames[i/18]);
	}
      if( tcf_for_tcd.data[i] & 0x8000 )
	printf("????   ");
      else
	printf("%4d   ",tcf_for_tcd.data[i]);
      
    }
  
  // Print the CD data read for testing purposes

  for( int i = 0; i < 18; i++ )
    {
      if( !(i%18) )
	{
	  printf("\n");
	  printf("%3s:   ",concnames[9]);
	}
      if( tcf_for_cd.data[i] & 0x8000 )
	printf("????   ");
      else
	printf("%4d   ",tcf_for_cd.data[i]);
    } 
  printf("\n\n");
  
  if( error_resolve(tcdinstr.close()) )
    return -1;
  
  return 0;
}
