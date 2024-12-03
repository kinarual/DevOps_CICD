#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int bflag;
  int eflag;
  int nflag;
  int sflag;
  int tflag;
  int vflag;
  int notPrintflag;
} Flags;

int readFlags(int argc, char *argv[], Flags *flags);
int needtoChange_check(Flags *flags);
void printwithFlags(Flags *flags, FILE *file);
void justPrint(FILE *file);

#endif