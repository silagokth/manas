#ifndef __VESYLA_IR_IR_MANAS_SWB_HPP__
#define __VESYLA_IR_IR_MANAS_SWB_HPP__

#include "IrManasBase.hpp"
namespace vesyla{
namespace ir{
class IrManasSwb : public IrManasBase{
public:
	int from_block;
	int from_address;
	int from_port;
	int to_block;
	int to_address;
	int to_port;
};
}
}
#endif // __VESYLA_IR_IR_MANAS_SWB_HPP__
