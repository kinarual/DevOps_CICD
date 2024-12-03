#include "s21_cat.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    Flags flags = {0};
    if ((readFlags(argc, argv, &flags)) >= 1) {
      fprintf(stderr,
              "usage: s21_cat [-benstv | GNU: --number-nonblank -E --number "
              "--squeeze-blank -T] [file ...]");
    } else {
      for (int i = optind; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
          fprintf(stderr, "Error while opening file %s", *argv);
        } else {
          if ((needtoChange_check(&flags)) == 1) {
            printwithFlags(&flags, file);
          } else {
            justPrint(file);
          }
        }
      }
    }
  }
  return 0;
}

int readFlags(int argc, char *argv[], Flags *flags) {
  int ch;
  struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                  {"number", 0, NULL, 'n'},
                                  {"squeeze-blank", 0, NULL, 's'},
                                  {NULL, 0, NULL, 0}};

  while ((ch = getopt_long(argc, argv, "+beEnstTv", long_options, NULL)) !=
         -1) {
    switch (ch) {
      case 'b':
        flags->bflag++;
        break;
      case 'e':
        flags->eflag++;
        flags->vflag++;
        break;
      case 'E':
        flags->eflag++;
        break;
      case 'n':
        flags->nflag++;
        break;
      case 's':
        flags->sflag++;
        break;
      case 't':
        flags->tflag++;
        flags->vflag++;
        break;
      case 'T':
        flags->tflag++;
        break;
      case 'v':
        flags->vflag++;
        break;
      case '?':
        flags->notPrintflag++;
        break;
      default:
        flags->notPrintflag++;
        break;
    }
  }
  return flags->notPrintflag;
}

int needtoChange_check(Flags *flags) {
  int change = 0;
  if ((flags->bflag != 0) || (flags->eflag != 0) || (flags->vflag != 0) ||
      (flags->nflag != 0) || (flags->sflag != 0) || (flags->tflag != 0)) {
    change = 1;
  }
  return change;
}

void printwithFlags(Flags *flags, FILE *file) {
  int ch;
  int prev_ch = '\n';
  int count_currblank = 0;
  int count_line = 1;
  while ((ch = fgetc(file)) != EOF) {
    if (flags->sflag >= 1) {
      if ((ch == '\n') && (prev_ch == '\n')) {
        count_currblank++;
        if (count_currblank >= 2) {
          continue;
        }
      } else {
        count_currblank = 0;
      }
    }

    if (flags->bflag >= 1) {
      if (prev_ch == '\n' && ch != '\n') {
        printf("%6d\t", count_line++);
      }

    } else if (flags->nflag >= 1) {
      if (prev_ch == '\n') {
        printf("%6d\t", count_line++);
      }
    }

    if (flags->tflag >= 1) {
      if (ch == '\t') {
        printf("^");
        ch = 'I';
      }
    }

    if (flags->eflag >= 1) {
      if (ch == '\n') {
        printf("$");
      }
    }

    if (flags->vflag >= 1) {
      if ((ch >= 0 && ch < 9) || (ch > 10 && ch < 32) || (ch > 126)) {
        printf("^");
        if ((ch > 126)) {
          ch -= 64;
        } else {
          ch += 64;
        }
      }
    }

    printf("%c", ch);
    prev_ch = ch;
  }
  fclose(file);
}

void justPrint(FILE *file) {
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    putchar(ch);
  }
  fclose(file);
}
