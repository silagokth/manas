#ifndef __VESYLA_IR_IR_MANAS_LOOPTAIL_HPP__
#define __VESYLA_IR_IR_MANAS_LOOPTAIL_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasLooptail : public IrManasBase{
public:
	int index_step;
	int pc_togo;
	int index_raccu_addr;
};

}
}

#endif // __VESYLA_IR_IR_MANAS_LOOPTAIL_HPP__
