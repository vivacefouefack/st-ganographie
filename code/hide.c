#include<stdio.h>
#include <stdint.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>
#include <unistd.h>
#include "bmp.h"



void printBinary(uint8_t value) {
    for (int i = 7; i >= 0; --i) {
        printf("%d", (value >> i) & 1);
    }
}


void textToBinary(const char * text) {
    while (*text) {
        for (int i = 7; i >= 0; --i) {
            printf("%d", (*text >> i) & 1);
        }
        printf(" ");
        ++text;
    }
    printf("\n");
}

char * textInBinary(const char * text) {
    int size = 0;
    while (text[size]) {
        size++;
    }

    char * binaryText = (char *)malloc(size * 8 + 1);
    if (binaryText == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        exit(1);
    }

    int index = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 7; j >= 0; --j) {
            binaryText[index++] = ((text[i] >> j) & 1) + '0';
        }
    }
    binaryText[index] = '\0';
    return binaryText;
}


void hideTextInImage(char * textName, char * imageFile,int bitPosition) {
    FILE * img =fopen(imageFile, "rb");
    FILE * textFile = fopen(textName, "rb");

    if (textFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier texte.\n");
        exit(1);
    }

    fseek(textFile, 0, SEEK_END);
    long textFileSize = ftell(textFile);
    fseek(textFile, 0, SEEK_SET);

    char * textBuffer = (char *)malloc(textFileSize);
    if (textBuffer == NULL) {
        printf("Erreur d'allocation mémoire pour le texte.\n");
        exit(1);
    }
    fread(textBuffer, 1, textFileSize, textFile);
    fclose(textFile);

    bmpheader header;
    bmpinfo info;
    fread(&header, sizeof(header), 1, img);
    fread(&info, sizeof(info), 1, img);
    info.pixels = (unsigned char *)malloc(info.sizeimage);
    if (info.pixels == NULL) {
        printf("Erreur d'allocation mémoire pour les pixels.\n");
        exit(1);
    }
    fread(info.pixels, 1, info.sizeimage, img);
    fclose(img);

    if ((textFileSize * 8) > info.sizeimage) {
        printf("Le texte est trop long pour être caché dans l'image.\n");
        exit(1);
    }

    textFileSize=textFileSize*8;
    printf("\nTaille des données de l'image : %u \n", info.sizeimage);
    char * binaryText=textInBinary(textBuffer);
    printf("Taille du texte secret en binaire : %ld \n\n", textFileSize);

    int binaryIndex = 0;
    for (int y = 0; y < info.height; ++y) {
        for (int x = 0; x < info.width; ++x) {
            int pixelIndex = (y * info.width + x) * 3; // chaque pixel a 3 composantes (R, G, B)

            for (int component = 0; component < 3; component++) {
                if (binaryIndex >= textFileSize){
                    break;
                }
                char pixelComponent = info.pixels[pixelIndex + component];
                char textBit = binaryText[binaryIndex ];
                int mask = 1 << bitPosition;
                info.pixels[pixelIndex + component] = (pixelComponent & ~mask) | ((textBit - '0') << bitPosition);
                binaryIndex++;

            }
             if (binaryIndex >= textFileSize)
                break;
        }
         if (binaryIndex >= textFileSize)
            break;
    }


    FILE * outputImg = fopen("imageOutput.bmp","wb");
    if (outputImg == NULL) {
        printf("Erreur lors de la création du fichier de sortie.\n");
        exit(1);
    }


    fwrite(&header, sizeof(header), 1, outputImg); // Écrire l'en-tête
    fwrite(&info, sizeof(info), 1, outputImg); // Écrire les informations de l'image modifiée

    fwrite(info.pixels, 1, info.sizeimage, outputImg);// Écrire les pixels de l'image modifiée
    fclose(outputImg);
    //libération des mémoires allouées
    free(info.pixels);
    free(textBuffer);
    free(binaryText);
    printf("\nTexte caché dans l'image imageOutput.bmp avec succès\n");
}


void hideTextInImageUpToPosition(char * textName, char *imageFile,int bitPosition) {
    FILE * img =fopen(imageFile, "rb");
    FILE * textFile = fopen(textName, "rb");

    if (bitPosition < 1 || bitPosition > 8) {
        printf("Position invalide.\n");
        exit(1);
    }

    if (textFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier texte.\n");
        exit(1);
    }

    fseek(textFile, 0, SEEK_END);
    long textFileSize = ftell(textFile);
    fseek(textFile, 0, SEEK_SET);

    char * textBuffer = (char *)malloc(textFileSize);
    if (textBuffer == NULL) {
        printf("Erreur d'allocation mémoire pour le texte.\n");
        exit(1);
    }
    fread(textBuffer, 1, textFileSize, textFile);
    fclose(textFile);

    bmpheader header;
    bmpinfo info;
    fread(&header, sizeof(header), 1, img);
    fread(&info, sizeof(info), 1, img);
    info.pixels = (unsigned char *)malloc(info.sizeimage);
    if (info.pixels == NULL) {
        printf("Erreur d'allocation mémoire pour les pixels.\n");
        exit(1);
    }
    fread(info.pixels, 1, info.sizeimage, img);
    fclose(img);

    if ((textFileSize * 8) > info.sizeimage*bitPosition) {
        printf("Le texte est trop long pour être caché dans l'image.\n");
        exit(1);
    }

    textFileSize=textFileSize*8;
    printf("\n Taille des données de l'image : %u \n", info.sizeimage);
    char * binaryText=textInBinary(textBuffer);
    printf(" Taille du texte secret en binaire : %ld \n\n", textFileSize);

    int binaryIndex = 0;
    for (int y = 0; y < info.height; ++y) {
        for (int x = 0; x < info.width; ++x) {
            int pixelIndex = (y * info.width + x) * 3; // chaque pixel a 3 composantes (R, G, B)

            for (int component = 0; component < 3; component++) {
                if (binaryIndex >= textFileSize){
                    break;
                }
                for (int p = 0; p < bitPosition; ++p) {
                    char pixelComponent = info.pixels[pixelIndex + component];
                    char textBit = binaryText[binaryIndex ];
                    int mask = 1 << p;
                    info.pixels[pixelIndex + component] = (pixelComponent & ~mask) | ((textBit - '0') << p);
                    binaryIndex++;
                    if (binaryIndex >= textFileSize){
                        break;
                    }
                }
            }
             if (binaryIndex >= textFileSize)
                break;
        }
         if (binaryIndex >= textFileSize)
            break;
    }
    FILE * outputImg = fopen("imageUpToPosition.bmp","wb");
    if (outputImg == NULL) {
        printf("Erreur lors de la création du fichier de sortie.\n");
        exit(1);
    }
    fwrite(&header, sizeof(header), 1, outputImg);
    fwrite(&info, sizeof(info), 1, outputImg);
    fwrite(info.pixels, 1, info.sizeimage, outputImg);
    fclose(outputImg);
    free(info.pixels);
    free(textBuffer);
    free(binaryText);
    printf("\nTexte caché avec succès dans l'image imageUpToPosition.bmp\n");
}
