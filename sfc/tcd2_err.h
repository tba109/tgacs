/*
 * Tyler Anderson
 * Tue Dec 20 21:35:10 EST 2011
 * These are error definitions
 */
#ifndef _TCD2_ERR_H_
#define _TCD2_ERR_H_

#define TCD2USB_OPEN_ERR 1
#define TCD2USB_SYS_FIND_BUSSES_ERR 2
#define TCD2USB_SYS_FIND_DEVICES_ERR 3
#define TCD2USB_FIND_DEVICE_ERR 4
#define TCD2USB_CLAIM_ERR 5
#define TCD2USB_ALTINTERFACE_ERR 6
#define TCD2USB_WRITE_ERR 7
#define TCD2USB_READ_ERR 8
#define TCD2USB_WRITE_TOO_MANY_BYTES_ERR 9

#define TCD2CMD_CHECK_NOP_ERR 10
#define TCD2CMD_CHECK_GET_ERR 11
#define TCD2CMD_CHECK_SET_ERR 12
#define TCD2CMD_CHECK_NODE_ERR 13
#define TCD2CMD_CHECK_PARAM_ERR 14
#define TCD2CMD_CHECK_HV_ERR 15
#define TCD2CMD_CHECK_ROSTACK_ERR 16

#define TCD2EVT_EVENT_BYTE_NUM_ERR 17

#define TCD2CMD_CHECK_CLR_NODE_ERR 18
#define TCD2CMD_CHECK_CLR_PARAM_ERR 19

#define TCD2USB_OPEN_IHX_FILE_ERR 20
#define TCD2USB_READ_IHX_FILE_ERR 21
#define TCD2USB_IHX_FORMAT_VIOLATION1_ERR 22
#define TCD2USB_IHX_FORMAT_VIOLATION2_ERR 23
#define TCD2USB_IHX_NBYTES_ERR 24
#define TCD2USB_IHX_FORMAT_VIOLATION3_ERR 25
#define TCD2USB_IHX_FORMAT_VIOLATION4_ERR 26
#define TCD2USB_CHECKSUM_ERR 27
#define TCD2USB_WRITE_RAM_ERR 28
#define TCD2USB_UNKNOWN_TYPE_ERR 29
#define TCD2USB_WRITE_RAM_CONNECTION_ERR 30
#define TCD2USB_RESET_FX2LP_ERR 31
#define TCD2USB_RESTART_FX2LP_ERR 32

#endif
