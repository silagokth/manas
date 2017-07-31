#ifndef __VESYLA_GENERATOR_GE_VSIM_HPP__
#define __VESYLA_GENERATOR_GE_VSIM_HPP__

#include "ir/IrManas/IrManas.hpp"
#include "generator/util/GeWriter.hpp"
#include <boost/dynamic_bitset.hpp>
#include <math.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace vesyla::ir;
using namespace std;

namespace vesyla{
namespace generator{

class GeVsim{
public:
	GeVsim(string design, IrManas* ir_in, string fabric_path, string output_dir);
	void gen_tb();
	void gen_cfg();
	void gen_profiler();
	void gen_util();
	void gen_script();

private:
	string _design;
	IrManas* _ir_in;
	string _output_dir;
	string _fabric_path;
	
	int _row ;
	int _col;
	int _instr_no;
	
	int _reg_load_cycles;
	int _refi_total_entry;
	int _reg_file_mem_addr_width;
	int _max_scheduled_cycle;
};

}
}

#endif // __VESYLA_GENERATOR_GE_VSIM_HPP__
