#ifndef __VESYLA_IR_IR_MANUS_HPP__
#define __VESYLA_IR_IR_MANUS_HPP__

#include "IrManusBase.hpp"
#include "IrManusJump.hpp"
#include "IrManusRefi3.hpp"
#include "IrManusBranch.hpp"
#include "IrManusLoopheader.hpp"
#include "IrManusRoute.hpp"
#include "IrManusDelay.hpp"
#include "IrManusLooptail.hpp"
#include "IrManusSramread.hpp"
#include "IrManusDpu.hpp"
#include "IrManusRaccu.hpp"
#include "IrManusSramwrite.hpp"
#include "IrManusHalt.hpp"
#include "IrManusRefi1.hpp"
#include "IrManusSwb.hpp"
#include "IrManusRefi2.hpp"

#include "IrManusVar.hpp"

#include <vector>
#include <map>
#include <iostream>

using namespace std;

namespace vesyla::ir{

class IrManus{
public:
	void reset(int num_row, int num_col, int refi_total_entry);
	shared_ptr<IrManusDpu> create_instr_dpu(int row, int col, int dpu_mode, int dpu_saturation, int dpu_out1, int dpu_out2, int dpu_acc_clear_rst, int dpu_acc_clear_sd, int dpu_acc_clear, int process_inout);
	shared_ptr<IrManusRefi1> create_instr_refi1(int row, int col, int reg_file_port, int subseq_instrs, int start_addrs_sd, int start_addrs, int no_of_addrs_sd, int no_of_addrs, int initial_delay_sd, int initial_delay);
	shared_ptr<IrManusRefi2> create_instr_refi2(int row, int col, int step_val_sd, int step_val, int step_val_sign, int refi_middle_delay_sd, int refi_middle_delay, int no_of_reps_sd, int no_of_reps, int rpt_step_value);
	shared_ptr<IrManusRefi3> create_instr_refi3(int row, int col, int rpt_delay_sd, int rpt_delay,int  mode, int outp_cntrl, int fft_state, int refi_middle_delay_ext, int no_of_rpt_ext, int rpt_step_value_ext, int fft_end_stage, int dimarch_mode);
	shared_ptr<IrManusDelay> create_instr_delay(int row, int col, int del_cycles_sd, int del_cycles);
	shared_ptr<IrManusRaccu> create_instr_raccu(int row, int col, int raccu_mode, int raccu_op1_sd,int  raccu_op1, int raccu_op2_sd, int raccu_op2, int raccu_result_addrs);
	shared_ptr<IrManusLoopheader> create_instr_loopheader(int row, int col, int index_raccu_addr, int index_start, int iter_no_sd, int iter_no);
	shared_ptr<IrManusLooptail> create_instr_looptail(int row, int col, int index_step, int pc_togo, int index_raccu_addr);
	shared_ptr<IrManusSwb> create_instr_swb(int row, int col, int from_block, int from_address, int from_port, int to_block, int to_address, int to_port);
	shared_ptr<IrManusBranch> create_instr_branch(int row, int col, int branch_mode, int branch_false_address);
	shared_ptr<IrManusJump> create_instr_jump(int row, int col, int true_address);
	shared_ptr<IrManusSramread> create_instr_sramread(int row, int col, int mode, int initial_address, int initial_delay, int loop1_iterations, int loop1_increment, int loop1_delay, int loop2_iterations, int loop2_increment, int loop2_delay, int sram_initial_address_sd, int sram_loop1_iteration_sd, int sram_loop2_iteration_sd, int sram_initial_delay_sd, int sram_loop1_delay_sd, int sram_loop2_delay_sd, int sram_loop1_increment_sd, int sram_loop2_increment_sd);
	shared_ptr<IrManusSramwrite> create_instr_sramwrite(int row, int col, int mode, int initial_address, int initial_delay, int loop1_iterations, int loop1_increment, int loop1_delay, int loop2_iterations, int loop2_increment, int loop2_delay, int sram_initial_address_sd, int sram_loop1_iteration_sd, int sram_loop2_iteration_sd, int sram_initial_delay_sd, int sram_loop1_delay_sd, int sram_loop2_delay_sd, int sram_loop1_increment_sd, int sram_loop2_increment_sd);
	shared_ptr<IrManusRoute> create_instr_route(int row, int col, int source_row, int source_col, int destination_row, int destination_col, int drra_sel, int union_flag, int union_port, int origin);
	shared_ptr<IrManusHalt> create_instr_halt(int row, int col);
	
	shared_ptr<IrManusVar> create_var(string name, DistrType distr, vector<vector<int>> refi, vector<int> value);
	shared_ptr<IrManusVar> create_mem_var(string name, DistrType distr, vector<vector<int>> refi, vector<vector<int>> value);
	
	void get_variable_element(int row, int col, int pos, string& name, int& index);
	int get_variable_init_value(string name, int index);
	map<string, shared_ptr<IrManusVar>> variables(){return _variables;}
	void get_mem_variable_element(int row, int col, int pos, string& name, int& index);
	vector<int> get_mem_variable_init_value(string name, int index);

	int row();
	int col();
	vector<vector<vector<shared_ptr<IrManusBase>>>> instructions();
	int refi_total_entry();
	
	void schedule();

	void dump();
	
private:
	vector<vector<vector<shared_ptr<IrManusBase> > > > _instructions;
	map<string, shared_ptr<IrManusVar>> _variables;
	vector<vector<int>> _refi_counter;
	int _refi_total_entry;
	int _row;
	int _col;
};

}

#endif // __VESYLA_IR_IR_MANUS_HPP__
