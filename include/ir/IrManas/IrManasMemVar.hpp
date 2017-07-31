#ifndef __VESYLA_IR_IR_MANAS_MEM_VAR_HPP__
#define __VESYLA_IR_IR_MANAS_MEM_VAR_HPP__

#include <boost/log/trivial.hpp>
#include <vector>

using namespace std;

namespace vesyla{
namespace ir{

string distr_type_to_str(DistrType t);

class IrManasVar{

public:
	string name;
	DistrType distr;
	vector<vector<int>> info;
	vector<vector<int>> value;
};

}
}


#endif // __VESYLA_IR_IR_MANAS_MEM_VAR_HPP__
