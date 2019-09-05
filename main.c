#include "defines.h"

//function verifying whether a string is a valid uint64_t integer
uint64_t validateEnergy(const char* addr) {
    size_t i = 0;

    //find the last element, strtok has null terminated the string
    while (addr[i]!='\0') {
        i++;
    }
    i--;

    bool noError = true;
    uint64_t currentSum=0, powersOf10=0, toAdd;

    //doesn't work for leading zeroes

    while (addr[i]!='\0') {
        if (powersOf10 > (UINT64_MAX / 10)) {
            noError = false;
        }
        else {
            if (powersOf10 == 0) {
                powersOf10 = 1;
            }
            else {
                powersOf10 = powersOf10 * 10;
            }
            if (addr[i] < '0' || addr[i] > '9') {
                noError = false;
            }
            else {
                if ((unsigned int)(addr[i] - '0') > (UINT64_MAX / powersOf10)) {
                    noError = false;
                }
                else {
                    toAdd = powersOf10 * (addr[i] - '0');
                    if (toAdd > UINT64_MAX - currentSum) {
                        noError = false;
                    }
                    else {
                        currentSum = currentSum + toAdd;
                    }
                }
            }
        }
        --i;
    }

    if (noError) {
        return currentSum;
    }
    else {
        return 0;
    }
}

bool validateQuantDictHistory(const char* addr) {

    if (addr == NULL) {
        return false;
    }
    int i=0;
    bool pass = true;
    while (addr[i] != '\0' && pass == true) {
        if (addr[i] < '0' || addr[i] > '3') {
            pass = false;
        }
        ++i;
    }
    return pass;
}

//return false if line cannot be read (EOF)
//loads from index 1
bool loadBufferUntilNewLine() {
    int i = 1;
    int c = getchar();
    while (c != '\n' && c != EOF) {
        buffer[i] = (char)c;
        //avoid string hijacking by inputting null terminator
        if (buffer[i] == '\0') {
            buffer[i] = '#';
        }
        ++i;
        if ((i + 1) == bufferSize) {
            bufferSize *= 2;
            buffer = realloc(buffer, (bufferSize * sizeof(char)));
            if (buffer == NULL) exit(1);
        }
        c = getchar();
    }
    buffer[i]='\0';
    if (c == '\n') {
        return true;
    }
    else {
        return false;
    }
}

void runInputParser() {
    bool noEOF = true;
    bool withoutError = false;
    int c;
    while(noEOF) {
        c = getchar();

        //finish getting input if first char is EOF
        if (c == EOF) {
            noEOF = false;
            withoutError = true;
        }

        //ignore lines beginning with hash
        else if (c == '#') {
            while (c != EOF && c != '\n') {
                c = getchar();
                if (c == EOF) {
                    noEOF = false;
                    withoutError = true;
                }
            }
        }

        else if (c != '\n') {
            buffer[0] = (char)c;
            //load from index 1 due to the first char already in the buffer
            if (loadBufferUntilNewLine()) {
                //before parsing with strtok
                size_t inputLineSize = (strlen(buffer) + 1);

                //split the strings with strtok
                char* command = strtok(buffer, " \n");
                char* arg1 = strtok(NULL, " \n");
                char* arg2 = strtok(NULL, " \n");
                char* trash = strtok(NULL, " \n");

                //check for extra whitespaces at the end of the string
                size_t check=0;
                if (command != NULL) {
                    check = check + strlen(command) + 1;
                }
                if (arg1 != NULL) {
                    check = check + strlen(arg1) + 1;
                }
                if (arg2 != NULL) {
                    check = check + strlen(arg2) + 1;
                }

                if (inputLineSize != check) {
                    fprintf(stderr, "ERROR\n");
                }
                //too many arguments
                else if (trash != NULL) {
                    fprintf(stderr, "ERROR\n");
                }
                else if (strcmp(command, "DECLARE") == 0) {
                    if (validateQuantDictHistory(arg1) && (arg2 == NULL)) {
                        allowHistory(myDict, arg1);
                        printf("OK\n");
                    }
                    else {
                        fprintf(stderr, "ERROR\n");
                    }
                }

                else if (strcmp(command, "REMOVE") == 0) {
                    if (validateQuantDictHistory(arg1) && (arg2 == NULL)) {
                        forgetHistory(myDict, arg1);
                        printf("OK\n");
                    }
                    else {
                        fprintf(stderr, "ERROR\n");
                    }
                }

                else if (strcmp(command, "VALID") == 0) {
                    if (validateQuantDictHistory(arg1) && (arg2 == NULL)) {
                        if (isHistoryPresent(myDict, arg1) == true) {
                            printf("YES\n");
                        }
                        else {
                            printf("NO\n");
                        }
                    }
                    else {
                        fprintf(stderr, "ERROR\n");
                    }
                }

                else if (strcmp(command, "ENERGY") == 0) {

                    if (arg2 == NULL) {
                        if (validateQuantDictHistory(arg1) && isHistoryPresent(myDict, arg1)) {
                            uint64_t newEnergy = getHistoryEnergy(myDict, arg1);
                            if (newEnergy != 0) {
                                printf("%lu\n", newEnergy);
                            }
                            else {
                                fprintf(stderr, "ERROR\n");
                            }
                        }
                        else {
                            fprintf(stderr, "ERROR\n");
                        }
                    }
                    else {
                        if (validateQuantDictHistory(arg1) && isHistoryPresent(myDict, arg1)) {
                            uint64_t param = validateEnergy(arg2);
                            if (param != 0) {
                                setHistoryEnergy(myDict, arg1, param);
                                printf("OK\n");
                            }
                            else {
                                fprintf(stderr, "ERROR\n");
                            }
                        }
                        else {
                            fprintf(stderr, "ERROR\n");
                        }
                    }
                }

                else if (strcmp(command, "EQUAL") == 0 ) {
                    if (validateQuantDictHistory(arg1) && validateQuantDictHistory(arg2)) {
                        if (equalizeEnergies(myDict, arg1, arg2)) {
                            printf("OK\n");
                        }
                        else {
                            fprintf(stderr, "ERROR\n");
                        }
                    }
                    else {
                        fprintf(stderr, "ERROR\n");
                    }
                }
                else {
                    fprintf(stderr, "ERROR\n");
                }
            }
            //if the read didn't succeed, finish getting input
            else {
                noEOF = false;
            }
        }
    }
    if (!withoutError) {
        fprintf(stderr, "ERROR\n");
    }
}

//data structures init
void initialize() {
    myDict = newDict();
    if (myDict == NULL) exit(1);
    bufferSize = 4;
    buffer = malloc((bufferSize * sizeof(char)));
    if (buffer == NULL) exit(1);
}

//data structures cleanup
void cleanup() {
    freeQuantDict(myDict);
    free(buffer);
}

int main() {
    initialize();
    runInputParser();
    cleanup();
    return 0;
}