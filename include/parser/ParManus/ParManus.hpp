#ifndef __VESYLA_PARSER_PAR_MANUS_HPP__
#define __VESYLA_PARSER_PAR_MANUS_HPP__

#include <boost/log/trivial.hpp>
#include "ParManus_parser.hpp"
#include "ir/IrManus/IrManus.hpp"

using namespace vesyla::ir;

namespace vesyla{
namespace parser{

class ParManus{

public:
	ParManus(int row_num, int col_num, int refi_total_entry, IrManus* ir_out);
	void run(string filename);
};

}
}

#endif // __VESYLA_PARSER_PAR_MANUS_HPP__
