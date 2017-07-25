#ifndef __VESYLA_IR_IR_MANUS_DPU_HPP__
#define __VESYLA_IR_IR_MANUS_DPU_HPP__

#include "IrManusBase.hpp"

namespace vesyla::ir{

class IrManusDpu : public IrManusBase{
public:
	
	int dpu_mode;
	int dpu_saturation;
	int dpu_out1;
	int dpu_out2;
	int dpu_acc_clear_rst;
	int dpu_acc_clear_sd;
	int dpu_acc_clear;
	int process_inout;
};

}
#endif // __VESYLA_IR_IR_MANUS_DPU_HPP__
