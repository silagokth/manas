#ifndef __VESYLA_PARSER_PAR_MANAS_HPP__
#define __VESYLA_PARSER_PAR_MANAS_HPP__

#include "util/Common.hpp"
#include "ParManas_parser.hpp"
#include "ir/IrManas/IrManas.hpp"

using namespace vesyla::ir;

namespace vesyla{
namespace parser{

class ParManas{

public:
	ParManas(int row_num, int col_num, int refi_total_entry, IrManas* ir_out);
	void run(string filename);
};

}
}

#endif // __VESYLA_PARSER_PAR_MANAS_HPP__
