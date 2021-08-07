#include "IR/CFG.h"

#include <cstring>
#include <iostream>
#include <cassert>

CFGBlock::CFGBlock(const char *name) {
  std::memcpy(blockName, name, sizeof(name));
}

void CFGBlock::print(std::ostream &OS) const {
  OS << '.' << blockName << '\n';
  for (const auto inst : instructions) {
    OS << "  ";
    inst->print(OS);
    OS << '\n';
  }
}

void CFGBlock::dump(std::ostream &OS) const {
  OS << "Block : " << blockName << " \n";

  OS << "Predecessors : ";
  for (const auto block : predecessors)
    OS << block->getName() << ' ';

  OS << '\n';

  OS << "Successors : ";
  for (const auto block : successors)
    OS << block->getName() << ' ';

  OS << '\n';
  for (const auto inst : instructions) {
    inst->dump(OS);
    OS << '\n';
  }
}

void CFG::addBlock(CFGBlock *b, bool isEntryBlock) {
  if (isEntryBlock) {
    assert(entryBlock == nullptr && "entry block can't be set twice");
    entryBlock = b;
  }
  blocks.push_back(b);
}

void CFG::print(std::ostream &OS) const {
  for (auto block : blocks) {
    block->print(OS);
    OS << '\n';
  }
}

void CFG::dump(std::ostream &OS) const {
  for (auto block : blocks) {
    block->dump(OS);
    OS << '\n';
  }
}
