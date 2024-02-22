/* 
    This is challenge 6 of set 1
    Break repeating-key XOR

    Nota para LabSec: Não consegui implementar totalmente esse desafio em C++,
    fiz só até a parte de transpor os blocos. E por algum motivo quando eu
    tentava ler o arquivo .txt não funcionava, então fiz esse esquema de jogar 
    tudo em uma variável. Sei que não é o correto, mas como entregas parciais 
    eram permitidas, fiz mesmo assim.
*/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <cmath>
#include <fstream>

char base64Table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
    't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/'
};

unsigned int *decimalToNDigitBin(unsigned int decimalNumber, unsigned int n) {
    // 2-digit hexdecimal will be 8-digit binary, so we need to have the same length
    unsigned int *bin = (unsigned int *)malloc(sizeof(unsigned int) * n);
    for (int i = (n - 1); i >= 0; i--) {
        if (decimalNumber < 1) {
            bin[i] = 0;
            continue;
        } 

        bin[i] = decimalNumber % 2;
        decimalNumber = int(decimalNumber / 2);
    }

    return bin;
}

unsigned int findBase64CharIndex(char c) {
    for (int i = 0; i < 64; i ++) {
        if (base64Table[i] == c) {
            return i;
        }
    }
    return 0;
}

unsigned int *base64ToBin(char *base64, unsigned int size) {
    unsigned int binSize = size * 6;
    unsigned int *longBin = (unsigned int*)malloc(sizeof(int) * binSize);
    // Converting each char to 6 digit bin and grouping them all
    for (int i = 0; i < size; i++) {
        char currentChar = base64[i];
        if (currentChar == '=') {
            break; // end of base64
        }

        unsigned int* bin6digits = decimalToNDigitBin(findBase64CharIndex(currentChar), 6);
        for (int j = 0; j < 6; j++) {
            longBin[(6 * i) + j] = bin6digits[j];
        }
        free(bin6digits);
    }

    return longBin;
}

unsigned int hammingDistanceWithTwoBin(unsigned int *binA, unsigned int *binB, int size) {
    unsigned int distance = 0;
    for (int i = 0; i < size; i++) {
        distance += binA[i] ^ binB[i]; 
    }
    return distance;
}
// Only used to ensure that "this is a test" and "wokka wokka!!!" distance is 37
unsigned int hammingDistanceWithTwoStrings(char *strA, char *strB, int size) {
    unsigned int distance = 0;
    for (int i = 0; i < size; i++) {
        unsigned int *binOfStrA = decimalToNDigitBin(strA[i], 8);
        unsigned int *binOfStrB = decimalToNDigitBin(strB[i], 8);

        distance += hammingDistanceWithTwoBin(binOfStrA, binOfStrB, 8);
        free(binOfStrA);
        free(binOfStrB);
    }
    return distance;
}

unsigned int guessKeySize(unsigned int *txtInBin, unsigned int minSize, unsigned int maxSize) {
    unsigned int bestKeysize = 0;
    float shortestDistance = 0; 

    // Trying for each possible keysize
    for (int keysize = minSize; keysize <= maxSize; keysize++) {
        // 1 byte is 8-digit binary (8 bits)
        unsigned int *blockA = (unsigned int*)malloc(sizeof(int) * keysize * 8);
        unsigned int *blockB = (unsigned int*)malloc(sizeof(int) * keysize * 8);
        unsigned int *blockC = (unsigned int*)malloc(sizeof(int) * keysize * 8);
        unsigned int *blockD = (unsigned int*)malloc(sizeof(int) * keysize * 8);

        // Getting the first and secong keysize blocks
        for (int i = 0; i < keysize * 8; i ++) {
            blockA[i] = txtInBin[i];
            blockB[i] = txtInBin[(keysize * 8) + i];
            blockC[i] = txtInBin[(keysize * 2 * 8) + i];
            blockD[i] = txtInBin[(keysize * 3 * 8) + i];
        }

        // using average distance of 4 blocks method
        unsigned int hammingDistanceAB = hammingDistanceWithTwoBin(blockA, blockB, keysize * 8);
        unsigned int hammingDistanceAC = hammingDistanceWithTwoBin(blockA, blockC, keysize * 8);
        unsigned int hammingDistanceAD = hammingDistanceWithTwoBin(blockA, blockD, keysize * 8);
        unsigned int hammingDistanceBC = hammingDistanceWithTwoBin(blockB, blockC, keysize * 8);
        unsigned int hammingDistanceBD = hammingDistanceWithTwoBin(blockB, blockD, keysize * 8);
        unsigned int hammingDistanceCD = hammingDistanceWithTwoBin(blockC, blockD, keysize * 8);

        float sumOfDistances = hammingDistanceAB + hammingDistanceAC + hammingDistanceAD + hammingDistanceBC + hammingDistanceBD + hammingDistanceCD;
        float avgDistance = sumOfDistances / 6;

        avgDistance /= keysize;

        if (avgDistance < shortestDistance || bestKeysize == 0) {
            bestKeysize = keysize;
            shortestDistance = avgDistance;
        }
        free(blockA);
        free(blockB);
        free(blockC);
        free(blockD);
    }

    return bestKeysize;   
}

unsigned int **binToNBlocks(unsigned int *bin, unsigned int binSize, unsigned int n) {
    // N means the number of rows of the matrix (number of rows will be keysize)
    unsigned int **blocksMatrix = (unsigned int **)malloc(sizeof(unsigned int*) * n);
    unsigned int columnNumber = int((binSize / n) + 1);
    for (int i = 0; i < n; i++) {
        // just allocating memory
        blocksMatrix[i] = (unsigned int *)malloc(sizeof(int) * columnNumber);
        for (int j = 0; j < columnNumber; j++) {
            blocksMatrix[i][j] = bin[(columnNumber * i) + j];
        }
    }

    return blocksMatrix;
}

unsigned int **transposeBlocks(unsigned int **binBlocks, unsigned int rowSize, unsigned int blockSize) {
    // The transposed matrix will have the column number of the original matrix 
    // as row number and the row number of the original matrix as column number
    unsigned int newRowSize = blockSize;
    unsigned int newColumnSize = rowSize;
    unsigned int **transposedMatrix = (unsigned int **)malloc(sizeof(unsigned int*) * newRowSize);
    for (int i = 0; i < newRowSize; i++) {
        // just allocating memory
        transposedMatrix[i] = (unsigned int *)malloc(sizeof(int) * newColumnSize);
        for (int j = 0; j < newColumnSize; j++) {
            transposedMatrix[i][j] = binBlocks[j][i];
        }
    }

    return transposedMatrix;
}

int main() {
    unsigned int keysizeMinLength = 2;
    unsigned int keysizeMaxLength = 40;

    // Challenge instructions suggests this test
    unsigned int a = hammingDistanceWithTwoStrings("this is a test", "wokka wokka!!!", 15);
    if (a != 37) {
        exit(1);
    } 

    char *text = "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";
    unsigned int textSize = strlen(text);
    unsigned int *bin = base64ToBin(text, textSize);
    unsigned int binSize = textSize * 6;

    unsigned int keysize = guessKeySize(bin, keysizeMinLength, keysizeMaxLength);
    printf("O tamanho da key é %d\n", keysize);


    unsigned int blockSize = int(binSize / keysize) - 17; // 17 is 3 bytes + 1
    unsigned int **binBlocks = binToNBlocks(bin, binSize, keysize);
    
    unsigned int **transposedBinBlocks = transposeBlocks(binBlocks, keysize, blockSize);
    
    free(bin);
    free(binBlocks);
}