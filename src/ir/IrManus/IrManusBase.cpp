#include "ir/IrManus/IrManusBase.hpp"

namespace vesyla::ir{

string instr_type_to_str(InstrType t){
	string s;
	switch(t){
	case INSTR_DPU: s = "INSTR_DPU"; break;
	case INSTR_REFI1: s = "INSTR_REFI1"; break;
	case INSTR_REFI2: s = "INSTR_REFI2"; break;
	case INSTR_REFI3: s = "INSTR_REFI3"; break;
	case INSTR_DELAY: s = "INSTR_DELAY"; break;
	case INSTR_RACCU: s = "INSTR_RACCU"; break;
	case INSTR_LOOPHEADER: s = "INSTR_LOOPHEADER"; break;
	case INSTR_LOOPTAIL: s = "INSTR_LOOPTAIL"; break;
	case INSTR_SWB: s = "INSTR_SWB"; break;
	case INSTR_BRANCH: s = "INSTR_BRANCH"; break;
	case INSTR_JUMP: s = "INSTR_JUMP"; break;
	case INSTR_SRAMREAD: s = "INSTR_SRAMREAD"; break;
	case INSTR_SRAMWRITE: s = "INSTR_SRAMWRITE"; break;
	case INSTR_ROUTE: s = "INSTR_ROUTE"; break;
	case INSTR_HALT: s = "INSTR_HALT"; break;
	default: BOOST_LOG_TRIVIAL(error) << "No such instruction type"; break;
	}
	return s;
}

}
