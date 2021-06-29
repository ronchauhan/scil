#ifndef IR_CFG_H
#define IR_CFG_H

#include "IR/Instruction.h"
#include <cstring>
#include <vector>

class CFGBlock {
  char blockName[8];
  std::vector<Instruction *> instructions;
  std::vector<CFGBlock *> predecessors;
  std::vector<CFGBlock *> successors;

public:
  CFGBlock(const char *name = nullptr);

  const char *getName() const { return blockName; }

  void addInst(Instruction *inst) { instructions.push_back(inst); }

  void addPredecessor(CFGBlock *b) { predecessors.push_back(b); }

  void addSuccessor(CFGBlock *b) { successors.push_back(b); }

  void print() const;

  void dump() const;
};

class CFG {
  CFGBlock *entryBlock;
  std::vector<CFGBlock *> blocks;

public:
  CFG() : entryBlock(nullptr){};

  void addBlock(CFGBlock *b, bool isEntryBlock = false);

  CFGBlock *getBlockWithName(const char *name) {
    for (auto block : blocks) {
      if (std::strcmp(block->getName(), name) == 0)
        return block;
    }
    return nullptr;
  }

  CFGBlock *getEntryBlock() { return entryBlock; }

  // Print parsable text to stdout
  void print() const;

  void dump() const;
};

#endif