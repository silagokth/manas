#include "ir/IrManus/IrManus.hpp"

namespace vesyla{
namespace ir{

void IrManus::reset(int num_row, int num_col, int refi_total_entry){
	if(num_row<=0 || num_col<=0 || refi_total_entry<=0){
		BOOST_LOG_TRIVIAL(error) << "fabric size error!";
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

shared_ptr<IrManusDpu> IrManus::create_instr_dpu(
	int row, int col,
	int dpu_mode, int dpu_saturation, int dpu_out1, int dpu_out2,
	int dpu_acc_clear_rst, int dpu_acc_clear_sd, int dpu_acc_clear,
	int process_inout
	){
	
	shared_ptr<IrManusDpu> p = make_shared<IrManusDpu>();
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

shared_ptr<IrManusRefi1> IrManus::create_instr_refi1(int row, int col,
	int reg_file_port, int subseq_instrs, int start_addrs_sd, int start_addrs,
	int no_of_addrs_sd, int no_of_addrs, int initial_delay_sd, int initial_delay){

	shared_ptr<IrManusRefi1> p = make_shared<IrManusRefi1>();
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

shared_ptr<IrManusRefi2> IrManus::create_instr_refi2(
	int row, int col,
	int step_val_sd, int step_val, int step_val_sign, int refi_middle_delay_sd,
	int refi_middle_delay, int no_of_reps_sd, int no_of_reps, int rpt_step_value){
	
	shared_ptr<IrManusRefi2> p = make_shared<IrManusRefi2>();
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

shared_ptr<IrManusRefi3> IrManus::create_instr_refi3(
	int row, int col,
	int rpt_delay_sd, int rpt_delay, int mode, int outp_cntrl, int fft_state,
	int refi_middle_delay_ext, int no_of_rpt_ext, int rpt_step_value_ext,
	int fft_end_stage, int dimarch_mode){
	
	shared_ptr<IrManusRefi3> p = make_shared<IrManusRefi3>();
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

shared_ptr<IrManusDelay> IrManus::create_instr_delay(
	int row, int col,
	int del_cycles_sd, int del_cycles){
	
	shared_ptr<IrManusDelay> p = make_shared<IrManusDelay>();
	p->instr_type = INSTR_DELAY;
	p->del_cycles_sd = del_cycles_sd;
	p->del_cycles = del_cycles;
	
	_instructions[row][col].push_back(p);
	return p;
}


shared_ptr<IrManusRaccu> IrManus::create_instr_raccu(
	int row, int col,
	int raccu_mode, int raccu_op1_sd, int raccu_op1, int raccu_op2_sd,
	int raccu_op2, int raccu_result_addrs){
		
	shared_ptr<IrManusRaccu> p = make_shared<IrManusRaccu>();
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

shared_ptr<IrManusLoopheader> IrManus::create_instr_loopheader(
	int row, int col,
	int index_raccu_addr, int index_start, int iter_no_sd, int iter_no){
	
	shared_ptr<IrManusLoopheader> p = make_shared<IrManusLoopheader>();
	p->instr_type = INSTR_LOOPHEADER;
	p->index_raccu_addr = index_raccu_addr;
	p->index_start = index_start;
	p->iter_no_sd = iter_no_sd;
	p->iter_no = iter_no;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManusLooptail> IrManus::create_instr_looptail(
	int row, int col,
	int index_step, int pc_togo, int index_raccu_addr){

	shared_ptr<IrManusLooptail> p = make_shared<IrManusLooptail>();
	p->instr_type = INSTR_LOOPTAIL;
	p->index_step = index_step;
	p->pc_togo = pc_togo;
	p->index_raccu_addr = index_raccu_addr;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManusSwb> IrManus::create_instr_swb(int row, int col, int from_block, int from_address, int from_port, int to_block, int to_address, int to_port){

	shared_ptr<IrManusSwb> p = make_shared<IrManusSwb>();
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

shared_ptr<IrManusBranch> IrManus::create_instr_branch(
	int row, int col,
	int branch_mode, int branch_false_address){

	shared_ptr<IrManusBranch> p = make_shared<IrManusBranch>();
	p->instr_type = INSTR_BRANCH;
	p->branch_mode = branch_mode;
	p->branch_false_address = branch_false_address;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManusJump> IrManus::create_instr_jump(
	int row, int col,
	int true_address){

	shared_ptr<IrManusJump> p = make_shared<IrManusJump>();
	p->instr_type = INSTR_JUMP;
	p->true_address = true_address;
	
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManusSramread> IrManus::create_instr_sramread(
	int row, int col,
	int mode, int initial_address, int initial_delay, int loop1_iterations,
	int loop1_increment, int loop1_delay, int loop2_iterations, int loop2_increment,
	int loop2_delay, int sram_initial_address_sd, int sram_loop1_iteration_sd,
	int sram_loop2_iteration_sd, int sram_initial_delay_sd, int sram_loop1_delay_sd,
	int sram_loop2_delay_sd, int sram_loop1_increment_sd, int sram_loop2_increment_sd){

	shared_ptr<IrManusSramread> p = make_shared<IrManusSramread>();
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

shared_ptr<IrManusSramwrite> IrManus::create_instr_sramwrite(
	int row, int col,
	int mode, int initial_address, int initial_delay, int loop1_iterations,
	int loop1_increment, int loop1_delay, int loop2_iterations, int loop2_increment,
	int loop2_delay, int sram_initial_address_sd, int sram_loop1_iteration_sd,
	int sram_loop2_iteration_sd, int sram_initial_delay_sd, int sram_loop1_delay_sd,
	int sram_loop2_delay_sd, int sram_loop1_increment_sd, int sram_loop2_increment_sd){

	shared_ptr<IrManusSramwrite> p = make_shared<IrManusSramwrite>();
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

shared_ptr<IrManusRoute> IrManus::create_instr_route(
	int row, int col,
	int source_row, int source_col, int destination_row, int destination_col,
	int drra_sel, int union_flag, int union_port, int origin){

	shared_ptr<IrManusRoute> p = make_shared<IrManusRoute>();
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

shared_ptr<IrManusHalt> IrManus::create_instr_halt(int row, int col){

	shared_ptr<IrManusHalt> p = make_shared<IrManusHalt>();
	p->instr_type = INSTR_HALT;
	_instructions[row][col].push_back(p);
	return p;
}

shared_ptr<IrManusVar> IrManus::create_var(string name, DistrType distr, vector<vector<int>> refi, vector<int> value){
	if(_variables.find(name)!=_variables.end()){
		BOOST_LOG_TRIVIAL(error) << "variable " << name << " has already existed!";
		return NULL;
	}
	if(value.size() <=0){
		BOOST_LOG_TRIVIAL(error) << "variable " << name << " doesn't have any element!";
		return NULL;
	}
	shared_ptr<IrManusVar> p = make_shared<IrManusVar>();
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
			BOOST_LOG_TRIVIAL(error) << "variable " << name << " with "<<value.size()<<" elements can't be fully distributed to "<<refi.size()<<" register files!";
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
				_refi_counter[curr_refi_x][curr_refi_y] += _refi_total_entry + element_count;
				element_count=0;
				break;
			}
		}
	}else{
		if(value.size()%refi.size()!=0){
			BOOST_LOG_TRIVIAL(error) << "variable " << name << " with "<<value.size()<<" elements can't be evenly distributed to "<<refi.size()<<" register files!";
			return NULL;
		}
		int part = value.size()/refi.size();
		for(int i=0; i<refi.size();i++){
			int curr_refi_x = refi[i][0];
			int curr_refi_y = refi[i][1];
			if(_refi_counter[curr_refi_x][curr_refi_y]+part>=_refi_total_entry){
				BOOST_LOG_TRIVIAL(error) << "variable " << name << " with "<<value.size()<<" elements can't be evenly distributed to register file ["<<curr_refi_x<<","<<curr_refi_y<<"]!";
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


//shared_ptr<IrManusVar> IrManus::create_mem_var(string name, DistrType distr, vector<vector<int>> refi, vector<vector<int>> value){
//	if(_variables.find(name)!=_variables.end()){
//		BOOST_LOG_TRIVIAL(error) << "variable " << name << " has already existed!";
//		return NULL;
//	}
//	if(value.size() <=0){
//		BOOST_LOG_TRIVIAL(error) << "variable " << name << " doesn't have any element!";
//		return NULL;
//	}
//	shared_ptr<IrManusVar> p = make_shared<IrManusVar>();
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
//			BOOST_LOG_TRIVIAL(error) << "variable " << name << " with "<<value.size()<<" elements can't be fully distributed to "<<refi.size()<<" register files!";
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
//			BOOST_LOG_TRIVIAL(error) << "variable " << name << " with "<<value.size()<<" elements can't be evenly distributed to "<<refi.size()<<" register files!";
//			return NULL;
//		}
//		int part = value.size()/refi.size();
//		for(int i=0; i<refi.size();i++){
//			int curr_refi_x = refi[i][0];
//			int curr_refi_y = refi[i][1];
//			if(_refi_counter[curr_refi_x][curr_refi_y]+part>=_refi_total_entry){
//				BOOST_LOG_TRIVIAL(error) << "variable " << name << " with "<<value.size()<<" elements can't be evenly distributed to register file ["<<curr_refi_x<<","<<curr_refi_y<<"]!";
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

void IrManus::get_variable_element(int row, int col, int pos, string& name, int& index){
	name = "";
	index = 0;
	bool flag_matched = false;
	for(auto it=_variables.begin(); it!=_variables.end(); it++){
		shared_ptr<IrManusVar> var = it->second;
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

//void IrManus::get_mem_variable_element(int row, int col, int pos, string& name, int& index){
//	name = "";
//	index = 0;
//	bool flag_matched = false;
//	for(auto it=_variables.begin(); it!=_variables.end(); it++){
//		shared_ptr<IrManusVar> var = it->second;
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

int IrManus::get_variable_init_value(string name, int index){
	if(_variables.find(name)==_variables.end()){
		BOOST_LOG_TRIVIAL(error) << "No such variable with name " << name;
		return -1;
	}
	return _variables[name]->value[index];
}

//vector<int> IrManus::get_mem_variable_init_value(string name, int index){
//	if(_variables.find(name)==_variables.end()){
//		BOOST_LOG_TRIVIAL(error) << "No such variable with name " << name;
//		return -1;
//	}
//	return _variables[name]->value[index];
//}

int IrManus::row(){
	return _row;
}

int IrManus::col(){
	return _col;
}

vector<vector<vector<shared_ptr<IrManusBase>>>> IrManus::instructions(){
	return _instructions;
}

int IrManus::refi_total_entry(){
	return _refi_total_entry;
}


void IrManus::schedule(){
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



void IrManus::dump(){
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
				shared_ptr<IrManusBase> p=_instructions[i][j][k];
				cout << "instr: " << instr_type_to_str(p->instr_type) << endl;
			}
		}
	}
}

}
}
