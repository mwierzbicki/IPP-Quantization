#include "FindUnion.h"

Relation newRelation(uint64_t energy) {
    struct infoNode* temp1;
    Relation temp2;
    temp1 = (struct infoNode*)malloc(sizeof(struct infoNode));
    if (temp1 == NULL) exit(1);
    temp2 = (Relation)malloc(sizeof(struct regNode));
    if (temp2 == NULL) exit(1);
    temp1->energy=energy;
    temp1->size=1;
    temp1->next=temp2;
    temp2->prev=NULL;
    temp2->next=NULL;
    temp2->master=temp1;
    return temp2;
}

uint64_t getRelationEnergy(Relation rel) {
    return (rel->master)->energy;
}

void setRelationEnergy(Relation rel, uint64_t newEnergy) {
    (rel->master)->energy = newEnergy;
}

//returns whether merging relations has succeeded via boolean value
bool mergeRelations(Relation rel1, Relation rel2) {

    //relations are identical - merged already
    if (rel1->master == rel2->master) {
        return true;
    }

    //both relations have no set energies - merge impossible
    if ((rel1->master)->energy == 0 && (rel2->master)->energy == 0) {
        return false;
    }

    //speed improvement - always add the smaller list to the bigger one
    if ((rel1->master)->size < (rel2->master)->size) {
        Relation temp = rel1;
        rel1 = rel2;
        rel2 = temp;
    }

    //temporary values for calculating the average of two energies
    uint64_t newValue = 0;
    uint64_t ltemp = (rel1->master)->energy;
    uint64_t rtemp = (rel2->master)->energy;
    if (ltemp == 0) {
        newValue = rtemp;
    }
    if (rtemp == 0) {
        newValue = ltemp;
    }
    if (ltemp != 0 && rtemp != 0){
        if ((ltemp % 2 == 1) && (rtemp % 2 == 1)) {
            newValue = 1;
        }
        newValue += (ltemp / 2) + (rtemp / 2);
    }

    //fix energy and size values
    (rel1->master)->energy = newValue;
    (rel1->master)->size += (rel2->master)->size;

    Relation currentNode = (rel2->master)->next;
    Relation returnNode = (rel1->master)->next;
    //link smaller list with larger one
    (rel1->master)->next = currentNode;

    //cleanup the master node of the smaller list
    free(rel2->master);

    //fix smaller list values
    currentNode->master = rel1->master;
    while (currentNode->next != NULL) {
        currentNode = currentNode->next;
        currentNode->master = rel1->master;
    }

    //finish linking the lists
    currentNode->next = returnNode;
    returnNode->prev = currentNode;

    return true;
}

void removeElement(Relation rel1) {
    //last element in relation
    if (rel1->prev == NULL && rel1->next == NULL) {
        free(rel1->master);
        free(rel1);
        return;
    }

    //no previous element - must fix master
    if (rel1->prev == NULL) {
        (rel1->master)->next = rel1->next;
    }

    //fix next element pointers
    if (rel1->next != NULL) {
        (rel1->next)->prev = rel1->prev;
    }

    //fix prev element pointers
    if (rel1->prev != NULL) {
        (rel1->prev)->next = rel1->next;
    }

    free(rel1);
}
