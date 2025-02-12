#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crt.h"
#include "calib.h"
#include "comm.h"
#include "data.h"

/* bad channel check functoin {{{ */
bool badCrtInCh(int ch) {
	if(!(MIN_CH_NO <= ch && ch <= CRT_IN_CH_NO)) {
		printf("Current input channel out of range![%d..%d]\n", MIN_CH_NO, CRT_IN_CH_NO);
		return true;
	}
	return false;
}
/* }}} */

const CliCmdType CMD_CRT_IN_READ = {/*{{{*/
	"rd",
	2,
	&doCrtInRead,
	"  rd               Read input current value(A)\n",
	"  Usage:           "PROGRAM_NAME" <id> rd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 rd 2 #Read amperage of input current channel #2 on board #0\n",
};
int doCrtInRead(int argc, char *argv[]) {
	if(argc != 4) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	int ch = atoi(argv[3]);
	if(badCrtInCh(ch)) {
		return ARG_RANGE_ERROR;
	}
	// current is signed so we can't use val16Get
	uint8_t buf[ANALOG_VAL_SIZE];
	if (OK != i2cMem8Read(dev, I2C_CRT_IN_VAL1_ADD + (ch - 1) * ANALOG_VAL_SIZE, buf, ANALOG_VAL_SIZE)) {
		return ERROR;
	}
	int16_t raw = 0;
	memcpy(&raw, buf, ANALOG_VAL_SIZE);
	float val = (float)raw / CRT_SCALE;
	printf("%0.2f\n", val);
	return OK;
}/*}}}*/
const CliCmdType CMD_CRT_RMS_IN_READ = {/*{{{*/
	"rmsrd",
	2,
	&doCrtInRmsRead,
	"  rmsrd            Read input current RMS value(A)\n",
	"  Usage:           "PROGRAM_NAME" <id> rmsrd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 rmsrd 2 #Read RMS amperage on input current channel #2 on board #0\n",
};
int doCrtInRmsRead(int argc, char *argv[]) {
	if(argc != 4) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	int ch = atoi(argv[3]);
	if(badCrtInCh(ch)) {
		return ARG_RANGE_ERROR;
	}
	uint8_t buf[ANALOG_VAL_SIZE];
	if (OK != i2cMem8Read(dev, I2C_CRT_IN_RMS_VAL1_ADD + (ch - 1) * ANALOG_VAL_SIZE, buf, ANALOG_VAL_SIZE)) {
		return ERROR;
	}
	uint16_t raw;
	memcpy(&raw, buf, ANALOG_VAL_SIZE);
	float val = (float)raw / CRT_SCALE;
	printf("%0.2f\n", val);
	return OK;
}/*}}}*/

const CliCmdType CMD_CRT_RANGE_READ = {/*{{{*/
	"rrd",
	2,
	&doCrtRangeRead,
	"  rrd              Read full scale for current sensor(A)\n",
	"  Usage:           "PROGRAM_NAME" <id> rrd <channel>\n",
	"  Example:         "PROGRAM_NAME" 0 rrd 2 #Read full scale for current sensor(A) channel #2 on board #0\n",
};
int doCrtRangeRead(int argc, char *argv[]) {
	if(argc != 4) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	int ch = atoi(argv[3]);
	if(badCrtInCh(ch)) {
		return ARG_RANGE_ERROR;
	}
	uint8_t buf[ANALOG_VAL_SIZE];
	if(OK != i2cMem8Read(dev, I2C_CRT_SNS_RANGE1_ADD + (ch - 1) * ANALOG_VAL_SIZE, buf, ANALOG_VAL_SIZE)) {
		return ERROR;
	}
	uint16_t raw = 0;
	memcpy(&raw, buf, ANALOG_VAL_SIZE);
	printf("%d\n", raw);
	return OK;
}/*}}}*/
const CliCmdType CMD_CRT_RANGE_WRITE = {/*{{{*/
	"rwr",
	2,
	&doCrtRangeWrite,
	"  rwr              Write sensor range(A)\n",
	"  Usage:           "PROGRAM_NAME" <id> rwr <channel> <value(A)>\n",
	"  Example:         "PROGRAM_NAME" 0 rwr 2 50 #Set the max range to +/-50A of sensor on channel#2 on board #0\n",
};
int doCrtRangeWrite(int argc, char *argv[]) {
	if(argc != 5) {
		return ARG_CNT_ERR;
	}
	int id = atoi(argv[1]);
	int dev = doBoardInit(id);
	if(dev < 0) {
		return ERROR;
	}
	int ch = atoi(argv[3]);
	if(badCrtInCh(ch)) {
		return ARG_RANGE_ERROR;
	}
	uint16_t val = atoi(argv[4]);
	if(!(1 <= val && val <= 500)) { // TODO: Change this range
		printf("Invalid amperage value, must be 1..500\n"); // TODO: Also here
		return ARG_RANGE_ERROR;
	}
	uint8_t buf[ANALOG_VAL_SIZE];
	memcpy(buf, &val, 2);
	if(OK != i2cMem8Write(dev, I2C_CRT_SNS_RANGE1_ADD + (ch - 1) * ANALOG_VAL_SIZE, buf, ANALOG_VAL_SIZE)) {
		return ERROR;
	}
	return OK;
}/*}}}*/

const CliCmdType CMD_CRT_IN_CAL = {/*{{{*/
	"cal",
	2,
	&doCrtInCal,
	"  cal              Calibrate current input channel, the calibration must be done in 2 points at min 10mA apart\n",
	"  Usage 1:         "PROGRAM_NAME" <id> cal <channel> <value(A)>\n"
	"  Usage 2:         "PROGRAM_NAME" <id> cal <channel> reset\n",
	"  Example:         "PROGRAM_NAME" 0 cal 1 5; Calibrate the current input channel #1 on board #0 at 5A\n"
};
int doCrtInCal(int argc, char *argv[]) {
	if(argc != 5) {
		return ARG_CNT_ERR;
	}
	int ch = atoi(argv[3]);
	if(badCrtInCh(ch)) {
		return ARG_RANGE_ERROR;
	}
	int dev = doBoardInit(atoi(argv[1]));
	if(dev < 0) {
		return ERROR;
	}
	float value = atof(argv[4]);
	if(strcasecmp(argv[4], "reset") == 0) {
		if(OK != calibReset(dev, CALIB_CRT_IN_CH1 + (ch - 1))) {
			return ERROR;
		}
		return OK;
	}
	if(OK != calibSet(dev, CALIB_CRT_IN_CH1 + (ch - 1), value)) {
		return ERROR;
	}
	return OK;
}/*}}}*/

const CliCmdType CMD_CRT_SNS_TYPE_READ = {/*{{{*/
	"trd",
        2,
        &doCrtSnsTypeRead,
        "  trd              Display the configured type of the current sensor (0 = 2.5V +/- 0.625V, 1 = 2.5V +/- 1V)\n",
        "  Usage 1:         "PROGRAM_NAME" <id> trd <ch[1.."STR(CRT_IN_CH_NO)"]>\n"
        "  Usage 2:         "PROGRAM_NAME" <id> trd \n",
        "  Example:         "PROGRAM_NAME" 0 trd 2 #Get the type of #2 sensor on board #0\n"
};
int doCrtSnsTypeRead(int argc, char *argv[]) {
        if(!(argc == 3 || argc == 4)) {
                return ARG_CNT_ERR;
        }
        int dev = doBoardInit(atoi(argv[1]));
        if(dev < 0) {
                return ERROR;
        }
        if(argc == 3) {
		uint8_t buf[1];
                if(OK != i2cMem8Read(dev, I2C_CRT_SNS_TYPE, buf, 1)) {
                        printf("Fail to read!\n");
                        return ERROR;
                }
		for(int ch = 1; ch <= CRT_IN_CH_NO; ++ch) {
			if(buf[0] & (1 << (ch - 1))) {
				printf("1 ");
			} else {
				printf("0 ");
			}
		}
		printf("\n");
        }
        else if(argc == 4) {
		uint8_t buf[1];
		if(OK != i2cMem8Read(dev, I2C_CRT_SNS_TYPE, buf, 1)) {
			printf("Fail to read!\n");
			return ERROR;
		}
		int ch = atoi(argv[3]);
		if(badCrtInCh(ch)) {
			return ARG_RANGE_ERROR;
		}
		if(buf[0] & (1 << (ch - 1))) {
			printf("1\n");
		} else {
			printf("0\n");
		}
        }
        return OK;
} /*}}}*/
const CliCmdType CMD_CRT_SNS_TYPE_WRITE = {/*{{{*/
	"twr",
        2,
        &doCrtSnsTypeWrite,
        "  twr              Set the type of the sensor(0 = 2.5V +/- 0.625V, 1 = 2.5V +/- 1V)\n",
        "  Usage 1:         "PROGRAM_NAME" <id> twr <ch[1.."STR(CRT_IN_CH_NO)"]> <state(0/1)>\n"
        "  Usage 2:         "PROGRAM_NAME" <id> twr <mask[0.."STR(MASK(CRT_IN_CH_NO))"]>\n",
        "  Example:         "PROGRAM_NAME" 0 twr 2 1 #Set the HALL sensor #2 on board #0 to 2.5V +/- 1V\n"
};
int doCrtSnsTypeWrite(int argc, char *argv[]) {
        if(!(argc == 4 || argc == 5)) {
                return ARG_CNT_ERR;
        }
        int dev = doBoardInit(atoi(argv[1]));
        if(dev < 0) {
                return ERROR;
        }
        if(argc == 4) {
                int mask = atoi(argv[3]);
                if(!(0 <= mask && mask < (1 << CRT_IN_CH_NO))) {
                        return ARG_RANGE_ERROR;
                }
                uint8_t buf[1];
                buf[0] = 0xff & mask;
                if(OK != i2cMem8Write(dev, I2C_CRT_SNS_TYPE, buf, 1)) {
                        printf("Fail to write sensor type bitmask!\n");
                        return ERROR;
                }
        }
        else if(argc == 5) {
                int ch = atoi(argv[3]);
		if(badCrtInCh(ch)) {
			return ARG_RANGE_ERROR;
		}
                int state = atoi(argv[4]);
                uint8_t buf[1];
		if (OK != i2cMem8Read(dev, I2C_CRT_SNS_TYPE, buf, 1)) {
			printf("Fail to read current sensor type!\n");
			return ERROR;
		}
		if (state > 0) {
			buf[0] |= (1 << (ch - 1));
		} else {
			buf[0] &= (~(1 << (ch - 1)));
		}
		if(OK != i2cMem8Write(dev, I2C_CRT_SNS_TYPE, buf, 1)) {
			printf("Fail to write sensor type!\n");
			return ERROR;
		}
        }
        return OK;
} /*}}}*/

// vi:fdm=marker
