#ifndef __VESYLA_IR_IR_MANUS_VAR_HPP__
#define __VESYLA_IR_IR_MANUS_VAR_HPP__

#include <boost/log/trivial.hpp>
#include <vector>

using namespace std;

namespace vesyla::ir{

enum DistrType{
	DISTR_FULL,
	DISTR_EVEN,
	DISTR_COUNT
};

string distr_type_to_str(DistrType t);

class IrManusVar{

public:
	string name;
	DistrType distr;
	vector<vector<int>> refi;
	vector<int> value;
};

}


#endif // __VESYLA_IR_IR_MANUS_VAR_HPP__
