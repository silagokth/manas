#include "generator/GeVsim/GeVsim.hpp"

namespace vesyla{
namespace generator{

GeVsim::GeVsim(string design, ir::IrManas* ir_in, string fabric_path, string output_dir){
	_design = design;
	_ir_in = ir_in;
	_output_dir = output_dir+"/";
	_fabric_path = fabric_path+"/";
	
	_row = _ir_in->row();
	_col = _ir_in->col();
	_instr_no=0;
	vector<vector<vector<shared_ptr<IrManasBase>>>> instructions = _ir_in->instructions();
	for(int i=0; i<_row; i++){
		for(int j=0; j<_col; j++){
			_instr_no += instructions[i][j].size();
		}
	}
	
	_reg_load_cycles = 0;
	_refi_total_entry = _ir_in->refi_total_entry();
	_reg_file_mem_addr_width=ceil(log2((float)(_refi_total_entry/16)));
	bool flag_first=true;
	for(int i=0; i<_row; i++){
		for(int j=0; j<_col; j++){
			bool flag_has_var=false;
			for(int k=0; k<_refi_total_entry; k++){
				string name;
				int index;
				_ir_in->get_variable_element(i, j, k, name, index);
				if(name!=""){
					flag_has_var=true;
				}
			}
			if(flag_has_var){
				for(int m=0; m<_refi_total_entry/16; m++){
					_reg_load_cycles ++;
				}
			}
		}
	}
	
	_max_scheduled_cycle = 100000;
	
	
}

void GeVsim::gen_tb(){
	string path = _output_dir + "testbench_"+_design+".vhd";
	
	GeWriter w(path);
	w.write_line("LIBRARY IEEE;");
	w.write_line("USE IEEE.NUMERIC_STD.ALL;");
	w.write_line("USE IEEE.STD_LOGIC_1164.ALL;");
	w.write_line("USE WORK.top_consts_types_package.ROWS;");
	w.write_line("USE WORK.top_consts_types_package.COLUMNS;");
	w.write_line("USE WORK.top_consts_types_package.INSTR_WIDTH;");
	w.write_line("USE WORK.top_consts_types_package.REG_FILE_MEM_ADDR_WIDTH;");
	w.write_line("USE WORK.top_consts_types_package.REG_FILE_MEM_DATA_WIDTH;");
	w.write_line("USE WORK.top_consts_types_package.SRAM_ADDRESS_WIDTH;");
	w.write_line("USE WORK.top_consts_types_package.SRAM_WIDTH;");
	w.write_line("USE WORK.tb_instructions.ALL;");
	w.write_line("USE WORK.const_package_"+_design+".ALL;");
	w.write_line("USE WORK.noc_types_n_constants.ALL;");
	w.write_line("");
	w.write_line("ENTITY testbench_"+_design+" IS");
	w.write_line("END testbench_"+_design+";");
	w.write_line("");
	w.write_line("ARCHITECTURE behavior OF testbench_"+_design+" IS");
	w.inc_tab();
	w.write_line("");
	w.write_line("SIGNAL clk            : std_logic := '0';");
	w.write_line("SIGNAL rst_n          : std_logic := '0';");
	w.write_line("SIGNAL instr_load     : std_logic := '0';");
	w.write_line("SIGNAL instr_input    : std_logic_vector(INSTR_WIDTH downto 0);");
	w.write_line("SIGNAL seq_address_rb : std_logic_vector(ROWS-1 downto 0);");
	w.write_line("SIGNAL seq_address_cb : std_logic_vector(COLUMNS-1 downto 0);");
	w.write_line("");
	w.write_line("SIGNAL fabric_reg_wr_2		: std_logic := '0';");
	w.write_line("SIGNAL fabric_reg_rd_2		: std_logic := '0';");
	w.write_line("SIGNAL fabric_reg_wr_addr_2 : std_logic_vector(REG_FILE_MEM_ADDR_WIDTH-1 downto 0) := (OTHERS => '0');");
	w.write_line("SIGNAL fabric_reg_rd_addr_2 : std_logic_vector(REG_FILE_MEM_ADDR_WIDTH-1 downto 0) := (OTHERS => '0');");
	w.write_line("SIGNAL fabric_data_in_reg_2 : signed(REG_FILE_MEM_DATA_WIDTH-1 DOWNTO 0) := (OTHERS => '0');");
	w.write_line("SIGNAL fabric_data_out_2 	: signed(REG_FILE_MEM_DATA_WIDTH-1 DOWNTO 0);");
	w.write_line("SIGNAL tb_or_dimarch 		: std_logic;");
	w.write_line("SIGNAL tb_en    : std_logic;");
	w.write_line("SIGNAL tb_addrs : std_logic_vector (SRAM_ADDRESS_WIDTH-1 downto 0);");
	w.write_line("SIGNAL tb_inp   : std_logic_vector (SRAM_WIDTH-1 downto 0);");
	w.write_line("SIGNAL tb_ROW   : unsigned (ROW_WIDTH-1 downto 0);");
	w.write_line("SIGNAL tb_COL   : unsigned (COL_WIDTH-1 downto 0);");
	w.write_line("");
	w.write_line("TYPE instruction_type IS ARRAY (0 TO instruction_no-1) OF std_logic_vector(INSTR_WIDTH downto 0);");
	w.write_line("SIGNAL instruction : instruction_type;");
	w.write_line("");
	w.write_line("TYPE cell_info_type IS RECORD");
	w.inc_tab();
	w.write_line("inst_no : integer range 0 to instruction_no;");
	w.write_line("row : std_logic_vector(ROWS-1 downto 0);");
	w.write_line("col : std_logic_vector(COLUMNS-1 downto 0);");
	w.dec_tab();
	w.write_line("END RECORD;");
	w.write_line("TYPE cell_infos_type IS ARRAY(0 TO ROWS*COLUMNS-1) OF cell_info_type;");
	w.write_line("CONSTANT cell_infos : cell_infos_type := (");
	w.inc_tab();
	
	int instr_no=0;
	vector<vector<vector<shared_ptr<IrManasBase>>>> instructions = _ir_in->instructions();
	for(int i=0; i<_row; i++){
		string s="";
		for(int j=0; j<_col; j++){
			instr_no += instructions[i][j].size();
			boost::dynamic_bitset<> r(_row, 1<<i);  
			boost::dynamic_bitset<> c(_col, 1<<j);
			string sr;
			string sc;
			to_string(r, sr);
			to_string(c, sc);
			s+="( " + to_string(instr_no) + ", \"" + sr + "\", \"" + sc + "\")";
			if(i!=(_row-1) || j!=(_col-1)){
				s+= ",";
			}else{
				s+=");";
			}
		}
		w.write_line(s);
	}
	
	w.dec_tab();
	w.write_line("");
	w.write_line("TYPE reg_value_type IS RECORD");
	w.inc_tab();
	w.write_line("address : std_logic_vector (REG_FILE_MEM_ADDR_WIDTH-1 downto 0);");
	w.write_line("row     : std_logic_vector (ROWS-1 downto 0);");
	w.write_line("col     : std_logic_vector (COLUMNS-1 downto 0);");
	w.write_line("data    : signed (REG_FILE_MEM_DATA_WIDTH-1 downto 0);");
	w.dec_tab();
	w.write_line("END RECORD;");
	w.write_line("TYPE reg_values_type IS ARRAY (0 to reg_load_cycles-1) OF reg_value_type;");
	w.write_line("");
	w.write_line("CONSTANT reg_init_values : reg_values_type := (");
	w.inc_tab();
	
	bool flag_first=true;
	for(int i=0; i<_row; i++){
		for(int j=0; j<_col; j++){
			bool flag_has_var=false;
			vector<int> v(_refi_total_entry, 0);
			for(int k=0; k<_refi_total_entry; k++){
				string name;
				int index;
				_ir_in->get_variable_element(i, j, k, name, index);
				if(name!=""){
					flag_has_var=true;
					v[k] = _ir_in->get_variable_init_value(name, index);
				}
			}
			if(flag_has_var){
				for(int m=0; m<_refi_total_entry/16; m++){
					boost::dynamic_bitset<> a(_reg_file_mem_addr_width, m);
					boost::dynamic_bitset<> r(_row, 1<<i);
					boost::dynamic_bitset<> c(_col, 1<<j);
					string sa, sr, sc;
					to_string(a, sa);
					to_string(r, sr);
					to_string(c, sc);
					string str;
					if(flag_first){
						str="  ";
						flag_first=false;
					}else{
						str=", ";
					}
					str += "(\""+sa+"\", \""+sr+"\", \""+sc+"\", X\"";
					for(int n=0; n<16; n++){
						std::ostringstream vstream;
						vstream << right << setfill('0') << setw(4) << hex << v[(m+1)*16 -1 - n];
						str += vstream.str();
					}
					str += "\")";
					w.write_line(str);
				}
			}
		}
	}
	w.dec_tab();
	w.write_line(");");
	w.write_line("");
	w.write_line("BEGIN");
	w.write_line("");
	w.write_line("PRF : ENTITY work.profiler_"+_design);
	w.inc_tab();
	w.write_line("PORT MAP (clk	=> clk,");
	w.write_line("          rst_n	=> rst_n);");
	w.write_line("");
	w.dec_tab();
	w.write_line("DUT : ENTITY work.fabric");
	w.inc_tab();
	w.write_line("PORT MAP (clk                  => clk,");
	w.write_line("          rst_n                => rst_n,");
	w.write_line("          instr_ld             => instr_load,");
	w.write_line("          instr_inp            => instr_input,");
	w.write_line("          seq_address_rb       => seq_address_rb,");
	w.write_line("          seq_address_cb       => seq_address_cb,");
	w.write_line("          fabric_reg_wr_2      => fabric_reg_wr_2,");
	w.write_line("          fabric_reg_rd_2      => fabric_reg_rd_2,");
	w.write_line("          fabric_reg_wr_addr_2 => fabric_reg_wr_addr_2,");
	w.write_line("          fabric_reg_rd_addr_2 => fabric_reg_rd_addr_2,");
	w.write_line("          fabric_data_in_reg_2 => fabric_data_in_reg_2,");
	w.write_line("          fabric_data_out_2    => fabric_data_out_2,");
	w.write_line("          tb_or_dimarch        => tb_or_dimarch,");
	w.write_line("          tb_en                => tb_en,");
	w.write_line("          tb_addrs             => tb_addrs,");
	w.write_line("          tb_inp               => tb_inp,");
	w.write_line("          tb_ROW               => tb_ROW,");
	w.write_line("          tb_COL               => tb_COL);");
	w.write_line("");
	w.dec_tab();
	w.write_line("rst_n <= '0' AFTER 2.5 ns, '1' AFTER 4 ns;");
	w.write_line("clk   <= NOT clk AFTER half_period;");
	w.write_line("");
	w.write_line("StimuliSequencer : PROCESS (clk, rst_n)");
	w.inc_tab();
	w.write_line("VARIABLE inst_counter	  : integer := 0;");
	w.write_line("VARIABLE curr_cell		  : integer := 0;");
	w.write_line("VARIABLE reg_load_counter : integer := 0;");
	w.dec_tab();
	w.write_line("BEGIN");
	w.inc_tab();
	w.write_line("IF (rst_n = '0') THEN");
	w.inc_tab();
	w.write_line("inst_counter	 := 0;");
	w.write_line("curr_cell		 := 0;");
	w.write_line("reg_load_counter := 0;");
	w.write_line("instr_load     <= '0';");
	w.write_line("instr_input    <= (OTHERS =>'0');");
	w.write_line("seq_address_rb <= (OTHERS =>'0');");
	w.write_line("seq_address_cb <= (OTHERS =>'0');");
	w.write_line("tb_or_dimarch  <= '0';");
	w.write_line("tb_en 	 <= '0';");
	w.write_line("tb_addrs <= (OTHERS => '0');");
	w.write_line("tb_inp	 <= (OTHERS => '0');");
	w.write_line("tb_ROW 	 <= (OTHERS => '0');");
	w.write_line("tb_COL 	 <= (OTHERS => '0');");
	w.dec_tab();
	w.write_line("ELSIF clk'EVENT AND clk = '0' THEN");
	w.write_line("");
	w.inc_tab();
	w.write_line("tb_en <= '0';");
	w.write_line("");
	w.write_line("IF (reg_load_counter < reg_load_cycles) THEN");
	w.write_line("");
	w.inc_tab();
	w.write_line("instr_load			 <= '0';");
	w.write_line("instr_input			 <= (OTHERS =>'0');");
	w.write_line("fabric_reg_wr_2		 <= '1';");
	w.write_line("seq_address_rb       <= reg_init_values(reg_load_counter).row;");
	w.write_line("seq_address_cb       <= reg_init_values(reg_load_counter).col;");
	w.write_line("fabric_reg_wr_addr_2 <= reg_init_values(reg_load_counter).address;");
	w.write_line("fabric_data_in_reg_2 <= reg_init_values(reg_load_counter).data;");
	w.write_line("");
	w.write_line("reg_load_counter := reg_load_counter + 1;");
	w.write_line("");
	w.dec_tab();
	w.write_line("ELSIF (inst_counter >= 0 AND inst_counter < instruction_no) THEN");
	w.inc_tab();
	w.write_line("");
	w.write_line("tb_or_dimarch  <= '1';");
	w.write_line("");
	w.write_line("WHILE (inst_counter >= cell_infos(curr_cell).inst_no) LOOP");
	w.inc_tab();
	w.write_line("curr_cell := curr_cell + 1;");
	w.dec_tab();
	w.write_line("END LOOP;");
	w.write_line("");
	w.write_line("seq_address_rb <= cell_infos(curr_cell).row;");
	w.write_line("seq_address_cb <= cell_infos(curr_cell).col;");
	w.write_line("instr_load     <= '1';");
	w.write_line("instr_input    <= std_logic_vector(instruction(inst_counter));");
	w.write_line("inst_counter   := inst_counter + 1;");
	w.write_line("");
	w.dec_tab();
	w.write_line("ELSE");
	w.inc_tab();
	w.write_line("instr_load      <= '0';");
	w.write_line("fabric_reg_wr_2 <= '0';");
	w.dec_tab();
	w.write_line("END IF;");
	w.dec_tab();
	w.write_line("END IF;");
	w.dec_tab();
	w.write_line("END PROCESS StimuliSequencer;");
	w.write_line("");
	w.write_line("InstSequencer : PROCESS IS");
	w.write_line("BEGIN");
	w.inc_tab();
	w.write_line("");
	
	int instr_idx=0;
	for(int i=0; i<_row; i++){
		for(int j=0; j<_col; j++){
			if (instructions[i][j].size()>0){
				w.write_line("-- -----------------------------------------------------");
				w.write_line("-- Sequencer<"+to_string(i)+", "+to_string(j)+">--------------------------------------");
				w.write_line("-- -----------------------------------------------------");
				for(int k=0; k<instructions[i][j].size(); k++){
					string str="";
					if(instructions[i][j][k]->instr_type == INSTR_DPU){
						shared_ptr<IrManasDpu> ins = static_pointer_cast<IrManasDpu>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= DPU("+to_string(ins->dpu_mode)+", "+to_string(ins->dpu_saturation)+", "+to_string(ins->dpu_out1)+", "+to_string(ins->dpu_out2)+", "+to_string(ins->dpu_acc_clear_rst)+", "+to_string(ins->dpu_acc_clear_sd)+", "+to_string(ins->dpu_acc_clear)+", "+to_string(ins->process_inout)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_REFI1){
						shared_ptr<IrManasRefi1> ins = static_pointer_cast<IrManasRefi1>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= REFI_1("+to_string(ins->reg_file_port)+", "+to_string(ins->subseq_instrs)+", "+to_string(ins->start_addrs_sd)+", "+to_string(ins->start_addrs)+", "+to_string(ins->no_of_addrs_sd)+", "+to_string(ins->no_of_addrs)+", "+to_string(ins->initial_delay_sd)+", "+to_string(ins->initial_delay)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_REFI2){
						shared_ptr<IrManasRefi2> ins = static_pointer_cast<IrManasRefi2>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= REFI_2("+to_string(ins->step_val_sd)+", "+to_string(ins->step_val)+", "+to_string(ins->step_val_sign)+", "+to_string(ins->refi_middle_delay_sd)+", "+to_string(ins->refi_middle_delay)+", "+to_string(ins->no_of_reps_sd)+", "+to_string(ins->no_of_reps)+", "+to_string(ins->rpt_step_value)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_REFI3){
						shared_ptr<IrManasRefi3> ins = static_pointer_cast<IrManasRefi3>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= REFI_3("+to_string(ins->rpt_delay_sd)+", "+to_string(ins->rpt_delay)+", "+to_string(ins->mode)+", "+to_string(ins->outp_cntrl)+", "+to_string(ins->fft_state)+", "+to_string(ins->refi_middle_delay_ext)+", "+to_string(ins->no_of_rpt_ext)+", "+to_string(ins->rpt_step_value_ext)+", "+to_string(ins->fft_end_stage)+", "+to_string(ins->dimarch_mode)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_DELAY){
						shared_ptr<IrManasDelay> ins = static_pointer_cast<IrManasDelay>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= DELAY("+to_string(ins->del_cycles_sd)+", "+to_string(ins->del_cycles)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_RACCU){
						shared_ptr<IrManasRaccu> ins = static_pointer_cast<IrManasRaccu>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= RACCU("+to_string(ins->raccu_mode)+", "+to_string(ins->raccu_op1_sd)+", "+to_string(ins->raccu_op1)+", "+to_string(ins->raccu_op2_sd)+", "+to_string(ins->raccu_op2)+", "+to_string(ins->raccu_result_addrs)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_LOOPHEADER){
						shared_ptr<IrManasLoopheader> ins = static_pointer_cast<IrManasLoopheader>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= LOOP_HEADER("+to_string(ins->index_raccu_addr)+", "+to_string(ins->index_start)+", "+to_string(ins->iter_no_sd)+", "+to_string(ins->iter_no)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_LOOPTAIL){
						shared_ptr<IrManasLooptail> ins = static_pointer_cast<IrManasLooptail>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= LOOP_TAIL("+to_string(ins->index_step)+", "+to_string(ins->pc_togo)+", "+to_string(ins->index_raccu_addr)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_SWB){
						shared_ptr<IrManasSwb> ins = static_pointer_cast<IrManasSwb>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= SWB("+to_string(ins->from_block)+", "+to_string(ins->from_address)+", "+to_string(ins->from_port)+", "+to_string(ins->to_block)+", "+to_string(ins->to_address)+", "+to_string(ins->to_port)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_BRANCH){
						shared_ptr<IrManasBranch> ins = static_pointer_cast<IrManasBranch>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= BRANCH("+to_string(ins->branch_mode)+", "+to_string(ins->branch_false_address)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_JUMP){
						shared_ptr<IrManasJump> ins = static_pointer_cast<IrManasJump>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= JUMP("+to_string(ins->true_address)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_SRAMREAD){
						shared_ptr<IrManasSramread> ins = static_pointer_cast<IrManasSramread>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= SRAM_READ("+to_string(ins->mode)+", "+to_string(ins->initial_address)+", "+to_string(ins->initial_delay)+", "+to_string(ins->loop1_iterations)+", "+to_string(ins->loop1_increment)+", "+to_string(ins->loop1_delay)+", "+to_string(ins->loop2_iterations)+", "+to_string(ins->loop2_increment)+", "+to_string(ins->loop2_delay)+", "+to_string(ins->sram_initial_address_sd)+", "+to_string(ins->sram_loop1_iteration_sd)+", "+to_string(ins->sram_loop2_iteration_sd)+", "+to_string(ins->sram_initial_delay_sd)+", "+to_string(ins->sram_loop1_delay_sd)+", "+to_string(ins->sram_loop2_delay_sd)+", "+to_string(ins->sram_loop1_increment_sd)+", "+to_string(ins->sram_loop2_increment_sd)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_SRAMWRITE){
						shared_ptr<IrManasSramwrite> ins = static_pointer_cast<IrManasSramwrite>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= SRAM_WRITE("+to_string(ins->mode)+", "+to_string(ins->initial_address)+", "+to_string(ins->initial_delay)+", "+to_string(ins->loop1_iterations)+", "+to_string(ins->loop1_increment)+", "+to_string(ins->loop1_delay)+", "+to_string(ins->loop2_iterations)+", "+to_string(ins->loop2_increment)+", "+to_string(ins->loop2_delay)+", "+to_string(ins->sram_initial_address_sd)+", "+to_string(ins->sram_loop1_iteration_sd)+", "+to_string(ins->sram_loop2_iteration_sd)+", "+to_string(ins->sram_initial_delay_sd)+", "+to_string(ins->sram_loop1_delay_sd)+", "+to_string(ins->sram_loop2_delay_sd)+", "+to_string(ins->sram_loop1_increment_sd)+", "+to_string(ins->sram_loop2_increment_sd)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_ROUTE){
						shared_ptr<IrManasRoute> ins = static_pointer_cast<IrManasRoute>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= ROUTE("+to_string(ins->source_row)+", "+to_string(ins->source_col)+", "+to_string(ins->destination_row)+", "+to_string(ins->destination_col)+", "+to_string(ins->drra_sel)+", "+to_string(ins->union_flag)+", "+to_string(ins->union_port)+", "+to_string(ins->origin)+");";
					}else if(instructions[i][j][k]->instr_type == INSTR_HALT){
						shared_ptr<IrManasHalt> ins = static_pointer_cast<IrManasHalt>(instructions[i][j][k]);
						str = "instruction("+to_string(instr_idx)+") <= HALT();";
					}
					instr_idx++;
					w.write_line(str);
				}
			}
		}
	}
	
	w.write_line("");
	w.write_line("WAIT;");
	w.write_line("");
	w.dec_tab();
	w.write_line("END PROCESS InstSequencer;");
	w.write_line("");
	w.dec_tab();
	w.write_line("END behavior;");
}

void GeVsim::gen_cfg(){
	string path = _output_dir + "const_package_"+_design+".vhd";
	
	GeWriter w(path);
	w.write_line("PACKAGE const_package_"+_design+" IS");
	w.write_line("");
	w.inc_tab();
	w.write_line("CONSTANT instruction_no         : integer := "+to_string(_instr_no)+";");
	w.write_line("CONSTANT mem_load_cycles        : integer := 0;");
	w.write_line("CONSTANT reg_load_cycles        : integer := "+to_string(_reg_load_cycles)+";");
	w.write_line("CONSTANT execution_start_cycle  : integer := instruction_no + reg_load_cycles + mem_load_cycles + 1;");
	w.write_line("CONSTANT total_execution_cycle  : integer := "+to_string(_max_scheduled_cycle+_instr_no+0+_reg_load_cycles)+";");
	w.write_line("CONSTANT period                 : time    := 10 NS;");
	w.write_line("CONSTANT half_period            : time    := 5 NS;");
	w.dec_tab();
	w.write_line("");
	w.write_line("END const_package_"+_design+";");
}


void GeVsim::gen_profiler(){
	string path = _output_dir + "profiler_"+_design+".sv";
	
	GeWriter w(path);

	w.write_line("`include \"macros.svh\"");
	w.write_line("import const_package_"+_design+"::*;");
	w.write_line("import name_mangling::*;");
	w.write_line("import test_util::*;");
	w.write_line("");
	w.write_line("module profiler_"+_design+"(input logic clk, rst_n);");
	w.write_line("");
	w.inc_tab();
	w.write_line("const string log_file1 = \"results/sv_results1_exc_"+_design+".txt\";");
	w.write_line("const string log_file2 = \"results/sv_results2_reg_"+_design+".txt\";");
	w.write_line("const string log_file3 = \"results/sv_results3_seq_"+_design+".txt\";");
	w.write_line("const string log_file4 = \"results/sv_results4_srm_"+_design+".txt\";");
	w.write_line("const string log_file5 = \"results/sv_results_"+_design+".txt\";");
	w.write_line("");
	w.write_line("const integer fileid1 = $fopen(log_file1, \"w\");");
	w.write_line("const integer fileid2 = $fopen(log_file2, \"w\");");
	w.write_line("const integer fileid3 = $fopen(log_file3, \"w\");");
	w.write_line("const integer fileid4 = $fopen(log_file4, \"w\");");
	w.write_line("const integer fileid5 = $fopen(log_file5, \"w\");");
	w.write_line("");
	w.write_line("int clkCounter = 0;");
	w.write_line("int executionCycle = 0;");
	w.write_line("string regWriteActivities;");
	w.write_line("string regReadActivities;");
	w.write_line("string resultActivities;");
	w.write_line("const string wtDelimiter = \"-------------------------------------------------------------------------------------------------\";");
	w.write_line("const string startOfExecutionStr = $sformatf(\"\\nStart of execution: %0d ns (cycle = 0)\\n\", (execution_start_cycle + 1) * period);");
	w.write_line("");
	w.write_line("const string regFileVariables[ROWS][COLUMNS][REG_FILE_DEPTH] = '{");
	
	for(int i=0; i<_row; i++){
		w.write_line("'{");
		w.inc_tab();
		for(int j=0; j<_col; j++){
			vector<string> v(_refi_total_entry, "");
			for(int k=0; k<_refi_total_entry; k++){
				string name;
				int index;
				_ir_in->get_variable_element(i, j, k, name, index);
				if(name!=""){
					v[k] = name;
				}
			}
			
			string str="'{";
			for(int k=0; k<_refi_total_entry; k++){
				str+="\""+v[k]+"\"";
				if(k!=_refi_total_entry-1){
					str+=", ";
				}
			}
			str += "}";
			if(j!=_col-1){
				str += ",";
			}
			w.write_line(str);
		}
		w.dec_tab();
		if(i!=_row-1){
			w.write_line("},");
		}else{
			w.write_line("}");
		}
	}
	w.write_line("};");
	w.write_line("");
	

	string str="const int variablesStartAddr[string] = '{";
	bool flag_first=true;
	map<string, shared_ptr<IrManasVar>> variables = _ir_in->variables();
	for(auto it=variables.begin(); it!=variables.end(); it++){
		if(!flag_first){
			str += ", ";
		}
		shared_ptr<IrManasVar> var = it->second;
		str+="\""+var->name+"\" : "+to_string(var->refi[0][2]);
		flag_first=false;
	}
	str+="};";
	w.write_line(str);
	w.write_line("");
	str = "const int addressOffset[string] = '{";
	flag_first=true;
	for(auto it=variables.begin(); it!=variables.end(); it++){

		shared_ptr<IrManasVar> var = it->second;
		int offset=0;
		for(int i=0; i<var->refi.size(); i++){
			if(!flag_first){
				str += ", ";
			}
			str+="\""+var->name+"_"+to_string(var->refi[i][0])+to_string(var->refi[i][1])+"\" : "+to_string(offset);
			offset += var->refi[i][3];
			flag_first=false;
		}
	}
	str+="};";
	w.write_line(str);
	w.write_line("");
	w.write_line("const int skipCyclesOfWriteAccess[ROWS][COLUMNS][int] = '{");
	w.inc_tab();
	
	for(int i=0; i<_row; i++){
		w.write_line("'{");
		w.inc_tab();
		for(int j=0; j<_col; j++){
			if(j!=_col-1){
				w.write_line("'{-1:0},");
			}else{
				w.write_line("'{-1:0}");
			}
		}
		w.dec_tab();
		if(i==_row-1){
			w.write_line("}");
		}else{
			w.write_line("},");
		}
	}
	w.dec_tab();
	w.write_line("};");
	w.write_line("");
	w.write_line("const bit regFileFixedPointStatus[ROWS][COLUMNS][REG_FILE_DEPTH] = '{");
	w.inc_tab();
	for(int i=0; i<_row; i++){
		w.write_line("'{");
		w.inc_tab();
		for(int j=0; j<_col; j++){
			vector<string> v(_refi_total_entry, "0");
			
			string str="'{";
			for(int k=0; k<_refi_total_entry; k++){
				str+=v[k];
				if(k!=_refi_total_entry-1){
					str+=", ";
				}
			}
			str += "}";
			if(j!=_col-1){
				str += ",";
			}
			w.write_line(str);
		}
		w.dec_tab();
		if(i!=_row-1){
			w.write_line("},");
		}else{
			w.write_line("}");
		}
	}
	w.dec_tab();
	w.write_line("};");
	w.write_line("");
	w.write_line("function automatic void printRegValue(shortint row, col, address, data, portNo, bit is_input);");
	w.inc_tab();
	w.write_line("");
	w.write_line("const string delimiter       = \"---------------\";");
	w.write_line("const string firstDelimiter  = is_input ? {delimiter, \"\\n\"} : \"\";");
	w.write_line("const string currPosition    = $sformatf(\"\\tin RegFile <%0d,%0d>,\\t@ cycle: %2d (%4d ns)\", row, col, executionCycle, $time);");
	w.write_line("const string addressOffsetKey= $sformatf(\"%s_%0d%0d\", `currRegVariable, row, col);");
	w.write_line("");
	w.write_line("string resultStr;");
	w.write_line("const string strLine = { firstDelimiter, getRegValueStr(row, col, address, data, portNo, executionCycle,");
	w.inc_tab();
	w.write_line("variablesStartAddr[`currRegVariable], addressOffset[addressOffsetKey], is_input,");
	w.write_line("`currRegVariable, `currFixedPointStatus, skipCyclesOfWriteAccess[row][col].exists(executionCycle), resultStr), currPosition };");
	w.dec_tab();
	w.write_line("");
	w.write_line("if (is_input)");
	w.inc_tab();
	w.write_line("regWriteActivities = {regWriteActivities, \"\\n\", strLine};");
	w.dec_tab();
	w.write_line("else");
	w.inc_tab();
	w.write_line("regReadActivities = {regReadActivities, \"\\n\", strLine};");
	w.dec_tab();
	w.write_line("");
	w.write_line("resultActivities = {resultActivities, resultStr};");
	w.write_line("");
	w.dec_tab();
	w.write_line("endfunction");
	w.write_line("");
	w.write_line("fill_data fd();");
	w.write_line("");
	w.write_line("always @(negedge rst_n or negedge clk)");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("if (rst_n == 1'b0)");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("clkCounter = 0;");
	w.write_line("$fdisplay(fileid1, startOfExecutionStr);");
	w.dec_tab();
	w.write_line("end");
	w.write_line("else");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("if (clkCounter >= execution_start_cycle)");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("executionCycle = clkCounter - execution_start_cycle;");
	w.write_line("");
	w.write_line("regWriteActivities = \"\";");
	w.write_line("regReadActivities  = \"\";");
	w.write_line("resultActivities   = \"\";");
	w.write_line("");
	w.write_line("for (int row = 0; row < ROWS; row++)");
	w.inc_tab();
	w.write_line("for (int col = 0; col < COLUMNS; col++)");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("if (`currRegData.addrenIn0 || `currRegData.addrenIn1 || `currRegData.addrenOut0 || `currRegData.addrenOut1)");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("if (`currRegData.addrenIn0)");
	w.inc_tab();
	w.write_line("printRegValue(row, col, `currRegData.addrIn0, `currRegData.dataIn0, 0, 1);");
	w.dec_tab();
	w.write_line("if (`currRegData.addrenIn1)");
	w.inc_tab();
	w.write_line("printRegValue(row, col, `currRegData.addrIn1, `currRegData.dataIn1, 1, 1);");
	w.dec_tab();
	w.write_line("if (`currRegData.addrenOut0)");
	w.inc_tab();
	w.write_line("printRegValue(row, col, `currRegData.addrOut0, `currRegData.dataOut0, 0, 0);");
	w.dec_tab();
	w.write_line("if (`currRegData.addrenOut1)");
	w.inc_tab();
	w.write_line("printRegValue(row, col, `currRegData.addrOut1, `currRegData.dataOut1, 1, 0);");
	w.dec_tab();
	w.dec_tab();
	w.write_line("end");
	w.write_line("");
	w.write_line("createSequencerActivity(row, col, executionCycle);");
	w.write_line("");
	w.dec_tab();
	w.write_line("end");
	w.dec_tab();
	w.write_line("if (regWriteActivities != \"\")");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("$fdisplay(fileid1, regWriteActivities);");
	w.write_line("$fdisplay(fileid1, wtDelimiter);");
	w.dec_tab();
	w.write_line("end");
	w.write_line("");
	w.write_line("if (regReadActivities != \"\")");
	w.inc_tab();
	w.write_line("$fdisplay(fileid1, regReadActivities);");
	w.dec_tab();
	w.write_line("if (resultActivities != \"\")");
	w.inc_tab();
	w.write_line("$fdisplay(fileid5, resultActivities);");
	w.dec_tab();
	w.write_line("printDimarchData(fileid4, executionCycle);");
	w.dec_tab();
	w.write_line("end");
	w.write_line("");
	w.write_line("if (clkCounter == total_execution_cycle-1)");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("$fdisplay(fileid2, startOfExecutionStr);");
	w.write_line("$fdisplay(fileid3, startOfExecutionStr);");
	w.write_line("");
	w.write_line("for (int row = 0; row < ROWS; row++)");
	w.inc_tab();
	w.write_line("for (int col = 0; col < COLUMNS; col++)");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("printRegFileActivity(fileid2, row, col);");
	w.write_line("printSequencerActivity(fileid3, row, col);");
	w.dec_tab();
	w.write_line("end");
	w.dec_tab();
	w.dec_tab();
	w.write_line("end");
	w.write_line("");
	w.write_line("clkCounter++;");
	w.dec_tab();
	w.write_line("end");
	w.dec_tab();
	w.write_line("end");
	w.write_line("");
	w.dec_tab();
	w.write_line("endmodule");
}

void GeVsim::gen_util(){
	string path = _output_dir + "fill_data.sv";
	
	GeWriter w(path);
	w.write_line("`include \"macros.svh\"");
	w.write_line("import name_mangling::*;");
	w.write_line("import test_util::*;");
	w.write_line("");
	w.write_line("module fill_data;");
	w.inc_tab();
	w.write_line("generate");
	w.inc_tab();
	w.write_line("for (genvar row = 0; row < ROWS; row++)");
	w.inc_tab();
	w.write_line("for (genvar col = 0; col < COLUMNS; col++)");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("// Correspondence: AGU_Wr_0 -> data_in_0; AGU_Wr_1 -> data_in_1; AGU_Rd_0 -> data_out_reg_0_left; AGU_Rd_1 -> data_out_reg_1_left;");
	w.write_line("assign `currRegData.dataIn0 		= `regFileData(data_in_0);");
	w.write_line("assign `currRegData.dataIn1 		= `regFileData(data_in_1);");
	w.write_line("assign `currRegData.dataOut0 		= `regFileData(data_out_reg_0_left);");
	w.write_line("assign `currRegData.dataOut1 		= `regFileData(data_out_reg_1_left);");
	w.write_line("assign `currRegData.addrIn0 		= `aguAddress(Wr_0);");
	w.write_line("assign `currRegData.addrIn1 		= `aguAddress(Wr_1);");
	w.write_line("assign `currRegData.addrOut0 		= `aguAddress(Rd_0);");
	w.write_line("assign `currRegData.addrOut1 		= `aguAddress(Rd_1);");
	w.write_line("assign `currRegData.addrenIn0 		= `aguAddressEn(Wr_0);");
	w.write_line("assign `currRegData.addrenIn1 		= `aguAddressEn(Wr_1);");
	w.write_line("assign `currRegData.addrenOut0		= `aguAddressEn(Rd_0);");
	w.write_line("assign `currRegData.addrenOut1		= `aguAddressEn(Rd_1);");
	w.write_line("assign `currRegData.instStartIn0	= `fabricRegTop.AGU_Wr_0_instantiate.instr_start;");
	w.write_line("");
	w.write_line("assign `currSequencerData.pc 		= `fabricCell.seq_gen.pc;");
	w.write_line("assign `currSequencerData.currInst 	= `fabricCell.seq_gen.instr;");
	w.dec_tab();
	w.write_line("end");
	w.dec_tab();
	w.dec_tab();
	w.write_line("endgenerate");
	w.write_line("");
	w.write_line("generate");
	w.inc_tab();
	w.write_line("for (genvar row = 0; row < ROWS-1; row++)");
	w.inc_tab();
	w.write_line("for (genvar col = 0; col < COLUMNS; col++)");
	w.write_line("begin");
	w.inc_tab();
	w.write_line("assign `currSramData.readEn 		= `sramReadEn;");
	w.write_line("assign `currSramData.writeEn		= `sramWriteEn;");
	w.write_line("assign `currSramData.writeAddress 	= `sramWriteAddress;");
	w.write_line("assign `currSramData.data 			= `sramMemData;");
	w.dec_tab();
	w.write_line("end");
	w.dec_tab();
	w.dec_tab();
	w.write_line("endgenerate");
	w.write_line("");
	w.write_line("initial");
	w.inc_tab();
	w.write_line("for (int row = 0; row < ROWS; row++)");
	w.inc_tab();
	w.write_line("for (int col = 0; col < COLUMNS; col++)");
	w.inc_tab();
	w.write_line("oldpcs[row][col] = -1;");
	w.dec_tab();
	w.dec_tab();
	w.dec_tab();
	w.dec_tab();
	w.write_line("endmodule");
	
	path = _output_dir + "macros.svh";
	GeWriter w1(path);

	w1.write_line("`ifndef _MACROS_");
	w1.write_line("`define _MACROS_");
	w1.write_line("");
	w1.write_line("`define fabricCell 			DUT.MTRF_COLS[col].MTRF_ROWS[row].SILEGO_cell.MTRF_cell");
	w1.write_line("`define fabricRegTop 		`fabricCell.reg_top");
	w1.write_line("`define aguAddressEn(port)	`fabricRegTop.AGU_``port``_instantiate.addr_en");
	w1.write_line("`define aguAddress(port)	`fabricRegTop.AGU_``port``_instantiate.addr_out");
	w1.write_line("`define regFileData(port)	`fabricRegTop.RegisterFile.``port");
	w1.write_line("`define dimarchDataIn 		`fabricRegTop.dimarch_data_in");
	w1.write_line("`define currRegVariable		regFileVariables[row][col][address]");
	w1.write_line("`define currRegData 		regData[row][col]");
	w1.write_line("`define currSequencerData	sequencerData[row][col]");
	w1.write_line("`define currInstruction		`currSequencerData.currInst");
	w1.write_line("`define currFixedPointStatus regFileFixedPointStatus[row][col][address]");
	w1.write_line("");
	w1.write_line("`define stile				DUT.DiMArch_COLS[col].DiMArch_ROWS[row].u_STILE");
	w1.write_line("`define sramReadEn			`stile.SRAM_rw_r");
	w1.write_line("`define sramWriteEn			`stile.SRAM_rw_w");
	w1.write_line("`define sramWriteAddress	`stile.SRAM_rw_addrs_out_w");
	w1.write_line("`define sramMemData			`stile.memory_out");
	w1.write_line("`define currSramData		sramData[0][col]");
	w1.write_line("");
	w1.write_line("`endif //_MACROS_");
	
	
	path = _output_dir + "name_mangling.svh";
	GeWriter w2(path);
	w2.write_line("`ifndef _NAME_MANGLING_");
	w2.write_line("`define _NAME_MANGLING_");
	w2.write_line("");
	w2.write_line("import top_consts_types_package::*;");
	w2.write_line("");
	w2.write_line("package name_mangling;");
	w2.inc_tab();
	w2.write_line("");
	w2.write_line("parameter REG_FILE_DATA_WIDTH 		= top_consts_types_package::reg_file_data_width;");
	w2.write_line("parameter REG_FILE_ADDR_WIDTH 		= top_consts_types_package::reg_file_addr_width;");
	w2.write_line("parameter REG_FILE_DEPTH	  		= top_consts_types_package::reg_file_depth;");
	w2.write_line("parameter REG_FILE_MEM_DATA_WIDTH 	= top_consts_types_package::reg_file_mem_data_width;");
	w2.write_line("parameter ROWS 				  		= top_consts_types_package::rows;");
	w2.write_line("parameter COLUMNS 			  		= top_consts_types_package::columns;");
	w2.write_line("parameter INSTR_WIDTH		    	= top_consts_types_package::instr_width;");
	w2.write_line("parameter INSTR_CODE_RANGE_BASE 	= top_consts_types_package::instr_code_range_base;");
	w2.write_line("parameter INSTR_CODE_RANGE_END 		= top_consts_types_package::instr_code_range_end;");
	w2.write_line("parameter SRAM_WIDTH				= top_consts_types_package::sram_width;");
	w2.write_line("");
	w2.write_line("// DPU parameters");
	w2.write_line("parameter DPU_MODE_SEL_RANGE_BASE 			= top_consts_types_package::dpu_mode_sel_range_base;");
	w2.write_line("parameter DPU_MODE_SEL_RANGE_END 			= top_consts_types_package::dpu_mode_sel_range_end;");
	w2.write_line("// REFI1 parameters");
	w2.write_line("parameter NR_OF_REG_FILE_PORTS_RANGE_BASE 	= top_consts_types_package::nr_of_reg_file_ports_range_base;");
	w2.write_line("parameter NR_OF_REG_FILE_PORTS_RANGE_END 	= top_consts_types_package::nr_of_reg_file_ports_range_end;");
	w2.write_line("parameter STARTING_ADDRS_RANGE_BASE 		= top_consts_types_package::starting_addrs_range_base;");
	w2.write_line("parameter STARTING_ADDRS_RANGE_END 			= top_consts_types_package::starting_addrs_range_end;");
	w2.write_line("parameter NR_OF_ADDRS_RANGE_BASE 			= top_consts_types_package::nr_of_addrs_range_base;");
	w2.write_line("parameter NR_OF_ADDRS_RANGE_END 			= top_consts_types_package::nr_of_addrs_range_end;");
	w2.write_line("parameter INIT_DELAY_RANGE_BASE 			= top_consts_types_package::init_delay_range_base;");
	w2.write_line("parameter INIT_DELAY_RANGE_END 				= top_consts_types_package::init_delay_range_end;");
	w2.write_line("// REFI2 parameters");
	w2.write_line("parameter STEP_VALUE_RANGE_BASE 			= top_consts_types_package::step_value_range_base;");
	w2.write_line("parameter STEP_VALUE_RANGE_END 				= top_consts_types_package::step_value_range_end;");
	w2.write_line("parameter STEP_VALUE_SIGN_RANGE_BASE 		= top_consts_types_package::step_value_sign_range_base;");
	w2.write_line("parameter STEP_VALUE_SIGN_RANGE_END 		= top_consts_types_package::step_value_sign_range_end;");
	w2.write_line("parameter REG_FILE_MIDDLE_DELAY_RANGE_BASE 	= top_consts_types_package::reg_file_middle_delay_range_base;");
	w2.write_line("parameter REG_FILE_MIDDLE_DELAY_RANGE_END 	= top_consts_types_package::reg_file_middle_delay_range_end;");
	w2.write_line("parameter NUM_OF_REPT_RANGE_BASE 			= top_consts_types_package::num_of_rept_range_base;");
	w2.write_line("parameter NUM_OF_REPT_RANGE_END 			= top_consts_types_package::num_of_rept_range_end;");
	w2.write_line("parameter REP_STEP_VALUE_RANGE_BASE 		= top_consts_types_package::rep_step_value_range_base;");
	w2.write_line("parameter REP_STEP_VALUE_RANGE_END 			= top_consts_types_package::rep_step_value_range_end;");
	w2.write_line("// SWB parameters");
	w2.write_line("parameter SWB_SRC_ADDR_ROW_BASE 			= top_consts_types_package::swb_src_addr_row_base;");
	w2.write_line("parameter SWB_SRC_ADDR_ROW_END 				= top_consts_types_package::swb_src_addr_row_end;");
	w2.write_line("parameter SWB_SRC_DPU_REFI_BASE 			= top_consts_types_package::swb_src_dpu_refi_base;");
	w2.write_line("parameter SWB_SRC_DPU_REFI_END 				= top_consts_types_package::swb_src_dpu_refi_end;");
	w2.write_line("parameter SWB_SRC_OUTPUT_NR_BASE 			= top_consts_types_package::swb_src_output_nr_base;");
	w2.write_line("parameter SWB_SRC_OUTPUT_NR_END 			= top_consts_types_package::swb_src_output_nr_end;");
	w2.write_line("parameter SWB_HB_INDEX_BASE 				= top_consts_types_package::swb_hb_index_base;");
	w2.write_line("parameter SWB_HB_INDEX_END 					= top_consts_types_package::swb_hb_index_end;");
	w2.write_line("parameter SWB_SEND_TO_OTHER_ROW_BASE 		= top_consts_types_package::swb_send_to_other_row_base;");
	w2.write_line("parameter SWB_SEND_TO_OTHER_ROW_END 		= top_consts_types_package::swb_send_to_other_row_end;");
	w2.write_line("parameter SWB_V_INDEX_BASE 					= top_consts_types_package::swb_v_index_base;");
	w2.write_line("parameter SWB_V_INDEX_END 					= top_consts_types_package::swb_v_index_end;");
	w2.write_line("// DELAY parameters");
	w2.write_line("parameter DLY_CYCLES_RANGE_BASE 			= top_consts_types_package::dly_cycles_range_base;");
	w2.write_line("parameter DLY_CYCLES_RANGE_END 				= top_consts_types_package::dly_cycles_range_end;");
	w2.write_line("// Branch parameters");
	w2.write_line("parameter BR_MODE_RANGE_BASE	 			= top_consts_types_package::br_mode_range_base;");
	w2.write_line("parameter BR_MODE_RANGE_END		 			= top_consts_types_package::br_mode_range_end;");
	w2.write_line("parameter BR_FALSE_ADDRS_RANGE_BASE			= top_consts_types_package::br_false_addrs_range_base;");
	w2.write_line("parameter BR_FALSE_ADDRS_RANGE_END 			= top_consts_types_package::br_false_addrs_range_end;");
	w2.write_line("// Jump parameters");
	w2.write_line("parameter TRUE_ADDRS_RANGE_BASE 			= top_consts_types_package::true_addrs_range_base;");
	w2.write_line("parameter TRUE_ADDRS_RANGE_END	 			= top_consts_types_package::true_addrs_range_end;");
	w2.write_line("// RACCU parameters");
	w2.write_line("parameter RACCU_MODE_SEL_RANGE_BASE = top_consts_types_package::raccu_mode_sel_range_base;");
	w2.write_line("parameter RACCU_MODE_SEL_RANGE_END = top_consts_types_package::raccu_mode_sel_range_end;");
	w2.write_line("parameter RACCU_OPERAND1_RANGE_BASE = top_consts_types_package::raccu_operand1_range_base;");
	w2.write_line("parameter RACCU_OPERAND1_RANGE_END = top_consts_types_package::raccu_operand1_range_end;");
	w2.write_line("parameter RACCU_OPERAND2_RANGE_BASE = top_consts_types_package::raccu_operand2_range_base;");
	w2.write_line("parameter RACCU_OPERAND2_RANGE_END = top_consts_types_package::raccu_operand2_range_end;");
	w2.write_line("parameter RACCU_RESULT_ADDR_RANGE_BASE = top_consts_types_package::raccu_result_addr_range_base;");
	w2.write_line("parameter RACCU_RESULT_ADDR_RANGE_END = top_consts_types_package::raccu_result_addr_range_end;");
	w2.write_line("// FOR_HEADER parameters");
	w2.write_line("parameter FOR_INDEX_ADDR_RANGE_BASE = top_consts_types_package::for_index_addr_range_base;");
	w2.write_line("parameter FOR_INDEX_ADDR_RANGE_END = top_consts_types_package::for_index_addr_range_end;");
	w2.write_line("parameter FOR_INDEX_START_RANGE_BASE = top_consts_types_package::for_index_start_range_base;");
	w2.write_line("parameter FOR_INDEX_START_RANGE_END = top_consts_types_package::for_index_start_range_end;");
	w2.write_line("parameter FOR_ITER_NO_RANGE_BASE = top_consts_types_package::for_iter_no_range_base;");
	w2.write_line("parameter FOR_ITER_NO_RANGE_END = top_consts_types_package::for_iter_no_range_end;");
	w2.write_line("// FOR_TAIL parameters");
	w2.write_line("parameter FOR_TAIL_INDEX_ADDR_RANGE_BASE = top_consts_types_package::for_tail_index_addr_range_base;");
	w2.write_line("parameter FOR_TAIL_INDEX_ADDR_RANGE_END = top_consts_types_package::for_tail_index_addr_range_end;");
	w2.write_line("parameter FOR_INDEX_STEP_RANGE_BASE = top_consts_types_package::for_index_step_range_base;");
	w2.write_line("parameter FOR_INDEX_STEP_RANGE_END = top_consts_types_package::for_index_step_range_end;");
	w2.write_line("parameter FOR_PC_TOGO_RANGE_BASE = top_consts_types_package::for_pc_togo_range_base;");
	w2.write_line("parameter FOR_PC_TOGO_RANGE_END = top_consts_types_package::for_pc_togo_range_end;");
	w2.write_line("");
	w2.dec_tab();
	w2.write_line("endpackage");
	w2.write_line("");
	w2.write_line("import name_mangling::*;");
	w2.write_line("");
	w2.write_line("`endif //_NAME_MANGLING_");
	
	
	path = _output_dir + "test_util.svh";
	GeWriter w3(path);
	w3.write_line("`ifndef _TEST_UTIL_");
	w3.write_line("`define _TEST_UTIL_");
	w3.write_line("");
	w3.write_line("`include \"macros.svh\"");
	w3.write_line("");
	w3.write_line("package test_util;");
	w3.inc_tab();
	w3.write_line("");
	w3.write_line("`include \"type_def.svh\"");
	w3.write_line("");
	w3.write_line("RegData regData[ROWS][COLUMNS];");
	w3.write_line("SequencerData sequencerData[ROWS][COLUMNS];");
	w3.write_line("SramData sramData[ROWS-1][COLUMNS];");
	w3.write_line("string regFileActivities[ROWS][COLUMNS];");
	w3.write_line("string sequencerActivities[ROWS][COLUMNS];");
	w3.write_line("int oldpcs[ROWS][COLUMNS];");
	w3.write_line("InstructionCode instructionCode;");
	w3.write_line("");
	w3.write_line("function automatic string getRegValueStr(shortint row, col, address, data, portNo, executionCycle, variablesStartAddr, addressOffset,");
	w3.inc_tab();
	w3.write_line(" bit is_input, string regVariable, bit is_fixed_point, bit ignoreWriteAccess, output string resultStr);");
	w3.write_line("");
	w3.write_line("const string name = is_input ? (portNo == 0 ? \"in0 :\" : \"in1 :\") : (portNo == 0 ? \"out0:\" : \"out1:\");");
	w3.write_line("const string dataValue = is_fixed_point ? $sformatf(\"%.4f\", data * 2.0 ** -15) : $sformatf(\"%2d\", data);");
	w3.write_line("const string lineStr = $sformatf({(is_input ? \"WT\" : \"RD\"), \": %1s(%2d) = \", dataValue, \"; \\t{ addr_\", name, \" %2d, data_\", name, \" %2d }\"},");
	w3.write_line("         regVariable, addressOffset + address - variablesStartAddr + 1, address, data);");
	w3.write_line("const string timeStr = $sformatf(\",\\t@ %5d ns (cycle: %2d)\", $time, executionCycle);");
	w3.write_line("");
	w3.write_line("regFileActivities[row][col] = {regFileActivities[row][col], lineStr, timeStr, \"\\n\"};");
	w3.write_line("resultStr = is_input && !ignoreWriteAccess ? $sformatf({\"\\n%1s(%2d)=\", dataValue}, regVariable, addressOffset + address - variablesStartAddr + 1) : \"\";");
	w3.write_line("");
	w3.write_line("return lineStr;");
	w3.dec_tab();
	w3.write_line("endfunction");
	w3.write_line("");
	w3.write_line("function automatic string printDimarchData(integer fileId, shortint executionCycle);");
	w3.write_line("");
	w3.inc_tab();
	w3.write_line("for (int col = 0; col < COLUMNS; col++)");
	w3.inc_tab();
	w3.write_line("if (`currSramData.writeEn)");
	w3.inc_tab();
	w3.write_line("$fdisplay(fileId, printStileData(0, col, executionCycle, `currSramData.writeAddress, `currSramData.data));");
	w3.write_line("");
	w3.dec_tab();
	w3.dec_tab();
	w3.dec_tab();
	w3.write_line("endfunction");
	w3.write_line("");
	w3.write_line("function automatic string printStileData(shortint row, col, executionCycle, address, logic [SRAM_WIDTH-1:0] data);");
	w3.inc_tab();
	w3.write_line("const string timeStr = $sformatf(\",\\t@ %5d ns (cycle: %2d)\", $time, executionCycle);");
	w3.write_line("return $sformatf(\"\\nWT to SRAM(%1d,%1d), ADDRESS(%3d) -> %h%s\", row, col, address, data, timeStr);");
	w3.dec_tab();
	w3.write_line("endfunction");
	w3.write_line("");
	w3.write_line("function automatic void createSequencerActivity(int row, col, executionCycle);");
	w3.inc_tab();
	w3.write_line("");
	w3.write_line("string instDetail;");
	w3.write_line("");
	w3.write_line("if (`currSequencerData.pc != oldpcs[row][col])");
	w3.write_line("begin");
	w3.inc_tab();
	w3.write_line("instructionCode = InstructionCode'(`currInstruction[INSTR_CODE_RANGE_BASE:INSTR_CODE_RANGE_END]);");
	w3.write_line("");
	w3.write_line("// Specifying the instruction details");
	w3.write_line("case (instructionCode)");
	w.inc_tab();
	w3.write_line("iDpu:");
	w3.write_line("instDetail = $sformatf(\"mode: %0d\", `currInstruction[DPU_MODE_SEL_RANGE_BASE : DPU_MODE_SEL_RANGE_END]);");
	w3.write_line("iRefi1:");
	w3.write_line("instDetail = $sformatf(\"port: %0d, startAddr: %0d, noOfAddr: %0d, initialDelay: %0d\",");
	w3.write_line(" `currInstruction[NR_OF_REG_FILE_PORTS_RANGE_BASE : NR_OF_REG_FILE_PORTS_RANGE_END],");
	w3.write_line(" `currInstruction[STARTING_ADDRS_RANGE_BASE 	  :       STARTING_ADDRS_RANGE_END],");
	w3.write_line(" `currInstruction[NR_OF_ADDRS_RANGE_BASE		  : 	     NR_OF_ADDRS_RANGE_END],");
	w3.write_line(" `currInstruction[INIT_DELAY_RANGE_BASE			  : 		  INIT_DELAY_RANGE_END]); ");
	w3.write_line("iRefi2:");
	w3.write_line("instDetail = $sformatf(\"stepVal: %0d, stepValSign: %0d, middleDelay: %0d, noOfRepetition: %0d, repetitionStepVal: %0d\",");
	w3.write_line(" `currInstruction[STEP_VALUE_RANGE_BASE 			: 			 STEP_VALUE_RANGE_END],");
	w3.write_line(" `currInstruction[STEP_VALUE_SIGN_RANGE_BASE		: 		STEP_VALUE_SIGN_RANGE_END],");
	w3.write_line(" `currInstruction[REG_FILE_MIDDLE_DELAY_RANGE_BASE	: REG_FILE_MIDDLE_DELAY_RANGE_END],");
	w3.write_line(" `currInstruction[NUM_OF_REPT_RANGE_BASE			: 			NUM_OF_REPT_RANGE_END],");
	w3.write_line(" `currInstruction[REP_STEP_VALUE_RANGE_BASE			:		 REP_STEP_VALUE_RANGE_END]); ");
	w3.write_line("iRefi3:");
	w3.write_line("instDetail = \"REFI3\"; ");
	w3.write_line("iSwb:");
	w3.write_line("instDetail =  $sformatf(\"srcRow: %0d, srcDpuOrRefi: %4s, HbIndex: %0d, VIndex: %0d, srcOutputNr: %0d, SendToOtherRow: %3s\",");
	w3.write_line(" `currInstruction[SWB_SRC_ADDR_ROW_BASE		 :		SWB_SRC_ADDR_ROW_END],");
	w3.write_line(" (`currInstruction[SWB_SRC_DPU_REFI_BASE	 :		SWB_SRC_DPU_REFI_END] == 0 ? \"REFI\" : \"DPU\"),");
	w3.write_line(" `currInstruction[SWB_HB_INDEX_BASE			 :			SWB_HB_INDEX_END],");
	w3.write_line(" `currInstruction[SWB_V_INDEX_BASE 			 :			 SWB_V_INDEX_END],");
	w3.write_line(" `currInstruction[SWB_SRC_OUTPUT_NR_BASE 	 :	   SWB_SRC_OUTPUT_NR_END],										 ");
	w3.write_line(" `currInstruction[SWB_SEND_TO_OTHER_ROW_BASE : SWB_SEND_TO_OTHER_ROW_END] == 0 ? \"NO\" : \"YES\"); ");
	w3.write_line("iDelay:");
	w3.write_line("instDetail = $sformatf(\"delay cycles: %0d\", `currInstruction[DLY_CYCLES_RANGE_BASE : DLY_CYCLES_RANGE_END]); ");
	w3.write_line("iBrnch:");
	w3.write_line("instDetail = $sformatf(\"mode: %0d, falseAddr: %0d\", ");
	w3.write_line("`currInstruction[BR_MODE_RANGE_BASE 	   : 		BR_MODE_RANGE_END],");
	w3.write_line("`currInstruction[BR_FALSE_ADDRS_RANGE_BASE : BR_FALSE_ADDRS_RANGE_END]);");
	w3.write_line("iJump:");
	w3.write_line("instDetail = $sformatf(\"Address: %0d\", `currInstruction[TRUE_ADDRS_RANGE_BASE : TRUE_ADDRS_RANGE_END]);");
	w3.write_line("");
	w3.write_line("iRaccu:");
	w3.write_line("instDetail = $sformatf(\"Mode: %0d, Op1: %0d, Op2: %0d, ResAddress: %0d\",");
	w3.write_line("`currInstruction[RACCU_MODE_SEL_RANGE_BASE 	  :    RACCU_MODE_SEL_RANGE_END],");
	w3.write_line("`currInstruction[RACCU_OPERAND1_RANGE_BASE 	  :    RACCU_OPERAND1_RANGE_END],");
	w3.write_line("`currInstruction[RACCU_OPERAND2_RANGE_BASE 	  :    RACCU_OPERAND2_RANGE_END],");
	w3.write_line("`currInstruction[RACCU_RESULT_ADDR_RANGE_BASE : RACCU_RESULT_ADDR_RANGE_END]);");
	w3.write_line("");
	w3.write_line("iForHead:");
	w3.write_line("instDetail = $sformatf(\"LoopId: %0d, IndexStart: %0d, IterationNo: %0d\", ");
	w3.write_line("`currInstruction[FOR_INDEX_ADDR_RANGE_BASE 	 :  FOR_INDEX_ADDR_RANGE_END],");
	w3.write_line("`currInstruction[FOR_INDEX_START_RANGE_BASE  : FOR_INDEX_START_RANGE_END],");
	w3.write_line("`currInstruction[FOR_ITER_NO_RANGE_BASE 	 :     FOR_ITER_NO_RANGE_END]);");
	w3.write_line("");
	w3.write_line("iForTail:");
	w3.write_line("instDetail = $sformatf(\"LoopId: %0d, IndexStep: %0d, PCtoGo: %0d\", ");
	w3.write_line("`currInstruction[FOR_TAIL_INDEX_ADDR_RANGE_BASE : FOR_TAIL_INDEX_ADDR_RANGE_END],");
	w3.write_line("`currInstruction[FOR_INDEX_STEP_RANGE_BASE  	:      FOR_INDEX_STEP_RANGE_END],");
	w3.write_line("`currInstruction[FOR_PC_TOGO_RANGE_BASE 	 	:         FOR_PC_TOGO_RANGE_END]);");
	w3.write_line("");
	w3.write_line("default:");
	w3.write_line("instDetail = \"\";");
	w3.dec_tab();
	w3.write_line("endcase");
	w3.write_line("");
	w3.write_line("sequencerActivities[row][col] = { sequencerActivities[row][col], ");
	w3.inc_tab();
	w3.write_line("$sformatf(\"%5d\\t%5d\\t%8h\\t%8s\\t--->\\t(%s)\\n\", executionCycle, `currSequencerData.pc,");
	w3.write_line("  `currInstruction, instructionCode.name(), instDetail) };");
	w3.dec_tab();
	w3.write_line("");
	w3.write_line("oldpcs[row][col] = `currSequencerData.pc;");
	w3.dec_tab();
	w3.write_line("end");
	w3.dec_tab();
	w3.write_line("endfunction");
	w3.write_line("");
	w3.write_line("function automatic void printRegFileActivity(int fileid, row, col);");
	w3.inc_tab();
	w3.write_line("if (regFileActivities[row][col].len() > 0)");
	w3.write_line("begin");
	w3.inc_tab();
	w3.write_line("$fdisplay(fileid, \"#----------------- RegFile <%0d,%0d> -----------------\\n\", row, col);");
	w3.write_line("$fdisplay(fileid, regFileActivities[row][col]);");
	w3.dec_tab();
	w3.write_line("end");
	w3.dec_tab();
	w3.write_line("endfunction");
	w3.write_line("");
	w3.write_line("function automatic void printSequencerActivity(int fileid, row, col);");
	w3.inc_tab();
	w3.write_line("if (sequencerActivities[row][col].len() > 0)");
	w3.write_line("begin");
	w3.inc_tab();
	w3.write_line("$fdisplay(fileid, \"#----------------- Sequencer <%0d,%0d> -----------------\", row, col);");
	w3.write_line("$fdisplay(fileid, \"%5s\\t%5s\\t%8s\\t%8s\\t--->\\t%s\\n\", \"CYCLE\", \"PC\", \"INST_VAL\", \"INST_TYPE\", \"DETAILS\");");
	w3.write_line("$fdisplay(fileid, sequencerActivities[row][col]);");
	w3.dec_tab();
	w3.write_line("end");
	w3.dec_tab();
	w3.write_line("endfunction");
	w3.dec_tab();
	w3.write_line("");
	w3.write_line("endpackage");
	w3.write_line("");
	w3.write_line("`endif //_TEST_UTIL_");
	w3.write_line("");
	
	path = _output_dir + "test_util_files.f";
	GeWriter w4(path);
	w4.write_line("name_mangling.svh");
	w4.write_line("macros.svh");
	w4.write_line("type_def.svh");
	w4.write_line("test_util.svh");
	w4.write_line("fill_data.sv");
	
	path = _output_dir + "type_def.svh";
	GeWriter w5(path);

	w5.write_line("`ifndef _TYPE_DEF_");
	w5.write_line("`define _TYPE_DEF_");
	w5.write_line("");
	w5.write_line("import name_mangling::*;");
	w5.write_line("");
	w5.write_line("typedef struct");
	w5.write_line("{");
	w5.inc_tab();
	w5.write_line("logic [REG_FILE_DATA_WIDTH-1 : 0] dataIn0;");
	w5.write_line("logic [REG_FILE_DATA_WIDTH-1 : 0] dataIn1;");
	w5.write_line("logic [REG_FILE_DATA_WIDTH-1 : 0] dataOut0;");
	w5.write_line("logic [REG_FILE_DATA_WIDTH-1 : 0] dataOut1;");
	w5.write_line("");
	w5.write_line("logic [REG_FILE_ADDR_WIDTH-1 : 0] addrIn0;");
	w5.write_line("logic [REG_FILE_ADDR_WIDTH-1 : 0] addrIn1;");
	w5.write_line("logic [REG_FILE_ADDR_WIDTH-1 : 0] addrOut0;");
	w5.write_line("logic [REG_FILE_ADDR_WIDTH-1 : 0] addrOut1;");
	w5.write_line("");
	w5.write_line("logic instStartIn0;");
	w5.write_line("logic addrenIn0;");
	w5.write_line("logic addrenIn1;");
	w5.write_line("logic addrenOut0;");
	w5.write_line("logic addrenOut1;");
	w5.dec_tab();
	w5.write_line("} RegData;");
	w5.write_line("");
	w5.write_line("typedef struct");
	w5.write_line("{");
	w5.inc_tab();
	w5.write_line("int pc;");
	w5.write_line("logic [INSTR_WIDTH-1 : 0] currInst;");
	w5.dec_tab();
	w5.write_line("} SequencerData;");
	w5.write_line("");
	w5.write_line("typedef struct");
	w5.write_line("{");
	w5.inc_tab();
	w5.write_line("logic readEn;");
	w5.write_line("logic writeEn;");
	w5.write_line("int writeAddress;");
	w5.write_line("logic [SRAM_WIDTH-1:0] data;");
	w5.dec_tab();
	w5.write_line("} SramData;");
	w5.write_line("");
	w5.write_line("typedef enum ");
	w5.write_line("{");
	w5.inc_tab();
	w5.write_line("iBrnch 		= 'b1011,");
	w5.write_line("iDelay 		= 'b0111,");
	w5.write_line("iDpu   		= 'b0100,");
	w5.write_line("iForTail   	= 'b1001,");
	w5.write_line("iForHead  	= 'b1000,");
	w5.write_line("iJump  		= 'b0110,");
	w5.write_line("iRaccu 		= 'b1010,");
	w5.write_line("iRefi1 		= 'b0001,");
	w5.write_line("iRefi2 		= 'b0010,");
	w5.write_line("iRefi3 		= 'b0011,");
	w5.write_line("iRoute		= 'b1100,");
	w5.write_line("iSRAM_R		= 'b1101,");
	w5.write_line("iSRAM_W		= 'b1110,");
	w5.write_line("iSwb   		= 'b0101");
	w5.dec_tab();
	w5.write_line("} InstructionCode;");
	w5.write_line("");
	w5.write_line("`endif //_TYPE_DEF_");
	
}

void GeVsim::gen_script(){
	string path = _output_dir + "run_cmd_"+_design+".do";
	GeWriter w(path);

	w.write_line("if [file exists results] {");
	w.inc_tab();
	w.write_line("rm -r results");
	w.dec_tab();
	w.write_line("}");
	w.write_line("mkdir -p results");
	w.write_line("if [file exists work] {");
	w.inc_tab();
	w.write_line("vdel -all");
	w.dec_tab();
	w.write_line("}");
	w.write_line("vlib work");
	w.write_line("");
	w.write_line("vcom "+_fabric_path+"/mtrf/util_package.vhd");
	w.write_line("vcom "+_fabric_path+"/hw_setting.vhd");
	w.write_line("vcom -mixedsvvh "+_fabric_path+"/mtrf/top_consts_types_package.vhd");
	w.write_line("vcom -F "+_fabric_path+"/fabric_files.f");
	w.write_line("vcom -mixedsvvh const_package_"+_design+".vhd");
	w.write_line("vlog -F test_util_files.f");
	w.write_line("vlog profiler_"+_design+".sv");
	w.write_line("vcom testbench_"+_design+".vhd");
	w.write_line("");
	w.write_line("vsim testbench_"+_design);
	w.write_line("set StdArithNoWarnings 1");
	w.write_line("set NumericStdNoWarnings 1");
	w.write_line("run 0 ns;");
	w.write_line("set StdArithNoWarnings 0");
	w.write_line("set NumericStdNoWarnings 0");
	w.write_line("run "+to_string(_max_scheduled_cycle+_instr_no+0+_reg_load_cycles)+"ns");
	w.write_line("quit");
}

}
}
