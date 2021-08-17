#ifndef IR_VALUE_H
#define IR_VALUE_H

#include <cstdint>
#include <iostream>

// Defines a value in SCIL
// A value is either a virtual register, a label, or a 64-bit signed integer
// (immediate value). For simplicity, we limit the name length to 7.
class Value {
public:
  enum { Invalid = 1, Immediate, Register, Label };

  union ValueHeld {
    int64_t immediateValue;
    char name[8]; // Name of virtual register or label.
  };

private:
  uint8_t kind;
  ValueHeld vHeld;

public:
  Value() : kind(Invalid) {}

  // for label and registers
  Value(uint8_t kind, const char *name);

  // for immediate values
  Value(uint8_t kind, int64_t immediateValue);

  unsigned getKind() const { return kind; }

  const char *getName() const;

  int64_t getValue() const;

  void print(std::ostream &OS) const;

  void dump(std::ostream &OS) const;
};

#endif
