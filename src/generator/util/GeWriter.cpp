#include "generator/util/GeWriter.hpp"

namespace vesyla{
namespace generator{

GeWriter::GeWriter(string filename){
	_f.close();
	_f.open(filename);
	if(!_f){
		LOG(ERROR) << "Can't open file " << filename << "!";
		return;
	}
	_depth = 0;
}

GeWriter::~GeWriter(){
	_f.close();
}

void GeWriter::write_line(string line){
	for(int i=0; i<_depth; i++){
		_f << "\t";
	}
	_f << line << endl;
}

void GeWriter::inc_tab(){
	_depth++;
}

void GeWriter::dec_tab(){
	_depth--;
}

int GeWriter::depth(){
	return _depth;
}

}
}
