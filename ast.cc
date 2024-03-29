#include "ast.hh"
#include <stdlib.h>
template class Number_Ast<double>;
template class Number_Ast<int>;

Ast::Ast(){
	
}

Ast::~Ast(){
	
}

Data_Type Ast::get_data_type(){
	return node_data_type;
}

void Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Ast::is_value_zero(){
	return false;
}

bool Ast::check_ast(){
	//return false;
}

Symbol_Table_Entry & Ast::get_symbol_entry(){

}

void Ast::print(ostream & file_buffer){
}

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	lhs = temp_lhs;
	rhs = temp_rhs;
	lineno = line;
	ast_num_child = binary_arity;
	this->set_data_type(lhs->get_data_type());

}

Assignment_Ast::~Assignment_Ast(){

}

bool Assignment_Ast::check_ast(){
	
	// << lhs->get_data_type() << " " << rhs->get_data_type() << "\n";
	//cout << lhs->get_data_type() << rhs->get_data_type();
	if(lhs->get_data_type() != rhs->get_data_type()){
		//error
       fprintf(stderr,"cs316: Error : File: , Line: %d: Assignment statement data type not compatible\n",lineno); 
        			exit(EXIT_FAILURE);
	}
	return true;
}

void Assignment_Ast::print(ostream & file_buffer){
		file_buffer << "\n         Asgn:\n            LHS (";
		lhs->print(file_buffer);
		file_buffer << ")\n            RHS (";
		rhs->print(file_buffer);
		file_buffer << ")";
}



Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
	variable_symbol_entry = &var_entry;
	//ast_num_child = 1;
	lineno = line;
	Data_Type dt = variable_symbol_entry->get_data_type();
	this->set_data_type(dt);
}

Name_Ast::~Name_Ast(){

}

Data_Type Name_Ast::get_data_type(){
	return variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
	return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Data_Type dt){
	variable_symbol_entry->set_data_type(dt);
}

void Name_Ast::print(ostream & file_buffer){
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}



template <class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){

	constant = number;
	node_data_type = constant_data_type;
	lineno = line;
}

template <class T>
Number_Ast<T>::~Number_Ast(){

}

template <class T>
Data_Type Number_Ast<T>::get_data_type(){
	return node_data_type;
}

template <class T>
void Number_Ast<T>::set_data_type(Data_Type dt){
	node_data_type = dt;
}

template <class T>
bool Number_Ast<T>::is_value_zero(){
	if(constant == 0){
		return true;
	}
	return false;
}


template <class T>
void Number_Ast<T>::print(ostream & file_buffer){
	file_buffer << "Num : " << constant;

}



Data_Type Arithmetic_Expr_Ast::get_data_type(){
	return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast(){
	
	if(lhs->get_data_type() != rhs->get_data_type()){
		//error
        fprintf(stderr,"cs316: Error : File: , Line: %d: Arithmetic statement data type not compatible\n",lineno); 
        exit(EXIT_FAILURE);

    }
    return true;
}

void Arithmetic_Expr_Ast::print(ostream & file_buffer){

}


Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
	ast_num_child = binary_arity;
	//this->check_ast();
	Data_Type dt =  l->get_data_type();

	this->set_data_type(dt);
	//cout << this->get_data_type() << "\n";

}




void Plus_Ast::print(ostream & file_buffer){
		file_buffer << "\n            Arith: PLUS\n";
		file_buffer << "               LHS (";
		lhs->print(file_buffer);
		file_buffer << ")\n               RHS (";
		rhs->print(file_buffer);
		file_buffer << ")";
}



Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
		//this->check_ast();

	Data_Type dt =  l->get_data_type();
	ast_num_child = binary_arity;

	this->set_data_type(dt);
}



void Minus_Ast::print(ostream & file_buffer){
 		file_buffer << "\n            Arith: MINUS\n";
		file_buffer << "               LHS (";
		lhs->print(file_buffer);
		file_buffer << ")\n               RHS (";
		rhs->print(file_buffer);
		file_buffer << ")";
}


Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
		//this->check_ast();

	Data_Type dt =  l->get_data_type();
		ast_num_child = binary_arity;

	this->set_data_type(dt);
}



void Divide_Ast::print(ostream & file_buffer){
		file_buffer << "\n            Arith: DIV\n";
		file_buffer << "               LHS (";
		lhs->print(file_buffer);
		file_buffer << ")\n               RHS (";
		rhs->print(file_buffer);
		file_buffer << ")";
}


Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line){
	lhs = l;
	rhs = r;
	lineno = line;
		//this->check_ast();

	Data_Type dt =  l->get_data_type();
	ast_num_child = binary_arity;

	this->set_data_type(dt);
}




void Mult_Ast::print(ostream & file_buffer){
	 	file_buffer << "\n            Arith: MULT\n";
		file_buffer << "               LHS (";
		lhs->print(file_buffer);
		file_buffer << ")\n               RHS (";
		rhs->print(file_buffer);
		file_buffer << ")";
}


UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
	lhs = l;
//	rhs = r;
	lineno = line;
	Data_Type dt = l->get_data_type();
	ast_num_child = unary_arity;

	this->set_data_type(dt);
}



void UMinus_Ast::print(ostream & file_buffer){
		file_buffer << "\n            Arith: UMINUS\n";
		file_buffer << "               LHS (";
		lhs->print(file_buffer);
		file_buffer << ")";
}


Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* mycond, Ast* l, Ast* r, int line){
	lhs = l;
	rhs = r;
	cond = mycond;
	lineno = line;
	Data_Type dt =  l->get_data_type();
	ast_num_child = ternary_arity;

	this->set_data_type(dt);
}


void Conditional_Expression_Ast::print(ostream & file_buffer){
		file_buffer << "\n         Cond:\n            IF_ELSE";
		cond->print(file_buffer);
		file_buffer << "\n            LHS (";
		lhs->print(file_buffer);
		file_buffer << ")\n            RHS (";
		rhs->print(file_buffer);
		file_buffer << ")";
}


Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line){
	lhs_condition = lhs;
	rhs_condition = rhs;
	rel_op = rop;
	lineno = line;
	Data_Type dt = lhs->get_data_type();
	this->set_data_type(dt);
}
Relational_Expr_Ast::~Relational_Expr_Ast(){

}

Data_Type Relational_Expr_Ast::get_data_type(){
	return node_data_type;
}
void Relational_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast(){

	if(lhs_condition->get_data_type() != rhs_condition->get_data_type()){
        fprintf(stderr,"cs316: Error : File: , Line: %d: Relational_Op statement data type not compatible\n",lineno); 
        exit(EXIT_FAILURE);

    }
    return true;
}

void Relational_Expr_Ast::print(ostream & file_buffer){
		if(rel_op == less_equalto){
			 file_buffer << "\n            Condition: LE\n";
		}
		if(rel_op == less_than)
					 file_buffer << "\n            Condition: LT\n";

		if(rel_op == greater_than)
					 file_buffer << "\n            Condition: GT\n";

		if(rel_op == greater_equalto)
					 file_buffer << "\n            Condition: GE\n";

		if(rel_op == equalto)
					 file_buffer << "\n            Condition: EQ\n";

		if(rel_op == not_equalto)
					 file_buffer << "\n            Condition: NE\n";



		file_buffer << "               LHS (";
		lhs_condition->print(file_buffer);
		file_buffer << ")\n               RHS (";
		rhs_condition->print(file_buffer);
		file_buffer << ")";
}


Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line){
	if(bop != _logical_not){
	lhs_op = lhs;} 
	rhs_op = rhs;
	bool_op = bop;
	lineno = line;

	this->set_data_type(void_data_type);
}
Logical_Expr_Ast::~Logical_Expr_Ast(){

}

Data_Type Logical_Expr_Ast::get_data_type(){
	return node_data_type;

}
void Logical_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Logical_Expr_Ast::check_ast(){
	// if(bool_op == _logical_not) {
	// 	return true;
	// }
	// if(lhs_op->get_data_type() != rhs_op->get_data_type()){
 //        fprintf(stderr,"cs316: Error : File: , Line: %d: Logical statement data type not compatible\n",lineno); 
 //        exit(EXIT_FAILURE);

 //    }
    return true;
}


void Logical_Expr_Ast::print(ostream & file_buffer){


		if(bool_op == _logical_not){
		file_buffer << "\n            Condition: NOT\n";
		file_buffer << "               RHS (";
		rhs_op->print(file_buffer);
		file_buffer << ")";
		return;
		}
		if(bool_op == _logical_or)
					 file_buffer << "\n            Condition: OR\n";

		if(bool_op == _logical_and)
					 file_buffer << "\n            Condition: AND\n";

	

		file_buffer << "               LHS (";
		lhs_op->print(file_buffer);
		file_buffer << ")\n               RHS (";
		rhs_op->print(file_buffer);
		file_buffer << ")";
}


Selection_Statement_Ast::Selection_Statement_Ast(Ast * cnd,Ast* t_part, Ast* e_part, int line){
	then_part = t_part;
	else_part = e_part;
	cond = cnd;
	lineno = line;
	this->set_data_type(void_data_type);

}
Selection_Statement_Ast::~Selection_Statement_Ast(){

}

Data_Type Selection_Statement_Ast::get_data_type(){
	return node_data_type;

}
void Selection_Statement_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast(){

}

void Selection_Statement_Ast::print(ostream & file_buffer){

	file_buffer << "\n         IF : \n         CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")\n         THEN (";
		then_part->print(file_buffer);
			file_buffer << ")";	

		if(else_part == NULL){
			return;
		}
		file_buffer << "\n         ELSE (";
		else_part->print(file_buffer);
		file_buffer << ")";

}


Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cnd, Ast* bdy, int line, bool do_form){
	cond = cnd;
	body = bdy;
	is_do_form = do_form;
	lineno = line;
	this->set_data_type(void_data_type);

}
Iteration_Statement_Ast::~Iteration_Statement_Ast(){

}

Data_Type Iteration_Statement_Ast::get_data_type(){
	return node_data_type;

}
void Iteration_Statement_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;

}

bool Iteration_Statement_Ast::check_ast(){

}

void Iteration_Statement_Ast::print(ostream & file_buffer){

	if(is_do_form){
		file_buffer << "\n         DO (";
		body->print(file_buffer);
		file_buffer << ")\n         WHILE CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")";
	}
		else{
	file_buffer << "\n         WHILE : \n         CONDITION (";
		cond->print(file_buffer);
		file_buffer << ")\n         BODY (";
		body->print(file_buffer);
		
		file_buffer << ")";}
}



Sequence_Ast::Sequence_Ast(int line){
	lineno = line;
}
Sequence_Ast::~Sequence_Ast(){

}
void Sequence_Ast::ast_push_back(Ast * ast){
	statement_list.push_back(ast);
}
void Sequence_Ast::print(ostream & file_buffer){
		list<Ast *>::iterator it;

		for (it = statement_list.begin();it != statement_list.end(); it++)
		{  file_buffer << "\n            ";
			(*it)->print(file_buffer);

		}

}


Print_Ast::Print_Ast(Ast *v, int line){

	var = v;

	lineno = line;

}

Print_Ast::~Print_Ast(){

}

void Print_Ast::print(ostream & file_buffer){
		file_buffer << "\n         Print :\n               (";
		var->print(file_buffer);
		file_buffer << ")";
}

Call_Ast::Call_Ast(string name, int line){
	procedure_name = name;
	lineno = line;
}


Call_Ast::~Call_Ast(){

}


Data_Type Call_Ast::get_data_type(){

 return	program_object.get_procedure_prototype(procedure_name)->get_return_type();
}

void Call_Ast::set_register(Register_Descriptor * reg){
	return_value_reg = reg;
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list){
	list<Symbol_Table_Entry *> curr_list = formal_param_list.get_table();
	

	if(curr_list.size() != actual_param_list.size()){
      fprintf(stderr,"cs316: Error : File: , Line: %d: num argument not equal\n",lineno); 
        			exit(EXIT_FAILURE);	
        		}

	list<Symbol_Table_Entry *>::iterator it;
	list<Ast *>::iterator ast_it;
	ast_it = actual_param_list.begin();

			for (it = curr_list.begin(); it != curr_list.end(); it++){
					if((*it)->get_data_type() != (*ast_it)->get_data_type()){
      fprintf(stderr,"cs316: Error : File: , Line: %d:  argument type does not match\n",lineno); 
        			exit(EXIT_FAILURE);					
        		} 
					ast_it++;
			}
}

void Call_Ast::set_actual_param_list(list<Ast *> & param_list){
	actual_param_list = param_list;
}

void Call_Ast::print(ostream & file_buffer){
	file_buffer << "\n         FN_CALL: " << procedure_name << "(\n            ";
	list<Ast *>::iterator it;

		for (it = actual_param_list.begin();it != actual_param_list.end(); it++)
		{  
			(*it)->print(file_buffer);
			file_buffer<<"\n";
		}
		file_buffer << ")";
}
	

Return_Ast::Return_Ast(Ast * ret_val, string name, int line){
	return_value = ret_val;
	proc_name = name;
	lineno = line;
}
Return_Ast::~Return_Ast(){

}

Data_Type Return_Ast::get_data_type(){

	 return	program_object.get_procedure_prototype(proc_name)->get_return_type();

}

void Return_Ast::print(ostream & file_buffer){
	file_buffer << "\n         RETURN ";
	return_value->print(file_buffer);
	file_buffer << "\n";

}