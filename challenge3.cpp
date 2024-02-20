/*
    This is challenge 3 of set 1
    Single-byte XOR cipher

    Input: 1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736

    Nota para Labsec: Já que era para ser criativo com a forma de avaliar as
    possíveis mensagens descriptografadas, utilizei o livro The Horror at Red
    Hook do autor H. P. Lovecraft para medir a frequência de letras num texto em
    inglês. O livro em "plain text" foi baixado do Project Gutenberg ("www.gutenberg.org")
    e seus diretos autorais já expiraram, então não tem problemas legais.
*/

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <fstream>

float *getCharFreqAvg() {
    float *charFrequencyAvg = (float *)calloc(128, sizeof(float)); // 128 char in ascii table
    int charCounter = 0;

    std::ifstream in("the_horror_at_red_hook.txt");
    char currentChar;
    if (in.is_open() && !in.eof()) {
        while (in.good()) {
            in.get(currentChar);
            if (currentChar >= 0 && currentChar <= 127) {
                charFrequencyAvg[int(currentChar)] += 1;
            }
            charCounter += 1;
        }
    } else {
        in.close();
    }

    // Calculating Average
    for (int i = 0; i < 128; i++) {
        charFrequencyAvg[i] /= charCounter;
    }

    return charFrequencyAvg;
}

float getSentenceScore(char *sentence, unsigned int size) {
    float *charFreqAvg = getCharFreqAvg();
    float score = 0.0f;

    for (int i = 0; i < size; i++) {
        if (sentence[i] >= 0 && sentence[i] <= 127) {
            score += charFreqAvg[int(sentence[i])];
        }
    }

    free(charFreqAvg);
    return score / size;
}

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

unsigned int *decimalTo8DigitBin(unsigned int decimalNumber) {
    // 2-digit hexdecimal will be 8-digit binary, so we need to have the same length
    unsigned int *bin = (unsigned int *)malloc(sizeof(unsigned int) * 8);
    bool isEnd = false;
    for (int i = 7; i >= 0; i--) {
        if (decimalNumber < 1) {
            bin[i] = 0;
            continue;
        } 

        bin[i] = decimalNumber % 2;
        decimalNumber = int(decimalNumber / 2);
    }

    return bin;
}

char bin8digitToAsciiChar(unsigned int *bin) {
    unsigned int decimal = 0;
    for (int i = 0; i < 8; i++) {
        decimal += bin[i] * pow(2, (7 - i));
    }

    if (decimal > 127) {
        exit(1);
    }

    return char(decimal);
}

unsigned int *binXORSingleChar(unsigned int *bin, unsigned int size, char key) {
    // First converting the key to 8-digit binary
    unsigned int *keyInBin = decimalTo8DigitBin(key);

    unsigned int *fixedBin = (unsigned int *)malloc(sizeof(unsigned int) * size); 
    for (int i = 0; i < size; i++) {
        fixedBin[i] = bin[i] ^ keyInBin[i];
    }

    free(keyInBin);
    return fixedBin;
}

char *decryptMessage(char *hexStr, unsigned int size, char key) {
    // Every 2 digits of hexStr will become an ascii char
    char *decryptedMessage = (char *)malloc(sizeof(char) * (size / 2));

    // We will be taken every 2-digit hexadecimal as bytes
    char tmp[2];
    for (int i = 0; i < size; i += 2) { 
        tmp[0] = hexStr[i];
        tmp[1] = hexStr[i + 1];

        unsigned int *bin = hexStrToBinaryArr(tmp, 2); // 2-digits hex
        unsigned int binSize = 2 * 4; // Every 1 digit hex becomes 4 digits bin 

        unsigned int *xordBin = binXORSingleChar(bin, binSize, key);
        free(bin); 

        decryptedMessage[int(i / 2)] = bin8digitToAsciiChar(xordBin);
        free(xordBin);
    } 

    return decryptedMessage;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        exit(1);
    }

    unsigned int hexStrLen = strlen(argv[1]);
    unsigned int sentenceSize = int(hexStrLen / 2);

    char *rightMessage = (char *)malloc(sizeof(char) * sentenceSize);
    char bestKey;
    float bestScore = 0.0;
    for (int i = 0; i < 128; i++) {
        char *message = decryptMessage(argv[1], strlen(argv[1]), char(i));
        float currentScore = getSentenceScore(message, sentenceSize);

        if (currentScore > bestScore) {
            bestKey = char(i);
            bestScore = currentScore;
            rightMessage = message;
        }
    }

    printf("A frase é: %s\n", rightMessage);
    printf("O caractere utilizado no XOR foi: %c\n", bestKey);
    printf("O score dessa frase foi: %f\n", bestScore);
    free(rightMessage);
}