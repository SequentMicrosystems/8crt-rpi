#ifndef CRT_H
#define CRT_H

#include "cli.h"

extern const CliCmdType CMD_CRT_IN_READ;
extern const CliCmdType CMD_CRT_RMS_IN_READ;

extern const CliCmdType CMD_CRT_IN_CAL;

extern const CliCmdType CMD_CRT_RANGE_READ;
extern const CliCmdType CMD_CRT_RANGE_WRITE;

extern const CliCmdType CMD_CRT_SNS_TYPE_READ;
extern const CliCmdType CMD_CRT_SNS_TYPE_WRITE;


int doCrtInRead(int argc, char *argv[]);
int doCrtInRmsRead(int argc, char *argv[]);

int doCrtInCal(int argc, char *argv[]);

int doCrtRangeRead(int argc, char *argv[]);
int doCrtRangeWrite(int argc, char *argv[]);

int doCrtSnsTypeRead(int argc, char *argv[]);
int doCrtSnsTypeWrite(int argc, char *argv[]);

#endif /* CRT_H */

// vi:fdm=marker
