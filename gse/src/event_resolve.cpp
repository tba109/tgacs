#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event_resolve.h"

tcd_stack stacks[10];

void FillData(unsigned char * buf, FILE * fp)
{
  // Need to do some better error checking than this, maybe
  // move the open into here...
  if( fp <= 0 )
    return;
  ////////////////////////////////////////////////////////////
  // Initialize the stack names
  ///////////////////////////////////////////////////////////
  for( int i = 0 ; i < 10; i++ )
    sprintf(stacks[i].name,"%s",stacknames[i]);

  ////////////////////////////////////////////////////////////
  // Taken from gse_daq's TCD2Data::FillData() routine
  ///////////////////////////////////////////////////////////
  int rbc = 0;
  // Parse the raw data into its appropriate size
  // We read CS1 (PD), CS2 (TDC), CS0 (Trig)
  // PD0, standard ordering of PXL, PXR,...S3, CD
  for(int j = 0; j < 10; j++)
    for(int i = 0; i < 4; i++)
      {
	stacks[j].pddata[0][i] = 
	  ((buf[rbc+1] << 8) & 0x0F00) | (buf[rbc] & 0x00FF);
	rbc+=2;
	fprintf(fp,"%d ",stacks[j].pddata[0][i]);
      }
  
  // PD1, standard ordering of PXL, PXR,...S3, CD
  for(int j = 0; j < 10; j++)
    {
      for(int i = 0; i < 4; i++)
	{
	  stacks[j].pddata[1][i] = 
	    ((buf[rbc+1] << 8) & 0x0F00) | (buf[rbc] & 0x00FF);
	  rbc+=2;
	  fprintf(fp,"%d ",stacks[j].pddata[1][i]);
	}
    }
  
  // TDC0, standard ordering of PXL, PXR,...S3, CD
  for(int j = 0; j < 10; j++)
    {
      for(int i = 0; i < 4; i++)
	{
	  stacks[j].tdcdata[0][i] =   
	    ((buf[rbc+1] << 8) & 0x0F00) | (buf[rbc] & 0x00FF);
	  rbc+=2;
	  fprintf(fp,"%d ",stacks[j].tdcdata[0][i]);
	}
      // Get VALID0
      stacks[j].valid[0] = buf[rbc];
      rbc+=2;
      fprintf(fp,"%d ",stacks[j].valid[0]);
    }
  
  // TDC1, standard ordering of PXL, PXR,...S3, CD
  for(int j = 0; j < 10; j++)
    {
      for(int i = 0; i < 4; i++)
	{
	  stacks[j].tdcdata[1][i] =   
	    ((buf[rbc+1] << 8) & 0x0F00) | (buf[rbc] & 0x00FF);
	  rbc+=2;
	  fprintf(fp,"%d ",stacks[j].tdcdata[1][i]);
	}
      // Get VALID1
      stacks[j].valid[1] = buf[rbc];
      rbc+=2;
      fprintf(fp,"%d ",stacks[j].valid[1]);
    }
  
  // Trigger stuff, standard ordering of PXL, PXR,...S3, CD
  for(int j = 0; j < 10; j++)
    {
      stacks[j].status = buf[rbc] & 0x0F; // status bits
      rbc+=2;
      fprintf(fp,"%d ",stacks[j].status);
      stacks[j].ecount = buf[rbc]; // event count bits--the running count since reset
      rbc+=2; 
      fprintf(fp,"%d ",stacks[j].ecount);
      
      stacks[j].eventno = // Event#, ordered as LSW MSW in rawdbuf 
	( (buf[rbc+3] << 24) & 0xFF000000 ) |
	( (buf[rbc+2] << 16) & 0x00FF0000 ) |
	( (buf[rbc+1] <<  8) & 0x0000FF00 ) |
	( (buf[rbc]        ) & 0x000000FF );
      rbc += 4;
      fprintf(fp,"%d ",stacks[j].eventno);
    }      
  fprintf(fp,"\n");
}

// Convert a hex number to a four bit
char * fourbitstr(int num)
{
  static char retstr[5];
  for(int i = 4; i != 0; i--)
    {
      retstr[i-1] = num & 1 ? '1' : '0';
      num /= 2;
    }
  retstr[4] = '\0';
  return retstr;
}

void PrintAll()
{
  printf("Stack     EVENTNO   STATUS   ECOUNT   TDC00   TDC01   TDC02   TDC03   VALID0   PD00   PD01   PD02   PD03   TDC10   TDC11   TDC12   TDC13   VALID1   PD10   PD11   PD12   PD13");

  for( int i = 0; i < 10; i++ )
    {
      printf("\n");
      printf("%5s   ",stacks[i].name);
      printf("%10d   ",stacks[i].eventno);
      printf("%6s   ",fourbitstr(stacks[i].status));
      printf("%6d   ",stacks[i].ecount);
      
      for( int k= 0; k < 4; k++ )
	printf("%5d   ",stacks[i].tdcdata[0][k]);
      printf("%6s   ",fourbitstr(stacks[i].valid[0]));
      
      for( int k= 0; k < 4; k++ )
	printf("%4d   ",stacks[i].pddata[0][k]);
      
      for( int k= 0; k < 4; k++ )
	printf("%5d   ",stacks[i].tdcdata[1][k]);
      printf("%6s   ",fourbitstr(stacks[i].valid[1]));
      
      for( int k= 0; k < 4; k++ )
	printf("%4d   ",stacks[i].pddata[1][k]);
	
      
    }
  printf("\n");
}
