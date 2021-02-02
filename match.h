#pragma once

extern char *ch;
extern char *nexch;

void match_init(char* line);
void match_done();
void skipblanks();
int get_ch(void);
int match(const char *lit);
int squal(const char *str1, const char *str2);
int alpha(char c);
int numeric(char c);
int an(char c);
void cerror(const char *error);
