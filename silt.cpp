#include <iostream>

#include "IR/CFG.h"
#include "IR/Instruction.h"
#include "Parser/Parser.h"

static bool printCFG = false;
static bool dumpCFG = false;

static char *fileName = nullptr;
static void showHelpAndExit(const char *);
static bool parseOption(const char *);

static void parseArguments(int argc, char *argv[]) {
  switch (argc) {
  case 2:
    printCFG = true;
    fileName = argv[1];
    return;

  case 3:
    if (!parseOption(argv[1])) {
      std::cout << "Invalid arguments\n\n";
      showHelpAndExit(argv[0]);
    }
    fileName = argv[2];
    return;

  default:
    showHelpAndExit(argv[0]);
  }
}

static bool parseOption(const char *option) {
  if (std::strcmp(option, "-p") == 0) {
    printCFG = true;
    return true;
  } else if (std::strcmp(option, "-d") == 0) {
    dumpCFG = true;
    return true;
  }
  return false;
}

static void showHelpAndExit(const char *toolName) {
  std::cout << toolName << " - Tool to work with SIL CFGs\n";
  std::cout << "usage :\n";
  std::cout << '\t' << toolName << "  [option]  <filename>.sil\n";
  std::cout << "options :\n";
  std::cout << "\t-p \t print CFG as parsable text (default)\n";
  std::cout << "\t-d \t dump CFG as represented in memory\n";
  exit(0);
}

int main(int argc, char *argv[]) {
  parseArguments(argc, argv);
  Parser parser(fileName);
  CFG *theCFG = parser.getCFG();

  if (printCFG)
    theCFG->print();
  else if (dumpCFG)
    theCFG->dump();
}
