#include "QuantDict.h"

QuantDict newDict() {
    QuantDict temp;
    temp = (QuantDict)malloc(sizeof(struct DictNode));
    if (temp == NULL) exit(1);
    temp->node = newRelation(0);
    for (int i=0; i < ALPHABET_SIZE; ++i) {
        temp->sons[i] = NULL;
    }
    return temp;
}

void freeQuantDict(QuantDict ptr) {
    if (ptr != NULL) {
        if (ptr->node != NULL) {
            removeElement(ptr->node);
        }
        for (int i=0; i<ALPHABET_SIZE; ++i) {
            freeQuantDict(ptr->sons[i]);
        }
    }
    free(ptr);
}

//function traversing the tree by given string route. if no such route is in the tree, returns NULL
QuantDict traverseDict(const QuantDict ptr, const char *str) {
    QuantDict temp = ptr;
    size_t i=0,j=0;
    while (i<strlen(str)) {
        if (temp->sons[str[j] - '0'] != NULL) {
            temp = temp->sons[str[j] - '0'];
            ++j;
        }
        ++i;
    }
    if (i == j) {
        return temp;
    }
    return NULL;
}

void allowHistory(const QuantDict ptr, const char *str) {
    QuantDict temp = ptr;
    for (size_t i=0; i<strlen(str); i++) {
        if (temp->sons[str[i]-'0'] == NULL) {
            temp->sons[str[i]-'0'] = newDict();
        }
        temp = temp->sons[str[i]-'0'];
    }
}

void forgetHistory(const QuantDict ptr, const char *str) {
    QuantDict temp = ptr;
    size_t i=0;
    while ((temp->sons[str[i] - '0'] != NULL) && i<strlen(str)-1) {
        temp = temp->sons[str[i] - '0'];
        ++i;
    }
    if (i == strlen(str)-1) {
        freeQuantDict(temp->sons[str[i] - '0']);
        temp->sons[str[i]-'0'] = NULL;
    }
}

bool isHistoryPresent(const QuantDict ptr, const char *str) {
    return (traverseDict(ptr, str) != NULL);
}

bool setHistoryEnergy(const QuantDict ptr, const char *str, uint64_t nval) {
    QuantDict temp = traverseDict(ptr, str);
    if (temp != NULL) {
        setRelationEnergy(temp->node, nval);
        return true;
    }
    return false;
}

uint64_t getHistoryEnergy(const QuantDict ptr, const char *str) {
    QuantDict temp = traverseDict(ptr, str);
    return getRelationEnergy(temp->node);
};

bool equalizeEnergies(const QuantDict ptr, const char *lstr, const char *rstr) {
    QuantDict ltemp = traverseDict(ptr, lstr);
    QuantDict rtemp = traverseDict(ptr, rstr);
    if (ltemp == NULL || rtemp == NULL) {
        return false;
    }
    return mergeRelations(ltemp->node, rtemp->node);
}








