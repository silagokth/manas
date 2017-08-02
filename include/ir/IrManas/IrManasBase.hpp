#ifndef __VESYLA_IR_IR_MANAS_BASE_HPP__
#define __VESYLA_IR_IR_MANAS_BASE_HPP__

#include "util/Common.hpp"
#include <memory>

using namespace std;

namespace vesyla{
namespace ir{

enum InstrType{
	INSTR_DPU,
	INSTR_REFI1,
	INSTR_REFI2,
	INSTR_REFI3,
	INSTR_DELAY,
	INSTR_RACCU,
	INSTR_LOOPHEADER,
	INSTR_LOOPTAIL,
	INSTR_SWB,
	INSTR_BRANCH,
	INSTR_JUMP,
	INSTR_SRAMREAD,
	INSTR_SRAMWRITE,
	INSTR_ROUTE,
	INSTR_HALT,
	INSTR_COUNT
};

string instr_type_to_str(InstrType t);

class IrManasBase{
public:
	InstrType instr_type;
};

}
}

#endif // __VESYLA_IR_IR_MANAS_BASE_HPP__
