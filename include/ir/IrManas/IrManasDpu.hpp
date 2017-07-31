#ifndef __VESYLA_IR_IR_MANAS_DPU_HPP__
#define __VESYLA_IR_IR_MANAS_DPU_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasDpu : public IrManasBase{
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
}
#endif // __VESYLA_IR_IR_MANAS_DPU_HPP__
