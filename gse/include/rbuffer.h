#ifndef _RBUFFER_H_
#define _RBUFFER_H_

////////////////////////////////////////////////////////////////////////////
// r stands for raw. This is the tcd raw buffer.
////////////////////////////////////////////////////////////////////////////
//64B buffer, 4B/command, so 16 command slots in buffer 
const int MAXRBUF = 16; 
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef struct
{
  BYTE target; // b7..b5 define GET (000) or SET (001). 
  // b4..b0 define which data node
  BYTE param;
  WORD datarg;
} tcd2_command;
  
class rbuffer 
{
 public:
  rbuffer();
  virtual ~rbuffer();
  // Clear the raw buffer
  void clear();
  char * getbuf();
  tcd2_command cmdbuf[MAXRBUF]; // command buffer
  unsigned char Ncmd; // Number of commands stored in the command buffer
}; 

#endif
