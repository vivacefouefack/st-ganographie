#ifndef HIDE_H
#define HIDE_H
#include <stdint.h>

void hideTextInImage(char * textName, char * imageFile, int position);
void hideTextInImageUpToPosition(char * textName, char *imageFile,int bitPosition);
char * textInBinary(const char * text);
void textToBinary(const char *text);
void printBinary(char value);

#endif
