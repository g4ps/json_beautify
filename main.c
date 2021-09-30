#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

char *mbuf = NULL;
int mpos = 0;
int msize = 0;

void add_to_buf(char c)
{
  if (mpos + 1 >= msize) {
    msize += BUFSIZ;
    char *n = calloc(sizeof(char), msize);
    if (n == NULL) {
      fprintf(stderr, "Error: not enough memory\n");
      exit(1);
    }
    memcpy(n, mbuf, mpos);
    free(mbuf);
    mbuf = n;
  }
  mbuf[mpos] = c;
  mpos++;
}

void add_str(char *str)
{
  while (*str) {
    add_to_buf(*str);
    str++;
  }
}

void print_buf()
{
  printf("%s\n", mbuf);
}

void clear_buf()
{
  free(mbuf);
  mpos = msize = 0;
}

void skip_spaces(FILE *inp)
{
  int c;
  while (isspace(c = fgetc(inp)))
    ;
  ungetc(c, inp);
}

int print_n_tabs(int n) {
  for (int i = 0; i < n; i++) {
    add_to_buf('\t');
  }
}

int main(int argc, char **argv)
{
  FILE* inp;
  if (argc == 1)
    inp = stdin;
  else
    inp = fopen(argv[1], "r");
  if (inp == NULL) {
    perror(argv[1]);
    exit(1);
  }
  int c;
  int level = 0;
  int ins_quotes = 0;
  char *line = calloc(BUFSIZ, sizeof(char));
  int pos = 0;
  while ((c = fgetc(inp)) >= 0) {
    if (c == '{') {
      print_n_tabs(level);
      add_str(line);
      level++;
      add_str(" {\n");
    }
    else if (c == '}') {
      print_n_tabs(level);
      add_str(line);
      add_to_buf('\n');
      level--;
      print_n_tabs(level);
      add_str("}\n");
      bzero(line, BUFSIZ);
      pos = 0;
    }
    else if (c == ',') {
      print_n_tabs(level);
      add_str(line);
      add_str(",\n");
      bzero(line, BUFSIZ);
      pos = 0;
      skip_spaces(inp);
    }
    else {
      line[pos] = c;
      pos++;
    }
  }
  print_buf();
  return 0;
}
