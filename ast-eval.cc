#include "ast.hh"
#include <stdlib.h>
template class Number_Ast<double>;
template class Number_Ast<int>;

int Ast::labelCounter;

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){

}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env){

}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){

}


Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){


	Data_Type dt = lhs->get_data_type();

	if(dt == int_data_type){
	Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
	if(eval_env.does_variable_exist(lhs->get_symbol_entry().get_variable_name())){
	eval_env.put_variable_value(rhs->evaluate(eval_env,file_buffer),lhs->get_symbol_entry().get_variable_name());	
			
			this->print(file_buffer);
			file_buffer << "\n" << VAR_SPACE << lhs->get_symbol_entry().get_variable_name() << " : " << lhs->get_value_of_evaluation(eval_env).get_int_value() << "\n\n";
			lhs->get_value_of_evaluation(eval_env).set_variable_status(true);
		return *(eval_int);
	}
	else{
		interpreter_global_table.put_variable_value(rhs->evaluate(eval_env,file_buffer),lhs->get_symbol_entry().get_variable_name());	
			
			this->print(file_buffer);
			file_buffer << "\n" << VAR_SPACE << lhs->get_symbol_entry().get_variable_name() << " : " << lhs->get_value_of_evaluation(interpreter_global_table).get_int_value() << "\n\n";
			lhs->get_value_of_evaluation(interpreter_global_table).set_variable_status(true);
		return *(eval_int);
	}
	}
	else if(dt = double_data_type){	
	Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();
	
	if(eval_env.does_variable_exist(lhs->get_symbol_entry().get_variable_name())){

	eval_env.put_variable_value(rhs->evaluate(eval_env,file_buffer),lhs->get_symbol_entry().get_variable_name());
				this->print(file_buffer);

				file_buffer << "\n" << VAR_SPACE << lhs->get_symbol_entry().get_variable_name() << " : " << lhs->get_value_of_evaluation(eval_env).get_double_value() << "\n\n";
				lhs->get_value_of_evaluation(eval_env).set_variable_status(true);

		return *(eval_double);
	}
	else{

	interpreter_global_table.put_variable_value(rhs->evaluate(eval_env,file_buffer),lhs->get_symbol_entry().get_variable_name());
				this->print(file_buffer);

				file_buffer << "\n" << VAR_SPACE << lhs->get_symbol_entry().get_variable_name() << " : " << lhs->get_value_of_evaluation(interpreter_global_table).get_double_value() << "\n\n";
				lhs->get_value_of_evaluation(interpreter_global_table).set_variable_status(true);

		return *(eval_double);

	}
	}

}


void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){

}
Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env){
	if(eval_env.does_variable_exist(this->get_symbol_entry().get_variable_name())){

	if(eval_env.get_variable_value(this->get_symbol_entry().get_variable_name())->is_variable_defined()){
		return  *(eval_env.get_variable_value(this->get_symbol_entry().get_variable_name()));
	}
	else{

       fprintf(stderr,"cs316: Error : File: , Line: %d: Variable not defined\n",lineno); 
        			exit(EXIT_FAILURE);
	}
	}

	else {
				return  *(interpreter_global_table.get_variable_value(this->get_symbol_entry().get_variable_name()));

	}
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
		if(eval_env.does_variable_exist(this->get_symbol_entry().get_variable_name())){

		eval_env.put_variable_value(result,this->get_symbol_entry().get_variable_name());

	}
	else{
				interpreter_global_table.put_variable_value(result,this->get_symbol_entry().get_variable_name());

	}

}
Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		return this->get_value_of_evaluation(eval_env);
}


template <class T>
Eval_Result & Number_Ast<T>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Data_Type dt = this->get_data_type();

	if(dt == int_data_type){
		Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
		eval_int->set_value(constant); 
		eval_int->set_result_enum(int_result);
		eval_int->set_variable_status(true);
		return *(eval_int);
	}
	else if(dt = double_data_type){	
		Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();
		eval_double->set_value(constant); 
		eval_double->set_result_enum(double_result);
		eval_double->set_variable_status(true);
		return *(eval_double);

	}
}


Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	Data_Type dt = this->get_data_type();

	if(dt == int_data_type){
		Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
		int value = lhs->evaluate(eval_env,file_buffer).get_int_value() + rhs->evaluate(eval_env,file_buffer).get_int_value();
		eval_int->set_value(value); 
		eval_int->set_result_enum(int_result);
		eval_int->set_variable_status(true);
		return *(eval_int);
	}
	else if(dt = double_data_type){	
		Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();
		double value = lhs->evaluate(eval_env,file_buffer).get_double_value() + rhs->evaluate(eval_env,file_buffer).get_double_value();
		eval_double->set_value(value); 
		eval_double->set_result_enum(double_result);
		eval_double->set_variable_status(true);
		return *(eval_double);
	}

}


Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	Data_Type dt = this->get_data_type();

	if(dt == int_data_type){
		Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
		int value = lhs->evaluate(eval_env,file_buffer).get_int_value() - rhs->evaluate(eval_env,file_buffer).get_int_value();
		eval_int->set_value(value); 
		eval_int->set_result_enum(int_result);
		eval_int->set_variable_status(true);
		return *(eval_int);
	}
	else if(dt = double_data_type){	
		Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();
		double value = lhs->evaluate(eval_env,file_buffer).get_double_value() - rhs->evaluate(eval_env,file_buffer).get_double_value();
		eval_double->set_value(value); 
		eval_double->set_result_enum(double_result);
		eval_double->set_variable_status(true);
		return *(eval_double);
	}

}

Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	Data_Type dt = this->get_data_type();

	if(dt == int_data_type){
		Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
		int value = lhs->evaluate(eval_env,file_buffer).get_int_value()*rhs->evaluate(eval_env,file_buffer).get_int_value();
		eval_int->set_value(value); 
		eval_int->set_result_enum(int_result);
		eval_int->set_variable_status(true);
		return *(eval_int);
	}
	else if(dt = double_data_type){	
		Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();
		double value = lhs->evaluate(eval_env,file_buffer).get_double_value()*rhs->evaluate(eval_env,file_buffer).get_double_value();
		eval_double->set_value(value); 
		eval_double->set_result_enum(double_result);
		eval_double->set_variable_status(true);
		return *(eval_double);
	}

}

Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	Data_Type dt = this->get_data_type();

	if(dt == int_data_type){
		Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
		if(rhs->evaluate(eval_env,file_buffer).get_int_value() == 0){

      		fprintf(stderr,"cs316: Error : File: , Line: %d: Division by zero error\n",lineno); 
        	exit(EXIT_FAILURE);
		}
		int value = lhs->evaluate(eval_env,file_buffer).get_int_value()/rhs->evaluate(eval_env,file_buffer).get_int_value();
		eval_int->set_value(value); 
		eval_int->set_result_enum(int_result);
		eval_int->set_variable_status(true);
		return *(eval_int);
	}
	else if(dt = double_data_type){	
		Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();
		if(rhs->evaluate(eval_env,file_buffer).get_double_value() == 0){

      		fprintf(stderr,"cs316: Error : File: , Line: %d: Division by zero error\n",lineno); 
        	exit(EXIT_FAILURE);
		}
		double value = lhs->evaluate(eval_env,file_buffer).get_double_value()/rhs->evaluate(eval_env,file_buffer).get_double_value();
		eval_double->set_value(value); 
		eval_double->set_result_enum(double_result);
		eval_double->set_variable_status(true);
		return *(eval_double);
	}

}


Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	Data_Type dt = this->get_data_type();

	if(dt == int_data_type){
		Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
		int value = -1*lhs->evaluate(eval_env,file_buffer).get_int_value() ;
		eval_int->set_value(value); 
		eval_int->set_result_enum(int_result);
		eval_int->set_variable_status(true);
		return *(eval_int);
	}
	else if(dt = double_data_type){	
		Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();
		double value = -1*lhs->evaluate(eval_env,file_buffer).get_double_value();
		eval_double->set_value(value); 
		eval_double->set_result_enum(double_result);
		eval_double->set_variable_status(true);
		return *(eval_double);
	}

}


Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	
	Data_Type dt = this->get_data_type();


	if(dt == int_data_type){
		Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();

		int value;


		if(cond->evaluate(eval_env,file_buffer).get_int_value() == 1){
			value = lhs->evaluate(eval_env,file_buffer).get_int_value();
		}
		else if(cond->evaluate(eval_env,file_buffer).get_int_value() == 0){
			value = rhs->evaluate(eval_env,file_buffer).get_int_value();
		}
		
		eval_int->set_value(value); 
		eval_int->set_result_enum(int_result);
		eval_int->set_variable_status(true);
		return *(eval_int);
	}
	else if(dt = double_data_type){	
		Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();

		double value;


		if(cond->evaluate(eval_env,file_buffer).get_double_value() == 1){
			value = lhs->evaluate(eval_env,file_buffer).get_double_value();
		}
		else if(cond->evaluate(eval_env,file_buffer).get_double_value() == 0){
			value = rhs->evaluate(eval_env,file_buffer).get_double_value();
		}
		
		eval_double->set_value(value); 
		eval_double->set_result_enum(double_result);
		eval_double->set_variable_status(true);
		return *(eval_double);
	}

}


Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

}

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

		Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
		eval_int->set_result_enum(int_result);
		eval_int->set_variable_status(true);

		if(rel_op == less_equalto){
			if(this->get_data_type() == int_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_int_value() <= rhs_condition->evaluate(eval_env,file_buffer).get_int_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
			if(this->get_data_type() == double_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_double_value() <= rhs_condition->evaluate(eval_env,file_buffer).get_double_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
		}


		if(rel_op == less_than){
			if(this->get_data_type() == int_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_int_value() < rhs_condition->evaluate(eval_env,file_buffer).get_int_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
			if(this->get_data_type() == double_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_double_value() < rhs_condition->evaluate(eval_env,file_buffer).get_double_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
		}

		if(rel_op == greater_than){
			if(this->get_data_type() == int_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_int_value() > rhs_condition->evaluate(eval_env,file_buffer).get_int_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
			if(this->get_data_type() == double_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_double_value() > rhs_condition->evaluate(eval_env,file_buffer).get_double_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
		}


		if(rel_op == greater_equalto){
			if(this->get_data_type() == int_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_int_value() >= rhs_condition->evaluate(eval_env,file_buffer).get_int_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
			if(this->get_data_type() == double_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_double_value() >= rhs_condition->evaluate(eval_env,file_buffer).get_double_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
		}


		if(rel_op == equalto){
			if(this->get_data_type() == int_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_int_value() == rhs_condition->evaluate(eval_env,file_buffer).get_int_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
			if(this->get_data_type() == double_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_double_value() == rhs_condition->evaluate(eval_env,file_buffer).get_double_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
		}


		if(rel_op == not_equalto){
			if(this->get_data_type() == int_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_int_value() != rhs_condition->evaluate(eval_env,file_buffer).get_int_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
			if(this->get_data_type() == double_data_type){
				if(lhs_condition->evaluate(eval_env,file_buffer).get_double_value() != rhs_condition->evaluate(eval_env,file_buffer).get_double_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
			}
		}

}








Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

		Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
		eval_int->set_result_enum(int_result);
		eval_int->set_variable_status(true);

		if(bool_op == _logical_or){
				if(lhs_op->evaluate(eval_env,file_buffer).get_int_value() || rhs_op->evaluate(eval_env,file_buffer).get_int_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
		}

		if(bool_op == _logical_and){
				if(lhs_op->evaluate(eval_env,file_buffer).get_int_value() && rhs_op->evaluate(eval_env,file_buffer).get_int_value()){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
		}

		if(bool_op == _logical_not){
				if(!(rhs_op->evaluate(eval_env,file_buffer).get_int_value())){
					eval_int->set_value(1);
					return *(eval_int);
				}
				else{
					eval_int->set_value(0);
					return *(eval_int);
				}
		}
}


Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(cond->evaluate(eval_env,file_buffer).get_int_value() == 1){
		return then_part->evaluate(eval_env,file_buffer);
	}
	if(else_part == NULL){
			return *new Eval_Result_Value_Int();
		}
	if(cond->evaluate(eval_env,file_buffer).get_int_value() == 0){
		return else_part->evaluate(eval_env,file_buffer);
	}
}




Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(is_do_form){
		do {
					body->evaluate(eval_env,file_buffer);
		}
	while(cond->evaluate(eval_env,file_buffer).get_int_value() == 1);
	}

	else{
	while(cond->evaluate(eval_env,file_buffer).get_int_value() == 1){
		body->evaluate(eval_env,file_buffer);
	}
	}
	Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
 
	return *eval_int;
}


Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	list<Ast *>::iterator it;

	for(it = statement_list.begin();it != statement_list.end();it++){
		(*it)->evaluate(eval_env,file_buffer);
	}
	Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
 
	return *eval_int;
}

Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

}


