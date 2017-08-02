#include "parser/ParManas/ParManas.hpp"

extern FILE *yyin;
extern int yydebug;
extern IrManas* ir_ptr;

namespace vesyla{
namespace parser{

ParManas::ParManas(int row_num, int col_num, int refi_total_entry, IrManas* ir_out){
	ir_ptr = ir_out;
	ir_ptr->reset(row_num, col_num, refi_total_entry);
}

void ParManas::run(string filename){
	FILE *f = NULL;
	f = fopen(filename.c_str(), "r");
	if(!f){
		LOG(FATAL) << "Can't open file" << filename << " !";
	}
	
	yyin = f;
	yydebug = 0; 

	yyparse();
}


}
}
