#include "parser/ParManus/ParManus.hpp"

extern FILE *yyin;
extern int yydebug;
extern IrManus* ir_ptr;

namespace vesyla{
namespace parser{

ParManus::ParManus(int row_num, int col_num, int refi_total_entry, IrManus* ir_out){
	ir_ptr = ir_out;
	ir_ptr->reset(row_num, col_num, refi_total_entry);
}

void ParManus::run(string filename){
	FILE *f = NULL;
	f = fopen(filename.c_str(), "r");
	if(!f){
		BOOST_LOG_TRIVIAL(fatal) << "Can't open file" << filename << " !";
		exit(-1);
	}
	
	yyin = f;
	yydebug = 1; 

	yyparse();
}


}
}
