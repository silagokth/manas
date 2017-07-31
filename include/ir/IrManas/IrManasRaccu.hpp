#ifndef __VESYLA_IR_IR_MANAS_RACCU_HPP__
#define __VESYLA_IR_IR_MANAS_RACCU_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasRaccu : public IrManasBase{
public:
	int raccu_mode;
	int raccu_op1_sd;
	int raccu_op1;
	int raccu_op2_sd;
	int raccu_op2;
	int raccu_result_addrs;
};

}
}

#endif // __VESYLA_IR_IR_MANAS_RACCU_HPP__
