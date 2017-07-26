#ifndef __VESYLA_IR_IR_MANUS_LOOPTAIL_HPP__
#define __VESYLA_IR_IR_MANUS_LOOPTAIL_HPP__

#include "IrManusBase.hpp"

namespace vesyla{
namespace ir{

class IrManusLooptail : public IrManusBase{
public:
	int index_step;
	int pc_togo;
	int index_raccu_addr;
};

}
}

#endif // __VESYLA_IR_IR_MANUS_LOOPTAIL_HPP__
