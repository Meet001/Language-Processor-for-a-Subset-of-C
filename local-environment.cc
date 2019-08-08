#include "local-environment.hh"


void Symbol_Table::create(Local_Environment& eval_env){
	list<Symbol_Table_Entry *>::iterator it;

	for (it = variable_table.begin();it != variable_table.end();it++)
	{
		if((*it)->get_symbol_scope() == global){
			if((*it)->get_data_type() == int_data_type){
				Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
				eval_int->set_value(0);
				eval_int->set_variable_status(true);
				eval_int->set_result_enum(int_result);
				eval_env.put_variable_value(*eval_int,(*it)->get_variable_name());

			}
			if((*it)->get_data_type() == double_data_type){
				Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();
				eval_double->set_value(0.0);
				eval_double->set_variable_status(true);
				eval_double->set_result_enum(double_result);
				eval_env.put_variable_value(*eval_double,(*it)->get_variable_name());
			}
		}
		if((*it)->get_symbol_scope() == local){
			if((*it)->get_data_type() == int_data_type){
				Eval_Result_Value_Int * eval_int = new Eval_Result_Value_Int();
				eval_int->set_variable_status(false);
				eval_int->set_result_enum(int_result);
				eval_env.put_variable_value(*eval_int,(*it)->get_variable_name());
			}
			if((*it)->get_data_type() == double_data_type){
				Eval_Result_Value_Double * eval_double = new Eval_Result_Value_Double();
				eval_double->set_variable_status(false);
				eval_double->set_result_enum(double_result);
				eval_env.put_variable_value(*eval_double,(*it)->get_variable_name());
			}
		}
	}
}

Local_Environment::Local_Environment(){

}

Local_Environment::~Local_Environment(){

}



void Local_Environment::print(ostream & file_buffer){
		map<string, Eval_Result *>::iterator it;

		for (it = variable_table.begin(); it != variable_table.end() ; it++)
		{	
			if(! (it->second->is_variable_defined())){
			file_buffer << VAR_SPACE << it->first << " : undefined" << "\n";
			continue;
			}
			if(it->second->get_result_enum() == int_result){
			file_buffer << VAR_SPACE << it->first << " : " << it->second->get_int_value() << "\n";
			continue;
			}
			if(it->second->get_result_enum() == double_result){
			file_buffer << VAR_SPACE << it->first << " : " << it->second->get_double_value() << "\n";
			continue;
			}
		}


}


bool Local_Environment::is_variable_defined(string name){
		map<string, Eval_Result *>::iterator it;

		for (it = variable_table.begin(); it != variable_table.end() ; it++)
		{
			if(name == it->first){
				return it->second->is_variable_defined();
			}
		}

		return false;
}


Eval_Result * Local_Environment::get_variable_value(string name){

	map<string, Eval_Result *>::iterator it;

		for (it = variable_table.begin(); it != variable_table.end() ; it++)
		{
			if(name == it->first){
				return it->second;
			}
		}

		return NULL;

}


void Local_Environment::put_variable_value(Eval_Result & value, string name){

	map<string, Eval_Result *>::iterator it;
	int is_puted = 0;
		for (it = variable_table.begin(); it != variable_table.end() ; it++)
		{
			if(name == it->first){
			variable_table.at(name) = &value;

				is_puted = 1;
				break;
			}
		}

		if(is_puted == 0){
			variable_table[name] = &value;
		}


}


bool Local_Environment::does_variable_exist(string name){

	map<string, Eval_Result *>::iterator it;

		for (it = variable_table.begin(); it != variable_table.end() ; it++)
		{
			if(name == it->first){
				return true;
			}
		}

		return false;
}



Eval_Result_Value_Double::Eval_Result_Value_Double(){
	defined = false;
}
Eval_Result_Value_Double::~Eval_Result_Value_Double(){

}

void Eval_Result_Value_Double::set_value(double number){
	value = number;
}

void Eval_Result_Value_Double::set_value(int number){
	value = number;
}
double Eval_Result_Value_Double::get_double_value(){
	return value;
}

void Eval_Result_Value_Double::set_variable_status(bool def){
	defined = def;
}
bool Eval_Result_Value_Double::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res){
	result_type = res;
}
Result_Enum Eval_Result_Value_Double::get_result_enum(){
	return result_type;
}






Eval_Result_Value_Int::Eval_Result_Value_Int(){
	defined = false;
}
Eval_Result_Value_Int::~Eval_Result_Value_Int(){

}

void Eval_Result_Value_Int::set_value(double number){
	value = number;
}

void Eval_Result_Value_Int::set_value(int number){
	value = number;
}
int Eval_Result_Value_Int::get_int_value(){
	return value;
}

void Eval_Result_Value_Int::set_variable_status(bool def){
	defined = def;
}
bool Eval_Result_Value_Int::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res){
	result_type = res;
}
Result_Enum Eval_Result_Value_Int::get_result_enum(){
	return result_type;
}



void Eval_Result_Value::set_value(double number){
}

void Eval_Result_Value::set_value(int number){
}

int Eval_Result_Value::get_int_value(){
}

double Eval_Result_Value::get_double_value(){
}



void Eval_Result::set_value(double number){
}

void Eval_Result::set_value(int number){
}

int Eval_Result::get_int_value(){
}

double Eval_Result::get_double_value(){
}

void Eval_Result::set_variable_status(bool def){

}


bool Eval_Result::is_variable_defined(){
}

void Eval_Result::set_result_enum(Result_Enum res){
}