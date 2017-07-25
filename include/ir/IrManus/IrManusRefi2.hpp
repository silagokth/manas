#ifndef __VESYLA_IR_IR_MANUS_REFI2_HPP__
#define __VESYLA_IR_IR_MANUS_REFI2_HPP__

#include "IrManusBase.hpp"

namespace vesyla::ir{

class IrManusRefi2 : public IrManusBase{
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

#endif // __VESYLA_IR_IR_MANUS_REFI2_HPP__
