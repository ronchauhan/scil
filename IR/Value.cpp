#include "IR/Value.h"

#include <cassert>
#include <iostream>

Value::Value(unsigned kind, const std::string &name) {
  assert((kind == Register || kind == Label) &&
         "Value kind must be Register or Label");
  this->kind = kind;
  this->name = name;
}

Value::Value(unsigned kind, int64_t immediateValue) {
  assert(kind == Immediate && "Value kind must be literal");
  this->kind = kind;
  this->immediateValue = immediateValue;
}

const std::string &Value::getName() {
  assert((kind == Register || kind == Label) &&
         "Value kind must be Register or Label");
  return name;
}

int64_t Value::getValue() {
  assert(kind == Immediate && "Value kind must be Immediate");
  return immediateValue;
}

void Value::dump() const {
  std::cout << '<';
  switch (kind) {
  case Register:
    std::cout << "Reg:" << name;
    break;

  case Immediate:
    std::cout << "Imm:" << immediateValue;
    break;

  case Label:
    std::cout << "Label:" << name;
    break;

  default:
    std::cout << "Invalid";
    break;
  }
  std::cout << '>';
}
