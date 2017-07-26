#ifndef __VESYLA_IR_IR_MANUS_BRANCH_HPP__
#define __VESYLA_IR_IR_MANUS_BRANCH_HPP__

#include "IrManusBase.hpp"

namespace vesyla{
namespace ir{

class IrManusBranch : public IrManusBase{
public:
	 int branch_mode;
	 int branch_false_address;
};

}
}

#endif // __VESYLA_IR_IR_MANUS_BRANCH_HPP__
