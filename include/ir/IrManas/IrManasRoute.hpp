#ifndef __VESYLA_IR_IR_MANAS_ROUTE_HPP__
#define __VESYLA_IR_IR_MANAS_ROUTE_HPP__

#include "IrManasBase.hpp"

namespace vesyla{
namespace ir{

class IrManasRoute : public IrManasBase{
public:
	int source_row;
	int source_col;
	int destination_row;
	int destination_col;
	int drra_sel;
	int union_flag;
	int union_port;
	int origin;
};
}
}

#endif // __VESYLA_IR_IR_MANAS_ROUTE_HPP__
