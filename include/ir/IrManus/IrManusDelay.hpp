#ifndef __VESYLA_IR_IR_MANUS_DELAY_HPP__
#define __VESYLA_IR_IR_MANUS_DELAY_HPP__

#include "IrManusBase.hpp"

namespace vesyla{
namespace ir{

class IrManusDelay : public IrManusBase{
public:
	int del_cycles_sd;
	int del_cycles;
};
}
}

#endif // __VESYLA_IR_IR_MANUS_DELAY_HPP__
