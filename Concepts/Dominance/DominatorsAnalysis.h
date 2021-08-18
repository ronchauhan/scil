#ifndef CONCEPTS_DOMINANCE_DOMINATORS_ANALYSIS_H
#define CONCEPTS_DOMINANCE_DOMINATORS_ANALYSIS_H

#include "IR/CFG.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

// Find dominators at In and Out points of basic blocks.
class DominatorsAnalysis {
  std::unordered_set<CFGBlock *> visitedSet;

  // Same as reverse postorder sequence
  std::vector<CFGBlock *> depthFirstSequence;

  std::unordered_map<unsigned, CFGBlock *> numberToBlockMap;
  std::unordered_map<CFGBlock *, unsigned> blockToNumberMap;

  using BitVector = std::vector<bool>;
  std::unordered_map<CFGBlock *, BitVector> In;
  std::unordered_map<CFGBlock *, BitVector> Out;

public:
  void run(CFG *theCFG);

  void printResults();

private:
  void runDFS(CFGBlock *block, unsigned &n);

  void updateValues(CFGBlock *block);

  void propagateValues(std::vector<bool> &dest, const std::vector<bool> &src);

  void applyMeetOperator(std::vector<bool> &dest, const std::vector<bool> &src);
};

#endif
