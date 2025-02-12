#include "crt.h"
#include "board.h"
#include "cli.h"
#include "help.h"
#include "led.h"
#include "rtc.h"
#include "wdt.h"

const CliCmdType *gCmdArray[] = {
	&CMD_HELP,
	&CMD_VERSION,
	&CMD_LIST,
	&CMD_BOARD,
	&CMD_LED_READ,
	&CMD_LED_WRITE,
	&CMD_CRT_IN_READ,
	&CMD_CRT_RMS_IN_READ,
	&CMD_CRT_IN_CAL,
	&CMD_CRT_RANGE_READ,
	&CMD_CRT_RANGE_WRITE,
	&CMD_CRT_SNS_TYPE_READ,
	&CMD_CRT_SNS_TYPE_WRITE,
	&CMD_RTC_GET,
	&CMD_RTC_SET,
	&CMD_WDT_RELOAD,
	&CMD_WDT_GET_PERIOD,
	&CMD_WDT_SET_PERIOD,
	&CMD_WDT_GET_INIT_PERIOD,
	&CMD_WDT_SET_INIT_PERIOD,
	&CMD_WDT_GET_OFF_PERIOD,
	&CMD_WDT_SET_OFF_PERIOD,
	&CMD_WDT_GET_RESET_COUNT,
	&CMD_WDT_CLR_RESET_COUNT,
	0
};
