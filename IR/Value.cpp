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

void Value::dump() const {
  std::cout << '<';
  switch (kind) {
  case Register:
    std::cout << "Reg:" << vHeld.name;
    break;

  case Immediate:
    std::cout << "Imm:" << vHeld.immediateValue;
    break;

  case Label:
    std::cout << "Label:" << vHeld.name;
    break;

  default:
    std::cout << "Invalid";
    break;
  }
  std::cout << '>';
}
