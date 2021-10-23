#ifndef CONCEPTS_LVN_LVN_PASS_H
#define CONCEPTS_LVN_LVN_PASS_H

#include "Concepts/LVN/LVN.h"
#include "IR/CFG.h"

class LVNPass {
  LVNTable theLVNTable;

public:
  LVNPass() {}

  void runOnBlock(CFGBlock *b);

  void printTable() const;
};

#endif
