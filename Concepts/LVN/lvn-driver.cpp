#include "IR/CFG.h"
#include "Parser/Parser.h"

#include <cassert>
#include <iostream>

#include "Concepts/LVN/LVN.h"
#include "Concepts/LVN/LVNPass.h"

int main(int argc, char *argv[]) {
  assert(argc == 2 && "expected exactly one argument");
  Parser parser(argv[1]);
  CFG *theCFG = parser.getCFG();

  if (parser.foundErrors()) {
    std::cout << "Exiting due to parsing errors\n";
    exit(1);
  }

  LVNPass LVNP{};
  const std::vector<CFGBlock *> blocks = theCFG->getBlocks();
  for (auto B : blocks) {
    LVNP.runOnBlock(B);

    std::cout << "In block " << B->getName() << '\n';
    LVNP.printTable();
    std::cout << '\n';
  }
}
