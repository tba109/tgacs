#include <stdio.h>
#include <string.h>
#include "tcd2_defs.h"
#include "cbuffer.h"

//////////////////////////////////////////////////////////////////////////
// These are used for coding/decoding
//////////////////////////////////////////////////////////////////////////
const char Ncmds = 8;
char cmds[Ncmds][12] = {"NOP","GET","SET","INIT","EVENT","SOFTRESET","FLUSH","CLEAR"};
const unsigned char cmdval[Ncmds] = {0,1,2,3,4,5,6,7};

const char Nnodes = 11;
char nodes[Nnodes][11] = {"PXL","PXR","PYL","PYR","MXL","MXR",
			  "MYL","MYR","S3","CD","ROSTACK"};
const unsigned char nodeval[Nnodes] = {0,1,2,3,4,5,6,7,8,9,10};

const char Nparams = 23;
char params[Nparams][10] = {"THR00","THR01","THR02","THR03",
			    "LEVEL0","HV00","HV01",
			    "THR10","THR11","THR12","THR13",
			    "LEVEL1","HV10","HV11",
			    "DISC0","DISC1","DISC2","DISC3",
			    "CLEAR_LT","CLEAR_MT",
			    "MODE","CLEV","STKMSK"};
const unsigned char paramval[Nparams] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22};
const unsigned char imode = 20; // position of mode

const char Nmodes = 3;
char modes[Nmodes][10] = {"INIT_MODE","CMD_MODE","EVT_MODE"};
const unsigned char modevals[Nmodes] = {0,1,2};

int command_resolve(char * text, unsigned int &cmd, unsigned int &node, unsigned int &param, unsigned short &value)
{
  cbuffer cmdbuf; // buffer containing raw commands
  char curcmd[10];
  char curnode[10];
  char curparam[10];
  char curvalue[10];
  int found = 0;
  
  memset(curcmd,'\0',10);
  memset(curnode,'\0',10);
  memset(curparam,'\0',10);
  memset(curvalue,'\0',10);
  
  cmdbuf.add(text);
    
  cmdbuf.posbegin();

  strcpy(curcmd,cmdbuf.gettosent(" ")); // Grab the command part
  curcmd[strlen(curcmd)-1] = '\0'; // Remove the " "

  strcpy(curnode,cmdbuf.gettosent(" ")); // Grab the command part
  curnode[strlen(curnode)-1] = '\0'; // Remove the " "

  strcpy(curparam,cmdbuf.gettosent(" ")); // Grab the command part
  curparam[strlen(curparam)-1] = '\0'; // Remove the " "

  strcpy(curvalue,cmdbuf.gettosent(" ")); // Grab the command part
  curvalue[strlen(curvalue)-1] = '\0'; // Remove the " "

  // Resolve the cmd part
  found = 0;
  for(int i = 0; i < Ncmds; i++)
    {
      if( !strcmp(curcmd,cmds[i]) )
	{
	  cmd = cmdval[i];
	  found = 1;
	  break;
	}
    }
  if( !found )
    {
      printf("CommandResolve: Bad cmd \n");
      return -1;
    }
  
  // Resolve the node part
  found = 0;
  for(int i = 0; i < Nnodes; i++)
    {
      if( !strcmp(curnode,nodes[i]) )
	{
	  node = nodeval[i];
	  found = 1;
	  break;
	}
    }
  if( !found )
    {
      node = 0;
      printf("CommandResolve: Bad node \n");
      return -1;
    }
  
  found = 0;
  for(int i = 0; i < Nparams; i++)
    {
      if( !strcmp(curparam,params[i]) )
	{
	  param = paramval[i];
	  found = 1;
	  break;
	}
    }
  if( !found )
    {
      printf("CommandResolve: Bad param \n");
      return -1;
    }

  value = (unsigned short)atoi(curvalue);
  
  return 0;
}
