#ifndef _XTOA_H
#define _XTOA_H

char far* itoa (  int val, char far* buf, int radix);
char far* ltoa (  long val, char far* buf, int radix);
char far* ultoa ( unsigned long val, char far* buf, int radix);

#endif /* _XTOA_H */

