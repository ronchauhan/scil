#include "Concepts/LVN/LVNPass.h"
#include "IR/CFG.h"

#include <cassert>
#include <iostream>

// Initial ad-hoc LVN
void LVNPass::runOnBlock(CFGBlock *b) {
  if (!theLVNTable.isEmpty())
    theLVNTable.clear();

  std::vector<Instruction *> &instructions = b->getInstructions();

  for (auto inst : instructions) {
    if (inst->isTerminator())
      continue;

    // Create entries for source values
    for (size_t idx = 1; idx < inst->getNumOperands(); ++idx) {
      theLVNTable.addEntry(Instruction::RegDef, -1, -1, inst->getOperand(idx));
    }

    const Value &holdingVal = inst->getOperand(0);
    int8_t v1 = theLVNTable.getValueNumberFor(inst->getOperand(1));
    assert(v1 != -1 && "v1 must be in the table but isnt!");

    if (inst->getNumOperands() == 2) {
      theLVNTable.addEntry(inst->getOpCode(), v1, -1, holdingVal);
      continue;
    }
    int8_t v2 = theLVNTable.getValueNumberFor(inst->getOperand(2));
    theLVNTable.addEntry(inst->getOpCode(), v1, v2, holdingVal);
  }
}

void LVNPass::printTable() const { theLVNTable.dump(std::cout); }
