#ifndef __VESYLA_IR_IR_MANAS_REFI1_HPP__
#define __VESYLA_IR_IR_MANAS_REFI1_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasRefi1 : public IrManasBase{
public:
	 int reg_file_port;
	 int subseq_instrs;
	 int start_addrs_sd;
	 int start_addrs;
	 int no_of_addrs_sd;
	 int no_of_addrs;
	 int initial_delay_sd;
	 int initial_delay;
};
}
}

#endif // __VESYLA_IR_IR_MANAS_REFI1_HPP__
