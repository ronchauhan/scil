#ifndef IR_INSTRUCTION_H
#define IR_INSTRUCTION_H

#include "Value.h"

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
  unsigned opCode;
  unsigned numOperands;
  Value op1, op2, op3;

public:
  // RegDef
  Instruction(unsigned opCode, const Value &dest, const Value &src);

  // branch, relational or binary operations
  Instruction(unsigned opCode, const Value &destOrCond,
              const Value &srcOrTrueLabel, const Value &srcOrFalseLabel);

  // jmp or label
  Instruction(unsigned opCode, const Value &label);

  // TODO:
  // subscript operator based on numOperands

  void dump() const;
};

#endif