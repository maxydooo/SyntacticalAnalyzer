/*******************************************************************************
* File name: Syntactical_Analyzer.h                                             *
* Project: CS 460 Project 2 Fall 2021                                          *
* Author:                                                                      *
* Date:                                                                        *
* Description: This file contains                                              *
*******************************************************************************/

#ifndef SYNTACTICALANALYZER_H
#define SYNTACTICALANALYZER_H

#include <iostream>
#include <fstream>
#include "Lexical_Analyzer.h"

using namespace std;

class Syntactical_Analyzer 
{
    public:
	Syntactical_Analyzer (char * filename);
	~Syntactical_Analyzer ();
    private:
	Lexical_Analyzer * lex;
	token_type token;
	ofstream p2file;
	ofstream listingFile; //NEEDED?
	ifstream input; //NEEDED?
	int program ();
	int more_defines();
	int define();
	int stmt_list();
    int stmt();
    int literal();
    int quoted_lit();
    int more_tokens();
    int param_list();
    int else_part();
    int stmt_pair();
    int stmt_pair_body();
    int assign_pair();
    int more_assigns();
    int action();
    int any_other_token();
};
	
#endif
