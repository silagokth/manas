#ifndef __VESYLA_IR_IR_MANAS_JUMP_HPP__
#define __VESYLA_IR_IR_MANAS_JUMP_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasJump : public IrManasBase{
public:
	int true_address;
};

}
}

#endif // __VESYLA_IR_IR_MANAS_JUMP_HPP__
