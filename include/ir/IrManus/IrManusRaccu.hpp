#ifndef __VESYLA_IR_IR_MANUS_RACCU_HPP__
#define __VESYLA_IR_IR_MANUS_RACCU_HPP__

#include "IrManusBase.hpp"

namespace vesyla::ir{

class IrManusRaccu : public IrManusBase{
public:
	int raccu_mode;
	int raccu_op1_sd;
	int raccu_op1;
	int raccu_op2_sd;
	int raccu_op2;
	int raccu_result_addrs;
};

}

#endif // __VESYLA_IR_IR_MANUS_RACCU_HPP__
