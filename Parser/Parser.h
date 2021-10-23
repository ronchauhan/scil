#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include "IR/CFG.h"
#include "IR/Instruction.h"

#include <string>
#include <vector>

struct Token {
  enum {
    Invalid = 1,
    Dot,
    Comma,
    Ident,
    IntLiteral,
    Assign,

    // OpCodes
    Add,
    Sub,
    Mul,
    Div,
    GreaterThan,
    LessThan,
    Jmp,
    Br
  };

  std::string string;
  unsigned kind;

  void clear() {
    string.clear();
    kind = Invalid;
  }

  bool isOpCode() const { return kind >= Add && kind <= Br; }
  bool isBinaryOpCode() const { return kind >= Add && kind <= Div; }
  bool isRelationalOpCode() const {
    return kind >= GreaterThan && kind <= LessThan;
  }
};

// Basic SCIL parser with ad-hoc error reporting.
// TODO:
// Enforce the following:
//  - First instruction must be a label (entry label).
//  - Instruction after a terminator must also be a label.
class Parser {
  std::string fileName;
  std::vector<Instruction *> instList;
  bool errGlobalState; // We stop creating IR objects once this is set. But we
                       // still continue parsing as we might find more errors.
  CFG *theCFG;

public:
  Parser(const std::string &fileName)
      : fileName(fileName), errGlobalState(false), theCFG(nullptr) {}

  bool foundErrors() const { return errGlobalState == true; }

  CFG *getCFG();

private:
  void tokenizeInstString(std::string instString, std::vector<Token> &tokens);

  // Methods that map tokenKind to instruction opcodes
  unsigned getInstBinaryOpCode(unsigned tokenKind) const;
  unsigned getInstRelationalOpCode(unsigned tokenKind) const;

  // Parse line by line, creating instructions
  void parseFile();

  bool isValidCharacter(char c) const;

  bool tryRecognizeOpCode(Token &token);

  bool parseInstruction(std::vector<Token> &tokens);

  bool parseLabelDef(std::vector<Token> &tokens);

  bool parseDef(std::vector<Token> &tokens);

  // dest = src
  bool parseRegDef(std::vector<Token> &tokens);

  // dest = binOp src1, src2
  bool parseBinaryOpInst(std::vector<Token> &tokens);

  // dest = src1 relOp src2
  bool parseRelationalOpInst(std::vector<Token> &tokens);

  // jmp label
  bool parseJumpInst(std::vector<Token> &tokens);

  // br condition trueLabel falseLabel
  bool parseBranchInst(std::vector<Token> &tokens);

  std::vector<Instruction *> &getParsedInstructionList() { return instList; }

  void buildCFG();
};

#endif
