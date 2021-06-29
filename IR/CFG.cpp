#include "IR/CFG.h"

#include <cstring>
#include <iostream>

CFGBlock::CFGBlock(const char *name) {
  std::memcpy(blockName, name, sizeof(name));
}

void CFGBlock::print() const {
  std::cout << '.' << blockName << '\n';
  for (const auto inst : instructions) {
    std::cout << "  ";
    inst->print();
    std::cout << '\n';
  }
}

void CFGBlock::dump() const {
  std::cout << "Block : " << blockName << " \n";

  std::cout << "Predecessors : ";
  for (const auto block : predecessors)
    std::cout << block->getName() << ' ';

  std::cout << '\n';

  std::cout << "Successors : ";
  for (const auto block : successors)
    std::cout << block->getName() << ' ';

  std::cout << '\n';
  for (const auto inst : instructions) {
    inst->dump();
    std::cout << '\n';
  }
}

void CFG::addBlock(CFGBlock *b, bool isEntryBlock) {
  if (isEntryBlock)
    entryBlock = b;
  blocks.push_back(b);
}

void CFG::print() const {
  for (auto block : blocks) {
    block->print();
    std::cout << '\n';
  }
}

void CFG::dump() const {
  for (auto block : blocks) {
    block->dump();
    std::cout << '\n';
  }
}
