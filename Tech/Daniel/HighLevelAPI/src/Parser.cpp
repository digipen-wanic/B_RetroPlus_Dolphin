//------------------------------------------------------------------------------
//
// File Name:	Parser.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Parser.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Exception class default constructor
// Params:
//   fileName     = The filename of the stream
//   errorDetails = The details to print
ParseException::ParseException(const std::string& fileName, const std::string& errorDetails)
	: std::exception(std::string("Error reading file " + fileName + ". " + errorDetails).c_str())
{
}

// Opens a file for loading.
// Params:
//   filename = The filename of the stream
//   mode	  = The mode to write/read
Parser::Parser(const std::string& filename, std::ios_base::openmode mode)
	: stream(filename, mode), filename(filename), indentLevel(0)
{
}

// Closes the currently open file.
Parser::~Parser()
{
	stream.close();
}

// Begins a new scope when writing to a file.
// Outputs a curly brace and increases the tab count.
void Parser::BeginScope()
{
	// Ensure file opened successfully
	CheckFileOpen();

	// Print correct number of indents
	for (unsigned i = 0; i < indentLevel; ++i)
		stream << tab;

	stream << "{" << std::endl;

	++indentLevel;
}

// Ends the current scope when writing to a file.
// Outputs an end curly brace and decreases tab count.
void Parser::EndScope()
{
	// Ensure file opened successfully
	CheckFileOpen();

	--indentLevel;

	// Print correct number of indents
	for (unsigned i = 0; i < indentLevel; ++i)
		stream << tab;

	stream << "}" << std::endl;
}

// Reads a piece of text from the currently open file
// and skips to the next word afterwards.
// Params:
//   text = The text to skip
void Parser::ReadSkip(const std::string& text)
{
	// Ensure file opened successfully
	CheckFileOpen();

	// Read in the variable name
	std::string str;
	stream >> str;

	// Ensure variable is the correct one to read
	if (str != text)
		throw ParseException(filename, "Variable with the given name could not be found.");
}

// Skips characters in the stream up until the next
// occurrence of the given delimiter.
// Params:
//   delimiter = The character to skip until
void Parser::ReadSkip(char delimiter)
{
	// Ensure file opened successfully
	CheckFileOpen();
	
	// Ignore 10 things until delimiter
	stream.ignore(10, delimiter);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Checks if the file was opened correctly. If not, throws an exception.
void Parser::CheckFileOpen()
{
	// Check whether stream is open, throw exception otherwise
	if (!stream.is_open())
		ParseException(filename, "File could not be opened.");
}

//------------------------------------------------------------------------------