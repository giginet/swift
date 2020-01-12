#include "swift/SILOptimizer/PassManager/Passes.h"
#include "swift/SIL/SILFunction.h"
#include "swift/SIL/SILInstruction.h"
#include "swift/SIL/SILModule.h"
#include "swift/SILOptimizer/Utils/Local.h"
#include "swift/SILOptimizer/PassManager/Transforms.h"
#include "llvm/Support/CommandLine.h"

using namespace swift;

namespace {
class WaiWaiOptimizer : public swift::SILFunctionTransform {
  /// The entry point to the transformation.
  void run() override {
      
  }
};
}


SILTransform *swift::createWaiWaiOptimizer() {
  return new WaiWaiOptimizer();
}
