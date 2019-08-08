#include "ast.hh"
template class Number_Ast<double>;
template class Number_Ast<int>;

int decide_torf;


Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){

}

Code_For_Ast & Assignment_Ast::compile(){
	// printf("lol\n");
	Code_For_Ast * rhs_compile_result = new Code_For_Ast();

	*rhs_compile_result = rhs->compile();

	Mem_Addr_Opd *my_opd = new Mem_Addr_Opd(lhs->get_symbol_entry());

	Register_Addr_Opd *register_opd = new Register_Addr_Opd(rhs_compile_result->get_reg());
	Move_IC_Stmt* move_stmt;
	if(lhs->get_data_type() == int_data_type)
		move_stmt = new Move_IC_Stmt(store,register_opd,my_opd); // check order
	if(lhs->get_data_type() == double_data_type)
		move_stmt = new Move_IC_Stmt(store_d,register_opd,my_opd); // check order

	rhs_compile_result->append_ics(*move_stmt);

	rhs_compile_result->get_reg()->reset_use_for_expr_result();
	// printf("lol1\n");

	return *rhs_compile_result;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Name_Ast::compile(){

	Data_Type dt = this->get_data_type();
	Register_Descriptor * new_reg = new Register_Descriptor();
	if(dt == int_data_type){

		new_reg =  machine_desc_object.get_new_register<int_reg>();

	}

	if(dt == double_data_type){

		new_reg =  machine_desc_object.get_new_register<float_reg>();

	}

	Register_Addr_Opd *register_opd = new Register_Addr_Opd(new_reg);

	// Symbol_Table_Entry entry = this->get_symbol_entry();

	Mem_Addr_Opd *my_opd = new Mem_Addr_Opd(this->get_symbol_entry());


	Move_IC_Stmt* move_stmt;
	if(this->get_data_type() == int_data_type)
		move_stmt = new Move_IC_Stmt(load,my_opd,register_opd); // check order
	if(this->get_data_type() == double_data_type)
		move_stmt = new Move_IC_Stmt(load_d,my_opd,register_opd); // check order

	Code_For_Ast * mycode = new  Code_For_Ast();
	mycode->append_ics(*move_stmt);

	mycode->set_reg(new_reg);

	return *mycode;

}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){

}


template <class T>
Code_For_Ast & Number_Ast<T>::compile(){
	//printf("xd\n");
	Data_Type dt = this->get_data_type();
	Register_Descriptor * new_reg = new Register_Descriptor();

	if(dt == int_data_type){
		new_reg =  machine_desc_object.get_new_register<int_reg>();
		Const_Opd<int> *my_opd = new Const_Opd<int>(constant);
		Register_Addr_Opd *register_opd = new Register_Addr_Opd(new_reg);
		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(imm_load ,  my_opd,register_opd); //check order
	//	move_stmt->print_icode(std::cout);

		Code_For_Ast * mycode = new  Code_For_Ast();
		mycode->append_ics(*move_stmt);
		mycode->set_reg(new_reg);

		return *mycode;

	}

	if(dt == double_data_type){

		new_reg =  machine_desc_object.get_new_register<float_reg>();
		Const_Opd<double> *my_opd = new Const_Opd<double>(constant);
		Register_Addr_Opd *register_opd = new Register_Addr_Opd(new_reg);
		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(imm_load_d ,  my_opd,register_opd); //check order

		Code_For_Ast * mycode = new  Code_For_Ast();
		mycode->append_ics(*move_stmt);

		mycode->set_reg(new_reg);

		return *mycode;

	}
}

template <class T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Plus_Ast::compile(){

	Code_For_Ast * lhs_compile_result = new Code_For_Ast();

	*lhs_compile_result = lhs->compile();

	Code_For_Ast * rhs_compile_result = new Code_For_Ast();

	*rhs_compile_result = rhs->compile();

	Data_Type dt = lhs->get_data_type();

	Register_Descriptor * new_reg = new Register_Descriptor();
	if(dt == int_data_type){

		new_reg =  machine_desc_object.get_new_register<int_reg>();

	}

	if(dt == double_data_type){

		new_reg =  machine_desc_object.get_new_register<float_reg>();

	}

	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = rhs_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		lhs_compile_result->append_ics((**it));
	}

	Register_Addr_Opd *lhs_opd = new Register_Addr_Opd(lhs_compile_result->get_reg());

	Register_Addr_Opd *rhs_opd = new Register_Addr_Opd(rhs_compile_result->get_reg());

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(new_reg);


	Compute_IC_Stmt * compute_stmt;
	if(lhs->get_data_type() == int_data_type)
		compute_stmt =  new Compute_IC_Stmt(add,lhs_opd,rhs_opd,result_opd); 
	if(lhs->get_data_type() == double_data_type)
		compute_stmt =  new Compute_IC_Stmt(add_d,lhs_opd,rhs_opd,result_opd); 

	lhs_compile_result->append_ics(*compute_stmt);

	lhs_compile_result->get_reg()->reset_use_for_expr_result();

	rhs_compile_result->get_reg()->reset_use_for_expr_result();

	lhs_compile_result->set_reg(new_reg);

	return *lhs_compile_result;

}


Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}	


Code_For_Ast & Conditional_Expression_Ast::compile(){

	Code_For_Ast * cond_compile_result = new Code_For_Ast();

	*cond_compile_result = cond->compile();

	string label1 = get_new_label();
	string label2 = get_new_label();


	Register_Addr_Opd *register_opd = new Register_Addr_Opd(cond_compile_result->get_reg());
                                                                                                                                   
	Control_Flow_IC_Stmt * control_stmt =  new Control_Flow_IC_Stmt(beq, register_opd ,NULL ,label1); 



	cond_compile_result->append_ics(*control_stmt);

	Code_For_Ast * lhs_compile_result = new Code_For_Ast();

	*lhs_compile_result = lhs->compile();	


	Code_For_Ast * rhs_compile_result = new Code_For_Ast();

	*rhs_compile_result = rhs->compile();	



	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = lhs_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		cond_compile_result->append_ics((**it));
	}


	Register_Addr_Opd *lhs_opd = new Register_Addr_Opd(lhs_compile_result->get_reg());

	Register_Descriptor * new_reg = machine_desc_object.get_new_register<int_reg>();

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(new_reg);

	Register_Addr_Opd * zero_reg = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);

	Compute_IC_Stmt * compute_stmt = new  Compute_IC_Stmt(or_t,lhs_opd ,zero_reg , result_opd); 

	cond_compile_result->append_ics(*compute_stmt);

	Label_IC_Stmt * label_stmt =  new Label_IC_Stmt(j ,label2); 

	cond_compile_result->append_ics(*label_stmt);

	Label_IC_Stmt * label_stmt1 =  new Label_IC_Stmt(label ,label1); 

	cond_compile_result->append_ics(*label_stmt1);


	
	list<Icode_Stmt *> rhslist = rhs_compile_result->get_icode_list();

	for(it = rhslist.begin(); it != rhslist.end(); it++){
		cond_compile_result->append_ics((**it));
	}

	Register_Addr_Opd *rhs_opd = new Register_Addr_Opd(rhs_compile_result->get_reg());

	Compute_IC_Stmt * compute_stmt1 = new  Compute_IC_Stmt(or_t,rhs_opd ,zero_reg , result_opd); 

	cond_compile_result->append_ics(*compute_stmt1);

	Label_IC_Stmt * label_stmt2 =  new Label_IC_Stmt(label ,label2); 

	cond_compile_result->append_ics(*label_stmt2);

	cond_compile_result->get_reg()->reset_use_for_expr_result();

	lhs_compile_result->get_reg()->reset_use_for_expr_result();

	rhs_compile_result->get_reg()->reset_use_for_expr_result();



	cond_compile_result->set_reg(new_reg);


	return *cond_compile_result;
	
}



Code_For_Ast & Selection_Statement_Ast::compile(){

	Code_For_Ast * cond_compile_result = new Code_For_Ast();

	*cond_compile_result = cond->compile();

	string label1 = get_new_label();
	string label2;
	Register_Addr_Opd *register_opd = new Register_Addr_Opd(cond_compile_result->get_reg());

	if(cond->get_data_type() == int_data_type){                                                                                                                                   
		Control_Flow_IC_Stmt * control_stmt =  new Control_Flow_IC_Stmt(beq, register_opd ,NULL ,label1); 

		cond_compile_result->append_ics(*control_stmt);
	}

	if(cond->get_data_type() == double_data_type){     
		if(decide_torf == 1){
			Label_IC_Stmt * control_stmt =  new Label_IC_Stmt(bc1t, label1); 
			cond_compile_result->append_ics(*control_stmt);
		}
		if(decide_torf == 2){
			Label_IC_Stmt * control_stmt =  new Label_IC_Stmt(bc1f, label1); 
			cond_compile_result->append_ics(*control_stmt);
		}
	}
	
	Code_For_Ast * then_compile_result = new Code_For_Ast();

	*then_compile_result = then_part->compile();	

	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = then_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		cond_compile_result->append_ics((**it));
	}

	if (else_part != NULL){
		label2 = get_new_label();
		Label_IC_Stmt * label_stmt1 =  new Label_IC_Stmt(j ,label2); 

		cond_compile_result->append_ics(*label_stmt1);
	}

	Label_IC_Stmt * label_stmt =  new Label_IC_Stmt(label ,label1); 

	cond_compile_result->append_ics(*label_stmt);

	if (else_part == NULL)
	{	
		cond_compile_result->get_reg()->reset_use_for_expr_result();

		return *cond_compile_result;
	}

	Code_For_Ast * else_compile_result = new Code_For_Ast();

	*else_compile_result = else_part->compile();	

	// list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> else_list = else_compile_result->get_icode_list();


	for(it = else_list.begin(); it != else_list.end(); it++){
		cond_compile_result->append_ics((**it));
	}

	Label_IC_Stmt * label_stmt2 =  new Label_IC_Stmt(label ,label2); 

	cond_compile_result->append_ics(*label_stmt2);

	cond_compile_result->get_reg()->reset_use_for_expr_result();

	return *cond_compile_result;
	
}


Code_For_Ast & Iteration_Statement_Ast::compile(){


	Code_For_Ast * iteration_result = new Code_For_Ast();

	Code_For_Ast * body_compile_result = new Code_For_Ast();


	Code_For_Ast * cond_compile_result = new Code_For_Ast();

	*cond_compile_result = cond->compile();
	int store = decide_torf;
	*body_compile_result = body->compile();

	string label1 = get_new_label();
	string label2 = get_new_label();

	if(is_do_form == 0){
		Label_IC_Stmt * label_stmt =  new Label_IC_Stmt(j ,label2); 

		iteration_result->append_ics(*label_stmt);
	}

	Label_IC_Stmt * label_stmt1 =  new Label_IC_Stmt(label ,label1); 

	iteration_result->append_ics(*label_stmt1);

	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = body_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		iteration_result->append_ics((**it));
	}

	Label_IC_Stmt * label_stmt2 =  new Label_IC_Stmt(label ,label2); 

	iteration_result->append_ics(*label_stmt2);

	list<Icode_Stmt *> condlist = cond_compile_result->get_icode_list();

	for(it = condlist.begin(); it != condlist.end(); it++){
		iteration_result->append_ics((**it));
	}


	Register_Addr_Opd *register_opd = new Register_Addr_Opd(cond_compile_result->get_reg());
                                                                                                                                   
	// Control_Flow_IC_Stmt * control_stmt =  new Control_Flow_IC_Stmt(bne, register_opd ,label1); 

	// iteration_result->append_ics(*control_stmt);

	if(cond->get_data_type() == int_data_type){                                                                                                                                   
		Control_Flow_IC_Stmt * control_stmt =  new Control_Flow_IC_Stmt(bne, register_opd ,NULL ,label1); 

		iteration_result->append_ics(*control_stmt);
	}

	if(cond->get_data_type() == double_data_type){                       
		if(store == 1){
			Label_IC_Stmt * control_stmt =  new Label_IC_Stmt(bc1f, label1); 
			iteration_result->append_ics(*control_stmt);
		}                                                                                                            
		if(store == 2){
			Label_IC_Stmt * control_stmt =  new Label_IC_Stmt(bc1t, label1); 
			iteration_result->append_ics(*control_stmt);
		}       
	}

	cond_compile_result->get_reg()->reset_use_for_expr_result();

	return *iteration_result;
	
}




Code_For_Ast & Sequence_Ast::compile(){

	Code_For_Ast * compile_result = new Code_For_Ast();

	list<Ast *>::iterator it;

	Code_For_Ast * temp_compile_result = new Code_For_Ast();

	for(it = statement_list.begin();it != statement_list.end();it++){
		
		*temp_compile_result = (*it)->compile();

		list<Icode_Stmt *>::iterator stmt_it;
		
		list<Icode_Stmt *> mylist = temp_compile_result->get_icode_list();

		for(stmt_it = mylist.begin(); stmt_it != mylist.end(); stmt_it++){
			compile_result->append_ics((**stmt_it));
		}

	}

	return *compile_result;

}






// ===========================================================================================












// =============================================================================================









Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & Lra){

}


Code_For_Ast & Minus_Ast::compile(){

	Code_For_Ast * lhs_compile_result = new Code_For_Ast();

	*lhs_compile_result = lhs->compile();

	Code_For_Ast * rhs_compile_result = new Code_For_Ast();

	*rhs_compile_result = rhs->compile();

	Data_Type dt = lhs->get_data_type();

	Register_Descriptor * new_reg = new Register_Descriptor();
	if(dt == int_data_type){

		new_reg =  machine_desc_object.get_new_register<int_reg>();

	}

	if(dt == double_data_type){

		new_reg =  machine_desc_object.get_new_register<float_reg>();

	}

	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = rhs_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		lhs_compile_result->append_ics((**it));
	}

	Register_Addr_Opd *lhs_opd = new Register_Addr_Opd(lhs_compile_result->get_reg());

	Register_Addr_Opd *rhs_opd = new Register_Addr_Opd(rhs_compile_result->get_reg());

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(new_reg);

	// Compute_IC_Stmt(sub,lhs_opd,rhs_opd,result_opd); 


	Compute_IC_Stmt * compute_stmt;
	if(lhs->get_data_type() == int_data_type)
		compute_stmt =  new Compute_IC_Stmt(sub,lhs_opd,rhs_opd,result_opd); 
	if(lhs->get_data_type() == double_data_type)
		compute_stmt =  new Compute_IC_Stmt(sub_d,lhs_opd,rhs_opd,result_opd); 

	lhs_compile_result->append_ics(*compute_stmt);

	lhs_compile_result->get_reg()->reset_use_for_expr_result();

	rhs_compile_result->get_reg()->reset_use_for_expr_result();

	lhs_compile_result->set_reg(new_reg);

	return *lhs_compile_result;

}



Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & Lra){

}


Code_For_Ast & Divide_Ast::compile(){

	Code_For_Ast * lhs_compile_result = new Code_For_Ast();

	*lhs_compile_result = lhs->compile();

	Code_For_Ast * rhs_compile_result = new Code_For_Ast();

	*rhs_compile_result = rhs->compile();

	Data_Type dt = lhs->get_data_type();

	Register_Descriptor * new_reg = new Register_Descriptor();
	if(dt == int_data_type){

		new_reg =  machine_desc_object.get_new_register<int_reg>();

	}

	if(dt == double_data_type){

		new_reg =  machine_desc_object.get_new_register<float_reg>();

	}

	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = rhs_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		lhs_compile_result->append_ics((**it));
	}

	Register_Addr_Opd *lhs_opd = new Register_Addr_Opd(lhs_compile_result->get_reg());

	Register_Addr_Opd *rhs_opd = new Register_Addr_Opd(rhs_compile_result->get_reg());

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(new_reg);

	// Compute_IC_Stmt(divd,lhs_opd,rhs_opd,result_opd); 
	Compute_IC_Stmt * compute_stmt;
	if(lhs->get_data_type() == int_data_type)
		compute_stmt =  new Compute_IC_Stmt(divd,lhs_opd,rhs_opd,result_opd); 
	if(lhs->get_data_type() == double_data_type)
		compute_stmt =  new Compute_IC_Stmt(div_d,lhs_opd,rhs_opd,result_opd); 

	lhs_compile_result->append_ics(*compute_stmt);

	lhs_compile_result->get_reg()->reset_use_for_expr_result();

	rhs_compile_result->get_reg()->reset_use_for_expr_result();

	lhs_compile_result->set_reg(new_reg);

	return *lhs_compile_result;

}



Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & Lra){

}


Code_For_Ast & Mult_Ast::compile(){

	Code_For_Ast * lhs_compile_result = new Code_For_Ast();

	*lhs_compile_result = lhs->compile();

	Code_For_Ast * rhs_compile_result = new Code_For_Ast();

	*rhs_compile_result = rhs->compile();

	Data_Type dt = lhs->get_data_type();

	Register_Descriptor * new_reg = new Register_Descriptor();
	if(dt == int_data_type){

		new_reg =  machine_desc_object.get_new_register<int_reg>();

	}

	if(dt == double_data_type){

		new_reg =  machine_desc_object.get_new_register<float_reg>();

	}

	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = rhs_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		lhs_compile_result->append_ics((**it));
	}

	Register_Addr_Opd *lhs_opd = new Register_Addr_Opd(lhs_compile_result->get_reg());

	Register_Addr_Opd *rhs_opd = new Register_Addr_Opd(rhs_compile_result->get_reg());

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(new_reg);

	// Compute_IC_Stmt(mult,lhs_opd,rhs_opd,result_opd); 


	Compute_IC_Stmt * compute_stmt;
	if(lhs->get_data_type() == int_data_type)
		compute_stmt =  new Compute_IC_Stmt(mult,lhs_opd,rhs_opd,result_opd); 
	if(lhs->get_data_type() == double_data_type)
		compute_stmt =  new Compute_IC_Stmt(mult_d,lhs_opd,rhs_opd,result_opd); 

	lhs_compile_result->append_ics(*compute_stmt);

	lhs_compile_result->get_reg()->reset_use_for_expr_result();

	rhs_compile_result->get_reg()->reset_use_for_expr_result();

	lhs_compile_result->set_reg(new_reg);

	return *lhs_compile_result;

}






Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & Lra){

}


Code_For_Ast & UMinus_Ast::compile(){

	Code_For_Ast * lhs_compile_result = new Code_For_Ast();

	*lhs_compile_result = lhs->compile();

	Data_Type dt = lhs->get_data_type();

	Register_Descriptor * new_reg = new Register_Descriptor();
	if(dt == int_data_type){

		new_reg =  machine_desc_object.get_new_register<int_reg>();

	}

	if(dt == double_data_type){

		new_reg =  machine_desc_object.get_new_register<float_reg>();

	}


	Register_Addr_Opd *lhs_opd = new Register_Addr_Opd(lhs_compile_result->get_reg());

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(new_reg);

	// Compute_IC_Stmt(mult,lhs_opd,rhs_opd,result_opd); 

	Compute_IC_Stmt * compute_stmt;
	if(lhs->get_data_type() == int_data_type)
		compute_stmt =  new Compute_IC_Stmt(uminus,lhs_opd,NULL,result_opd); 
	if(lhs->get_data_type() == double_data_type)
		compute_stmt =  new Compute_IC_Stmt(uminus_d,lhs_opd,NULL,result_opd); 
	lhs_compile_result->append_ics(*compute_stmt);

	lhs_compile_result->get_reg()->reset_use_for_expr_result();

	lhs_compile_result->set_reg(new_reg);

	return *lhs_compile_result;

}





Code_For_Ast & Relational_Expr_Ast::compile(){

	Code_For_Ast * lhs_compile_result = new Code_For_Ast();

	*lhs_compile_result = lhs_condition->compile();

	Code_For_Ast * rhs_compile_result = new Code_For_Ast();

	*rhs_compile_result = rhs_condition->compile();

	// Code_For_Ast * opr_compile_result = new Code_For_Ast();

	// *opr_compile_result = rel_op->compile();


	Register_Descriptor * new_reg = new Register_Descriptor();

	// if(dt == int_data_type){

	new_reg =  machine_desc_object.get_new_register<int_reg>();

	// }

	// if(dt == double_data_type){

	// 	new_reg =  machine_desc_object.get_new_register<float_reg>();

	// }

	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = rhs_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		lhs_compile_result->append_ics((**it));
	}

	Register_Addr_Opd *lhs_opd = new Register_Addr_Opd(lhs_compile_result->get_reg());

	Register_Addr_Opd *rhs_opd = new Register_Addr_Opd(rhs_compile_result->get_reg());

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(new_reg);

	Compute_IC_Stmt * compute_stmt;	 

	if (rel_op == less_equalto){
		decide_torf = 2;
		if(lhs_condition->get_data_type() == int_data_type)
			compute_stmt =  new Compute_IC_Stmt(sle,lhs_opd,rhs_opd,result_opd); 
		if(lhs_condition->get_data_type() == double_data_type)
			compute_stmt =  new Compute_IC_Stmt(sle_d,lhs_opd,rhs_opd,NULL); 
	}
	else if (rel_op == less_than){
		decide_torf = 2;
		if(lhs_condition->get_data_type() == int_data_type)
			compute_stmt =  new Compute_IC_Stmt(slt,lhs_opd,rhs_opd,result_opd); 
		if(lhs_condition->get_data_type() == double_data_type)
			compute_stmt =  new Compute_IC_Stmt(slt_d,lhs_opd,rhs_opd,NULL); 
	}
	else if (rel_op == greater_than){
		decide_torf = 1;

		if(lhs_condition->get_data_type() == int_data_type)
			compute_stmt =  new Compute_IC_Stmt(sgt,lhs_opd,rhs_opd,result_opd); 
		if(lhs_condition->get_data_type() == double_data_type)
			compute_stmt =  new Compute_IC_Stmt(sgt_d,lhs_opd,rhs_opd,NULL); 
	}
	else if (rel_op == greater_equalto){
		decide_torf = 1;

		if(lhs_condition->get_data_type() == int_data_type)
			compute_stmt =  new Compute_IC_Stmt(sge,lhs_opd,rhs_opd,result_opd); 
		if(lhs_condition->get_data_type() == double_data_type)
			compute_stmt =  new Compute_IC_Stmt(sge_d,lhs_opd,rhs_opd,NULL); 
	}
	else if (rel_op == equalto){
		decide_torf = 2;

		if(lhs_condition->get_data_type() == int_data_type)
			compute_stmt =  new Compute_IC_Stmt(seq,lhs_opd,rhs_opd,result_opd); 
		if(lhs_condition->get_data_type() == double_data_type)
			compute_stmt =  new Compute_IC_Stmt(seq_d,lhs_opd,rhs_opd,NULL); 
	}
	else if (rel_op == not_equalto){
		decide_torf = 1;

		if(lhs_condition->get_data_type() == int_data_type)
			compute_stmt =  new Compute_IC_Stmt(sne,lhs_opd,rhs_opd,result_opd); 
		if(lhs_condition->get_data_type() == double_data_type)
			compute_stmt =  new Compute_IC_Stmt(sne_d,lhs_opd,rhs_opd,NULL); 
	}
	
	// Compute_IC_Stmt * compute_stmt =  new Compute_IC_Stmt(add,lhs_opd,rhs_opd,result_opd); 

	lhs_compile_result->append_ics(*compute_stmt);

	lhs_compile_result->get_reg()->reset_use_for_expr_result();

	rhs_compile_result->get_reg()->reset_use_for_expr_result();

	lhs_compile_result->set_reg(new_reg);

	return *lhs_compile_result;
}







Code_For_Ast & Logical_Expr_Ast::compile(){

	if (bool_op == _logical_not){

	Register_Descriptor * new_reg1 = new Register_Descriptor();

	new_reg1 =  machine_desc_object.get_new_register<int_reg>();
	Const_Opd<int> *my_opd = new Const_Opd<int>(1);
	Register_Addr_Opd *register_opd = new Register_Addr_Opd(new_reg1);
	Move_IC_Stmt * move_stmt = new Move_IC_Stmt(imm_load ,  my_opd,register_opd); //check order

	Code_For_Ast * not_compile_result = new Code_For_Ast();

	not_compile_result->append_ics(*move_stmt);

	Code_For_Ast * rhs_compile_result = new Code_For_Ast();

	*rhs_compile_result = rhs_op->compile();

	Register_Descriptor * new_reg = new Register_Descriptor();

	new_reg =  machine_desc_object.get_new_register<int_reg>();

	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = rhs_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		not_compile_result->append_ics((**it));
	}

	Register_Addr_Opd *rhs_opd = new Register_Addr_Opd(rhs_compile_result->get_reg());

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(new_reg);


	Compute_IC_Stmt * compute_stmt =  new Compute_IC_Stmt(not_t,rhs_opd,register_opd,result_opd);

		not_compile_result->append_ics(*compute_stmt);


		rhs_compile_result->get_reg()->reset_use_for_expr_result();
		new_reg1->reset_use_for_expr_result();

		not_compile_result->set_reg(new_reg);

		return *not_compile_result;

	}
	
	Code_For_Ast * lhs_compile_result = new Code_For_Ast();

	*lhs_compile_result = lhs_op->compile();

	Code_For_Ast * rhs_compile_result = new Code_For_Ast();

	*rhs_compile_result = rhs_op->compile();

	// Code_For_Ast * opr_compile_result = new Code_For_Ast();

	// *opr_compile_result = rel_op->compile();

	// Data_Type dt = lhs->get_data_type();

	Register_Descriptor * new_reg = new Register_Descriptor();

	// if(dt == int_data_type){

	new_reg =  machine_desc_object.get_new_register<int_reg>();

	// }

	// if(dt == double_data_type){

	// 	new_reg =  machine_desc_object.get_new_register<float_reg>();

	// }

	list<Icode_Stmt *>::iterator it;
	
	list<Icode_Stmt *> mylist = rhs_compile_result->get_icode_list();

	for(it = mylist.begin(); it != mylist.end(); it++){
		lhs_compile_result->append_ics((**it));
	}

	Register_Addr_Opd *lhs_opd = new Register_Addr_Opd(lhs_compile_result->get_reg());

	Register_Addr_Opd *rhs_opd = new Register_Addr_Opd(rhs_compile_result->get_reg());

	Register_Addr_Opd *result_opd = new Register_Addr_Opd(new_reg);

	Compute_IC_Stmt * compute_stmt;



	if (bool_op == _logical_or){
		compute_stmt =  new Compute_IC_Stmt(or_t,lhs_opd,rhs_opd,result_opd);

		lhs_compile_result->append_ics(*compute_stmt);

		lhs_compile_result->get_reg()->reset_use_for_expr_result();

		rhs_compile_result->get_reg()->reset_use_for_expr_result();

		lhs_compile_result->set_reg(new_reg);

		return *lhs_compile_result;
	}

	else if (bool_op == _logical_and){
		compute_stmt =  new Compute_IC_Stmt(and_t,lhs_opd,rhs_opd,result_opd);

		lhs_compile_result->append_ics(*compute_stmt);

		lhs_compile_result->get_reg()->reset_use_for_expr_result();

		rhs_compile_result->get_reg()->reset_use_for_expr_result();

		lhs_compile_result->set_reg(new_reg);

		return *lhs_compile_result;
	}

	
	// Compute_IC_Stmt * compute_stmt =  new Compute_IC_Stmt(add,lhs_opd,rhs_opd,result_opd); 

	
}

Code_For_Ast & Print_Ast::compile(){

	Code_For_Ast * var_compile_result = new Code_For_Ast();

	if(var->get_data_type() == int_data_type){

		
		Const_Opd<int> *my_opd = new Const_Opd<int>(1);
		Register_Addr_Opd *register_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]);
		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(imm_load ,  my_opd,register_opd); //check order
		var_compile_result->append_ics(*move_stmt);

		Register_Addr_Opd *register_opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[a0]);

		Mem_Addr_Opd *my_opd1 = new Mem_Addr_Opd(var->get_symbol_entry());

		Move_IC_Stmt* move_stmt1;
		move_stmt1 = new Move_IC_Stmt(load,my_opd1,register_opd1); // check order
		var_compile_result->append_ics(*move_stmt1);

	}
	if(var->get_data_type() == double_data_type){
	
		
		Const_Opd<int> *my_opd = new Const_Opd<int>(3);
		Register_Addr_Opd *register_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]);
		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(imm_load ,  my_opd,register_opd); //check order
		var_compile_result->append_ics(*move_stmt);

		Register_Addr_Opd *register_opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[f12]);

		Mem_Addr_Opd *my_opd1 = new Mem_Addr_Opd(var->get_symbol_entry());

		Move_IC_Stmt* move_stmt1;
		move_stmt1 = new Move_IC_Stmt(load_d,my_opd1,register_opd1); // check order

		var_compile_result->append_ics(*move_stmt1);
	}


	 Print_IC_Stmt * print_ic = new Print_IC_Stmt();
	 var_compile_result->append_ics(*print_ic);

	 return *var_compile_result;
}

Code_For_Ast & Call_Ast::compile(){
	// cout << "xdcalll";

	Code_For_Ast * fun_compile_result = new Code_For_Ast();

	list<Ast *>::iterator it;
	int offset = 0;

	int num_args = 0;

	list<Ast *> temp_param_list = actual_param_list;
	temp_param_list.reverse();
	
	for(it = temp_param_list.begin(); it != temp_param_list.end(); it++){
		Code_For_Ast * arg_compile_result = new Code_For_Ast();
		*arg_compile_result = (*it)->compile();

		list<Icode_Stmt *>::iterator ic_it;
		
		list<Icode_Stmt *> mylist = arg_compile_result->get_icode_list();

		// mylist.reverse();

		for(ic_it = mylist.begin(); ic_it != mylist.end(); ic_it++){
			fun_compile_result->append_ics((**ic_it));
		}

		// ?? store icode stmt
		string var = "lel";
		Symbol_Table_Entry * sp_jugaad = new Symbol_Table_Entry(var, int_data_type , 0, sp_ref);
		sp_jugaad->set_start_offset(offset*(-1));
		sp_jugaad->set_symbol_scope(formal);
		Mem_Addr_Opd *my_opd = new Mem_Addr_Opd(*sp_jugaad);

		Register_Addr_Opd *register_opd = new Register_Addr_Opd(arg_compile_result->get_reg());

		Move_IC_Stmt* move_stmt;
		if((*it)->get_data_type() == int_data_type){
			offset = offset + 4;
			move_stmt = new Move_IC_Stmt(store,register_opd,my_opd); // check order
		}
		if((*it)->get_data_type() == double_data_type){
			offset = offset + 8;
			move_stmt = new Move_IC_Stmt(store_d,register_opd,my_opd); // check order
		}
		num_args++;

		fun_compile_result->append_ics(*move_stmt);
		arg_compile_result->get_reg()->reset_use_for_expr_result();

	}
	int stack_down = offset;
	if(num_args != 0){
		Register_Addr_Opd * stack_reg  = new Register_Addr_Opd(machine_desc_object.spim_register_table[sp]);
		Const_Opd<int> *my_opd = new Const_Opd<int>(stack_down);

		Compute_IC_Stmt * sub_stack = new Compute_IC_Stmt(sub,stack_reg,my_opd,stack_reg);
		fun_compile_result->append_ics(*sub_stack);

		Label_IC_Stmt * label_stmt =  new Label_IC_Stmt(jal ,this->procedure_name); 
		fun_compile_result->append_ics(*label_stmt);

		Compute_IC_Stmt * add_stack = new Compute_IC_Stmt(add,stack_reg,my_opd,stack_reg);
		fun_compile_result->append_ics(*add_stack);
	}
	if (num_args == 0) {
		Label_IC_Stmt * label_stmt =  new Label_IC_Stmt(jal ,this->procedure_name); 
		fun_compile_result->append_ics(*label_stmt);
	}

	Data_Type dt = this->get_data_type();


	if(dt == int_data_type){
		Register_Descriptor * new_reg = machine_desc_object.get_new_register<int_reg>();
		
		Register_Addr_Opd *register_opd = new Register_Addr_Opd(new_reg);

		Register_Addr_Opd *register_opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(mov ,register_opd1, register_opd); //check order
	
		fun_compile_result->append_ics(*move_stmt);
		fun_compile_result->set_reg(new_reg);

		if(this->return_value_reg != NULL){
			fun_compile_result->get_reg()->reset_use_for_expr_result();
		}
	}

	if(dt == double_data_type){

		Register_Descriptor * new_reg = machine_desc_object.get_new_register<float_reg>();

		Register_Addr_Opd *register_opd = new Register_Addr_Opd(new_reg);

		Register_Addr_Opd *register_opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(move_d,register_opd1,register_opd); //check order
	
		fun_compile_result->append_ics(*move_stmt);
		fun_compile_result->set_reg(new_reg);

		if(this->return_value_reg != NULL){
			fun_compile_result->get_reg()->reset_use_for_expr_result();
		}

	}


	return *fun_compile_result;



}

Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}

Code_For_Ast & Return_Ast::compile(){
		// cout << "retcall";

	Code_For_Ast * ret_compile_result = new Code_For_Ast();

	if(return_value == NULL){

		string epilouge_jump = "epilogue_" + this->proc_name ; 

		Label_IC_Stmt * label_stmt =  new Label_IC_Stmt(j ,epilouge_jump); 

		ret_compile_result->append_ics(*label_stmt);

		// ret_compile_result->get_reg()->reset_use_for_expr_result();

		return *ret_compile_result;

	}



	*ret_compile_result = return_value->compile();

	Data_Type dt = this->get_data_type();

	Register_Addr_Opd *register_opd = new Register_Addr_Opd(ret_compile_result->get_reg());

	if(dt == int_data_type){

		Register_Addr_Opd *register_opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(mov , register_opd,register_opd1); //check order
	ret_compile_result->append_ics(*move_stmt);

	}

	if(dt == double_data_type){

		Register_Addr_Opd *register_opd1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);

		Move_IC_Stmt * move_stmt = new Move_IC_Stmt(move_d , register_opd,register_opd1); //check order
	
	ret_compile_result->append_ics(*move_stmt);


	}

	string epilouge_jump = "epilogue_" + this->proc_name ; 

	Label_IC_Stmt * label_stmt =  new Label_IC_Stmt(j ,epilouge_jump); 

	ret_compile_result->append_ics(*label_stmt);

	ret_compile_result->get_reg()->reset_use_for_expr_result();

	return *ret_compile_result;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	
}


