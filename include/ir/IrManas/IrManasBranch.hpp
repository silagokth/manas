#ifndef __VESYLA_IR_IR_MANAS_BRANCH_HPP__
#define __VESYLA_IR_IR_MANAS_BRANCH_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasBranch : public IrManasBase{
public:
	 int branch_mode;
	 int branch_false_address;
};

}
}

#endif // __VESYLA_IR_IR_MANAS_BRANCH_HPP__
