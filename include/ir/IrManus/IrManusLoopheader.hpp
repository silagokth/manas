#ifndef __VESYLA_IR_IR_MANUS_LOOPHEADER_HPP__
#define __VESYLA_IR_IR_MANUS_LOOPHEADER_HPP__

#include "IrManusBase.hpp"

namespace vesyla{
namespace ir{

class IrManusLoopheader : public IrManusBase{
public:
	int index_raccu_addr;
	int index_start;
	int iter_no_sd;
	int iter_no;
};

}
}

#endif // __VESYLA_IR_IR_MANUS_LOOPHEADER_HPP__
