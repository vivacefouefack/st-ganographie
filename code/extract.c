#include<stdio.h>
#include <stdint.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>
#include <unistd.h>
#include "bmp.h"


void extractTextToFile(char * imageFile, int textFileSize, int bitPosition) {
    FILE *img = fopen(imageFile, "rb");

    if (img == NULL) {
        printf("Erreur lors de l'ouverture du fichier image.\n");
        exit(1);
    }

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

    FILE *outputTextFile = fopen("extractFile.txt", "wb");

    if (outputTextFile == NULL) {
        printf("Erreur lors de la création du fichier de sortie pour le texte extrait.\n");
        exit(1);
    }

    int binaryIndex = 0;
    char currentByte = 0;
    int bitCount = 0;
    for (int y = 0; y < info.height; ++y) {
        for (int x = 0; x < info.width; ++x) {
            int pixelIndex = (y * info.width + x) * 3; // chaque pixel a 3 composantes (R, G, B)

            for (int component = 0; component < 3; ++component) {
                if (binaryIndex >= textFileSize)//si nous avons atteint la taille du texte caché, nous nous arrêtons
                    break;

                char pixelComponent = info.pixels[pixelIndex + component];
                char extractedBit = (pixelComponent >> bitPosition) & 0x01;
                currentByte = (currentByte << 1) | extractedBit;
                ++bitCount;
                if (bitCount == 8) {// Si nous avons atteint 8 bits, écrivons l'octet dans le fichier de sortie
                    fputc(currentByte, outputTextFile);
                    currentByte = 0;
                    bitCount = 0;
                }

                ++binaryIndex;
            }
            if (binaryIndex >= textFileSize)
                break;
        }
    }

    fclose(outputTextFile);
    printf("\nExtraction réussie, le fichier extractFile.txt a été créé");
}


void extractTextToFileUpToPosition(char * imageFile, int textFileSize,int position) {
    FILE *img = fopen(imageFile, "rb");

    if (img == NULL) {
        printf("Erreur lors de l'ouverture du fichier image.\n");
        exit(1);
    }
    if (position<1 || position>8) {
        printf("Erreur nombre de bit invalide.\n");
        exit(1);
    }


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

    FILE *outputTextFile = fopen("extractFileMulti.txt", "wb");

    if (outputTextFile == NULL) {
        printf("Erreur lors de la création du fichier de sortie pour le texte extrait.\n");
        exit(1);
    }

    int binaryIndex = 0;
    char currentByte = 0;
    int bitCount = 0;
    for (int y = 0; y < info.height; ++y) {
        for (int x = 0; x < info.width; ++x) {
            int pixelIndex = (y * info.width + x) * 3; // chaque pixel a 3 composantes (R, G, B)

            for (int component = 0; component < 3; ++component) {
                if (binaryIndex >= textFileSize)//si nous avons atteint la taille du texte caché, nous nous arrêtons
                    break;

                for (int p = 0; p < position; ++p) {
                    char pixelComponent = info.pixels[pixelIndex + component];
                    char extractedBit = (pixelComponent >> p) & 0x01;
                    currentByte = (currentByte << 1) | extractedBit;
                    ++bitCount;
                    if (bitCount == 8) {// Si nous avons atteint 8 bits, écrivons l'octet dans le fichier de sortie
                        fputc(currentByte, outputTextFile);
                        currentByte = 0;
                        bitCount = 0;
                    }

                    ++binaryIndex;
                }
            }
            if (binaryIndex >= textFileSize)
                break;
        }
    }

    fclose(outputTextFile);
    printf("\nExtraction réussie, le fichier extractFileMulti.txt a été créé");
}
