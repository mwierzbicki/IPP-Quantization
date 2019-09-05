#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "FindUnion.h"

#ifndef QUANTIZATION_QUANTDICT_H
#define QUANTIZATION_QUANTDICT_H

#define ALPHABET_SIZE 4

typedef struct DictNode* QuantDict;

struct DictNode {
    Relation node;
    QuantDict sons[ALPHABET_SIZE];
};

QuantDict newDict();
void freeQuantDict(QuantDict ptr);

QuantDict traverseDict(QuantDict ptr, const char *str);
void allowHistory(QuantDict ptr, const char *str);
void forgetHistory(QuantDict ptr, const char *str);
bool isHistoryPresent(QuantDict ptr, const char *str);
bool setHistoryEnergy(QuantDict ptr, const char *str, uint64_t nval);
uint64_t getHistoryEnergy(QuantDict ptr, const char *str);
bool equalizeEnergies(QuantDict ptr, const char *lstr, const char *rstr);

#endif //QUANTIZATION_QUANTDICT_H



