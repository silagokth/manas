#include "vesyla/main.hpp"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using namespace std;
using namespace boost::program_options;
using namespace vesyla;


_INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]){

	// Argument parsing
	bool is_unroll=false;
	string path_output;
	string path_fabric;
	vector<string> file_list;
	
	options_description desc("Options"); 
	desc.add_options()
	("help,h", "Print help messages")
	("fabric-path,f", value<string>(&path_fabric)->default_value("fabric"), "Fabric description path")
	("output,o", value<string>(&path_output)->default_value("."), "Output directory");
	
	variables_map vm;
	try{
		parsed_options parsed = parse_command_line(argc, argv, desc);
		store(parsed, vm);
		if(vm.count("help")){
			cout	<< "Manas" << endl 
						<< "=============================================" << endl
						<< desc << endl; 
			return 0; 
		}
		file_list = collect_unrecognized(parsed.options, include_positional);
		notify(vm);
	}catch(error& e){
		cerr << endl;
		cerr << "ERROR:" << e.what() << endl;
		cerr << endl;
		cerr << desc << endl;
		return -1;
	}
	
	if(file_list.empty()){
		LOG(FATAL) << "No source file specified!";
	}
	
	// red fabric setting
	int hw_reg_file_depth =0;
	int hw_rows = 0;
	int hw_cols = 0;
	ifstream file(path_fabric+"/hw_setting.vhd");
	string str;
	boost::regex e1("\\s*CONSTANT\\s+HW_REG_FILE_DEPTH[^=]*=\\s*([\\d]+).*");
	boost::regex e2("\\s*CONSTANT\\s+HW_ROWS[^=]*=\\s*([\\d]+).*");
	boost::regex e3("\\s*CONSTANT\\s+HW_COLUMNS[^=]*=\\s*([\\d]+).*");
	while (std::getline(file, str)){
		boost::cmatch what;
		if(boost::regex_match(str.c_str(), what, e1)){
			hw_reg_file_depth = stoi(what[1]);
		}else if(boost::regex_match(str.c_str(), what, e2)){
			hw_rows = stoi(what[1]);
		}else if(boost::regex_match(str.c_str(), what, e3)){
			hw_cols = stoi(what[1]);
		}
	}
	if(hw_reg_file_depth<=0||hw_rows<=0||hw_cols<=0){
		LOG(FATAL) << "Fabric size is wrong!";
		exit(-1);
	} 
	
	if(!boost::filesystem::exists(path_output)){
		boost::filesystem::path odir(path_output);
		boost::filesystem::create_directories(odir);
	}
	
	ir::IrManas ir0;
	parser::ParManas par(hw_rows, hw_cols, hw_reg_file_depth, &ir0);
	par.run(file_list[0]);
	
	ir0.dump();
	
	boost::filesystem::path p(file_list[0]);
	string design = p.stem();
	generator::GeVsim g(design, &ir0, boost::filesystem::current_path().string()+"/"+path_fabric, boost::filesystem::current_path().string()+"/"+path_output);
	g.gen_tb();
	g.gen_cfg();
	g.gen_profiler();
	g.gen_util();
	g.gen_script();
	
	return 0;

}
