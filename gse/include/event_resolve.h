#ifndef _EVENT_RESOLVE_H_
#define _EVENT_RESOLVE_H_
/*
 * Tyler Anderson Sun Jan  8 10:16:23 EST 2012
 * Functions for performing the mundane tasks of
 * sorting out raw data into a struct and printing it
 */

struct tcd_stack
{
  char name[10]; // name of the stack
  unsigned short pddata[2][4]; // peak detector data
  unsigned short tdcdata[2][4]; // tdc data
  unsigned char valid[4]; // bits 3..0 are tdc valid
  unsigned char status; // bits 3..0 are INTERRUPT, LATCHCONFIRM#, EVENT#, EN_VALID
  unsigned char ecount; // event processed since last reset
  unsigned int eventno; // event number for this event
};

const char stacknames[10][10] = 
  {"pxl","pxr","pyl","pyr","mxl","mxr","myl","myr","s3","cd"};

//////////////////////////////////////////////////////////////////
// Taken from the old gse_daq's TCD2Data::FillData() routine
//////////////////////////////////////////////////////////////////
void FillData(unsigned char * buf, FILE * fp);

/////////////////////////////////////////////////////////////////
// Convert number to a four bit string
/////////////////////////////////////////////////////////////////
char * fourbitstr(int num);

/////////////////////////////////////////////////////////////////
// Print the data nicely
/////////////////////////////////////////////////////////////////
void PrintAll();

#endif
