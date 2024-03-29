#include "icode.hh"
template class Const_Opd<double>;
template class Const_Opd<int>;
/* 
This file defines classes for intermediate form of the code generated 
by our compiler. It also defined classes for machine instructions.

An intermediate code (IC) statement consists of an instruction,
possibly two operands and a result.
*/


///////////////////////Instruction Descriptor ///////////////////////////////////


Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string myname, string mnn, string ics, Icode_Format icf, Assembly_Format af){
	inst_op = op;
	name = myname;
	ic_format  = icf;
	mnemonic = mnn;
	assem_format = af;
	ic_symbol = ics;
}
Instruction_Descriptor::Instruction_Descriptor(){

}

Tgt_Op Instruction_Descriptor::get_op(){
	return inst_op;
}

string Instruction_Descriptor::get_name(){
	return name;
}

string Instruction_Descriptor::get_mnemonic(){
	return mnemonic;
}

string Instruction_Descriptor::get_ic_symbol(){
	return ic_symbol;
}

Icode_Format Instruction_Descriptor::get_ic_format(){
	return ic_format;
}

Assembly_Format Instruction_Descriptor::get_assembly_format(){
	return assem_format;
}

void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer){
	file_buffer << name ;
}


///////////////////////////Icode statement operand ///////////////////////////////////


Register_Descriptor * Ics_Opd::get_reg(){}


Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){
	symbol_entry = &se;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer){
		// printf("name printed");
	 file_buffer << symbol_entry->get_variable_name();
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer){
	// file_buffer << symbol_entry->get_mnemonic();
		if (symbol_entry->get_symbol_scope() == global)
		{
		  	file_buffer << symbol_entry->get_variable_name();
		  	return;
		}
		if(symbol_entry->get_ref_offset() == fp_ref){
			file_buffer << symbol_entry->get_start_offset() << "($fp)"; 
		}
		if(symbol_entry->get_ref_offset() == sp_ref){
			file_buffer << symbol_entry->get_start_offset() << "($sp)"; 
		}

}

Mem_Addr_Opd & Mem_Addr_Opd::operator= (const Mem_Addr_Opd & rhs){
	symbol_entry = rhs.symbol_entry;
	return *this;
}



Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){
	register_description = rd;
}

Register_Descriptor * Register_Addr_Opd::get_reg(){
	return register_description;
}
void Register_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer << register_description->get_name();
}
void Register_Addr_Opd::print_asm_opd(ostream & file_buffer){
	//file_buffer << register_description->get_mnemonic();
	file_buffer << "$" << register_description->get_name();
}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs){
	this->register_description = rhs.register_description;
	return *this;
}

template <class T>
Const_Opd<T>::Const_Opd (T mynum){
	num = mynum;
}

template <class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer){
	// printf("newlol\n");
	file_buffer << num;
}

template <class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer){
	file_buffer << num;
}

template <class T>
Const_Opd<T> & Const_Opd<T>::operator=(const Const_Opd & rhs){
	num = rhs.num;
	return *this;
}

///////////////////////////////Intermediate code statements //////////////////////////


Instruction_Descriptor & Icode_Stmt::get_op(){
	return op_desc;
}

Ics_Opd * Icode_Stmt::get_opd1(){}

Ics_Opd * Icode_Stmt::get_opd2(){}

Ics_Opd * Icode_Stmt::get_result(){}

void Icode_Stmt::set_opd1(Ics_Opd * io){}
void Icode_Stmt::set_opd2(Ics_Opd * io){}
void Icode_Stmt::set_result(Ics_Opd * io){}


Print_IC_Stmt::Print_IC_Stmt(){

}
Print_IC_Stmt::~Print_IC_Stmt(){

}
void Print_IC_Stmt::print_icode(ostream & file_buffer) {
	file_buffer << "\tprint\n";
}
void Print_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer << "\tsyscall\n";

}


Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * op1, Ics_Opd * res){
	op_desc =  *machine_desc_object.spim_instruction_table[inst_op];
	opd1 = op1;
	result = res;
}

Move_IC_Stmt & Move_IC_Stmt::operator=(const Move_IC_Stmt & rhs){
	op_desc = rhs.op_desc;
    opd1 = rhs.opd1;
	result = rhs.result;
	return *this;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Move_IC_Stmt::get_opd1(){
	return opd1;
}
void Move_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Move_IC_Stmt::get_result(){
	return result;
}
void Move_IC_Stmt::set_result(Ics_Opd * io){
	result = io;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer){

	// printf("in Move_IC_Stmt\n");
	file_buffer << "\t";
 	op_desc.print_instruction_descriptor(file_buffer);
 	file_buffer << ":    \t";
 	result->print_ics_opd(file_buffer);
 	file_buffer << " <- ";
 	opd1->print_ics_opd(file_buffer);
 	file_buffer << "\n";
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer){

	if(op_desc.get_op() == store || op_desc.get_op() == store_d){

		file_buffer << "\t" << op_desc.get_mnemonic();
	 	file_buffer << " ";
	 	opd1->print_asm_opd(file_buffer);

	 	file_buffer << ", ";
	 	result->print_asm_opd(file_buffer);

	 	file_buffer << "\n";

	 	return;

 	}

 		file_buffer << "\t" << op_desc.get_mnemonic();
	 	file_buffer << " ";
	 	result->print_asm_opd(file_buffer);

	 	file_buffer << ", ";
	 	opd1->print_asm_opd(file_buffer);

	 	file_buffer << "\n";
}



Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * op1, Ics_Opd * op2, Ics_Opd * res){
	op_desc = *machine_desc_object.spim_instruction_table[inst_op];

	opd1 = op1;
	opd2 = op2;
	result = res;
}
Compute_IC_Stmt & Compute_IC_Stmt::operator=(const Compute_IC_Stmt & rhs){
	op_desc = rhs.op_desc;
    opd1 = rhs.opd1;
 	opd2 = rhs.opd2;
	result = rhs.result;
	return *this;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Compute_IC_Stmt::get_opd1(){
	return opd1;
}
void Compute_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Compute_IC_Stmt::get_opd2(){
	return opd2;
}
void Compute_IC_Stmt::set_opd2(Ics_Opd * io){
	opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result(){
	return result;
}
void Compute_IC_Stmt::set_result(Ics_Opd * io){
	result = io;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer){
		file_buffer << "\t";

	op_desc.print_instruction_descriptor(file_buffer);
 	file_buffer << ":    \t";
 	result->print_ics_opd(file_buffer);
 	file_buffer << " <- ";
 	opd1->print_ics_opd(file_buffer);
 	if(opd2 != NULL){
 	file_buffer << " , ";
 	opd2->print_ics_opd(file_buffer);	
 	}
 	file_buffer << "\n";
}
void Compute_IC_Stmt::print_assembly(ostream & file_buffer){
		file_buffer << "\t" << op_desc.get_mnemonic();
	 	file_buffer << " ";
	 	
	 	if(result != NULL){
			result->print_asm_opd(file_buffer);
	 		file_buffer << ", ";
	 	}
	 	opd1->print_asm_opd(file_buffer);
	 	if(opd2 != NULL){
	 		file_buffer << ", ";
	 		opd2->print_asm_opd(file_buffer);	
	 	} 	
	 	file_buffer << "\n";
}



Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op inst_op, Ics_Opd * my_opd1, Ics_Opd * my_opd2,string mylabel,int size){
	op_desc =  *machine_desc_object.spim_instruction_table[inst_op];
	opd1 = my_opd1;
	opd2 = my_opd2;
	offset = mylabel;
}


Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs){
	opd1 = rhs.opd1;
	offset = rhs.offset;
	op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1(){
		return opd1;
}

void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io){
	opd1 = io;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd2(){
		return opd2;
}

void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io){
	opd2 = io;
}


string Control_Flow_IC_Stmt::get_Offset(){
	return offset;
}

void Control_Flow_IC_Stmt::set_Offset(string mylabel){
	offset = mylabel;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer){
		file_buffer << "\t";

	op_desc.print_instruction_descriptor(file_buffer);
 	file_buffer << ":    \t";
 	opd1->print_ics_opd(file_buffer);
 	file_buffer << " , zero : goto " << offset <<"\n";
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer << "\t";

	file_buffer << op_desc.get_mnemonic() ;
 	file_buffer << " ";
 	opd1->print_asm_opd(file_buffer);
 	file_buffer << ", $zero, " << offset <<" \n";
}



Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string mylabel){
	op_desc =  *machine_desc_object.spim_instruction_table[inst_op];

	label = mylabel;
}


Label_IC_Stmt & Label_IC_Stmt::operator=(const Label_IC_Stmt & rhs){
	label = rhs.label;
	op_desc = rhs.op_desc;
	return *this;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics(){
	return op_desc;
}

string Label_IC_Stmt::get_label(){
	return label;
}

void Label_IC_Stmt::set_label(string mylabel){
	label = mylabel;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer){
	cout << op_desc.get_op() << "\n";
	if(op_desc.get_op() == j){
		
	 	file_buffer << "\t";
	 	file_buffer << "goto " << this->label;
		}
	if(op_desc.get_op() == jal){
		
	 	file_buffer << "\t";
	 	file_buffer << "jal " << this->label;
	 	return;
		}
	else{
		file_buffer << "\n";
	 	file_buffer << this->label;		
	 	file_buffer << ":    \t";
	 	
	}
	file_buffer << "\n";
}
void Label_IC_Stmt::print_assembly(ostream & file_buffer){

		
	if(op_desc.get_op() == jal){
		
	 	file_buffer << "\t";
	 	file_buffer << "jal " << this->label << "\n";
	 	return;
		}

	if(op_desc.get_op() == j){
		
	 	file_buffer << "\t";
	 	file_buffer << "j " << this->label;
		}


	else if (op_desc.get_op() == bc1t){
		file_buffer << "\tbc1t ";
	 	file_buffer << this->label;		
	 	file_buffer << " ";
	}
	else if (op_desc.get_op() == bc1f){
		file_buffer << "\tbc1f ";
	 	file_buffer << this->label;		
	 	file_buffer << " ";

	}

	else {
		file_buffer << "\n";
	 	file_buffer << this->label;		
	 	file_buffer << ":    \t"; 	
	}

	file_buffer << "\n";	
}


//////////////////////Intermediate code for Ast statements ////////////////////////

Code_For_Ast::Code_For_Ast(){}


Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg){
	ics_list = ic_l;
	result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ics){
	ics_list.push_back(& ics);
}


list<Icode_Stmt *> & Code_For_Ast::get_icode_list(){
	return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg(){
	return result_register;			
}

void Code_For_Ast::set_reg(Register_Descriptor * reg){
	result_register = reg;
}

Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs){
	ics_list = rhs.ics_list;
	result_register = rhs.result_register;

	return *this;
}
