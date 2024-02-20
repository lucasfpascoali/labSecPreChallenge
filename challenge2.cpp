/*
    This is challenge 2 of set 1
    Fixed XOR

    Inputs: 1c0111001f010100061a024b53535009181c 686974207468652062756c6c277320657965
    Output: 746865206b696420646f6e277420706c6179

    Nota para Labsec: estarei reutilizando funções do exercício anterior, copiei
    uma por uma pois achei mais prático nesse caso (e sou iniciante em C++)
*/

#include <stdio.h>
#include <string.h>
#include <cmath>

unsigned int *hexCharToBinary(char hexChar) {
    unsigned int *binary = (unsigned int *)malloc(sizeof(unsigned int) * 4); // Allocate memory for 4 integers
    
    switch (hexChar)
    {
        case '0': binary[0] = 0; binary[1] = 0; binary[2] = 0; binary[3] = 0; break;
        case '1': binary[0] = 0; binary[1] = 0; binary[2] = 0; binary[3] = 1; break;
        case '2': binary[0] = 0; binary[1] = 0; binary[2] = 1; binary[3] = 0; break;
        case '3': binary[0] = 0; binary[1] = 0; binary[2] = 1; binary[3] = 1; break;
        case '4': binary[0] = 0; binary[1] = 1; binary[2] = 0; binary[3] = 0; break;
        case '5': binary[0] = 0; binary[1] = 1; binary[2] = 0; binary[3] = 1; break;
        case '6': binary[0] = 0; binary[1] = 1; binary[2] = 1; binary[3] = 0; break;
        case '7': binary[0] = 0; binary[1] = 1; binary[2] = 1; binary[3] = 1; break;
        case '8': binary[0] = 1; binary[1] = 0; binary[2] = 0; binary[3] = 0; break;
        case '9': binary[0] = 1; binary[1] = 0; binary[2] = 0; binary[3] = 1; break;
        case 'A': case 'a': binary[0] = 1; binary[1] = 0; binary[2] = 1; binary[3] = 0; break;
        case 'B': case 'b': binary[0] = 1; binary[1] = 0; binary[2] = 1; binary[3] = 1; break;
        case 'C': case 'c': binary[0] = 1; binary[1] = 1; binary[2] = 0; binary[3] = 0; break;
        case 'D': case 'd': binary[0] = 1; binary[1] = 1; binary[2] = 0; binary[3] = 1; break;
        case 'E': case 'e': binary[0] = 1; binary[1] = 1; binary[2] = 1; binary[3] = 0; break;
        case 'F': case 'f': binary[0] = 1; binary[1] = 1; binary[2] = 1; binary[3] = 1; break;
        default: binary[0] = 0; binary[1] = 0; binary[2] = 0; binary[3] = 0; break;
    }
    
    return binary;
}

unsigned int *hexStrToBinaryArr(char *hexStr, unsigned int size) {
    // size * 4, because every hex digit becomes a 4-digit binary
    unsigned int *binaryArr = (unsigned int *)malloc(sizeof(unsigned int) * size * 4); 

    for (unsigned int i = 0; i < size; i++) {
        unsigned int *tmp = hexCharToBinary(hexStr[i]);
        binaryArr[i * 4] = tmp[0];
        binaryArr[i * 4 + 1] = tmp[1];
        binaryArr[i * 4 + 2] = tmp[2];
        binaryArr[i * 4 + 3] = tmp[3];
    }

    return binaryArr;
}

unsigned int *fixedXOR(unsigned int *binA, unsigned int *binB, unsigned int size) {
    unsigned int *fixedBin = (unsigned int *)malloc(sizeof(unsigned int) * size); 
    for (int i = 0; i < size; i++) {
        fixedBin[i] = binA[i] ^ binB[i];
    }

    return fixedBin;
}

char *binToHexStr(unsigned int *bin, int size) {
    char *hexStr = (char *)malloc(sizeof(char) * (size / 4));

    for (int i = 0; i < size; i += 4) {
        unsigned int valueInDecimal = 0;
        valueInDecimal += bin[i] * pow(2, 3 - (i % 4));
        valueInDecimal += bin[i + 1] * pow(2, 3 - ((i + 1) % 4));
        valueInDecimal += bin[i + 2] * pow(2, 3 - ((i + 2) % 4));
        valueInDecimal += bin[i + 3] * pow(2, 3 - ((i + 3) % 4));
        
        if (valueInDecimal < 10) {
            hexStr[int(i / 4)] = '0' + valueInDecimal;
        } else {
            hexStr[int(i / 4)] = 'a' + (valueInDecimal - 10);
        }
    }

    return hexStr;
}

char *fixedXORWithTwoHexStrOfSameSize(char *hexStrA, char *hexStrB, unsigned int size) {
    unsigned int *binA = hexStrToBinaryArr(hexStrA, size);
    unsigned int *binB = hexStrToBinaryArr(hexStrB, size);

    unsigned int *fixedBin = fixedXOR(binA, binB, size * 4);

    free(binA);
    free(binB);

    char *fixedHexStr = binToHexStr(fixedBin, size * 4);
    free(fixedBin);

    return fixedHexStr;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        exit(1);
    }

    char *fixedXORHexStr = fixedXORWithTwoHexStrOfSameSize(argv[1], argv[2], strlen(argv[1]));
    printf("%s\n", fixedXORHexStr);
    free(fixedXORHexStr);

    return 0;
}
