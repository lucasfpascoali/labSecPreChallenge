/*
    This is challenge 1 of set 1.
    Converting hex to base64

    Input: 49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d
    Output: SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t
*/

#include <stdio.h>
#include <string.h>
#include <cmath>

char base64Table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
    't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/'
};

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

unsigned int *rightPadWithZero(unsigned int *bin, unsigned int binSize, unsigned int numberOfZeros) {
    unsigned int *paddedArr = (unsigned int *)malloc(sizeof(unsigned int) * (binSize + numberOfZeros));

    // Copying the binary digits
    for (unsigned int i = 0; i < binSize; i++) {
        paddedArr[i] = bin[i];
    }

    for (unsigned int i = 0; i < numberOfZeros; i++) {
        paddedArr[binSize + i] = 0;
    }

    return paddedArr;
}

unsigned int *splitBinIn6DigitAndConvertToDecimal(unsigned int *bin, unsigned int size) {
    // (size / 6) to get the number of 6-digit blocks
    unsigned int *decimalArr = (unsigned int *)malloc(sizeof(unsigned int) * (size / 6));
    int total = 0;
    for (int i = 0; i < size; i++) {
        total += bin[i] * pow(2, 5 - (i % 6));

        if ((i + 1) % 6 == 0) {
            int index = static_cast<int>(i / 6);
            decimalArr[index] = total;
            total = 0;
        }
    }

    return decimalArr;
}

char *hexStrToBase64(char *hexStr, unsigned int size) {
    unsigned int *binaryArr = hexStrToBinaryArr(hexStr, size);
    unsigned int binSize = size * 4; // size * 4, because every hex digit becomes a 4-digit binary
    
    // Binary length isn't a multiple of 6, so we need to pad with zeros 
    if (binSize % 6 != 0) {
        binaryArr = rightPadWithZero(binaryArr, binSize, 6 - (binSize % 6));
        binSize += 6 - (binSize % 6);
    }

    unsigned int *decimalArr = splitBinIn6DigitAndConvertToDecimal(binaryArr, binSize);
    unsigned int decimalArrSize = static_cast<int>(binSize / 6);
    char *base64Str = (char*)malloc(sizeof(char) * decimalArrSize); 
    for (int i = 0; i < decimalArrSize; i++) {
        base64Str[i] = base64Table[decimalArr[i]];
    }

    free(decimalArr);
    free(binaryArr);

    return base64Str;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        exit(1);
    }

    char *base64Str = hexStrToBase64(argv[1], strlen(argv[1]));
    printf("%s\n", base64Str);
    free(base64Str);

    return 0;
}
