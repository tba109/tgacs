//////////////////////////////////////////////////////
// Tyler Anderson Tue Dec 20 16:58:54 EST 2011
// This program sets the startup parameters and
// then prints the TCD configuration
//////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "HTCD2.h"
#include "tcd2_defs.h"
#include "crmevt_fake.h" // TODO: replace this with the real header
#include "command_resolve.h"
#include "error_resolve.h"

int main()
{
  tcf_data tcf_for_tcd;
  tcf_data tcf_for_cd;

  HTCD2 tcdinstr;
  
  const char concnames[10][5] = {"PXL","PXR","PYL","PYR","MXL","MXR","MYL","MYR","S3","CD"};
  const char params[18][10] = {"THR00","THR01","THR02","THR03",
			       "THR10","THR11","THR12","THR13",
			       "LEVEL0","LEVEL1",
			       "DISC0","DISC1","DISC2","DISC3",
			       "HV00","HV01","HV10","HV11"};
  
  
  const char values[18][10] = {"200", "550", "500", "300",
			       "200", "550", "500", "300",
			       "3000", "3000",
			       "800","800","800","800",
			       "0", "0","0","0"};
  
  /*
  const char values[18][10] = {"100", "200", "300", "400",
			       "500", "600", "700", "800",
			       "900", "1000",
			       "1100","1200","1300","1400",
			       "1500", "1600","1700","1800"};
  */

  ////////////////////////////////////////////////////////////////////////
  // Open the USB system 
  ////////////////////////////////////////////////////////////////////////
  printf("main: Opening TCD2 USB device\n");
  if( error_resolve(tcdinstr.open(0x04b4,0x0098)) )
    return -1;
  
  ///////////////////////////////////////////////////////////////////////
  // Set the defaults
  ///////////////////////////////////////////////////////////////////////
  char sendbuf[100];
  unsigned int cmd = 0, node = 0, param = 0;
  unsigned short value = 0;
  
  printf("Sending defaults\n");
  for( int i = 0; i < 10; i++ )
    {
      for( int j = 0; j < 18; j++ )
	{
	  memset(sendbuf,'\0',100);
	  sprintf(sendbuf,"%s %s %s %s ","SET",concnames[i],params[j],values[j]);
	  if ( command_resolve(sendbuf,cmd,node,param,value) )
	    continue;
	  
	  error_resolve(tcdinstr.tcd2_execute(cmd,node,param,&value));
	  // printf("SET = %d, STACK = %d, PARAM = %d, value = %d\n",cmd,node,param,value);
	}
    }
  
  printf("\n\n");
  
  ///////////////////////////////////////////////////////////////////////
  // Get the TCD configuration
  ///////////////////////////////////////////////////////////////////////
  if( error_resolve(tcdinstr.tcd2_getconf(tcf_for_tcd,tcf_for_cd)) )
    return -1;
  
  ///////////////////////////////////////////////////////////////////////
  // Print the configuration
  ///////////////////////////////////////////////////////////////////////
  printf("tcf_for_tcd.h = %x\n",tcf_for_tcd.h);  
  printf("tcf_for_cd.h = %x\n",tcf_for_cd.h);
  printf("\n\n");
  
  printf("       THR00  THR01  THR02  THR03  THR10  THR11  THR12  THR13 LEVEL0 LEVEL1  DISC0  DISC1  DISC2  DISC3  HV00   HV01   HV10   HV11   ");
  
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
