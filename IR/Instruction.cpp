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

void Instruction::print(std::ostream &OS) const {
  switch (opCode) {
  case RegDef:
    op1.print(OS);
    OS << " = ";
    op2.print(OS);
    break;

  case Br:
    OS << "br ";
    op1.print(OS);
    OS << ' ';
    op2.print(OS);
    OS << ' ';
    op3.print(OS);
    break;

  case Jmp:
    OS << "jmp ";
    op1.print(OS);
    break;

  case LabelDef:
    OS << '.';
    op1.print(OS);
    break;

  case Add:
    op1.print(OS);
    OS << " = add ";
    op2.print(OS);
    OS << ", ";
    op3.print(OS);
    break;

  case Sub:
    op1.print(OS);
    OS << " = sub ";
    op2.print(OS);
    OS << ", ";
    op3.print(OS);
    break;

  case Mul:
    op1.print(OS);
    OS << " = mul ";
    op2.print(OS);
    OS << ", ";
    op3.print(OS);
    break;

  case Div:
    op1.print(OS);
    OS << " = div ";
    op2.print(OS);
    OS << ", ";
    op3.print(OS);
    break;

  case Gt:
    op1.print(OS);
    OS << " = ";
    op2.print(OS);
    OS << " > ";
    op3.print(OS);
    break;

  case Lt:
    op1.print(OS);
    OS << " = ";
    op2.print(OS);
    OS << " < ";
    op3.print(OS);
    break;

  default:
    OS << "Unkown";
    break;
  }
}

void Instruction::dump(std::ostream &OS) const {
  OS << '<';
  switch (opCode) {
  case RegDef:
    OS << "RegDef";
    break;

  case Br:
    OS << "Br";
    break;

  case Jmp:
    OS << "Jmp";
    break;

  case LabelDef:
    OS << "LabelDef";
    break;

  case Add:
    OS << "Add";
    break;

  case Sub:
    OS << "Sub";
    break;

  case Mul:
    OS << "Mul";
    break;

  case Div:
    OS << "Div";
    break;

  case Gt:
    OS << "Gt";
    break;

  case Lt:
    OS << "Lt";
    break;

  default:
    OS << "Unkown";
    break;
  }
  OS << '>';
  OS << ' ';
  op1.dump(OS);

  if (numOperands >= 2) {
    OS << ' ';
    op2.dump(OS);
  }

  if (numOperands == 3) {
    OS << ' ';
    op3.dump(OS);
  }
}
