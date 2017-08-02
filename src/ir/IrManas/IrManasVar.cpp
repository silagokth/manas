#include "ir/IrManas/IrManasVar.hpp"

namespace vesyla{
namespace ir{

string distr_type_to_str(DistrType t){
	string s;
	switch(t){
	case DISTR_FULL: s = "DISTR_FULL"; break;
	case DISTR_EVEN: s = "DISTR_EVEN"; break;
	default: LOG(ERROR) << "No such distribution type"; break;
	}
	return s;
}

}
}
