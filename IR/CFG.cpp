#include "IR/CFG.h"

#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

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

void CFG::emitToFile(const char *fileName, bool simplified) const {
  std::ofstream outStream;
  outStream.open(fileName);
  if (!outStream) {
    std::cerr << "Couldn't open " << fileName << ". Exiting...\n";
    exit(1);
  }

  const char *NBSP4 = "    ";
  outStream << "digraph {\n";
  outStream << NBSP4 << "node [shape=box]\n\n";

  // First emit all the blocks
  for (auto block : blocks) {
    outStream << NBSP4;
    outStream << block->getName() << ' ';

    if (simplified)
      emitBlockNameOnly(block, outStream);
    else
      emitBlockFull(block, outStream); // Emit the instructions too.
  }

  outStream << '\n';

  // Now emit all the edges
  for (auto block : blocks) {
    auto &successors = block->getSuccessors();
    for (auto successor : successors) {
      outStream << NBSP4;
      outStream << block->getName() << " -> " << successor->getName() << ";\n";
    }
  }
  outStream << "}";
}

void CFG::emitBlockFull(CFGBlock *block, std::ostream &outStream) const {
  outStream << '[';
  outStream << "label=\"<" << block->getName() << ">";
  outStream << '\\' << 'l';

  auto &instructions = block->getInstructions();
  for (auto inst : instructions) {
    inst->print(outStream);
    outStream << '\\' << 'l';
  }
  outStream << "\"];\n";
}

void CFG::emitBlockNameOnly(CFGBlock *block, std::ostream &outStream) const {
  outStream << "[label=\"" << block->getName() << "\"];\n";
}
