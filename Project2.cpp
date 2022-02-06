/*******************************************************************************
* File name: Project2.cpp                                                      *
* Project: CS 460 Project 2 Fall 2021                                          *
* Author: Dr. Watts                                                            *
* Date: Fall 2021                                                              *
* Description: This file contains the driver program for Project 2.            *
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "Set_Limits.h"
#include "Syntactical_Analyzer.h"

int main (int argc, char * argv[])
{
	if (argc < 2)
	{
		printf ("format: %s <filename>\n", argv[0]);
		exit (1);
	}
	Set_Limits ();
	Syntactical_Analyzer parse (argv[1]);

	return 0;
}
