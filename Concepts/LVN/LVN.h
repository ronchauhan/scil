#ifndef CONCEPTS_LVN_LVN_TABLE_H
#define CONCEPTS_LVN_LVN_TABLE_H

#include "IR/Instruction.h"
#include <ostream>
#include <unordered_map>
#include <vector>

class LVNEntry;
class LVNTable;

//----------------------------------------------------------------------------//
//  LVN table, meant to be set up separately for each basic block
//
//  For example :
//  +--------------+-----------------------------+
//  |              |          LVNEntry           |
//  |  ValueNumber +--------------+--------------+  example:
//  |     (#)      |  Expression  |   Reg/Imm    |
//  +--------------+--------------+--------------+
//  |      0       |      4       |      4       |  a = 4
//  |      1       |      #1      |      a       |  b = 5
//  |      2       |      5       |      5       |  c = a + b
//  |      3       |      #2      |      b       |
//  |      4       |  Add #1 #3   |      c       |
//  |     ...      |     ...      |     ...      |
//  |     ...      |     ...      |     ...      |
//  |              |              |              |
//
//----------------------------------------------------------------------------//

class LVNTable {
  // The mapped number is the value number, as well as the index of the
  // corresponding LVNEntry
  std::unordered_map<Value, int8_t, ValueHashFunction> valueNumberMap;
  std::vector<LVNEntry> LVNEntries;

public:
  LVNTable() {}

  void clear();

  int8_t getValueNumberFor(const Value &V) const;

  bool addEntry(uint8_t opCode, int8_t operand1Number, int8_t operand2Number,
                const Value &holdingValue);

  bool isEmpty() const { return LVNEntries.empty(); }

  void dump(std::ostream &OS) const;
};

// Storing value (#n1 op #n2) or constant number or register name
class LVNEntry {
  uint8_t opCode; // Same as instruction opcode

  // These hold the entry numbers for mapped operands. Using shorter ints as
  // basic blocks won't be large anyways.
  int8_t operand1ValueNumber;
  int8_t operand2ValueNumber;

  Value holdingValue; // The register name holding the value number, or the
                      // constant value itself (see IR/Value.h).

  friend bool LVNTable::addEntry(uint8_t opCode, int8_t operand1Number,
                                 int8_t operand2Number,
                                 const Value &holdingValue);

public:
  LVNEntry(uint8_t opCode, int8_t operand1ValueNumber,
           int8_t operand2ValueNumber, const Value &holdingValue);

  const Value &getholdingValue() const { return holdingValue; }

  void dump(std::ostream &OS) const;

  void dumpOpCode(std::ostream &OS) const;
};

#endif
