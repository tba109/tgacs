/*
 * Tyler Anderson Tue Dec 20 23:11:08 EST 2011
 * This is just a simple error function for the working main programs
 */

#include <stdio.h>
#include "tcd2_err.h"

int error_resolve( int error_code )
{
  switch( error_code )
    {
      
    case 0 :
      break;
      
    case -TCD2USB_OPEN_ERR :
      printf("TCD2Usb: Open failed \n");
      break;
      
    case -TCD2USB_SYS_FIND_BUSSES_ERR :
      printf("TCD2Usb: usb_find_busses failed \n");  
      break;
      
    case -TCD2USB_SYS_FIND_DEVICES_ERR :
      printf("TCD2Usb: usb_find_devices failed \n");  
      break;

    case -TCD2USB_FIND_DEVICE_ERR : 
      printf("TCD2Usb: could not find requested USB device in system device list \n");
      break;

    case -TCD2USB_CLAIM_ERR :
      printf("TCD2Usb: Failed to claim interface 0 \n");  
      break;
      
    case -TCD2USB_ALTINTERFACE_ERR :
      printf("TCD2Usb: Failed to set altinterface 2 \n");

    case -TCD2USB_READ_ERR :
      printf("TCD2Usb: Read attempt failed \n");
      break;
      
    case -TCD2USB_WRITE_ERR :
      printf("TCD2Usb: Write attempt failed \n");
      break;

    case -TCD2USB_WRITE_TOO_MANY_BYTES_ERR :
      printf("TCD2Usb: > 64 bytes for EP1OUT write \n");
      break;
      
    case -TCD2CMD_CHECK_NOP_ERR :
      printf("TCD2CMD: Error in checking NOP \n");
      break;
      
    case -TCD2CMD_CHECK_GET_ERR :
      printf("TCD2CMD: Error in checking GET \n");
      break;

    case -TCD2CMD_CHECK_SET_ERR :
      printf("TCD2CMD: Error in checking SET \n");
      break;
      
    case -TCD2CMD_CHECK_NODE_ERR :
      printf("TCD2CMD: Error in checking node \n");
      break;
    
    case -TCD2CMD_CHECK_PARAM_ERR :
      printf("TCD2CMD: Error in checking parameter \n");
      break;
      
    case -TCD2CMD_CHECK_HV_ERR :
      printf("TCD2CMD: Error in checking HV range \n");
      break;

    case -TCD2CMD_CHECK_ROSTACK_ERR :
      printf("TCD2CMD: Error in checking ROSTACK \n");
      break;
      
    case -TCD2EVT_EVENT_BYTE_NUM_ERR :
      printf("TCD2EVT: Wrong number of bytes for event \n");
      break;

    case -TCD2CMD_CHECK_CLR_NODE_ERR :
      printf("TCD2Cmd: Bad node \n");
      break;

    case -TCD2CMD_CHECK_CLR_PARAM_ERR :
      printf("TCD2Cmd: Bad param \n");
      break;

    case -TCD2USB_OPEN_IHX_FILE_ERR :
      printf("TCD2Usb: Failed to open .ihx file for programming \n");
      break;

    case -TCD2USB_READ_IHX_FILE_ERR :
      printf("TCD2Usb: Failed in reading .ihx file for programming \n");
      break;

    case -TCD2USB_IHX_FORMAT_VIOLATION1_ERR :
      printf("TCD2Usb: IHX format violation type 1 \n");
      break;
      
    case -TCD2USB_IHX_FORMAT_VIOLATION2_ERR :
      printf("TCD2Usb: IHX format violation type 2 \n");
      break;

    case -TCD2USB_IHX_NBYTES_ERR :
      printf("TCD2Usb: Number of bytes either < 0 or > 256 \n");
      break;

    case -TCD2USB_IHX_FORMAT_VIOLATION3_ERR :
      printf("TCD2Usb: IHX format violation type 3 \n");
      break;

    case -TCD2USB_IHX_FORMAT_VIOLATION4_ERR :
      printf("TCD2Usb: IHX format violation type 4 \n");
      break;

    case -TCD2USB_CHECKSUM_ERR :
      printf("TCD2Usb: Checksum mismatch \n");
      break;

    case -TCD2USB_WRITE_RAM_ERR :
      printf("TCD2Usb: Error writing to FX2LP RAM \n");
      break;

    case -TCD2USB_UNKNOWN_TYPE_ERR :
      printf("TCD2Usb: Unknown type of .ihx file \n");
      break;

    case -TCD2USB_WRITE_RAM_CONNECTION_ERR :
      printf("TCD2Usb: Error in trying to write to RAM \n");
      break;

    case -TCD2USB_RESET_FX2LP_ERR :
      printf("TCD2Usb: Error in resetting the FX2LP \n");
      break;
      
    case -TCD2USB_RESTART_FX2LP_ERR :
      printf("TCD2Usb: Error in restarting the FX2LP \n");
      break;
      
    default :
      printf("Unknown error: %d\n",error_code);
      
    }

  return error_code;
}

