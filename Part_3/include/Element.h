#ifndef ELEMENT_H_
#define ELEMENT_H_

struct Element
{
    int money;   // gain
    int weight;  // weight
    int touched; // Iteration number of last move
    bool in_x;   // is in the bag?
};

#endif // ELEMENT_H_