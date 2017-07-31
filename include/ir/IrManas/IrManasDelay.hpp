#ifndef __VESYLA_IR_IR_MANAS_DELAY_HPP__
#define __VESYLA_IR_IR_MANAS_DELAY_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasDelay : public IrManasBase{
public:
	int del_cycles_sd;
	int del_cycles;
};
}
}

#endif // __VESYLA_IR_IR_MANAS_DELAY_HPP__
