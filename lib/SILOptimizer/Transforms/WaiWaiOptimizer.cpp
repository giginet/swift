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
class WaiWaiOptimizer : public swift::SILFunctionTransform {
  /// The entry point to the transformation.
private:
    
  // DO NOT Change: 変更しないでください
  // やること: 後片付け
  void onFinished() {
    MarkAsUsefulInstSet.clear();
  }

  // [No.2]: minDCEを書く（関数を実装してください）      
  // やること: * UsefulなInsturctionの条件１を判定してMarkする　
  //          - BBの中からReturn Instを探して、それをUsefulとしてMarkする (markedAsUsefulInstSetにinsert)
  void checkReturnInstAndSetToLiveInst() {
  }
    
  // [No.2]: minDCEを書く（関数を実装してください） 
  // やること: * UsefulなInsturctionの条件２を判定してMarkする (markedAsUsefulInstSetにinsert)     
  //          - UsefulなInstructionのOperandをたどって、中にあるSILValueのInsructionをUsefulなものとしてMarkする
  //          - UsefulなInstructionの `// user: ...` をたどって、それらをUsefulなものとしてMarkする
  // Hint1: すでにMarkしたものも十分に条件２で走査しましょう。再帰をつかうとすっきりします。
  // Hint2: 十分に走査した後は、MarkされたInstructionは増えません。つまり、Markされたものが増えたら走査するべき対象がまだあるということです。
  void propagateUseful() {
  }
    
  // [No.2]: minDCEを書く        
  // やること: Functionの中のUsefulでMarkされて *ない* Instructionをすべて削除する
  void deleteNotUseful() {
  }
    
public:
  void run() override {
      
    // BBが2つ以上ある or terminatorがreturnじゃないFunctionは無視
    // 資料にある問題２の実装です。
    // * Basic Blockが1つだけで、最後のInstructionがreturnなFunctionのみ最適化する。
    //  - Basic Blockが２つ以上あるFunctionをつかうと、条件3. が必要になるため(問題の簡略化)
    // * 最初の条件1.はreturnのみUsefulなInstrucitonとして扱う
    //  - 本当は副作用のあるInstructionなどを考慮に入れないといけないが、問題の簡略化のため
    auto& BB = getFunction()->getBlocks();
    if(!(BB.size() == 1 && isa<ReturnInst>(BB.begin()->getTerminator()))) {
      return;
    }
            
    // [No.2]: minDCEを書く（実装）       
    // Hint: ここでは、propagateUsefulとcheckReturnInstAndSetToLiveInstとdeleteNotUsefulを適切な順番で呼ぶこと



        
    // 後片付け、書き換えないで良い
    onFinished();
  }
};
}


SILTransform *swift::createWaiWaiOptimizer() {
  return new WaiWaiOptimizer();
}
