#ifndef IR_INSTRUCTION_H
#define IR_INSTRUCTION_H

#include "Value.h"
#include <cstdint>
#include <iostream>

// Defines a SIL instruction.
// Every statement in SIL is considered to be an instruction.
// Exhaustive list of all kinds of instructions in SIL :
//  dest = src
//  dest = binOp src1, src2
//  dest = src1 relOp src2
//  jmp label
//  br condition trueLabel falseLabel
// .label
class Instruction {
public:
  enum { RegDef = 1, Br, Jmp, LabelDef, Add, Sub, Mul, Div, Gt, Lt };

private:
  uint8_t opCode : 4;
  uint8_t numOperands : 4;
  Value op1, op2, op3;

public:
  // RegDef
  Instruction(uint8_t opCode, const Value &dest, const Value &src);

  // branch, relational or binary operations
  Instruction(uint8_t opCode, const Value &destOrCond,
              const Value &srcOrTrueLabel, const Value &srcOrFalseLabel);

  // jmp or label
  Instruction(uint8_t opCode, const Value &label);

  Value getOperand(uint8_t i) const;

  bool isLabelDef() const { return opCode == LabelDef; }

  bool isTerminator() const { return opCode == Jmp || opCode == Br; }

  bool isBranch() const { return opCode == Br; }

  void print(std::ostream &OS) const;

  void dump(std::ostream &OS) const;
};

#endif
