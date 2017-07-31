#ifndef __VESYLA_IR_IR_MANAS_LOOPHEADER_HPP__
#define __VESYLA_IR_IR_MANAS_LOOPHEADER_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasLoopheader : public IrManasBase{
public:
	int index_raccu_addr;
	int index_start;
	int iter_no_sd;
	int iter_no;
};

}
}

#endif // __VESYLA_IR_IR_MANAS_LOOPHEADER_HPP__
