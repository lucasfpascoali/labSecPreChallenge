/*
    This is challenge 5 of set 1
    Implement repeating-key XOR

    Input: Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal
    Output: 0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f

    Nota para labsec: como esse input tem um \n, deixei o valor já salvo em uma
    variável; O compilador dá um warning, mas acredito que pra esse desafio, não
    tem problema.
*/

#include <stdio.h>
#include <string.h>
#include <cmath>

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

char *charToHexWithRepeatingKeyXOR(char *sentence, unsigned int size, char *key, unsigned int keySize) {
    char *hexStr = (char *)malloc(sizeof(char) * size * 2); // 1 char == 8 digit binary == 2 digit hex
    for (int i = 0; i < size; i++) {
        unsigned int *bin = decimalTo8DigitBin(int(sentence[i])); 
        // bin size will be always 8. i % keysize ensures that the index will 
        // always be between 0 and keysize - 1.
        unsigned int *xordBin = binXORSingleChar(bin, 8, key[i % keySize]);
        char *twoDigitsHex = binToHexStr(xordBin, 8);
        hexStr[i * 2] = twoDigitsHex[0];
        hexStr[(i * 2) + 1] = twoDigitsHex[1];

        free(xordBin);
        free(bin); 
    }

    return hexStr;

}


int main() {
    char *input = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    unsigned int inputSize = strlen(input);

    char *key = "ICE";
    unsigned int keySize = strlen(key);

    char *encryptedSentence = charToHexWithRepeatingKeyXOR(input, inputSize, key, keySize); 
    printf("%s\n", encryptedSentence);
}
