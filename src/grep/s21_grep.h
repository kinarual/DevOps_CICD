#ifndef S21_GREP_H
#define S21_GREP_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int eflag;
  int iflag;
  int vflag;
  int cflag;
  int lflag;
  int nflag;
  int hflag;
  int sflag;
  int fflag;
  int oflag;
  int changes;
  int wrong;
} Flags;

int readFlags(int argc, char *argv[], Flags *flags, char *str, char **files_arr,
              int *p_file_count);
void addPtrn(char *argv[], char *str, Flags *flags, int ptrn_count);
void addFile(char *argv[], char **files_arr, int *file_count);
void patternComp(Flags *flags, char *str, regex_t *reg_ptrn);
void grepExec(Flags *flags, FILE *file, regex_t reg_ptrn, char **files_arr,
              int *p_i, int *p_file_count);
void grepExec_o(regex_t reg_ptrn, char *line_from_file, size_t nmatch,
                regmatch_t *pmatch);

#endif