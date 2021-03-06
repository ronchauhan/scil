#include "IR/CFG.h"
#include "Parser/Parser.h"

#include <cassert>
#include <iostream>

#include "Concepts/Dominance/DominatorsAnalysis.h"

int main(int argc, char *argv[]) {
  assert(argc == 2 && "expected exactly one argument");
  Parser parser(argv[1]);
  CFG *theCFG = parser.getCFG();

  if (parser.foundErrors()) {
    std::cout << "Exiting due to parsing errors\n";
    exit(1);
  }

  DominatorsAnalysis DA;
  DA.run(theCFG);
  DA.printResults();
}
