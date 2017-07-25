#ifndef __VESYLA_IR_IR_MANUS_JUMP_HPP__
#define __VESYLA_IR_IR_MANUS_JUMP_HPP__

#include "IrManusBase.hpp"

namespace vesyla::ir{

class IrManusJump : public IrManusBase{
public:
	int true_address;
};

}

#endif // __VESYLA_IR_IR_MANUS_JUMP_HPP__
