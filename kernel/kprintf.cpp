#include "kernel/arch/x86_64/registers/getReg.hpp"
#include "limine/limine.h"
#include <cstdint>

extern limine_framebuffer *glbf;
char *fontPointer;

int termx = 0, termy = 0;

// only support 8x16 font...
void test_put(int x, int y, uint16_t ch) {
  x = x * 9;
  y = y * 16;
  // get glyph address
  char *glyph = fontPointer + ch * 16;
  uint32_t *fba = (uint32_t *)glbf->address;

  // height
  for (int i = 0; i < 16; i++) {
    // width
    char row = glyph[i];
    int c = 7;
    for (int j = 0; j < 8; j++) {
      if (((row >> j) & 1) == 1) {
        fba[(i + y) * (glbf->pitch / 4) + (c + x)] = 0;
      }
      c--;
    }
  }
}

void sprint(const char *st) {
  int i = 0;
  while (st[i] != 0) {
    if (st[i] == '\n') {
      termy++;
      termx = 0;
    } else {
      test_put(termx, termy, st[i]);
      termx++;
    }

    i++;

    if ((uint64_t)(termy * 16) > glbf->height) {
      termy = 0;
      termx = 0;
    }
    if ((uint64_t)(termx * 9) > glbf->width) {
      termx = 0;
      termy++;
      test_put(termx, termy, st[i]);
    }
  }
}

void printchar(char ch) {
  if (ch == '\n') {
    termy++;
    termx = 0;
    return;
  }
  test_put(termx, termy, ch);
  termx++;
  if ((uint64_t)(termx * 9) > glbf->width) {
    termy++;
    termx = 0;
  }
  if ((uint64_t)(termy * 16) > glbf->height) {
    termy = 0;
    termx = 0;
  }
}

void printuint(uint64_t num) {
  if (num == 0) {
    printchar('0');
    return;
  }
  char buf[20];
  int i = 19;

  while (num != 0) {
    buf[i] = num % 10 + 48;
    i--;
    num /= 10;
  }

  for (i = i + 1; i < 20; i++) {
    printchar(buf[i]);
  }
}

void printint(int64_t num) {
  if (num == 0) {
    printchar('0');
    return;
  }
  char buf[20];
  int i = 19;
  if (num < 0) {
    printchar('-');
    num *= -1;
  }
  while (num != 0) {
    buf[i] = num % 10 + 48;
    i--;
    num /= 10;
  }

  for (i = i + 1; i < 20; i++) {
    printchar(buf[i]);
  }
}

const char hextab[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                       '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void printhex(uint64_t num) {
  if (num == 0) {
    printchar('0');
    return;
  }
  char buf[20];
  int i = 19;

  while (num != 0) {
    buf[i] = hextab[num % 16];
    i--;
    num /= 16;
  }

  for (i = i + 1; i < 20; i++) {
    printchar(buf[i]);
  }
}

void printbin(uint64_t num) {
  if (num == 0) {
    printchar('0');
    return;
  }
  char buf[64];
  int i = 63;

  while (num != 0) {
    buf[i] = hextab[num % 2];
    i--;
    num /= 2;
  }

  for (i = i + 1; i < 64; i++) {
    printchar(buf[i]);
  }
}

void printstr(const char *st, uint64_t length) {
  for (int i = 0; i < length; i++) {
    printchar(st[i]);
  }
}

void kprintf(const char *fmt, ...) {
  getReg();
  uint64_t buf[5];
  buf[0] = registersData.rsi;
  buf[1] = registersData.rdx;
  buf[2] = registersData.rcx;
  buf[3] = registersData.r8;
  buf[4] = registersData.r9;
  int i = 0;
  int k = 0;

  while (fmt[i] != 0) {
    if (fmt[i] == '%' && k < 5) {
      if (fmt[i + 1] == 'u') {
        printuint(buf[k]);
        k++;
        i += 2;
      } else if (fmt[i + 1] == 'i') {
        printint((int64_t)buf[k]);
        k++;
        i += 2;
      } else if (fmt[i + 1] == 'x') {
        printhex(buf[k]);
        k++;
        i += 2;
      } else if (fmt[i + 1] == 'b') {
        printbin(buf[k]);
        k++;
        i += 2;
      } else if (fmt[i + 1] == 'c') {
        printchar(buf[k]);
        k++;
        i += 2;
      } else if (fmt[i + 1] == 's') {
        sprint((const char *)buf[k]);
        k++;
        i += 2;
      } else {
        printchar(fmt[i]);
        i++;
      }
    } else {
      printchar(fmt[i]);
      i++;
    }
  }
}