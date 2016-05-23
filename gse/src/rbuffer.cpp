/*
 * Tyler Anderson Sun Oct 17 11:13:21 EDT 2010
 * This is the DAQ raw buffer class implementation
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "rbuffer.h"

const int DISPLAYSIZE = 11*MAXRBUF+1;
char displaybuf[DISPLAYSIZE];

rbuffer::rbuffer()
{
  for(int i = 0; i < MAXRBUF; i++)
    {
      cmdbuf[i].target = 0x00;
      cmdbuf[i].param = 0x00;
      cmdbuf[i].datarg = 0x0000;
    }
  Ncmd = 0;
}

rbuffer::~rbuffer()
{
  ;
}

void rbuffer::clear()
{
  for(int i = 0; i < MAXRBUF; i++)
    {
      cmdbuf[i].target = 0x00;
      cmdbuf[i].param = 0x00;
      cmdbuf[i].datarg = 0x0000;
    }
  Ncmd = 0;
}

char * rbuffer::getbuf()
{
  char temp[6];
  memset(temp,'\0',5);
  memset(displaybuf,'\0',DISPLAYSIZE);

  for(int i = 0; i < MAXRBUF; i++)
    {
      sprintf(temp,"%02X ",cmdbuf[i].target);
      
      displaybuf[11*i] = temp[0];
      displaybuf[11*i+1] = temp[1];
      displaybuf[11*i+2] = temp[2];
      
      sprintf(temp,"%02X ",cmdbuf[i].param);
      
      displaybuf[11*i+3] = temp[0];
      displaybuf[11*i+4] = temp[1];
      displaybuf[11*i+5] = temp[2];
      
      sprintf(temp,"%04X ",cmdbuf[i].datarg);
      displaybuf[11*i+6] = temp[0];
      displaybuf[11*i+7] = temp[1];
      displaybuf[11*i+8] = temp[2];
      displaybuf[11*i+9] = temp[3];
      displaybuf[11*i+10] = temp[4];
    }
  displaybuf[DISPLAYSIZE-1] = '\0';
  return displaybuf;
}
