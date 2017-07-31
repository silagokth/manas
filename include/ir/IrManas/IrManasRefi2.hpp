#ifndef __VESYLA_IR_IR_MANAS_REFI2_HPP__
#define __VESYLA_IR_IR_MANAS_REFI2_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasRefi2 : public IrManasBase{
public:
	int step_val_sd;
	int step_val;
	int step_val_sign;
	int refi_middle_delay_sd;
	int refi_middle_delay;
	int no_of_reps_sd;
	int no_of_reps;
	int rpt_step_value;
};
}
}

#endif // __VESYLA_IR_IR_MANAS_REFI2_HPP__
