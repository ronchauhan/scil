#ifndef IR_VALUE_H
#define IR_VALUE_H

#include <string>

// Defines a value in SIL
// A value is either a virtual register, a label, or a 64-bit signed integer
// (immediate value)
class Value {
public:
  enum { Invalid = 1, Immediate, Register, Label };

private:
  unsigned kind;
  std::string name; // Name of virtual register or label.
  int64_t immediateValue;

public:
  Value() : kind(Invalid) {}

  // for label and registers
  Value(unsigned kind, const std::string &name);

  // for immediate values
  Value(unsigned kind, int64_t immediateValue);

  unsigned getKind() const { return kind; }

  const std::string &getName();

  int64_t getValue();

  void dump() const;
};

#endif
