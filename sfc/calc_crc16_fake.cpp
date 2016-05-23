/*
 * Tyler Anderson Mon Dec 19 15:40:18 EST 2011
 * This is an ever-so-fake calc_crc function just to 
 * satisfy the external requirements of the 
 * calc_crc16 function in TCD2EvtProc.cpp
 * 
 * UMD should definitely remove this and replace with 
 * whatever function really should service calc_crc16
 */

unsigned short calc_crc16(unsigned char * first, unsigned short second)
{
  return 0;
}
