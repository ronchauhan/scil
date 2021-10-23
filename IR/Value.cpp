#include "IR/Value.h"

#include <cassert>
#include <cstring>
#include <iostream>

Value::Value(uint8_t kind, const char *name) {
  assert((kind == Register || kind == Label) &&
         "Value kind must be Register or Label");
  this->kind = kind;

  size_t nameLength = std::strlen(name);
  assert(nameLength <= 7 && "Name length for Register or Label must be <= 7");
  std::memcpy(this->vHeld.name, name, nameLength + 1);
}

Value::Value(uint8_t kind, int64_t immediateValue) {
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

void Value::operator=(const Value &other) {
  this->kind = other.getKind();
  switch (this->kind) {
  case Immediate:
    this->vHeld.immediateValue = other.getValue();
    break;
  case Register:
  case Label:
    std::memcpy((void *)this->vHeld.name, (const void *)other.getName(), 8);
    break;
  }
}

bool Value::operator==(const Value &other) const {
  if (this->kind != other.getKind())
    return false;

  if (this->kind == Immediate)
    return this->getValue() == other.getValue();

  return strcmp(this->getName(), other.getName()) == 0;
}

bool Value::operator!=(const Value &other) const { return !(*this == other); }

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

size_t ValueHashFunction::operator()(const Value &V) const {
  size_t H1 = static_cast<size_t>(V.getKind());
  size_t H2 = 0;

  if (V.getKind() == Value::Immediate)
    H2 = static_cast<size_t>(V.getValue());
  else
    H2 = static_cast<size_t>(*V.getName());

  return H1 ^ (H2 - 1);
}
