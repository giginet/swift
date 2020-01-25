#include "swift/SILOptimizer/PassManager/Passes.h"
#include "swift/SIL/SILFunction.h"
#include "swift/SIL/SILInstruction.h"
#include "swift/SIL/SILModule.h"
#include "swift/SILOptimizer/Utils/Local.h"
#include "swift/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/CommandLine.h"
#include <iostream>

using namespace swift;
using namespace std;

namespace {
class WaiWaiOptimizer : public SILFunctionTransform {
  /// The entry point to the transformation.
  void run() override {
    // [No.1]: Assume Single Threadedを書く
    // Hint: Referenceカウンタを操作するInstructionをすべてnon atomicにする
  }
};
}


SILTransform *swift::createWaiWaiOptimizer() {
  return new WaiWaiOptimizer();
}