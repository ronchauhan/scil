#include "Concepts/Dominance/DominatorsAnalysis.h"

#include <cassert>
#include <iostream>
#include <queue>

void DominatorsAnalysis::applyMeetOperator(std::vector<bool> &dest,
                                           const std::vector<bool> &src) {
  assert(dest.size() == src.size());
  for (int i = 0; i < dest.size(); ++i) {
    dest[i] = dest[i] & src[i];
  }
}

void DominatorsAnalysis::propagateValues(std::vector<bool> &dest,
                                         const std::vector<bool> &src) {
  assert(dest.size() == src.size());
  for (int i = 0; i < dest.size(); ++i) {
    dest[i] = src[i];
  }
}

void DominatorsAnalysis::updateValues(CFGBlock *block) {
  // Apply meet operator
  auto predecessors = block->getPredecessors();
  if (!predecessors.empty()) {
    std::vector<bool> newIn(In[block].size(), 1);
    for (auto predecessor : predecessors) {
      applyMeetOperator(newIn, Out[predecessor]);
    }
    In[block] = newIn;
  }

  // Transfer function
  unsigned blockIdx = blockToNumberMap[block];
  propagateValues(Out[block], In[block]);
  Out[block][blockIdx] = 1;
}

void DominatorsAnalysis::runDFS(CFGBlock *block, unsigned &n) {
  visitedSet.insert(block);
  depthFirstSequence[n] = block;
  numberToBlockMap[n] = block;
  blockToNumberMap[block] = n;

  auto successors = block->getSuccessors();
  for (auto *successor : successors) {
    if (visitedSet.find(successor) == visitedSet.end()) {
      runDFS(successor, ++n);
    }
  }
}

void DominatorsAnalysis::run(CFG *theCFG) {
  unsigned dfsNumber = 0;
  unsigned numBlocks = theCFG->getNumBlocks();
  depthFirstSequence.resize(numBlocks);

  CFGBlock *entryBlock = theCFG->getEntryBlock();
  runDFS(entryBlock, dfsNumber);
  assert(dfsNumber == numBlocks - 1);

  std::queue<CFGBlock *> workList;
  // Initialize values
  for (int i = 0; i < depthFirstSequence.size(); ++i) {
    CFGBlock *block = depthFirstSequence[i];

    In[block].resize(numBlocks);
    std::fill(In[block].begin(), In[block].end(), 0);

    Out[block].resize(numBlocks);
    std::fill(Out[block].begin(), Out[block].end(), 1);

    workList.push(block);
  }

  while (!workList.empty()) {
    CFGBlock *currentBlock = workList.front();
    workList.pop();

    std::vector<bool> oldOut(Out[currentBlock]);
    updateValues(currentBlock);
    auto &newOut = Out[currentBlock];

    // Check whether data flow values changed
    if (oldOut != newOut) {
      // Add successors to worklist
      for (auto successor : currentBlock->getSuccessors())
        workList.push(successor);
    }
  }
}

void DominatorsAnalysis::printResults() {
  unsigned numBlocks = numberToBlockMap.size();
  for (int i = 0; i < numBlocks; ++i) {
    CFGBlock *block = numberToBlockMap[i];

    std::cout << "dominators at In[" << block->getName() << "] : ";
    for (int j = 0; j < numBlocks; ++j) {
      if (In[block][j]) {
        CFGBlock *dominator = numberToBlockMap[j];
        std::cout << dominator->getName() << ' ';
      }
    }
    std::cout << '\n';

    std::cout << "dominators at Out[" << block->getName() << "] : ";
    for (int j = 0; j < numBlocks; ++j) {
      if (Out[block][j]) {
        CFGBlock *dominator = numberToBlockMap[j];
        std::cout << dominator->getName() << ' ';
      }
    }

    std::cout << '\n';
    if (i != numBlocks - 1)
      std::cout << '\n';
  }
}
