#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Flags flags = {0};

  char *str;
  str = (char *)calloc(sizeof(char), 10000);

  char **files_arr;
  int file_lines = argc - 2;
  files_arr = calloc(file_lines, sizeof(char *));
  for (int i = 0; i < file_lines; i++) {
    files_arr[i] = calloc(1024, sizeof(char));
  }

  int f_count = 0;  //сколько файлов записал
  int *p_file_count = &f_count;

  regex_t reg_ptrn;  //для записи регулярного выражения

  if (readFlags(argc, argv, &flags, str, files_arr, p_file_count) != 1) {
    patternComp(&flags, str, &reg_ptrn);
    int i = 0;  //счетчик файлов при открытии
    int *p_i = &i;
    for (; i < *p_file_count; i++) {
      FILE *file = fopen(files_arr[i], "r");
      if (file == NULL) {
        if (flags.sflag == 0)
          fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                  files_arr[i]);
      } else {
        grepExec(&flags, file, reg_ptrn, files_arr, p_i, p_file_count);
        fclose(file);
      }
    }
    regfree(&reg_ptrn);
  } else {
    fprintf(
        stderr,
        "usage: s21_grep [-e pattern][-ivclnhso][-f file][pattern] [file ...]");
  }
  free(str);  //очищаем массив паттернов
  for (int i = 0; i < file_lines;
       i++) {  //очищаем строки двумерного массива с файлами
    free(files_arr[i]);
  }
  free(files_arr);  //очищаем указатели на двумерный массив
  return 0;
}

int readFlags(int argc, char *argv[], Flags *flags, char *str, char **files_arr,
              int *p_file_count) {
  int ptrn_count = 0;
  while (optind < argc) {
    int prech = getopt(argc, argv, "e:ivclnhsf:o");
    if (prech == 'e' || prech == 'f') ptrn_count = 1;
    if (prech == -1) optind++;
  }
  optind = 1;
  while (optind < argc) {
    int ch = getopt(argc, argv, "e:ivclnhsf:o");
    switch (ch) {
      case 'e':
        flags->eflag = 1;
        addPtrn(argv, str, flags, ptrn_count);
        flags->eflag = 0;
        ptrn_count++;
        break;
      case 'i':
        flags->iflag = 1;
        break;
      case 'v':
        flags->vflag = 1;
        break;
      case 'c':
        flags->cflag = 1;
        break;
      case 'l':
        flags->lflag = 1;
        break;
      case 'n':
        flags->nflag = 1;
        break;
      case 'h':
        flags->hflag = 1;
        break;
      case 's':
        flags->sflag = 1;
        break;
      case 'f':
        flags->fflag = 1;
        ptrn_count++;
        addPtrn(argv, str, flags, ptrn_count);
        flags->fflag = 0;
        break;
      case 'o':
        flags->oflag = 1;
        break;
      case '?':
        flags->wrong = 1;
        break;
      case -1:
        if (ptrn_count == 0) {
          addPtrn(argv, str, flags, ptrn_count);
          ptrn_count = 1;
        } else {
          addFile(argv, files_arr, p_file_count);
        }
        optind++;
        break;
      default:
        printf("Нет такой опции...\n");
    }
  }
  return flags->wrong;
}

void addPtrn(char *argv[], char *str, Flags *flags, int ptrn_count) {
  if (flags->eflag == 1) {
    if (ptrn_count != 1) {
      strcat(str, "|");
    }
    strcat(str, optarg);
  } else if (flags->fflag == 1) {
    FILE *pat_file = fopen(optarg, "r");
    if (pat_file == NULL) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", optarg);
    } else {
      char pattern_from_file[10000] = {0};
      char symbol = '\n';
      char *search = NULL;
      while (fgets(pattern_from_file, 10000, pat_file) != NULL) {
        search = strchr(pattern_from_file, symbol);
        if (strlen(str) != 0) {
          strcat(str, "|");
        }
        if (pattern_from_file[0] != '\n' && search) {
          pattern_from_file[strlen(pattern_from_file) - 1] = '\0';
        }
        strcat(str, pattern_from_file);
      }
      fclose(pat_file);
    }
  } else {
    strcat(str, argv[optind]);
  }
}

void addFile(char *argv[], char **files_arr, int *p_file_count) {
  strcpy(files_arr[*p_file_count], argv[optind]);
  *p_file_count += 1;
}

void patternComp(Flags *flags, char *str, regex_t *reg_ptrn) {
  if (flags->iflag == 0) {
    regcomp(reg_ptrn, str, REG_EXTENDED | REG_NEWLINE);
  } else {
    regcomp(reg_ptrn, str, REG_ICASE | REG_EXTENDED | REG_NEWLINE);
  }
}

void grepExec(Flags *flags, FILE *file, regex_t reg_ptrn, char **files_arr,
              int *p_i, int *p_file_count) {
  size_t nmatch = 2;
  regmatch_t pmatch[2];

  char line_from_file[2048] = {0};
  int m_l_counter = 0;  //количество совпадающих строк
  int line_counter = 1;  //количество строк

  while (fgets(line_from_file, 2048, file) != NULL) {
    if (regexec(&reg_ptrn, line_from_file, nmatch, pmatch, 0) ==
        (flags->vflag)) {
      m_l_counter++;
      if (flags->vflag) flags->oflag = 0;
      if (!flags->cflag && !flags->lflag) {
        if (*p_file_count > 1 && !flags->hflag) printf("%s:", files_arr[*p_i]);
        if (flags->nflag) printf("%d:", line_counter);
        if (flags->oflag) {
          grepExec_o(reg_ptrn, line_from_file, nmatch, pmatch);
        } else {
          printf("%s", line_from_file);
          if (!strchr(line_from_file, '\n')) printf("\n");
        }
      }
    }
    line_counter++;
  }
  if (flags->cflag) {
    if (flags->lflag && m_l_counter) m_l_counter = 1;
    if (*p_file_count > 1 && !flags->hflag) printf("%s:", files_arr[*p_i]);
    printf("%d\n", m_l_counter);
  }
  if (flags->lflag && m_l_counter != 0) printf("%s\n", files_arr[*p_i]);
}

void grepExec_o(regex_t reg_ptrn, char *line_from_file, size_t nmatch,
                regmatch_t *pmatch) {
  char line_from_file_1[1024];
  for (int y = 0; line_from_file[y] != '\0'; y++) {
    line_from_file_1[y] = line_from_file[y];
    line_from_file_1[y + 1] = '\0';
  }
  if (regexec(&reg_ptrn, line_from_file_1, nmatch, pmatch, 0) == 0) {
    if (pmatch[0].rm_so != -1) {
      char *result;
      size_t m_len = pmatch[0].rm_eo - pmatch[0].rm_so;
      result = (char *)calloc(sizeof(char), (m_len + 1));
      strncpy(result, line_from_file_1 + pmatch[0].rm_so,
              m_len);  //запись результата совпадения на вывод
      result[m_len] = '\0';
      printf("%s\n", result);
      m_len = pmatch[0].rm_eo - pmatch[0].rm_so;
      while ((strlen(line_from_file_1 + pmatch[0].rm_eo)) > m_len) {
        strncpy(line_from_file_1, line_from_file_1 + pmatch[0].rm_eo,
                1000);  //запись отрезанной строчки
        if (regexec(&reg_ptrn, line_from_file_1, nmatch, pmatch, 0) == 0) {
          m_len = pmatch[0].rm_eo - pmatch[0].rm_so;
          strncpy(result, line_from_file_1 + pmatch[0].rm_so, m_len);
          result[m_len] = '\0';
          printf("%s\n", result);
          // m_len = pmatch[0].rm_eo - pmatch[0].rm_so;
        }
        m_len = pmatch[0].rm_eo - pmatch[0].rm_so;
      }
      free(result);
    }
  }
}