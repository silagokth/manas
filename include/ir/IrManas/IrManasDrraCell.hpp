#ifndef __VESYLA_IR_IR_MANAS_DRRA_CELL_HPP__
#define __VESYLA_IR_IR_MANAS_DRRA_CELL_HPP__

namespace vesyla{
namespace ir{

class IrManasDrraCell(){

public:
	IrManasDpu dpu;
	IrManasRefi refi;
	IrManasSequncer sequencer;
	IrManasSwitchBox swb_r;
	IrManasSwitchBox swb_d;
	vector<IrManasInstruction> instr_list;
};

}
}



#endif // __VESYLA_IR_IR_MANAS_DRRA_CELL_HPP__
