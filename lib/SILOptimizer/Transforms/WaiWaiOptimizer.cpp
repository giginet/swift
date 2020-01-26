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
private:
    SmallSetVector<SILInstruction*, 32> UsefulInstractions;
    // [No.2]: minDCEを書く（必要であれば実装します）
    // やること: 後片付け。次のFunctionのminDCEを回すときに残ってまずいものは、ここでどうにかしておくこと
    // 配列とか・・・？
    void onFinished() {
        UsefulInstractions.clear();
    }
    
    // [No.2]: minDCEを書く（関数を実装してください）
    // やること: * UsefulなInsturctionの条件１を判定してMarkする
    //          - BBの中からReturn Instを探して、それをUsefulとしてMarkする
    void checkReturnInstAndSetToLiveInst() {
        auto& BB = *getFunction()->getBlocks().begin();
        for (auto &inst : BB) {
            if (auto returnI = dyn_cast<ReturnInst>(&inst)) {
                UsefulInstractions.insert(returnI);
            }
        }
        
    }
    
    // [No.2]: minDCEを書く（関数を実装してください）
    // やること: * UsefulなInsturctionの条件２を判定してMarkする
    //          - UsefulなInstructionのOperandをたどって、中にあるSILValueのInsructionをUsefulなものとしてMarkする
    //          - UsefulなInstructionの `// user: ...` をたどって、それらをUsefulなものとしてMarkする
    // Hint1: すでにMarkしたものも十分に条件２で走査しましょう。再帰をつかうとすっきりします。
    // Hint2: 十分に走査した後は、MarkされたInstructionは増えません。つまり、Markされたものが増えたら走査するべき対象がまだあるということです。
    void propagateUseful() {
        bool dirty = false;
        for (auto inst : UsefulInstractions) {
            // return
            if(auto valueI = dyn_cast<SingleValueInstruction>(inst)) {
                for (auto useO: valueI->getUses()) {
                    if(auto UseI = useO->getUser()) {
                        if (!UsefulInstractions.count(UseI)) {
                            UsefulInstractions.insert(UseI);
                            dirty = true;
                        }
                    }
                }
            }
            for(auto& o: inst->getAllOperands()) {
                if (auto I = o.get()->getDefiningInstruction()) {
                    if (!UsefulInstractions.count(I)) {
                        UsefulInstractions.insert(I);
                        dirty = true;
                    }
                }
            }
        }
        if (dirty) {
            propagateUseful();
        }
    }
    
    // [No.2]: minDCEを書く
    // やること: Functionの中のUsefulでMarkされて *ない* Instructionをすべて削除する
    void deleteNotUseful() {
        auto& BB = *getFunction()->getBlocks().begin();
        for (auto it = BB.begin(); it != BB.end();) {
            auto *inst = &*it;
            it++;
            if (!UsefulInstractions.count(inst)) {
                inst->replaceAllUsesOfAllResultsWithUndef();
                inst->eraseFromParent();
            }
        }
    }
    
public:
    void run() override {
        if (!getOptions().enableWaiwai) return;
        
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
        
        checkReturnInstAndSetToLiveInst();
        propagateUseful();
        deleteNotUseful();
        
        invalidateAnalysis(SILAnalysis::InvalidationKind::Instructions);
        
        // 後片付け、書き換えないで良い
        onFinished();
    }
};
}


SILTransform *swift::createWaiWaiOptimizer() {
    return new WaiWaiOptimizer();
}
