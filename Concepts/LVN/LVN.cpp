#include "Concepts/LVN/LVN.h"

#include <ostream>

//----------------------------------------------------------------------------//
//
// LVNEntry
//
//----------------------------------------------------------------------------//

LVNEntry::LVNEntry(uint8_t opCode, int8_t operand1ValueNumber,
                   int8_t operand2ValueNumber, const Value &holdingValue) {
  switch (opCode) {
  case Instruction::RegDef:
    this->opCode = opCode;
    this->holdingValue = holdingValue;
    this->operand1ValueNumber = operand1ValueNumber;
    this->operand2ValueNumber = -1;
    break;

  case Instruction::Add:
  case Instruction::Sub:
  case Instruction::Mul:
  case Instruction::Div:
  case Instruction::Gt:
  case Instruction::Lt:
    this->opCode = opCode;
    this->operand1ValueNumber = operand1ValueNumber;
    this->operand2ValueNumber = operand2ValueNumber;
    this->holdingValue = holdingValue;
    break;

  default:
    break;
  }

  this->holdingValue = holdingValue;
}

void LVNEntry::dump(std::ostream &OS) const {
  OS << '(';
  dumpOpCode(OS);

  OS << ' ';
  if (operand1ValueNumber != -1)
    OS << '#' << (int)operand1ValueNumber;
  if (operand2ValueNumber != -1)
    OS << ' ' << '#' << (int)operand2ValueNumber;

  if (operand1ValueNumber == -1 && operand2ValueNumber == -1)
    holdingValue.print(OS);

  OS << ')';
  OS << " in ";
  holdingValue.dump(OS);
}

void LVNEntry::dumpOpCode(std::ostream &OS) const {
  switch (opCode) {
  case Instruction::RegDef:
    OS << "";
    break;

  case Instruction::Add:
    OS << "Add";
    break;

  case Instruction::Sub:
    OS << "Sub";
    break;

  case Instruction::Mul:
    OS << "Mul";
    break;

  case Instruction::Div:
    OS << "Div";
    break;

  case Instruction::Gt:
    OS << "Gt";
    break;

  case Instruction::Lt:
    OS << "Lt";
    break;

  default:
    OS << "Unhandled " << (int)opCode;
    break;
  }
}

//----------------------------------------------------------------------------//
//
// LVNTable
//
//----------------------------------------------------------------------------//

bool LVNTable::addEntry(uint8_t opCode, int8_t operand1ValueNumber,
                        int8_t operand2ValueNumber, const Value &holdingValue) {
  int8_t valueNumber = getValueNumberFor(holdingValue);
  if (valueNumber != -1)
    return false;

  LVNEntries.emplace_back(
      LVNEntry(opCode, operand1ValueNumber, operand2ValueNumber, holdingValue));

  valueNumberMap[holdingValue] = LVNEntries.size() - 1;
  return true;
}

int8_t LVNTable::getValueNumberFor(const Value &V) const {
  auto it = valueNumberMap.find(V);
  if (it == valueNumberMap.end())
    return -1;

  return it->second;
}

void LVNTable::clear() {
  LVNEntries.clear();
  valueNumberMap.clear();
}

void LVNTable::dump(std::ostream &OS) const {
  std::cout << LVNEntries.size() << "\n";
  for (size_t idx = 0; idx < LVNEntries.size(); ++idx) {
    OS << idx << "  ";
    LVNEntries[idx].dump(OS);
    OS << '\n';
  }
}
