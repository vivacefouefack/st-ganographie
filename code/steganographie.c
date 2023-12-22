#include<stdio.h>
#include <stdint.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <fcntl.h>
#include <unistd.h>
#include "bmp.h"
#include "extract.h"
#include "hide.h"


void showImageInfo(char *adr,bmpheader header,bmpinfo info);
int getPosition(char * message);

int main(int argc, char *argv[]) {
    bmpheader header;
    bmpinfo info;

    while(1) {
        int choice;
        printf("\n\n \t\t**************STEGANOGRAPHIE**************\n Que souhaitez-vous faire ?\n\n 1 - Afficher les information de l'image\n\n 2 - Cacher un texte dans une image\n\n 3 - Cacher un texte dans une image, sur plusieurs bits\n\n 4 - Extraire un fichier secret dans une image\n\n 5 - Extraction sur plusieurs bits d'un fichier secret caché dans une image\n\n choix : ");
        scanf("%d",&choice);
        if(choice==1){
            printf("\t\t**************STEGANOGRAPHIE**************\n\n Choix 1 Afficher les infos de l'image\n : ");
            showImageInfo(argv[1],header,info);
        }else if(choice==2){
            printf("\t\t**************STEGANOGRAPHIE**************\n\n Choix 2 Cacher un texte dans une image\n");
            int position= getPosition("Position du bit à modifier pour chaque composant (0 à 7) : ");
            hideTextInImage(argv[2], argv[1], position);
        }else if(choice==3){
            printf("\t\t**************STEGANOGRAPHIE**************\n\n Choix 3 Cacher un texte dans une image, sur plusieurs bits\n");
            int position=0;
            printf("Combien de bit par pixel voulez vous modifier (1 à 8) : ");
            scanf("%d",&position);
            hideTextInImageUpToPosition(argv[3], argv[1], position);
        }else if(choice==4){
            printf("\t\t**************STEGANOGRAPHIE**************\n\n Choix 4 Extraction d'un fichier secret dans une image\n");
            int textFileSize=0;
            int position=getPosition("Quel bit voulez vous extraire (0 à 7) : ");
            printf("Entrez la taille du texte : ");
            scanf("%d", &textFileSize);
            extractTextToFile(argv[4],textFileSize,position);
        }else if(choice==5){
            printf("\t\t**************STEGANOGRAPHIE**************\n\n Choix 5 Extraction sur plusieurs bits d'un fichier secret\n");
            int textFileSize=0;
            int position=0;
            printf("Quel Nombre de bit par composant voulez vous extraire (1 à 8) : ");
            scanf("%d",&position);
            printf("Entrez la taille du texte : ");
            scanf("%d", &textFileSize);
            extractTextToFileUpToPosition(argv[5],textFileSize,position);
        }else{
            exit(0);
        }
    }
    return 0;
}

int getPosition(char * message){
    int position=-1;
    printf("%s",message);
    scanf("%d",&position);
    while(position < 0 || position > 7){
        printf("Position invalide.\n");
        printf("%s",message);
        scanf("%d",&position);
    }
    return position;
}


void showImageInfo(char *adr,bmpheader header,bmpinfo info) {
    FILE *img = fopen(adr, "rb");
    fread(&header, sizeof(header), 1, img);
    fread(&info, sizeof(info), 1, img);
    info.pixels = (unsigned char *)malloc(info.sizeimage);
    if (info.pixels == NULL) {
        printf("Erreur d'allocation mémoire pour les pixels.\n");
        exit(1);
    }
    fread(info.pixels, 1, info.sizeimage, img);
    fclose(img);
    printf("\n Image Bitmap chargée \n");
    printf("*************************************En-tête de fichie");
    printf("\n Type de l'image : %x \n", header.type);
    printf("\n Taille du fichier : %u \n", header.size);
    printf("\n Réserve 1 : %x \n", header.reserved1);
    printf("\n Réserve 2 : %x \n", header.reserved2);
    printf("\n Décalage : %u \n", header.offsetbits);
    printf("************************************Information de l'image");
    printf("\n Taille de l'en-tête de l'image: %u \n", info.headersize);
    printf("\n Hauteur de l'image : %d \n", info.height);
    printf("\n Largeur de l'image : %d \n", info.width);
    printf("\n Type de compression : %u \n", info.compression);
    printf("\n Taille des données : %u \n", info.sizeimage);
    printf("\n Nombre de bit par pixel : %u \n", info.bitcount);
    printf("\n Tableau de pixels : \n");

    if (info.bitcount == 24) {
        for (int y = 0; y < 10; ++y) { //j'affiche partiellement le tableau de pixels
            for (int x = 0; x < 10; ++x) {
                int index = (y * info.width + x) * 3;
                printf("Pixel [%d, %d] - R: ", x, y);
                printBinary(info.pixels[index]);
                printf(", G: ");
                printBinary(info.pixels[index + 1]);
                printf(", B: ");
                printBinary(info.pixels[index + 2]);
                printf("\n");
            }
        }
         free(info.pixels);
    } else {
        printf("La profondeur des bits %u n'est pas prise en charge.\n", info.bitcount);
    }
}
