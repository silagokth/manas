#include "ir/IrManas/IrManas.hpp"

namespace vesyla{
namespace ir{

void IrManas::reset(int num_row, int num_col, int refi_total_entry){
	if(num_row<=0 || num_col<=0 || refi_total_entry<=0){
		LOG(ERROR) << "fabric size error!";
		return;
	}
	
	_row = num_row;
	_col = num_col;
	
	_instructions.clear();
	_instructions.resize(num_row);
	for(auto it=_instructions.begin(); it!=_instructions.end(); it++){
		(*it).resize(num_col);
	}
	
	_variables.clear();
	
	_refi_counter.clear();
	_refi_counter.resize(num_row);
	for(auto it=_refi_counter.begin(); it!=_refi_counter.end(); it++){
		(*it).resize(num_col);
	}
	
	_refi_total_entry = refi_total_entry;
}

shared_ptr<IrManasDpu> IrManas::create_instr_dpu(
	int row, int col,
	int dpu_mode, int dpu_saturation, int dpu_out1, int dpu_out2,
	int dpu_acc_clear_rst, int dpu_acc_clear_sd, int dpu_acc_clear,
	int process_inout
	){
	
	if(
		dpu_mode<0 || dpu_mode>12 ||
		dpu_saturation<0 || dpu_saturation>3 ||
		dpu_out1<0 || dpu_out1>3 ||
		dpu_out2<0 || dpu_out2>3 ||
		dpu_acc_clear_rst<0 || dpu_acc_clear_rst >1 ||
		dpu_acc_clear_sd<0 || dpu_acc_clear_sd >1 ||
		dpu_acc_clear<0 || dpu_acc_clear >255 ||
		process_inout<0 || process_inout>3
	){
		LOG(FATAL) << "DPU instruction parameter value error!";
		return NULL;
	}
	
	shared_ptr<IrManasDpu> p = make_shared<IrManasDpu>();
	p->instr_type = INSTR_DPU;
	p->dpu_mode = dpu_mode;
	p->dpu_saturation = dpu_saturation;
	p->dpu_out1 = dpu_out1;
	p->dpu_out2 = dpu_out2;
	p->dpu_acc_clear_rst = dpu_acc_clear_rst;
	p->dpu_acc_clear_sd = dpu_acc_clear_sd;
	p->dpu_acc_clear = dpu_acc_clear;
	p->process_inout = process_inout;
	
	_instructions[row][col].push_back(p);
	
	return p;
}

shared_ptr<IrManasRefi1> IrManas::create_instr_refi1(int row, int col,
	int reg_file_port, int subseq_instrs, int start_addrs_sd, int start_addrs,
	int no_of_addrs_sd, int no_of_addrs, int initial_delay_sd, int initial_delay){
	
	if(
		reg_file_port<0 || reg_file_port>3 ||
		subseq_instrs<0 || subseq_instrs>3 ||
		start_addrs_sd<0 || start_addrs_sd>1 ||
		start_addrs<0 || start_addrs>63 ||
		no_of_addrs_sd<0 || no_of_addrs_sd >1 ||
		no_of_addrs<0 || no_of_addrs >63 ||
		initial_delay_sd<0 || initial_delay_sd>1 ||
		initial_delay<0 || initial_delay>15
	){
		LOG(FATAL) << "REFI1 instruction parameter value error!";
		return NULL;
	}
	
	shared_ptr<IrManasRefi1> p = make_shared<IrManasRefi1>();
	p->instr_type = INSTR_REFI1;
	p->reg_file_port = reg_file_port;
	p->subseq_instrs = subseq_instrs;
	p->start_addrs_sd = start_addrs_sd;
	p->start_addrs = start_addrs;
	p->no_of_addrs_sd = no_of_addrs_sd;
	p->no_of_addrs = no_of_addrs;
	p->initial_delay_sd = initial_delay_sd;
	p->initial_delay = initial_delay;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasRefi2> IrManas::create_instr_refi2(
	int row, int col,
	int step_val_sd, int step_val, int step_val_sign, int refi_middle_delay_sd,
	int refi_middle_delay, int no_of_reps_sd, int no_of_reps, int rpt_step_value){
	
	if(
		step_val_sd<0 || step_val_sd>1 ||
		step_val<0 || step_val>63 ||
		step_val_sign<0 || step_val_sign>1 ||
		refi_middle_delay_sd<0 || refi_middle_delay_sd>1 ||
		refi_middle_delay<0 || refi_middle_delay>15 ||
		no_of_reps_sd<0 || no_of_reps_sd>1 ||
		no_of_reps<0 || no_of_reps>31 ||
		rpt_step_value<0 || rpt_step_value>15
	){
		LOG(FATAL) << "REFI2 instruction parameter value error!";
		return NULL;
	}
	
	shared_ptr<IrManasRefi2> p = make_shared<IrManasRefi2>();
	p->instr_type = INSTR_REFI2;
	p->step_val_sd = step_val_sd;
	p->step_val = step_val;
	p->step_val_sign = step_val_sign;
	p->refi_middle_delay_sd = refi_middle_delay_sd;
	p->refi_middle_delay = refi_middle_delay;
	p->no_of_reps_sd = no_of_reps_sd;
	p->no_of_reps = no_of_reps;
	p->rpt_step_value = rpt_step_value;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasRefi3> IrManas::create_instr_refi3(
	int row, int col,
	int rpt_delay_sd, int rpt_delay, int mode, int outp_cntrl, int fft_state,
	int refi_middle_delay_ext, int no_of_rpt_ext, int rpt_step_value_ext,
	int fft_end_stage, int dimarch_mode){
	
	if(
		rpt_delay_sd<0 || rpt_delay_sd>1 ||
		rpt_delay<0 || rpt_delay>63 ||
		mode<0 || mode>1 ||
		outp_cntrl<0 || outp_cntrl>3 ||
		fft_state<0 || fft_state>5 ||
		refi_middle_delay_ext<0 || refi_middle_delay_ext>3 ||
		no_of_rpt_ext<0 || no_of_rpt_ext>1 ||
		rpt_step_value_ext<0 || rpt_step_value_ext>3 ||
		fft_end_stage<0 || fft_end_stage>5 ||
		dimarch_mode<0 || dimarch_mode>1
	){
		LOG(FATAL) << "REFI3 instruction parameter value error!";
		return NULL;
	}
	
	shared_ptr<IrManasRefi3> p = make_shared<IrManasRefi3>();
	p->instr_type = INSTR_REFI3;
	p->rpt_delay_sd = rpt_delay_sd;
	p->rpt_delay = rpt_delay;
	p->mode = mode;
	p->outp_cntrl = outp_cntrl;
	p->fft_state = fft_state;
	p->refi_middle_delay_ext = refi_middle_delay_ext;
	p->no_of_rpt_ext = no_of_rpt_ext;
	p->rpt_step_value_ext = rpt_step_value_ext;
	p->fft_end_stage = fft_end_stage;
	p->dimarch_mode = dimarch_mode;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasDelay> IrManas::create_instr_delay(
	int row, int col,
	int del_cycles_sd, int del_cycles){
	
	if(
		del_cycles_sd<0 || del_cycles_sd>1 ||
		del_cycles<0 || del_cycles>32767
	){
		LOG(FATAL) << "DELAY instruction parameter value error!";
		return NULL;
	}
	
	shared_ptr<IrManasDelay> p = make_shared<IrManasDelay>();
	p->instr_type = INSTR_DELAY;
	p->del_cycles_sd = del_cycles_sd;
	p->del_cycles = del_cycles;
	
	_instructions[row][col].push_back(p);
	return p;
}


shared_ptr<IrManasRaccu> IrManas::create_instr_raccu(
	int row, int col,
	int raccu_mode, int raccu_op1_sd, int raccu_op1, int raccu_op2_sd,
	int raccu_op2, int raccu_result_addrs){
	
	shared_ptr<IrManasRaccu> p = make_shared<IrManasRaccu>();
	p->instr_type = INSTR_RACCU;
	p->raccu_mode = raccu_mode;
	p->raccu_op1_sd = raccu_op1_sd;
	p->raccu_op1 = raccu_op1;
	p->raccu_op2_sd = raccu_op2_sd;
	p->raccu_op2 = raccu_op2;
	p->raccu_result_addrs = raccu_result_addrs;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasLoopheader> IrManas::create_instr_loopheader(
	int row, int col,
	int index_raccu_addr, int index_start, int iter_no_sd, int iter_no){
	
	shared_ptr<IrManasLoopheader> p = make_shared<IrManasLoopheader>();
	p->instr_type = INSTR_LOOPHEADER;
	p->index_raccu_addr = index_raccu_addr;
	p->index_start = index_start;
	p->iter_no_sd = iter_no_sd;
	p->iter_no = iter_no;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasLooptail> IrManas::create_instr_looptail(
	int row, int col,
	int index_step, int pc_togo, int index_raccu_addr){

	shared_ptr<IrManasLooptail> p = make_shared<IrManasLooptail>();
	p->instr_type = INSTR_LOOPTAIL;
	p->index_step = index_step;
	p->pc_togo = pc_togo;
	p->index_raccu_addr = index_raccu_addr;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasSwb> IrManas::create_instr_swb(int row, int col,
	int from_block, int from_address, int from_port, int to_block, int to_address,
	int to_port){
	
	if(
		from_block<0 || from_block>1 ||
		from_address<0 || from_address>_row*_col ||
		from_port<0 || from_port>3 ||
		to_block<0 || to_block>1 ||
		to_address<0 || to_address>=_row*_col ||
		to_port<0 || to_port>3
	){
		LOG(FATAL) << "SWB instruction parameter value error!";
		return NULL;
	}
	
	shared_ptr<IrManasSwb> p = make_shared<IrManasSwb>();
	p->instr_type = INSTR_SWB;
	p->from_block = from_block;
	p->from_address = from_address;
	p->from_port = from_port;
	p->to_block = to_block;
	p->to_address = to_address;
	p->to_port = to_port;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasBranch> IrManas::create_instr_branch(
	int row, int col,
	int branch_mode, int branch_false_address){
	
	if(
		branch_mode<0 || branch_mode>3 ||
		branch_false_address<0 || branch_false_address>63
	){
		LOG(FATAL) << "BRANCH instruction parameter value error!";
		return NULL;
	}
	
	shared_ptr<IrManasBranch> p = make_shared<IrManasBranch>();
	p->instr_type = INSTR_BRANCH;
	p->branch_mode = branch_mode;
	p->branch_false_address = branch_false_address;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasJump> IrManas::create_instr_jump(
	int row, int col,
	int true_address){

	if(
		true_address<0 || true_address>63
	){
		LOG(FATAL) << "JUMP instruction parameter value error!";
		return NULL;
	}
	
	shared_ptr<IrManasJump> p = make_shared<IrManasJump>();
	p->instr_type = INSTR_JUMP;
	p->true_address = true_address;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasSramread> IrManas::create_instr_sramread(
	int row, int col,
	int mode, int initial_address, int initial_delay, int loop1_iterations,
	int loop1_increment, int loop1_delay, int loop2_iterations, int loop2_increment,
	int loop2_delay, int sram_initial_address_sd, int sram_loop1_iteration_sd,
	int sram_loop2_iteration_sd, int sram_initial_delay_sd, int sram_loop1_delay_sd,
	int sram_loop2_delay_sd, int sram_loop1_increment_sd, int sram_loop2_increment_sd){

	shared_ptr<IrManasSramread> p = make_shared<IrManasSramread>();
	p->instr_type = INSTR_SRAMREAD;
	p->mode = mode;
	p->initial_address = initial_address;
	p->initial_delay = initial_delay;
	p->loop1_iterations = loop1_iterations;
	p->loop1_increment = loop1_increment;
	p->loop1_delay = loop1_delay;
	p->loop2_iterations = loop2_iterations;
	p->loop2_increment = loop2_increment;
	p->loop2_delay = loop2_delay;
	p->sram_initial_address_sd = sram_initial_address_sd;
	p->sram_loop1_iteration_sd = sram_loop1_iteration_sd;
	p->sram_loop2_iteration_sd = sram_loop2_iteration_sd;
	p->sram_initial_delay_sd = sram_initial_delay_sd;
	p->sram_loop1_delay_sd = sram_loop1_delay_sd;
	p->sram_loop2_delay_sd = sram_loop2_delay_sd;
	p->sram_loop1_increment_sd = sram_loop1_increment_sd;
	p->sram_loop2_increment_sd = sram_loop2_increment_sd;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasSramwrite> IrManas::create_instr_sramwrite(
	int row, int col,
	int mode, int initial_address, int initial_delay, int loop1_iterations,
	int loop1_increment, int loop1_delay, int loop2_iterations, int loop2_increment,
	int loop2_delay, int sram_initial_address_sd, int sram_loop1_iteration_sd,
	int sram_loop2_iteration_sd, int sram_initial_delay_sd, int sram_loop1_delay_sd,
	int sram_loop2_delay_sd, int sram_loop1_increment_sd, int sram_loop2_increment_sd){

	shared_ptr<IrManasSramwrite> p = make_shared<IrManasSramwrite>();
	p->instr_type = INSTR_SRAMWRITE;
	p->mode = mode;
	p->initial_address = initial_address;
	p->initial_delay = initial_delay;
	p->loop1_iterations = loop1_iterations;
	p->loop1_increment = loop1_increment;
	p->loop1_delay = loop1_delay;
	p->loop2_iterations = loop2_iterations;
	p->loop2_increment = loop2_increment;
	p->loop2_delay = loop2_delay;
	p->sram_initial_address_sd = sram_initial_address_sd;
	p->sram_loop1_iteration_sd = sram_loop1_iteration_sd;
	p->sram_loop2_iteration_sd = sram_loop2_iteration_sd;
	p->sram_initial_delay_sd = sram_initial_delay_sd;
	p->sram_loop1_delay_sd = sram_loop1_delay_sd;
	p->sram_loop2_delay_sd = sram_loop2_delay_sd;
	p->sram_loop1_increment_sd = sram_loop1_increment_sd;
	p->sram_loop2_increment_sd = sram_loop2_increment_sd;
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasRoute> IrManas::create_instr_route(
	int row, int col,
	int source_row, int source_col, int destination_row, int destination_col,
	int drra_sel, int union_flag, int union_port, int origin){

	shared_ptr<IrManasRoute> p = make_shared<IrManasRoute>();
	p->instr_type = INSTR_ROUTE;
	p->source_row = source_row;
	p->source_col = source_col;
	p->destination_row = destination_row;
	p->destination_col = destination_col;
	p->drra_sel = drra_sel;
	p->union_flag = union_flag;
	p->union_port = union_port;
	p->origin = origin;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasHalt> IrManas::create_instr_halt(int row, int col){

	shared_ptr<IrManasHalt> p = make_shared<IrManasHalt>();
	p->instr_type = INSTR_HALT;
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManasVar> IrManas::create_var(string name, DistrType distr, vector<vector<int>> refi, vector<int> value){
	if(_variables.find(name)!=_variables.end()){
		LOG(ERROR) << "variable " << name << " has already existed!";
		return NULL;
	}
	if(value.size() <=0){
		LOG(ERROR) << "variable " << name << " doesn't have any element!";
		return NULL;
	}
	shared_ptr<IrManasVar> p = make_shared<IrManasVar>();
	p->name = name;
	p->distr = distr;
	p->value = value;
	for(int i=0; i<refi.size();i++){
		vector<int> v(4);
		v[0] = refi[i][0];
		v[1] = refi[i][1];
		v[2] = _refi_counter[refi[i][0]][refi[i][1]];
		v[3] = 0;
		p->refi.push_back(v);
	}
	
	if(distr==DISTR_FULL){
		int total_empty_slot=0;
		for(int i=0; i<refi.size();i++){
			int curr_refi_x = refi[i][0];
			int curr_refi_y = refi[i][1];
			total_empty_slot += (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y] -1);
		}
		if(total_empty_slot<value.size()){
			LOG(ERROR) << "variable " << name << " with "<<value.size()<<" elements can't be fully distributed to "<<refi.size()<<" register files!";
			return NULL;
		}
		
		int element_count = value.size();
		for(int i=0; i<refi.size();i++){
			if(element_count<=0){
				break;
			}
			int curr_refi_x = refi[i][0];
			int curr_refi_y = refi[i][1];
			element_count -= (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y]);
			if(element_count>0){
				p->refi[i][3] = (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y]);
				_refi_counter[curr_refi_x][curr_refi_y] = _refi_total_entry;
			}else if(element_count==0){
				p->refi[i][3] = (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y]);
				_refi_counter[curr_refi_x][curr_refi_y] = _refi_total_entry;
				break;
			}else{
				p->refi[i][3] = (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y] + element_count);
				_refi_counter[curr_refi_x][curr_refi_y] = _refi_total_entry + element_count;
				element_count=0;
				break;
			}
		}
	}else{
		if(value.size()%refi.size()!=0){
			LOG(ERROR) << "variable " << name << " with "<<value.size()<<" elements can't be evenly distributed to "<<refi.size()<<" register files!";
			return NULL;
		}
		int part = value.size()/refi.size();
		for(int i=0; i<refi.size();i++){
			int curr_refi_x = refi[i][0];
			int curr_refi_y = refi[i][1];
			if(_refi_counter[curr_refi_x][curr_refi_y]+part>=_refi_total_entry){
				LOG(ERROR) << "variable " << name << " with "<<value.size()<<" elements can't be evenly distributed to register file ["<<curr_refi_x<<","<<curr_refi_y<<"]!";
				return NULL;
			}
		}
		for(int i=0; i<refi.size();i++){
			int curr_refi_x = refi[i][0];
			int curr_refi_y = refi[i][1];
			p->refi[i][3] = part;
			_refi_counter[curr_refi_x][curr_refi_y]+=part;
		}
	}
	
	_variables[name] = p;
	return p;
}


//shared_ptr<IrManasVar> IrManas::create_mem_var(string name, DistrType distr, vector<vector<int>> refi, vector<vector<int>> value){
//	if(_variables.find(name)!=_variables.end()){
//		LOG(ERROR) << "variable " << name << " has already existed!";
//		return NULL;
//	}
//	if(value.size() <=0){
//		LOG(ERROR) << "variable " << name << " doesn't have any element!";
//		return NULL;
//	}
//	shared_ptr<IrManasVar> p = make_shared<IrManasVar>();
//	p->name = name;
//	p->distr = distr;
//	p->value = value;
//	
//	for(int i=0; i<refi.size();i++){
//		vector<int> v(4);
//		v[0] = refi[i][0];
//		v[1] = refi[i][1];
//		v[2] = _refi_counter[refi[i][0]][refi[i][1]];
//		v[3] = 0;
//		p->refi.push_back(v);
//	}
//	
//	if(distr==DISTR_FULL){
//		int total_empty_slot=0;
//		for(int i=0; i<refi.size();i++){
//			int curr_refi_x = refi[i][0];
//			int curr_refi_y = refi[i][1];
//			total_empty_slot += (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y] -1);
//		}
//		if(total_empty_slot<value.size()){
//			LOG(ERROR) << "variable " << name << " with "<<value.size()<<" elements can't be fully distributed to "<<refi.size()<<" register files!";
//			return NULL;
//		}
//		
//		int element_count = value.size();
//		for(int i=0; i<refi.size();i++){
//			if(element_count<=0){
//				break;
//			}
//			int curr_refi_x = refi[i][0];
//			int curr_refi_y = refi[i][1];
//			element_count -= (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y]);
//			if(element_count>0){
//				p->refi[i][3] = (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y]);
//				_refi_counter[curr_refi_x][curr_refi_y] = _refi_total_entry;
//			}else if(element_count==0){
//				p->refi[i][3] = (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y]);
//				_refi_counter[curr_refi_x][curr_refi_y] = _refi_total_entry;
//				break;
//			}else{
//				p->refi[i][3] = (_refi_total_entry - _refi_counter[curr_refi_x][curr_refi_y] + element_count);
//				_refi_counter[curr_refi_x][curr_refi_y] += _refi_total_entry + element_count;
//				element_count=0;
//				break;
//			}
//		}
//	}else{
//		if(value.size()%refi.size()!=0){
//			LOG(ERROR) << "variable " << name << " with "<<value.size()<<" elements can't be evenly distributed to "<<refi.size()<<" register files!";
//			return NULL;
//		}
//		int part = value.size()/refi.size();
//		for(int i=0; i<refi.size();i++){
//			int curr_refi_x = refi[i][0];
//			int curr_refi_y = refi[i][1];
//			if(_refi_counter[curr_refi_x][curr_refi_y]+part>=_refi_total_entry){
//				LOG(ERROR) << "variable " << name << " with "<<value.size()<<" elements can't be evenly distributed to register file ["<<curr_refi_x<<","<<curr_refi_y<<"]!";
//				return NULL;
//			}
//		}
//		for(int i=0; i<refi.size();i++){
//			int curr_refi_x = refi[i][0];
//			int curr_refi_y = refi[i][1];
//			p->refi[i][3] = part;
//			_refi_counter[curr_refi_x][curr_refi_y]+=part;
//		}
//	}
//	
//	_variables[name] = p;
//	return p;
//}

void IrManas::get_variable_element(int row, int col, int pos, string& name, int& index){
	name = "";
	index = 0;
	bool flag_matched = false;
	for(auto it=_variables.begin(); it!=_variables.end(); it++){
		shared_ptr<IrManasVar> var = it->second;
		for(int i=0; i<var->refi.size();i++){
			if(var->refi[i][0] == row && var->refi[i][1] == col){
				if((pos >= var->refi[i][2]) && (pos < (var->refi[i][2]+var->refi[i][3]))){
					flag_matched = true;
					name=var->name;
					index += pos - var->refi[i][2];
					break;
				}else{
					break;
				}
			}else{
				index += var->refi[i][3];
			}
		}
		if(!flag_matched){
			index = 0;
		}else{
			break;
		}
	}
}

//void IrManas::get_mem_variable_element(int row, int col, int pos, string& name, int& index){
//	name = "";
//	index = 0;
//	bool flag_matched = false;
//	for(auto it=_variables.begin(); it!=_variables.end(); it++){
//		shared_ptr<IrManasVar> var = it->second;
//		for(int i=0; i<var->refi.size();i++){
//			if(var->refi[i][0] == row && var->refi[i][1] == col){
//				if((pos >= var->refi[i][2]) && (pos < (var->refi[i][2]+var->refi[i][3]))){
//					flag_matched = true;
//					name=var->name;
//					index += pos - var->refi[i][2];
//					break;
//				}else{
//					break;
//				}
//			}else{
//				index += var->refi[i][3];
//			}
//		}
//		if(!flag_matched){
//			index = 0;
//		}else{
//			break;
//		}
//	}
//}

int IrManas::get_variable_init_value(string name, int index){
	if(_variables.find(name)==_variables.end()){
		LOG(ERROR) << "No such variable with name " << name;
		return -1;
	}
	return _variables[name]->value[index];
}

//vector<int> IrManas::get_mem_variable_init_value(string name, int index){
//	if(_variables.find(name)==_variables.end()){
//		LOG(ERROR) << "No such variable with name " << name;
//		return -1;
//	}
//	return _variables[name]->value[index];
//}

int IrManas::row(){
	return _row;
}

int IrManas::col(){
	return _col;
}

vector<vector<vector<shared_ptr<IrManasBase>>>> IrManas::instructions(){
	return _instructions;
}

int IrManas::refi_total_entry(){
	return _refi_total_entry;
}


void IrManas::schedule(){
	for(int i=0; i<_row; i++){
		for(int j=0; j<_col; j++){
			int pc=0;
			int cycle=0;
			while(cycle < 10000 && pc<_instructions[i][j].size()){
				if(_instructions[i][j][pc]->instr_type==INSTR_DPU){
					;
				}
			}
		}
	}
}



void IrManas::dump(){
	cout << "DATA SEGMENT:===============================" << endl;
	for(int i=0; i<_instructions.size(); i++){
		for(int j=0; j<_instructions[i].size(); j++){
			cout << "RF [" << i << "," << j << "]: " << endl;
			for(int k=0; k<_refi_total_entry; k++){
				string name="";
				int index=k;
				get_variable_element(i, j, k, name, index);
				if(name!="")
					cout << name <<",";
				else
					cout << "*" <<",";
			}
			cout <<endl;
			for(int k=0; k<_refi_total_entry; k++){
				string name="";
				int index=k;
				get_variable_element(i, j, k, name, index);
				cout << index <<",";
			}
			cout <<endl;
		}
	}
	cout << "CODE SEGMENT:===============================" << endl;
	for(int i=0; i<_instructions.size(); i++){
		for(int j=0; j<_instructions[i].size(); j++){
			cout << "DPU [" << i << "," << j << "]-----------------------------------" << endl;
			for(int k=0; k<_instructions[i][j].size(); k++){
				shared_ptr<IrManasBase> p=_instructions[i][j][k];
				cout << "instr: " << instr_type_to_str(p->instr_type) << endl;
			}
		}
	}
}

}
}
