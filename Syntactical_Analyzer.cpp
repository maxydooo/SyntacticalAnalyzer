/*******************************************************************************
* File name: Syntactical_Analyzer.cpp                                          *
* Project: CS 460 Project 2 Fall 2021                                          *
* Author: Max Lankford                                                         *
* Date: 10/29/21                                                               *
* Description: This file contains                                              *
*******************************************************************************/

#include <iostream>
#include <set>
#include "Syntactical_Analyzer.h"
#include "Lexical_Analyzer.h"

using namespace std;

Syntactical_Analyzer::Syntactical_Analyzer (char * filename)
{
    lex = new Lexical_Analyzer (filename);
	token = lex->Get_Token();
  string ss(filename);
  if (ss.substr((ss.rfind('.'))) != ".ss"){
      cout << "Incorrect file type: Must be a .ss file type" << endl;
      exit(1);
  }

  string p2(filename);
  p2 = p2.substr(0, p2.find_last_of('.'));
  p2 += ".p2";
  p2file.open(p2);

  int totalErrors = program ();
}

Syntactical_Analyzer::~Syntactical_Analyzer ()
{
	delete lex;
	p2file.close();
}

int Syntactical_Analyzer::program ()
//1 <program> -> LPAREN_T <define> LPAREN_T <more_defines> EOF_T
{
	int errors = 0;
	set <token_type> firsts = {LPAREN_T, EOF_T};
    set <token_type> follows = {EOF_T};
	char message [100];
	sprintf(message, "Entering program function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

	while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
	    errors++;
	    lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
	    token = lex->Get_Token();
	}

	if (token == LPAREN_T){
	  p2file << "Using Rule 1" << endl;
	  token = lex->Get_Token();
	  errors += define();
	  if (token == LPAREN_T){
	    token = lex->Get_Token();
	    errors += more_defines();
	    if (token == EOF_T){
	    }
	    else {
	      errors++;
	      lex->Report_Error("end of file expected");
	    }
	  }
	  else {
	      errors++;
          lex->Report_Error("( expected");
	  }
	}
	else {
	  errors++;
        lex->Report_Error("( expected");
	}

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting program function; current token is: %s", lex->Get_Token_Name(token).c_str());
	lex->debugFile << message << endl;
	return errors;
}
int Syntactical_Analyzer::more_defines(){
    //2 <more_defines> -> <define> LPAREN_T <more_defines>
    //3 <more_defines> -> IDENT_T <stmt_list> RPAREN_T
    int errors = 0;
    set <token_type> firsts = {IDENT_T, DEFINE_T, EOF_T};
    set <token_type> follows = {EOF_T};
    char message [100];
    sprintf(message, "Entering more_defines function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == DEFINE_T){
        p2file << "Using Rule 2" << endl;
        errors += define();
        if (token == LPAREN_T){
            token = lex->Get_Token();
            errors += more_defines();
        }
        else {
            errors++;
            lex->Report_Error("( expected");
        }
    }
    else if (token == IDENT_T){
        p2file << "Using Rule 3" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
        if (token == RPAREN_T){
            token = lex->Get_Token();
        }
        else {
            errors++;
            lex->Report_Error(") expected");
        }
    }
    else {
        errors++;
        lex->Report_Error("ident or define expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting more_defines function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::define(){
    //4 <define> -> DEFINE_T LPAREN_T IDENT_T <param_list> RPAREN_T <stmt> <stmt_list> RPAREN_T
    int errors = 0;
    set <token_type> firsts = {DEFINE_T, EOF_T};
    set <token_type> follows = {LPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering define function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == DEFINE_T){
        p2file << "Using Rule 4" << endl;
        token = lex->Get_Token();
        if (token == LPAREN_T){
            token = lex->Get_Token();
            if (token == IDENT_T){
                token = lex->Get_Token();
                errors += param_list();
                if (token == RPAREN_T){
                    token = lex->Get_Token();
                    errors += stmt();
                    errors += stmt_list();
                    if (token == RPAREN_T){
                        token = lex->Get_Token();
                    }
                    else {
                        errors++;
                        lex->Report_Error(") expected");
                    }
                }
                else {
                    errors++;
                    lex->Report_Error(") expected");
                }
            }
            else {
                errors++;
                lex->Report_Error("ident expected");
            }
        }
        else {
            errors++;
            lex->Report_Error("( expected");
        }
    }
    else {
        errors++;
        lex->Report_Error("define expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting define function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::stmt_list(){
    //5 <stmt_list> -> <stmt> <stmt_list>
    //6 <stmt_list> -> {}
    int errors = 0;
    set <token_type> firsts = {IDENT_T, LPAREN_T, RPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, EOF_T};
    set <token_type> follows = {RPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering stmt_list function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T){
        p2file << "Using Rule 5" << endl;
        errors += stmt();
        errors += stmt_list();
    }
    else if (token == RPAREN_T){
        p2file << "Using Rule 6" << endl;
    }
    else {
        errors++;
        lex->Report_Error("ident or ( or numlit or strlit or squote or ) expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting stmt_list function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::stmt(){
    //7	<stmt> -> <literal>
    //8	<stmt> -> IDENT_T
    //9	<stmt> -> LPAREN_T <action> RPAREN_T
    int errors = 0;
    set <token_type> firsts = {IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, EOF_T};
    set <token_type> follows = {RPAREN_T, IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, EOF_T};
    char message [100];
    sprintf(message, "Entering stmt function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T){
        p2file << "Using Rule 7" << endl;
        errors += literal();
    }
    else if (token == IDENT_T) {
        p2file << "Using Rule 8" << endl;
        token = lex->Get_Token();
    }
    else if (token == LPAREN_T){
        p2file << "Using Rule 9" << endl;
        token = lex->Get_Token();
        errors += action();
        if (token == RPAREN_T){
            token = lex->Get_Token();
        }
        else {
            errors++;
            lex->Report_Error(") expected");
        }
    }
    else {
        errors++;
        lex->Report_Error("ident or lparen or numlit or strlit or squote expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting stmt function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::literal(){
    //10 <literal> -> NUMLIT_T
    //11 <literal> -> STRLIT_T
    //12 <literal> -> SQUOTE_T <quoted_lit>
    int errors = 0;
    set <token_type> firsts = {NUMLIT_T, STRLIT_T, SQUOTE_T, EOF_T};
    set <token_type> follows = {RPAREN_T, IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, EOF_T};
    char message [100];
    sprintf(message, "Entering literal function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == NUMLIT_T) {
        p2file << "Using Rule 10" << endl;
        token = lex->Get_Token();
    }
    else if (token == STRLIT_T) {
        p2file << "Using Rule 11" << endl;
        token = lex->Get_Token();
    }
    else if (token == SQUOTE_T){
        p2file << "Using Rule 12" << endl;
        token = lex->Get_Token();
        errors += quoted_lit();
    }
    else {
        errors++;
        lex->Report_Error("numlit, strlit, or squote expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting literal function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::quoted_lit(){
    //13 <quoted_lit> -> <any_other_token>
    int errors = 0;
    set <token_type> firsts = {LPAREN_T, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP1_T,
                               LISTOP2_T, AND_T, OR_T, NOT_T, DEFINE_T, LET_T, NUMBERP_T, LISTP_T, ZEROP_T,
                               NULLP_T, STRINGP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T, ROUND_T, EQUALTO_T,
                               GT_T, LT_T, GTE_T, LTE_T, SQUOTE_T, COND_T, ELSE_T, EOF_T};
    set <token_type> follows = {RPAREN_T, IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, EOF_T};
    char message [100];
    sprintf(message, "Entering quoted_lit function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == IF_T || token == DISPLAY_T ||
    token == NEWLINE_T || token == LISTOP1_T || token == LISTOP2_T || token == AND_T || token == OR_T || token == NOT_T ||
    token == DEFINE_T || token == LET_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T || token == NULLP_T ||
    token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T || token == MODULO_T ||
    token == ROUND_T || token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T || token == SQUOTE_T ||
    token == COND_T || token == ELSE_T){
        p2file << "Using Rule 13" << endl;
        errors += any_other_token();
    }
    else {
        errors++;
        lex->Report_Error("Token from firsts set expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting quoted_lit function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::more_tokens(){
    //14	<more_tokens> -> <any_other_token> <more_tokens>
    //15	<more_tokens> -> {}
    int errors = 0;
    set <token_type> firsts = {LPAREN_T, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP1_T,
                               LISTOP2_T, AND_T, OR_T, NOT_T, DEFINE_T, LET_T, NUMBERP_T, LISTP_T, ZEROP_T,
                               NULLP_T, STRINGP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T, ROUND_T, EQUALTO_T,
                               GT_T, LT_T, GTE_T, LTE_T, SQUOTE_T, COND_T, ELSE_T, RPAREN_T, EOF_T};
    set <token_type> follows = {RPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering more_tokens function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == IF_T || token == DISPLAY_T ||
    token == NEWLINE_T || token == LISTOP1_T || token == LISTOP2_T || token == AND_T || token == OR_T || token == NOT_T ||
    token == DEFINE_T || token == LET_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T || token == NULLP_T ||
    token == STRINGP_T || token == PLUS_T || token == MINUS_T || token == DIV_T || token == MULT_T || token == MODULO_T ||
    token == ROUND_T || token == EQUALTO_T || token == GT_T || token == LT_T || token == GTE_T || token == LTE_T || token == SQUOTE_T ||
    token == COND_T || token == ELSE_T){
        p2file << "Using Rule 14" << endl;
        errors += any_other_token();
        errors += more_tokens();
    }
    else if (token == RPAREN_T){
        p2file << "Using Rule 15" << endl;
    }
    else {
        errors++;
        lex->Report_Error("Token from firsts set expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting more_tokens function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::param_list(){
    //16	<param_list> -> IDENT_T <param_list>
    //17	<param_list> -> {}
    int errors = 0;
    set <token_type> firsts = {IDENT_T, RPAREN_T, EOF_T};
    set <token_type> follows = {RPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering param_list function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == IDENT_T){
        p2file << "Using Rule 16" << endl;
        token = lex->Get_Token();
        errors += param_list();
    }
    else if (token == RPAREN_T){
        p2file << "Using Rule 17" << endl;
    }
    else {
        errors++;
        lex->Report_Error("ident or ) expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting param_list function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::else_part(){
    //18	<else_part> -> <stmt>
    //19	<else_part> -> {}
    int errors = 0;
    set <token_type> firsts = {IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, RPAREN_T, EOF_T};
    set <token_type> follows = {RPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering else_part function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T){
        p2file << "Using Rule 18" << endl;
        errors += stmt();
    }
    else if (token == RPAREN_T){
        p2file << "Using Rule 19" << endl;
    }
    else {
        errors++;
        lex->Report_Error("ident or ( or numlit or strlit or squote or ) expected");

    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting else_part function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::stmt_pair(){
    //20	<stmt_pair> -> LPAREN_T <stmt_pair_body>
    //21	<stmt_pair> -> {}
    int errors = 0;
    set <token_type> firsts = {LPAREN_T, RPAREN_T, EOF_T};
    set <token_type> follows = {RPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering stmt_pair function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == LPAREN_T){
        p2file << "Using Rule 20" << endl;
        token = lex->Get_Token();
        errors += stmt_pair_body();
    }
    else if (token == RPAREN_T){
        p2file << "Using Rule 21" << endl;
    }
    else {
        errors++;
        lex->Report_Error("( or ) expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting stmt_pair function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::stmt_pair_body(){
    //22	<stmt_pair_body> -> <stmt> <stmt> RPAREN_T <stmt_pair>
    //23	<stmt_pair_body> -> ELSE_T <stmt> RPAREN_T
    int errors = 0;
    set <token_type> firsts = {ELSE_T, IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, EOF_T};
    set <token_type> follows = {RPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering stmt_pair_body function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == IDENT_T || token == LPAREN_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T) {
        p2file << "Using Rule 22" << endl;
        errors += stmt();
        errors += stmt();
        if (token == RPAREN_T){
            token = lex->Get_Token();
            errors += stmt_pair();
        }
        else {
            errors++;
            lex->Report_Error(") expected");
        }
    }
    else if (token == ELSE_T){
        p2file << "Using Rule 23" << endl;
        token = lex->Get_Token();
        errors += stmt();
        if (token == RPAREN_T){
            token = lex->Get_Token();
        }
        else {
            errors++;
            lex->Report_Error(") expected");
        }
    }
    else {
        errors++;
        lex->Report_Error("else or ident or ( or numlit or strlit or squote expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting stmt_pair_body function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::assign_pair(){
    //24	<assign_pair> -> LPAREN_T IDENT_T <stmt> RPAREN_T
    int errors = 0;
    set <token_type> firsts = {LPAREN_T, EOF_T};
    set <token_type> follows = {LPAREN_T, RPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering assign_pair function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == LPAREN_T){
        p2file << "Using Rule 24" << endl;
        token = lex->Get_Token();
        if (token == IDENT_T){
            token = lex->Get_Token();
            errors += stmt();
            if (token == RPAREN_T){
                token = lex->Get_Token();
            }
            else {
                errors++;
                lex->Report_Error(") expected");
            }
        }
        else {
            errors++;
            lex->Report_Error("ident expected");
        }
    }
    else {
        errors++;
        lex->Report_Error("( expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting assign_pair function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::more_assigns(){
    //25	<more_assigns> -> <assign_pair> <more_assigns>
    //26	<more_assigns> -> {}
    int errors = 0;
    set <token_type> firsts = {LPAREN_T, RPAREN_T, EOF_T};
    set <token_type> follows = {RPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering more_assigns function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == LPAREN_T){
        p2file << "Using Rule 25" << endl;
        errors += assign_pair();
        errors += more_assigns();
    }
    else if (token == RPAREN_T){
        p2file << "Using Rule 26" << endl;
    }
    else {
        errors++;
        lex->Report_Error("( or ) expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting more_assigns function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::action(){
    int errors = 0;
    set <token_type> firsts = {IF_T, COND_T, LET_T, LISTOP1_T, LISTOP2_T, AND_T, OR_T, NOT_T, NUMBERP_T, ZEROP_T, LISTP_T,
                               NULLP_T, STRINGP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T, ROUND_T, EQUALTO_T,
                               GT_T, LT_T, GTE_T, LTE_T, IDENT_T, DISPLAY_T, NEWLINE_T, EOF_T};
    set <token_type> follows = {RPAREN_T, EOF_T};
    char message [100];
    sprintf(message, "Entering action function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == IF_T){
        p2file << "Using Rule 27" << endl;
        token = lex->Get_Token();
        errors += stmt();
        errors += stmt();
        errors += else_part();
    }
    else if (token == COND_T){
        p2file << "Using Rule 28" << endl;
        token = lex->Get_Token();
        if (token == LPAREN_T){
            token = lex->Get_Token();
            errors += stmt_pair_body();
        }
        else {
            errors++;
            lex->Report_Error("( expected");
        }
    }
    else if (token == LET_T){
        p2file << "Using Rule 29" << endl;
        token = lex->Get_Token();
        if (token == LPAREN_T){
            token = lex->Get_Token();
            errors += more_assigns();
            if (token == RPAREN_T){
                token = lex->Get_Token();
                errors += stmt();
                errors += stmt_list();
            }
            else {
                errors++;
                lex->Report_Error("let or ( or ) expected");
            }
        }

        else {
            errors++;
            lex->Report_Error(") expected");
        }
    }
    else if (token == LISTOP1_T){
        p2file << "Using Rule 30" << endl;
        token = lex->Get_Token();
        errors += stmt();
    }
    else if (token == LISTOP2_T){
        p2file << "Using Rule 31" << endl;
        token = lex->Get_Token();
        errors += stmt();
        errors += stmt();
    }
    else if (token == AND_T){
        p2file << "Using Rule 32" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == OR_T){
        p2file << "Using Rule 33" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == NOT_T){
        p2file << "Using Rule 34" << endl;
        token = lex->Get_Token();
        errors += stmt();
    }
    else if (token == NUMBERP_T){
        p2file << "Using Rule 35" << endl;
        token = lex->Get_Token();
        errors += stmt();
    }
    else if (token == LISTP_T){
        p2file << "Using Rule 36" << endl;
        token = lex->Get_Token();
        errors += stmt();
    }
    else if (token == ZEROP_T){
        p2file << "Using Rule 37" << endl;
        token = lex->Get_Token();
        errors += stmt();
    }
    else if (token == NULLP_T){
        p2file << "Using Rule 38" << endl;
        token = lex->Get_Token();
        errors += stmt();
    }
    else if (token == STRINGP_T){
        p2file << "Using Rule 39" << endl;
        token = lex->Get_Token();
        errors += stmt();
    }
    else if (token == PLUS_T){
        p2file << "Using Rule 40" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == MINUS_T){
        p2file << "Using Rule 41" << endl;
        token = lex->Get_Token();
        errors += stmt();
        errors += stmt_list();
    }
    else if (token == DIV_T){
        p2file << "Using Rule 42" << endl;
        token = lex->Get_Token();
        errors += stmt();
        errors += stmt_list();
    }
    else if (token == MULT_T){
        p2file << "Using Rule 43" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == MODULO_T){
        p2file << "Using Rule 44" << endl;
        token = lex->Get_Token();
        errors += stmt();
        errors += stmt();
    }
    else if (token == ROUND_T){
        p2file << "Using Rule 45" << endl;
        token = lex->Get_Token();
        errors += stmt();
    }
    else if (token == EQUALTO_T){
        p2file << "Using Rule 46" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == GT_T){
        p2file << "Using Rule 47" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == LT_T){
        p2file << "Using Rule 48" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == GTE_T){
        p2file << "Using Rule 49" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == LTE_T){
        p2file << "Using Rule 50" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == IDENT_T){
        p2file << "Using Rule 51" << endl;
        token = lex->Get_Token();
        errors += stmt_list();
    }
    else if (token == DISPLAY_T){
        p2file << "Using Rule 52" << endl;
        token = lex->Get_Token();
        errors += stmt();
    }
    else if (token == NEWLINE_T){
        p2file << "Using Rule 53" << endl;
        token = lex->Get_Token();
    }
    else {
        errors++;
        lex->Report_Error("Token from the firsts set expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting action function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}
int Syntactical_Analyzer::any_other_token(){
    int errors = 0;
    set <token_type> firsts = {LPAREN_T, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP1_T,
                               LISTOP2_T, AND_T, OR_T, NOT_T, DEFINE_T, LET_T, NUMBERP_T, LISTP_T, ZEROP_T,
                               NULLP_T, STRINGP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T, ROUND_T, EQUALTO_T,
                               GT_T, LT_T, GTE_T, LTE_T, SQUOTE_T, COND_T, ELSE_T, EOF_T};
    set <token_type> follows = {RPAREN_T, IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP1_T,
                     LISTOP2_T, AND_T, OR_T, NOT_T, DEFINE_T, LET_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T,
                     STRINGP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T, ROUND_T, EQUALTO_T, GT_T, LT_T, GTE_T,
                     LTE_T, SQUOTE_T, COND_T, ELSE_T, EOF_T};
    char message [100];
    sprintf(message, "Entering any_other_token function; current token is: %s, lexeme: %s", lex->Get_Token_Name(token).c_str(), lex->Get_Lexeme().c_str());
    lex->debugFile << message << endl;

    while (firsts.find(token) == firsts.end() && follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    if (token == LPAREN_T){
        p2file << "Using Rule 54" << endl;
        token = lex->Get_Token();
        errors += more_tokens();
        if (token == RPAREN_T){
            token = lex->Get_Token();
        }
        else {
            errors++;
            lex->Report_Error(") expected");
        }
    }
    else if (token == IDENT_T){
        p2file << "Using Rule 55" << endl;
        token = lex->Get_Token();
    }
    else if (token == NUMLIT_T){
        p2file << "Using Rule 56" << endl;
        token = lex->Get_Token();
    }
    else if (token == STRLIT_T){
        p2file << "Using Rule 57" << endl;
        token = lex->Get_Token();
    }
    else if (token == IF_T){
        p2file << "Using Rule 58" << endl;
        token = lex->Get_Token();
    }
    else if (token == DISPLAY_T){
        p2file << "Using Rule 59" << endl;
        token = lex->Get_Token();
    }
    else if (token == NEWLINE_T){
        p2file << "Using Rule 60" << endl;
        token = lex->Get_Token();
    }
    else if (token == LISTOP1_T){
        p2file << "Using Rule 61" << endl;
        token = lex->Get_Token();
    }
    else if (token == LISTOP2_T){
        p2file << "Using Rule 62" << endl;
        token = lex->Get_Token();
    }
    else if (token == AND_T){
        p2file << "Using Rule 63" << endl;
        token = lex->Get_Token();
    }
    else if (token == OR_T){
        p2file << "Using Rule 64" << endl;
        token = lex->Get_Token();
    }
    else if (token == NOT_T){
        p2file << "Using Rule 65" << endl;
        token = lex->Get_Token();
    }
    else if (token == DEFINE_T){
        p2file << "Using Rule 66" << endl;
        token = lex->Get_Token();
    }
    else if (token == LET_T){
        p2file << "Using Rule 67" << endl;
        token = lex->Get_Token();
    }
    else if (token == NUMBERP_T){
        p2file << "Using Rule 68" << endl;
        token = lex->Get_Token();
    }
    else if (token == LISTP_T){
        p2file << "Using Rule 69" << endl;
        token = lex->Get_Token();
    }
    else if (token == ZEROP_T){
        p2file << "Using Rule 70" << endl;
        token = lex->Get_Token();
    }
    else if (token == NULLP_T){
        p2file << "Using Rule 71" << endl;
        token = lex->Get_Token();
    }
    else if (token == STRINGP_T){
        p2file << "Using Rule 72" << endl;
        token = lex->Get_Token();
    }
    else if (token == PLUS_T){
        p2file << "Using Rule 73" << endl;
        token = lex->Get_Token();
    }
    else if (token == MINUS_T){
        p2file << "Using Rule 74" << endl;
        token = lex->Get_Token();
    }
    else if (token == DIV_T){
        p2file << "Using Rule 75" << endl;
        token = lex->Get_Token();
    }
    else if (token == MULT_T){
        p2file << "Using Rule 76" << endl;
        token = lex->Get_Token();
    }
    else if (token == MODULO_T){
        p2file << "Using Rule 77" << endl;
        token = lex->Get_Token();
    }
    else if (token == ROUND_T){
        p2file << "Using Rule 78" << endl;
        token = lex->Get_Token();
    }
    else if (token == EQUALTO_T){
        p2file << "Using Rule 79" << endl;
        token = lex->Get_Token();
    }
    else if (token == GT_T){
        p2file << "Using Rule 80" << endl;
        token = lex->Get_Token();
    }
    else if (token == LT_T){
        p2file << "Using Rule 81" << endl;
        token = lex->Get_Token();
    }
    else if (token == GTE_T){
        p2file << "Using Rule 82" << endl;
        token = lex->Get_Token();
    }
    else if (token == LTE_T){
        p2file << "Using Rule 83" << endl;
        token = lex->Get_Token();
    }
    else if (token == SQUOTE_T){
        p2file << "Using Rule 84" << endl;
        token = lex->Get_Token();
        errors += any_other_token();
    }
    else if (token == COND_T){
        p2file << "Using Rule 85" << endl;
        token = lex->Get_Token();
    }
    else if (token == ELSE_T){
        p2file << "Using Rule 86" << endl;
        token = lex->Get_Token();
    }
    else {
        errors++;
        lex->Report_Error("Token from the firsts set expected");
    }

    while (follows.find(token) == follows.end()){
        errors++;
        lex->Report_Error("Unexpected: " + lex->Get_Lexeme());
        token = lex->Get_Token();
    }

    sprintf(message, "Exiting any_other_token function; current token is: %s", lex->Get_Token_Name(token).c_str());
    lex->debugFile << message << endl;
    return errors;
}