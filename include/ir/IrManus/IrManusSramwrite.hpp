#ifndef __VESYLA_IR_IR_MANUS_SRAMWRITE_HPP__
#define __VESYLA_IR_IR_MANUS_SRAMWRITE_HPP__

#include "IrManusBase.hpp"
namespace vesyla::ir{

class IrManusSramwrite : public IrManusBase{
public:
	int mode;
	int initial_address;
	int initial_delay;
	int loop1_iterations;
	int loop1_increment;
	int loop1_delay;
	int loop2_iterations;
	int loop2_increment;
	int loop2_delay;
	int sram_initial_address_sd;
	int sram_loop1_iteration_sd;
	int sram_loop2_iteration_sd;
	int sram_initial_delay_sd;
	int sram_loop1_delay_sd;
	int sram_loop2_delay_sd;
	int sram_loop1_increment_sd;
	int sram_loop2_increment_sd;
};
}
#endif // __VESYLA_IR_IR_MANUS_SRAMWRITE_HPP__
