%{
#include <stdio.h>
#include <iostream>
#include <string>
extern "C" void yyerror(char *s);
extern int yylex(void);
extern int yylineno;
extern int yyparse();
extern void store_token_name(std::string token_name, char *lexeme,int lineno);

Symbol_Table * global_table ;
Symbol_Table * local_table;
std::string main_proc_name;
std::string proc_name;

%}
%union {
    int integer_value;
    double double_value;
    std::string * string_value;
    list<Ast *> * ast_list;
    list<Symbol_Table_Entry *> * symbol_entry_list;
    Ast * ast;
    Symbol_Table * symbol_table;
    Symbol_Table_Entry *symbol_entry;
    Basic_Block * basic_block;
    Procedure * procedure;
    }
%token <integer_value> INTEGER_NUMBER 
%token <double_value> DOUBLE_NUMBER 
%token <string_value> NAME 
%token  INTEGER 
%token  FLOAT 
%token  ASSIGN 
%token  VOID 
%token  RETURN
%token  IF
%token  ELSE
%token  DO
%token  WHILE
%token  AND
%token  OR
%token  NOT
%token  LESS_THAN
%token  LESS_THAN_EQUAL
%token  GREATER_THAN
%token  GREATER_THAN_EQUAL
%token  EQUAL
%token  NOT_EQUAL
%token PRINT

%type <symbol_table> loc_decls
%type <symbol_table> var_declaration_list
%type <symbol_table> var_declaration
%type <symbol_table> declaration 
%type <symbol_table> formal_decls
%type <symbol_table> formal_decls_without_name
%type <symbol_table> argument
%type <symbol_entry_list> variable_list 
%type <integer_value> T 
%type <ast_list> cond_assignment
%type <ast_list> nonemptycond_stmt
%type <ast_list> expr_list
%type <ast> returnstmt
%type <ast> single_stmt
%type <ast> printstmt
%type <ast> ifelsestmt
%type <ast> relation_expr
%type <ast> expression
%type <ast> dowhilestmt
%type <ast> whilestmt
%type <ast> assignment_stmt
%type <ast> FUNCTION_CALL
%type <ast> B
%left '?' ':'
%left OR
%left AND
%left EQUAL NOT_EQUAL
%left LESS_THAN LESS_THAN_EQUAL GREATER_THAN GREATER_THAN_EQUAL
%left '+' '-'
%left '*' '/'
%right UMIN
%left NOT
%%

G :{global_table = new Symbol_Table();
  local_table = new Symbol_Table();
  //cout << "G_decc\n";
} Program {
  	 // if(($2)->variable_in_symbol_list_check(main_proc_name)){
    //                    	yyerror("cs316: Error : procedure name global symbol conflict");
				// 		exit(2);
    //   }
    program_object.set_global_table(*global_table);
   //program_object.set_procedure($3,yylineno);
  }
  ;

Program : glob_decls Program
		| proc_declaration Program
		| procedure_definition {
			string main_name = "main";
			if(!program_object.is_procedure_exists(main_name)){
				yyerror("cs316: Error : function called is not declared");
				exit(2);	
			}
			if(!program_object.get_procedure_prototype(main_name)->is_proc_defined()) {
				yyerror("cs316: Error : function called is not defined");
					exit(2);	
			}
			if(program_object.get_procedure_prototype(main_name)->get_return_type() != void_data_type) {
				yyerror("cs316: Error : main should be of type void");
					exit(2);	
			}
			// if(program_object.get_procedure_prototype(main_name)->get_formal_param_list().get_table().size() != 0) {
			// 	yyerror("cs316: Error : main can't have arguments");
			// 		exit(2);	
			// }
		}
  ;


glob_decls : var_declaration{ 

                // ($2)->set_table_scope(global);
	//cout << "glob\n";
				global_table->append_list(*($1),yylineno);
	   	   		global_table->set_table_scope(global);
              	// $$ = $1; 
			}
           ;

proc_declaration : T NAME '(' formal_decls ')' ';'
                      {
                	   main_proc_name = *($2);
                       Procedure * global_procedure;

                       if($1 == 3)
                     	  global_procedure = new Procedure(void_data_type,*($2),yylineno);
                 	   if($1 == 1)
                 	   	  global_procedure = new Procedure(int_data_type,*($2),yylineno);
                 	   if($1 == 2)
                 	      global_procedure = new Procedure(double_data_type,*($2),yylineno);

                 	   global_procedure->set_formal_param_list(*($4));

                       program_object.set_proc_to_map(*($2),global_procedure);
                      }
                      | T NAME '(' formal_decls_without_name ')' ';'
                      {
                	   main_proc_name = *($2);
                       Procedure * global_procedure;

                       if($1 == 3)
                     	  global_procedure = new Procedure(void_data_type,*($2),yylineno);
                 	   if($1 == 1)
                 	   	  global_procedure = new Procedure(int_data_type,*($2),yylineno);
                 	   if($1 == 2)
                 	      global_procedure = new Procedure(double_data_type,*($2),yylineno);

                 	   global_procedure->set_formal_param_list(*($4));

                       program_object.set_proc_to_map(*($2),global_procedure);
                      }
					  ;



procedure_definition : proc_definition procedure_definition 
					 ;
					 | proc_definition
					 ;

proc_definition : T NAME '(' formal_decls ')' '{' { 
							Data_Type dt;
							    if($1 == 3)
		                     	  dt = void_data_type;
		                 	   if($1 == 1)
		                     	  dt = int_data_type;
		                 	   if($1 == 2)
		                     	  dt = double_data_type;

		                     

		                       if(program_object.is_procedure_exists(*$2)){
		                     	 
		                     	 if(program_object.get_procedure_prototype(*($2))->get_return_type() != dt){
		                     	 		yyerror("cs316: Error : return type don't match");
										exit(2);
		                     	 }

		                     	if(program_object.get_procedure_prototype(*($2))->is_proc_defined()) {
		                     		yyerror("cs316: Error : function re-declaration");
		                     			exit(2);	
		                     	}

		                       }
		                       proc_name = *($2);

		                	   main_proc_name = *($2);
		                       Procedure * global_procedure;

		                       if($1 == 3)
		                     	  global_procedure = new Procedure(void_data_type,*($2),yylineno);
		                 	   if($1 == 1)
		                 	   	  global_procedure = new Procedure(int_data_type,*($2),yylineno);
		                 	   if($1 == 2)
		                 	      global_procedure = new Procedure(double_data_type,*($2),yylineno);

		                 	   global_procedure->set_formal_param_list(*($4));

		                  //     global_procedure->set_local_list(*($7));
		                  //     global_procedure->set_ast_list(*($8));
		                       global_procedure->set_proc_is_defined();
		                       program_object.set_proc_to_map(*($2),global_procedure); 
		               } 
		                      loc_decls { 
		                               	program_object.get_procedure_prototype(*($2))->set_local_list(*($8));
		                        } 
		                        cond_assignment '}' {
                     	program_object.get_procedure_prototype(*($2))->set_ast_list(*($10));
                      }
					  ;

		 


 
 formal_decls : formal_decls ',' argument {
	 			($1)->append_list(*($3),yylineno);
	 			($1)->set_table_scope(formal);
	 			$$ = $1;
			  }
			  | argument {
				$$ = $1;
			  }
			  | %empty {
							Symbol_Table * symbol_table = new Symbol_Table(); 
							symbol_table->set_table_scope(formal);

			     	   		$$ = symbol_table;
			  }
			  ;

argument : INTEGER NAME {
			Symbol_Table * symbol_table = new Symbol_Table(); 
			Symbol_Table_Entry * table_entry = new Symbol_Table_Entry(*($2),int_data_type,yylineno);
			symbol_table->push_symbol(table_entry);
			symbol_table->set_table_scope(formal);

			$$ = symbol_table;
		 }
		 | FLOAT NAME {
			 Symbol_Table * symbol_table = new Symbol_Table(); 
			 Symbol_Table_Entry * table_entry = new Symbol_Table_Entry(*($2),double_data_type,yylineno);
			 symbol_table->push_symbol(table_entry);
			 symbol_table->set_table_scope(formal);
			 $$ = symbol_table;

		 }
		 ;

formal_decls_without_name : formal_decls_without_name ',' INTEGER {
	string mystring = "dummy" + to_string(($1)->get_table().size()+1);
Symbol_Table_Entry * table_entry = new Symbol_Table_Entry(mystring,int_data_type,yylineno);
						  	  ($1)->push_symbol(table_entry);
						  	  ($1)->set_table_scope(formal);
						  	  $$ = $1;
						  }
						  | formal_decls_without_name ',' FLOAT {
	string mystring = "dummy" + to_string(($1)->get_table().size()+1);
Symbol_Table_Entry * table_entry = new Symbol_Table_Entry(mystring,double_data_type,yylineno);
						  	  ($1)->push_symbol(table_entry);
						  	  ($1)->set_table_scope(formal);
						  	  $$ = $1;
						  }
						  | INTEGER {
						  	  string mystring = "dummy1";
						  	  Symbol_Table * symbol_table = new Symbol_Table(); 
						  	  Symbol_Table_Entry * table_entry = new Symbol_Table_Entry(mystring,int_data_type,yylineno);
						  	  symbol_table->push_symbol(table_entry);
						  	  symbol_table->set_table_scope(formal);
						  	  $$ = symbol_table;

						  }
						  | FLOAT {
						  	string mystring = "dummy1";

							  Symbol_Table * symbol_table = new Symbol_Table(); 
							  Symbol_Table_Entry * table_entry = new Symbol_Table_Entry(mystring,double_data_type,yylineno);
							  symbol_table->push_symbol(table_entry);
							  symbol_table->set_table_scope(formal);
							  $$ = symbol_table;
						  }
						  ;
 
loc_decls : var_declaration_list { 
				($1)->set_table_scope(local);
				// local_table = $1;
				$$ = $1; 
			}
	   	   | %empty { Symbol_Table * symbol_table = new Symbol_Table(); 

	   	   	$$ = symbol_table; }
           ;

var_declaration_list : var_declaration { 
						$$ = $1;
						} 
				 	 | var_declaration_list var_declaration {
		               	 
				 	 	($1)->append_list(*($2),yylineno);
				 	 	$$ = $1;
				 	 }
				 	 ;

var_declaration : declaration ';' {
				$$ = $1;
				}
				;

declaration : T variable_list {
			
			Symbol_Table * symbol_table = new Symbol_Table();

			list<Symbol_Table_Entry *>::iterator it;
			
			for (it = ((*$2)).begin(); it != ((*$2)).end(); it++){
				Symbol_Table * push_table = new Symbol_Table();
				if( $1 == 1){
				(*it)->set_data_type(int_data_type);
				push_table->push_symbol(*it);
				}
				if( $1 == 2){
				(*it)->set_data_type(double_data_type);
				push_table->push_symbol(*it);

				}
				if ($1 == 3){
				yyerror("cs316: Error : variable can't be of type void");
				exit(2);
				}
				symbol_table->append_list(*push_table,yylineno);
			}
				$$ = symbol_table;
				}
			;

variable_list : NAME {
				list<Symbol_Table_Entry *> * variable_list = new list<Symbol_Table_Entry *>();
				Symbol_Table_Entry * table_entry = new Symbol_Table_Entry(*($1),void_data_type,yylineno);
			
				(*variable_list).push_back(table_entry);
				
				$$ = variable_list;
				
				}
			  | NAME ',' variable_list {
				Symbol_Table_Entry * table_entry = new Symbol_Table_Entry(*($1),void_data_type,yylineno);
   			
				(*($3)).push_front(table_entry);
				$$ = $3;
			  }
			  ;


T : INTEGER { $$ = 1; }
  | FLOAT { $$ = 2; }
  | VOID { $$ = 3; }
  ;

cond_assignment : nonemptycond_stmt {
					$$ = $1;
				}
				| %empty {
				list<Ast *> * ast_list = new list<Ast *>();
				$$ = ast_list; 
				}
				;

nonemptycond_stmt : cond_assignment single_stmt {
				(*($1)).push_back($2);			
				$$ = $1;
				//cout << "singlle_stmt\n";
				}
				;

ifelsestmt : IF '(' expression ')' single_stmt {


			Selection_Statement_Ast	* ifast = new Selection_Statement_Ast($3,$5,NULL,yylineno);
			$$ = ifast;

		   }
		   | IF '(' expression ')' single_stmt ELSE single_stmt {

			Selection_Statement_Ast	* ifelseast = new Selection_Statement_Ast($3,$5,$7,yylineno);
			$$ = ifelseast;

		   }
		   ;

single_stmt : assignment_stmt {
				$$ = $1;
				//cout<<"ass_stmt\n";

			}
			| ifelsestmt {
				$$ = $1;

			}
			| dowhilestmt {
				$$ = $1;

			}
			| FUNCTION_CALL {
				((Call_Ast*)($1))->set_register(new Register_Descriptor());
				$$ = $1;
			}
			| whilestmt {
				$$ = $1;
			}
			| printstmt {
				$$ = $1;

			}
			| returnstmt {
				$$ = $1;
			}
			| '{' nonemptycond_stmt '}' {
					Sequence_Ast * mylist = new	Sequence_Ast(yylineno);

			list<Ast *>::iterator it;
					
			for (it = ((*$2)).begin(); it != ((*$2)).end(); it++){
				mylist->ast_push_back(*(it));
				}

				$$ = mylist;
			}
			;

expression : expression OR expression {

		Logical_Expr_Ast * or_ast = new Logical_Expr_Ast($1,_logical_or,$3, yylineno);
		Data_Type dt = $1->get_data_type();
		or_ast->set_data_type(dt);
		or_ast->check_ast();

		$$ = or_ast;
			}
	| expression AND expression {
		Logical_Expr_Ast * and_ast = new Logical_Expr_Ast($1,_logical_and,$3, yylineno);
		Data_Type dt = $1->get_data_type();
		and_ast->set_data_type(dt);
		and_ast->check_ast();
		$$ = and_ast;
	}
  | NOT expression {

		Logical_Expr_Ast * not_ast = new Logical_Expr_Ast(NULL,_logical_not,$2, yylineno);

		Data_Type dt = $2->get_data_type();


		not_ast->set_data_type(dt);


		not_ast->check_ast();


		$$ = not_ast;
	}
  | '(' expression ')' {
  	$$ = $2;
  } 
  | relation_expr {
  	$$ = $1;
  }
  ;


relation_expr : B LESS_THAN B {
		Relational_Expr_Ast * rel_exp = new Relational_Expr_Ast($1, less_than, $3, yylineno);
		Data_Type dt = $1->get_data_type();
		rel_exp->set_data_type(dt);
		rel_exp->check_ast();
		$$ = rel_exp;
		}
			  | B LESS_THAN_EQUAL B {
		Relational_Expr_Ast * rel_exp = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
		Data_Type dt = $1->get_data_type();
		rel_exp->set_data_type(dt);
		rel_exp->check_ast();
		$$ = rel_exp;
		}
			  | B GREATER_THAN B {
		Relational_Expr_Ast * rel_exp = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
		Data_Type dt = $1->get_data_type();
		rel_exp->set_data_type(dt);
		rel_exp->check_ast();
		$$ = rel_exp;
		}
			  | B GREATER_THAN_EQUAL B {
		Relational_Expr_Ast * rel_exp = new Relational_Expr_Ast($1, greater_equalto, $3, yylineno);
		Data_Type dt = $1->get_data_type();
		rel_exp->set_data_type(dt);
		rel_exp->check_ast();
		$$ = rel_exp;
		}
			  | B EQUAL B {
		Relational_Expr_Ast * rel_exp = new Relational_Expr_Ast($1, equalto, $3, yylineno);
		Data_Type dt = $1->get_data_type();
		rel_exp->set_data_type(dt);
		rel_exp->check_ast();
		$$ = rel_exp;
		}
			  | B NOT_EQUAL B {
		Relational_Expr_Ast * rel_exp = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
		Data_Type dt = $1->get_data_type();
		rel_exp->set_data_type(dt);
		rel_exp->check_ast();
		$$ = rel_exp;
		}
			  ;

dowhilestmt : DO single_stmt WHILE '(' expression ')' ';' {
			
			Iteration_Statement_Ast * dowhile_ast = new Iteration_Statement_Ast($5, $2, yylineno, 1);

			$$ = dowhile_ast;

			}
			;

whilestmt : WHILE '(' expression ')' single_stmt {
			
			Iteration_Statement_Ast * dowhile_ast = new Iteration_Statement_Ast($3, $5, yylineno, 0);

			$$ = dowhile_ast;

			}
		  ;
 
printstmt : PRINT NAME ';' {
		Symbol_Table_Entry * table_entry = new Symbol_Table_Entry() ;
		if ( program_object.get_procedure_prototype(proc_name)->get_local_list().variable_in_symbol_list_check(*($2)) ) {
			table_entry = &program_object.get_procedure_prototype(proc_name)->get_local_list().get_symbol_table_entry(*($2));
		}
		else if ( program_object.get_procedure_prototype(proc_name)->get_formal_param_list().variable_in_symbol_list_check(*($2)) ) {
			table_entry = &program_object.get_procedure_prototype(proc_name)->get_formal_param_list().get_symbol_table_entry(*($2));
		}
		else if ( global_table->variable_in_symbol_list_check(*($2)) ) {
			table_entry = &global_table->get_symbol_table_entry(*($2));
		}
		else {
			yyerror("cs316: Error : variable used in print not declared");
			exit(2);
		}

		Name_Ast * name_ast = new Name_Ast(*($2),*table_entry,yylineno);

		Print_Ast * ast_print = new Print_Ast(name_ast, yylineno);



		$$ = ast_print;

}


assignment_stmt : NAME ASSIGN B ';' {

		Symbol_Table_Entry * table_entry = new Symbol_Table_Entry() ;
		
		if ( program_object.get_procedure_prototype(proc_name)->get_local_list().variable_in_symbol_list_check(*($1)) ) {
			table_entry = &program_object.get_procedure_prototype(proc_name)->get_local_list().get_symbol_table_entry(*($1));
		}
		else if ( program_object.get_procedure_prototype(proc_name)->get_formal_param_list().variable_in_symbol_list_check(*($1)) ) {
			table_entry = &program_object.get_procedure_prototype(proc_name)->get_formal_param_list().get_symbol_table_entry(*($1));
		}
		else if ( global_table->variable_in_symbol_list_check(*($1)) ) {
			table_entry = &global_table->get_symbol_table_entry(*($1));
		}
		else {
			yyerror("cs316: Error : variable used in assignment not declared");
			exit(2);
		}
		Name_Ast * name_ast = new Name_Ast(*($1),*table_entry,yylineno);
		
		Assignment_Ast * assignment_ast = new Assignment_Ast(name_ast,$3,yylineno);
		assignment_ast->check_ast();
		
		$$ = assignment_ast;
    }
    | NAME ASSIGN FUNCTION_CALL {


	    Symbol_Table_Entry * table_entry = new Symbol_Table_Entry() ;
		
		if ( program_object.get_procedure_prototype(proc_name)->get_local_list().variable_in_symbol_list_check(*($1)) ) {
			table_entry = &program_object.get_procedure_prototype(proc_name)->get_local_list().get_symbol_table_entry(*($1));
		}
		else if ( program_object.get_procedure_prototype(proc_name)->get_formal_param_list().variable_in_symbol_list_check(*($1)) ) {
			table_entry = &program_object.get_procedure_prototype(proc_name)->get_formal_param_list().get_symbol_table_entry(*($1));
		}
		else if ( global_table->variable_in_symbol_list_check(*($1)) ) {
			table_entry = &global_table->get_symbol_table_entry(*($1));
		}
		else {
			yyerror("cs316: Error : variable used in assignment not declared");
			exit(2);
		}
		Name_Ast * name_ast = new Name_Ast(*($1),*table_entry,yylineno);
		
		Assignment_Ast * assignment_ast = new Assignment_Ast(name_ast,$3,yylineno);
		assignment_ast->check_ast();
		 

		$$ = assignment_ast;
		//cout<<"func_call_ass\n";

    }
  ;

FUNCTION_CALL : NAME '(' expr_list ')' ';' {
		
	if(!program_object.is_procedure_exists(*($1))) {
		yyerror("cs316: Error : function called is not declared");
			exit(2);	
	}

	Call_Ast * funcall_ast = new Call_Ast((*$1), yylineno);
	funcall_ast->set_actual_param_list(*($3));
	funcall_ast->check_actual_formal_param(program_object.get_procedure_prototype(*($1))->get_formal_param_list());

	$$ = funcall_ast;

	//cout << "func_call\n";


	}

expr_list : B {
			list<Ast *> * ast_list = new list<Ast *>();
			ast_list->push_back($1);
			$$ = ast_list; 
		  }
		  | expr_list ',' B {
		  	 ($1)->push_back($3);
		  	 $$ = $1;
		  }
		  | %empty {
			list<Ast *> * ast_list = new list<Ast *>();
			$$ = ast_list; 
		  }
		  ; 


returnstmt : RETURN B ';' {
	Return_Ast * ret_ast = new Return_Ast($2, proc_name, yylineno);

	if($2->get_data_type() != program_object.get_procedure_prototype(proc_name)->get_return_type()){
			yyerror("cs316: Error : return and function declared type don't match");
			exit(2);
	}
	$$ = ret_ast;
		}
		| RETURN ';' {
	Return_Ast * ret_ast = new Return_Ast(NULL, proc_name, yylineno);

	if(program_object.get_procedure_prototype(proc_name)->get_return_type() != void_data_type ){
			yyerror("cs316: Error : return and function declared type don't match");
			exit(2);
	}
	$$ = ret_ast;
}



B : B '+' B {
	Plus_Ast * plus_ast = new Plus_Ast($1,$3,yylineno);
	Data_Type dt = $1->get_data_type();
	plus_ast->set_data_type(dt);
	plus_ast->check_ast();
	
	$$ = plus_ast;
	}
  | B '-' B {
  	Minus_Ast * min_ast = new Minus_Ast($1,$3,yylineno);
  	Data_Type dt = $1->get_data_type();
	min_ast->set_data_type(dt);
	min_ast->check_ast();
	
  	$$ = min_ast;
   } 
   | B '*' B {
	Mult_Ast * mul_ast = new Mult_Ast($1,$3,yylineno);
	Data_Type dt = $1->get_data_type();
	mul_ast->set_data_type(dt);
	mul_ast->check_ast();
	
	$$ = mul_ast;
 	}
  | B '/' B {
  	Divide_Ast * div_ast = new Divide_Ast($1,$3,yylineno);
  	Data_Type dt = $1->get_data_type();
	div_ast->set_data_type(dt);
	div_ast->check_ast();
	
  	$$ = div_ast;
  	}
  | '-' B %prec UMIN {
	UMinus_Ast * unary_ast = new UMinus_Ast($2,NULL,yylineno);
	Data_Type dt = $2->get_data_type();
	unary_ast->set_data_type(dt);
//	unary_ast->check_ast();
	$$ = unary_ast;
	 }
  |  expression '?' B ':' B {
	Conditional_Expression_Ast * cond_ast = new Conditional_Expression_Ast($1, $3 , $5, yylineno);
	Data_Type dt = $3->get_data_type();
	cond_ast->set_data_type(dt);
	cond_ast->check_ast();
	
	$$ = cond_ast;
}
  | NAME {
		Symbol_Table_Entry * table_entry = new Symbol_Table_Entry();
	if ( program_object.get_procedure_prototype(proc_name)->get_local_list().variable_in_symbol_list_check(*($1)) ) {
			table_entry = &program_object.get_procedure_prototype(proc_name)->get_local_list().get_symbol_table_entry(*($1));
		}
		else if ( program_object.get_procedure_prototype(proc_name)->get_formal_param_list().variable_in_symbol_list_check(*($1)) ) {
			table_entry = &program_object.get_procedure_prototype(proc_name)->get_formal_param_list().get_symbol_table_entry(*($1));
		}
		else if ( global_table->variable_in_symbol_list_check(*($1)) ) {
			table_entry = &global_table->get_symbol_table_entry(*($1));
		}
		else {
			yyerror("cs316: Error : variable used in assignment not declared");
			exit(2);
		}
		Name_Ast * name_ast = new Name_Ast(*($1),*table_entry,yylineno);
        $$ = name_ast;
	}
  | INTEGER_NUMBER {
 	
 	 Number_Ast<int> * num_ast = new Number_Ast<int>($1,int_data_type,yylineno);
 	 $$ = num_ast;
  }
  | DOUBLE_NUMBER {
  	 Number_Ast<double> * num_ast = new Number_Ast<double>($1,double_data_type,yylineno);
 	 $$ = num_ast;
  }
  | '(' B ')' {
  	 $$ = $2;
  }
    ;
%%
