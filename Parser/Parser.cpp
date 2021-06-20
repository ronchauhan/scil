#include "Parser/Parser.h"
#include "IR/Instruction.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

void Parser::parseFile() {
  std::ifstream file(fileName);
  std::string instString;
  std::vector<Token> tokens;
  unsigned lineNo = 1;

  while (std::getline(file, instString)) {
    tokenizeInstString(instString, tokens);

    if (tokens.empty()) {
      ++lineNo;
      continue;
    }

    bool success = parseInstruction(tokens);
    if (!success) {
      errGlobalState = true;
      std::cerr << "Error in parsing line " << lineNo << '\n';
    }
    ++lineNo;
    tokens.clear();
  }
}

bool Parser::isValidCharacter(char c) const {
  return std::isspace(c) || std::isalnum(c) || c == '=' || c == ',' ||
         c == '.' || c == '>' || c == '<';
}

bool Parser::tryRecognizeOpCode(Token &token) {
  if (token.string.length() != 3 && token.string.length() != 2)
    return false;

  if (token.string == "add")
    token.kind = Token::Add;
  else if (token.string == "sub")
    token.kind = Token::Sub;
  else if (token.string == "mul")
    token.kind = Token::Mul;
  else if (token.string == "div")
    token.kind = Token::Div;
  else if (token.string == "jmp")
    token.kind = Token::Jmp;
  else if (token.string == "br")
    token.kind = Token::Br;

  return token.isOpCode();
}

void Parser::tokenizeInstString(std::string instString,
                                std::vector<Token> &tokens) {
  Token token;
  int i = 0;

  while (i < instString.length()) {

    if (!isValidCharacter(instString[i])) {
      token.kind = Token::Invalid;
      token.string.push_back(instString[i]);
      ++i;
    }

    // whitespace
    else if (std::isspace(instString[i])) {
      while (std::isspace(instString[i]))
        ++i;

      continue;
    }

    // identifier or opcode
    else if (std::isalpha(instString[i])) {
      token.kind = Token::Ident;
      while (std::isalnum(instString[i])) {
        token.string.push_back(instString[i]);
        ++i;
      }
      tryRecognizeOpCode(token);
    }

    else if (instString[i] == '=') {
      token.kind = Token::Assign;
      ++i;
    }

    else if (instString[i] == '>') {
      token.kind = Token::GreaterThan;
      ++i;
    }

    else if (instString[i] == '<') {
      token.kind = Token::LessThan;
      ++i;
    }

    else if (instString[i] == ',') {
      token.kind = Token::Comma;
      ++i;
    }

    else if (instString[i] == '.') {
      token.kind = Token::Dot;
      ++i;
    }

    // integer literal
    else if (std::isdigit(instString[i])) {
      token.kind = Token::IntLiteral;
      while (std::isdigit(instString[i])) {
        token.string.push_back(instString[i]);
        ++i;
      }
    }
    tokens.push_back(token);
    token.clear();
  }
}

bool Parser::parseInstruction(std::vector<Token> &tokens) {
  switch (tokens[0].kind) {
  case Token::Ident:
    return parseDef(tokens);
  case Token::Dot:
    return parseLabelDef(tokens);
  case Token::Br:
    return parseBranchInst(tokens);
  case Token::Jmp:
    return parseJumpInst(tokens);
  default:
    return false;
  }
}

bool Parser::parseLabelDef(std::vector<Token> &tokens) {
  assert(tokens[0].kind == Token::Dot && "Expected Token::Dot");

  if (tokens.size() != 2)
    return false;

  if (tokens[1].kind != Token::Ident)
    return false;

  if (!errGlobalState) {
    Value labelName(Value::Label, tokens[1].string.c_str());
    instList.emplace_back(Instruction(Instruction::LabelDef, labelName));
  }
  return true;
}

bool Parser::parseDef(std::vector<Token> &tokens) {
  assert(tokens[0].kind == Token::Ident && "Expected Token::Ident");

  switch (tokens.size()) {
  case 3:
    return parseRegDef(tokens);
  case 5:
    return parseRelationalOpInst(tokens);
  case 6:
    return parseBinaryOpInst(tokens);
  default:
    return false;
  }
}

bool Parser::parseRegDef(std::vector<Token> &tokens) {
  assert(tokens.size() == 3 && "RegDef must have 3 tokens");
  assert(tokens[0].kind == Token::Ident && "Expected Token::Ident");

  if (tokens[1].kind != Token::Assign)
    return false;
  if (tokens[2].kind != Token::IntLiteral && tokens[2].kind != Token::Ident)
    return false;

  if (!errGlobalState) {
    Value dest(Value::Register, tokens[0].string.c_str());

    unsigned srcKind = tokens[2].kind;
    Value src;
    if (srcKind == Token::IntLiteral)
      src = Value(Value::Immediate, std::stoll(tokens[2].string));
    else
      src = Value(Value::Register, tokens[2].string.c_str());

    instList.emplace_back(Instruction(Instruction::RegDef, dest, src));
  }
  return true;
}

unsigned Parser::getInstBinaryOpCode(unsigned tokenKind) const {
  assert(tokenKind >= Token::Add && tokenKind <= Token::Div &&
         "Expected a binary opcode");
  switch (tokenKind) {
  case Token::Add:
    return Instruction::Add;
  case Token::Sub:
    return Instruction::Sub;
  case Token::Mul:
    return Instruction::Mul;
  case Token::Div:
    return Instruction::Div;
  }
}

unsigned Parser::getInstRelationalOpCode(unsigned tokenKind) const {
  assert(tokenKind >= Token::GreaterThan && tokenKind <= Token::LessThan &&
         "Expected a relational opcode");
  switch (tokenKind) {
  case Token::GreaterThan:
    return Instruction::Gt;
  case Token::LessThan:
    return Instruction::Lt;
  }
}

bool Parser::parseBinaryOpInst(std::vector<Token> &tokens) {
  assert(tokens.size() == 6 && "BinaryOp inst has 6 tokens");
  assert(tokens[0].kind == Token::Ident && "Expected Token::Ident");

  if (tokens[1].kind != Token::Assign)
    return false;
  if (!tokens[2].isBinaryOpCode())
    return false;
  if (tokens[3].kind != Token::Ident && tokens[3].kind != Token::IntLiteral)
    return false;
  if (tokens[4].kind != Token::Comma)
    return false;
  if (tokens[5].kind != Token::Ident && tokens[5].kind != Token::IntLiteral)
    return false;

  if (!errGlobalState) {
    Value dest(Value::Register, tokens[0].string.c_str());

    unsigned srcKind = tokens[3].kind;
    Value src1;
    if (srcKind == Token::IntLiteral)
      src1 = Value(Value::Immediate, std::stoll(tokens[3].string));
    else
      src1 = Value(Value::Register, tokens[3].string.c_str());

    srcKind = tokens[5].kind;
    Value src2;
    if (srcKind == Token::IntLiteral)
      src2 = Value(Value::Immediate, std::stoll(tokens[5].string));
    else
      src2 = Value(Value::Register, tokens[5].string.c_str());

    unsigned opCode = getInstBinaryOpCode(tokens[2].kind);
    instList.emplace_back(Instruction(opCode, dest, src1, src2));
  }
  return true;
}

bool Parser::parseRelationalOpInst(std::vector<Token> &tokens) {
  assert(tokens.size() == 5 && "BinaryOp inst has 5 tokens");
  assert(tokens[0].kind == Token::Ident && "Expected Token::Ident");

  if (tokens[1].kind != Token::Assign)
    return false;
  if (tokens[2].kind != Token::Ident && tokens[2].kind != Token::IntLiteral)
    return false;
  if (!tokens[3].isRelationalOpCode())
    return false;
  if (tokens[4].kind != Token::Ident && tokens[4].kind != Token::IntLiteral)
    return false;

  if (!errGlobalState) {
    Value dest(Value::Register, tokens[0].string.c_str());

    unsigned srcKind = tokens[2].kind;
    Value src1;
    if (srcKind == Token::IntLiteral)
      src1 = Value(Value::Immediate, std::stoll(tokens[2].string));
    else
      src1 = Value(Value::Register, tokens[2].string.c_str());

    srcKind = tokens[4].kind;
    Value src2;
    if (srcKind == Token::IntLiteral)
      src2 = Value(Value::Immediate, std::stoll(tokens[4].string));
    else
      src2 = Value(Value::Register, tokens[4].string.c_str());

    unsigned opCode = getInstRelationalOpCode(tokens[3].kind);
    instList.emplace_back(Instruction(opCode, dest, src1, src2));
  }
  return true;
}

bool Parser::parseBranchInst(std::vector<Token> &tokens) {
  assert(tokens[0].kind == Token::Br && "Expected Token::Br");

  if (tokens.size() != 4)
    return false;

  for (int i = 1; i < tokens.size(); ++i) {
    if (tokens[i].kind != Token::Ident)
      return false;
  }

  if (!errGlobalState) {
    Value cond(Value::Register, tokens[1].string.c_str());
    Value trueLabel(Value::Label, tokens[2].string.c_str());
    Value falseLabel(Value::Label, tokens[3].string.c_str());
    instList.emplace_back(
        Instruction(Instruction::Br, cond, trueLabel, falseLabel));
  }
  return true;
}

bool Parser::parseJumpInst(std::vector<Token> &tokens) {
  assert(tokens[0].kind == Token::Jmp && "Expected Token::Jmp");

  if (tokens.size() != 2)
    return false;
  if (tokens[1].kind != Token::Ident)
    return false;

  if (!errGlobalState) {
    Value targetLabel(Value::Label, tokens[1].string.c_str());
    instList.emplace_back(Instruction(Instruction::Jmp, targetLabel));
  }
  return true;
}
