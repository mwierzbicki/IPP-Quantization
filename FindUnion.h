#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef QUANTIZATION_FINDUNION_H
#define QUANTIZATION_FINDUNION_H

typedef struct regNode* Relation;

struct infoNode {
    uint64_t energy;
    size_t size;
    struct regNode* next;
};

struct regNode {
    struct infoNode* master;
    struct regNode* prev; //if NULL, node parent is master
    struct regNode* next; //if NULL, node has no son
};

Relation newRelation(uint64_t energy);
uint64_t getRelationEnergy(Relation rel);
void setRelationEnergy(Relation rel, uint64_t newEnergy);
bool mergeRelations(Relation rel1, Relation rel2);
void removeElement(Relation rel1);

#endif //QUANTIZATION_FINDUNION_H