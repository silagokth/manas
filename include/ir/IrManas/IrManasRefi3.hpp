#ifndef __VESYLA_IR_IR_MANAS_REFI3_HPP__
#define __VESYLA_IR_IR_MANAS_REFI3_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasRefi3 : public IrManasBase{
public:
	int rpt_delay_sd;
	int rpt_delay;
	int mode;
	int outp_cntrl;
	int fft_state;
	int refi_middle_delay_ext;
	int no_of_rpt_ext;
	int rpt_step_value_ext;
	int fft_end_stage;
	int dimarch_mode;
};

}
}

#endif // __VESYLA_IR_IR_MANAS_REFI3_HPP__
