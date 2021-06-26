#include "IR/Instruction.h"

#include <cassert>
#include <iostream>

Instruction::Instruction(unsigned opCode, const Value &dest, const Value &src) {
  assert(opCode == RegDef && "Expected a simple assignment");
  this->opCode = opCode;
  this->op1 = dest;
  this->op2 = src;
  numOperands = 2;
}

Instruction::Instruction(unsigned opCode, const Value &destOrCond,
                         const Value &srcOrTrueLabel,
                         const Value &srcOrFalseLabel) {
  assert((opCode == Br || (opCode >= Add && opCode <= Lt)) &&
         "Expected branch or binary or relational opcode");
  this->opCode = opCode;
  this->op1 = destOrCond;
  this->op2 = srcOrTrueLabel;
  this->op3 = srcOrFalseLabel;
  numOperands = 3;
}

Instruction::Instruction(unsigned opCode, const Value &label) {
  assert((opCode == Jmp || opCode == LabelDef) &&
         "Expected Jmp or LabelDef as the opcode");
  this->opCode = opCode;
  this->op1 = label;
  numOperands = 1;
}

Value Instruction::getOperand(unsigned i) const {
  assert(i < numOperands);
  switch (i) {
  case 0:
    return op1;
  case 1:
    return op2;
  default:
    return op3;
  }
}

void Instruction::dump() const {
  std::cout << '<';
  switch (opCode) {
  case RegDef:
    std::cout << "RegDef";
    break;

  case Br:
    std::cout << "Br";
    break;

  case Jmp:
    std::cout << "Jmp";
    break;

  case LabelDef:
    std::cout << "LabelDef";
    break;

  case Add:
    std::cout << "Add";
    break;

  case Sub:
    std::cout << "Sub";
    break;

  case Mul:
    std::cout << "Mul";
    break;

  case Div:
    std::cout << "Div";
    break;

  case Gt:
    std::cout << "Gt";
    break;

  case Lt:
    std::cout << "Lt";
    break;

  default:
    std::cout << "Unkown";
    break;
  }
  std::cout << '>';
  std::cout << ' ';
  op1.dump();

  if (numOperands >= 2) {
    std::cout << ' ';
    op2.dump();
  }

  if (numOperands == 3) {
    std::cout << ' ';
    op3.dump();
  }
}
