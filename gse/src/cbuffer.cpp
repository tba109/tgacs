/*
 * TBA Fri Oct 15 20:45:57 EDT 2010
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cbuffer.h"

///////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
cbuffer::cbuffer(int N)
{
  Nsize = N;
  buf = new char[Nsize];
  memset(buf,'\0',Nsize);
  pos = 0;
}

///////////////////////////////////////////////////////////////////////////
// Operations on Nsize
///////////////////////////////////////////////////////////////////////////
int cbuffer::getsize()
{
  return Nsize;
}

///////////////////////////////////////////////////////////////////////////
// Operations on pos
//////////////////////////////////////////////////////////////////////////
void cbuffer::posbegin()
{
  pos = 0;
}

void cbuffer::posend()
{
  pos = Nsize - 1;
}

int cbuffer::posdown()
{
  if( pos > 0 )
    {
      pos--;
      return 1;
    }
  else
    return 0;
}

int cbuffer::posup()
{
  if( pos <= Nsize-1)
    {
      pos++;
      return 1;
    }
  else 
    return 0;
}

int cbuffer::getpos()
{
  return pos;
}

int cbuffer::validpos( int i )
{
  if( (getpos()+i <= Nsize-1) && (getpos()+i >= 0) )
    return 1;
  else
    return 0;
}

////////////////////////////////////////////////////////////////////////////
// Operations on buf
////////////////////////////////////////////////////////////////////////////
int cbuffer::full()
{
  if( pos == Nsize - 1 )
    return 1;
  else
    return 0;
}

int cbuffer::putchar(char c)
{
  if( !full() )
    {
      buf[pos] = c; 
      posup();
      return 1;
    }
  else
    {
      fprintf(stdout,"cbuffer: putcur failed! buf is full!\n");
      return 0;
    }
}

char cbuffer::getchar()
{
  char c;
  if( !full() )
    {
      c = buf[pos];
      posup();
      return c;
    }
  else
    return '\0';
}


int cbuffer::backspace()
{
  if(validpos(0))
    {
      posdown();
      buf[getpos()] = '\0';
      return 1;
    }
  else
    return 0;
}

char cbuffer::peeki(int i)
{
  if( validpos(i) )
    return buf[i+getpos()];
  else
    return '\0';
}

int cbuffer::add(char * str)
{
  int i = 0;
  if((int)strlen(str) < Nsize-getpos()-1)
    {
      strcat(buf,str);
      while( i < (int)strlen(str) )
	{
	  posup();
	  i++;
	}
      return 1;
    }
  else
    {
      fprintf(stdout,"cbuffer::add() Could not add \"%s\"! Not enough "
	      "space left in buf!\n",str);
      return 0;
      
    }
}

char * cbuffer::getbuf()
{
  return buf;
}

char * cbuffer::gettosent(char * sentinel)
{
  char temp[getsize()+1];
  memset(temp,'\0',getsize()+1);// terminated C string
  int i = 0;
  int Nsentinels = strlen(sentinel);

  bool stop = 0;
     
  // Bleed off any leading sentinel characters
  for(int j = 0; j < Nsentinels; )
    {
      if(peeki() == sentinel[j])
	{
	  posup();
	  j=0;
	}
      else
	j++;
    }
  
  while( !stop )
    {
      temp[i] = getchar();
      //printf("temp[%d] = %c\n",i,temp[i]);
      for(int j = 0; j < Nsentinels; j++)
	{
	  if(temp[i] == sentinel[j] || temp[i] == '\0')
	    {
	      stop = 1;
	      break;
	    }
	}
      if(!validpos(1))
	break;
      i++;
     }
  return temp;
}

void cbuffer::clear()
{
  memset(buf,'\0',Nsize);
  pos = 0;
}

cbuffer::~cbuffer()
{
  delete [] buf;
  //delete [] temp;
  pos = 0;
  Nsize = 0;
  buf = NULL;
}
