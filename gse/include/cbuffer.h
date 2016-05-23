#ifndef _CBUFFER_H_
#define _CBUFFER_H_
/*
 *
 * Tyler Anderson Fri Oct 15 20:43:55 EDT 2010
 * Buffer class
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////
// TBA Fri Oct 15 21:52:43 EDT 2010
// c is for character!
// buffer with various buffer commands. I was doing
// all this manually, and then realized that was stupid, and
// it made the code look cluttered to have to write a few lines
// of code every time I wanted to perform an operation on a buffer!
//
///////////////////////////////////////////////////////////////////////
class cbuffer 
{
 public:
  //////////////////////////////////////////////////////////////////////////
  // Constructor
  //////////////////////////////////////////////////////////////////////////
  cbuffer(int N = 20000);
  
  /////////////////////////////////////////////////////////////////////////
  // Operations on Nsize
  ////////////////////////////////////////////////////////////////////////
  
  // return the current size of buf
  int getsize();

  //////////////////////////////////////////////////////////////////////////
  // Operations on pos
  //////////////////////////////////////////////////////////////////////////
  // reset pos to zero
  void posbegin();

  // set pos to the end of buf. Nice for parsing backwards.
  void posend();

  // decrement pos until the floor
  // return 1 if you were able to decrement pos
  // return 0 if you hit the floor
  int posdown();

  // increment pos to the until the ceiling
  // return 1 if you were able to increment
  // return 0 if you hit the ceiling
  int posup();

  // return pos
  int getpos();

  // check if the given poition i spaces from pos is valid
  // return 1 if it is
  // return 0 if it is not
  int validpos(int i = 0);

  //////////////////////////////////////////////////////////////////////////
  // Operations on buf
  /////////////////////////////////////////////////////////////////////////
  // check if buffer is full
  // return 1 if it's full
  // return 0 if it's not
  int full();

  // put a character in buf and advance one place
  // return 1 if you are able to put the char in
  // return 0 if you are trying to insert on a full buffer
  int putchar(char c='\0');

  // return the char at the current and increment pos
  // return '\0' if your at the end of buf
  char getchar();

  // remove the character one space back from pos and dec pos until floor
  // if you are able to remove a char okay, return 1
  // if you hit the floor, return 0
  int backspace();
   
  // return the char i positions away from pos
  // if that is not a valid position, return '\0'
  char peeki(int i = 0);

  // add str to the buf
  // return 1 if added, 0 otherwise
  int add(char * str = "");

  // return buf
  char * getbuf();

  // starting at pos, search for the characters in the sentinel C-string, and
  // return a C-string (i.e. NULL terminated) of characters between pos
  // and where you hit the sentinel
  //
  // A useful sidenote: if you want to dump the rest of the buffer, starting
  // at the current position, just use a '\0' sentinel
  char * gettosent(char * sentinel);

  // clear out buf and reset pos
  void clear();

  char * buf; // the buffer
  int Nsize; // Size of buffer
  int pos; // position in buffer

  /////////////////////////////////////////////////////////////////////////
  // Destructor
  /////////////////////////////////////////////////////////////////////////
  virtual ~cbuffer();
};

#endif
