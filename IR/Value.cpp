#include "IR/Value.h"

#include <cassert>
#include <cstring>
#include <iostream>

Value::Value(unsigned kind, const char *name) {
  assert((kind == Register || kind == Label) &&
         "Value kind must be Register or Label");
  this->kind = kind;

  size_t nameLength = std::strlen(name);
  assert(nameLength <= 7 && "Name length for Register or Label must be <= 7");
  std::memcpy(this->vHeld.name, name, nameLength + 1);
}

Value::Value(unsigned kind, int64_t immediateValue) {
  assert(kind == Immediate && "Value kind must be immediate");
  this->kind = kind;
  this->vHeld.immediateValue = immediateValue;
}

const char *Value::getName() const {
  assert((kind == Register || kind == Label) &&
         "Value kind must be Register or Label");
  return vHeld.name;
}

int64_t Value::getValue() const {
  assert(kind == Immediate && "Value kind must be Immediate");
  return vHeld.immediateValue;
}

void Value::print(std::ostream &OS) const {
  switch (kind) {
  case Register:
    OS << vHeld.name;
    break;

  case Immediate:
    OS << vHeld.immediateValue;
    break;

  case Label:
    OS << vHeld.name;
    break;

  default:
    OS << "Invalid";
    break;
  }
}

void Value::dump(std::ostream &OS) const {
  OS << '<';
  switch (kind) {
  case Register:
    OS << "Reg:" << vHeld.name;
    break;

  case Immediate:
    OS << "Imm:" << vHeld.immediateValue;
    break;

  case Label:
    OS << "Label:" << vHeld.name;
    break;

  default:
    OS << "Invalid";
    break;
  }
  OS << '>';
}
