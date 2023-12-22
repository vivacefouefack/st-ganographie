#ifndef EXTRACT_H
#define EXTRACT_H
#include <stdint.h>

void extractTextToFile(char * imageFile, int textFileSize, int bitPosition);
void extractTextToFileUpToPosition(char * imageFile, int textFileSize,int position);

#endif
