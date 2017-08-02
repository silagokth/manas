#ifndef __VESYLA_IR_IR_MANAS_VAR_HPP__
#define __VESYLA_IR_IR_MANAS_VAR_HPP__

#include "util/Common.hpp"
#include <vector>

using namespace std;

namespace vesyla{
namespace ir{

enum DistrType{
	DISTR_FULL,
	DISTR_EVEN,
	DISTR_COUNT
};

string distr_type_to_str(DistrType t);

class IrManasVar{

public:
	string name;
	DistrType distr;
	vector<vector<int>> refi;
	vector<int> value;
};

}
}

#endif // __VESYLA_IR_IR_MANAS_VAR_HPP__
