#ifndef __VESYLA_IR_IR_MANUS_REFI1_HPP__
#define __VESYLA_IR_IR_MANUS_REFI1_HPP__

#include "IrManusBase.hpp"

namespace vesyla::ir{

class IrManusRefi1 : public IrManusBase{
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

#endif // __VESYLA_IR_IR_MANUS_REFI1_HPP__
