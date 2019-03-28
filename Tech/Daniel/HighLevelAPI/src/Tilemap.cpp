//------------------------------------------------------------------------------
//
// File Name:	Tilemap.cpp
// Author(s):	Daniel Walther (daniel.walther)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Tilemap.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a tilemap from the given parameters.
// Params:
//   numColumns = The width of the map.
//   numRows = The height of the map.
//   data = The array containing the map data.
Tilemap::Tilemap(unsigned numColumns, unsigned numRows, int** data)
	: numColumns(numColumns), numRows(numRows), data(data)
{
}

// Destructor.
Tilemap::~Tilemap()
{
	// Free data
	for (unsigned i = 0; i < numColumns; i++)
	{
		delete[] data[i];
	}

	delete[] data;
}

// Gets the width of the map (in tiles).
unsigned Tilemap::GetWidth() const
{
	return numColumns;
}

// Get the height of the map (in tiles).
unsigned Tilemap::GetHeight() const
{
	return numRows;
}

// Gets the value of the cell in the map with the given indices.
// Params:
//   column = The column of the cell.
//   row = The row of the cell.
// Returns:
//   -1 if the indices are invalid, 0 if the cell is empty, 
//   or a positive integer otherwise.
int Tilemap::GetCellValue(unsigned column, unsigned row) const
{
	// Make sure row/column are valid
	if (column < numColumns && row < numRows)
	{
		return data[column][row];
	}

	return -1;
}

// Create a tilemap from the given file.
// Params:
//   filename = The name of the file containing the tilemap data.
Tilemap* Tilemap::CreateTilemapFromFile(const std::string& filename)
{
	std::ifstream infile(filename);

	// ensure file has been properly opened
	if (infile.is_open())
	{
		int columns;
		int rows;

		// read-in rows/columns
		ReadIntegerVariable(infile, "width", columns);
		ReadIntegerVariable(infile, "height", rows);

		// allocate a map array
		int** mapArray = ReadArrayVariable(infile, "data", columns, rows);

		// return Tilemap if mapArray was properly created
		if (mapArray != nullptr)
			return new Tilemap(columns, rows, mapArray);
	}

	// return null if file could not be opened
	std::cout << "Tilemap::CreateTilemapFromFile: File could not be opened" << std::endl;
	return nullptr;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Helper function for reading in values of integers.
// Params:
//   file = The file stream to read from.
//   name = The text to look for before reading the value.
//   variable = If the name is valid, the variable to put the value in.
// Returns:
//   True if a value with the given name was found, false otherwise.
bool Tilemap::ReadIntegerVariable(std::ifstream& file, const std::string& name, int& variable)
{
	std::string word;

	file >> word;

	// make sure correct value is accessed
	if (name != word)
	{
		return false;
	}
	
	// read next integer and store in variable
	file >> variable;

	return true;
}

// Helper function for reading in arrays of integers.
// Params:
//   file = The file stream to read from.
//   name = The text to look for before reading the array.
//   columns = The number of columns in the array.
//   rows = The number of rows in the array.
// Returns:
//   A pointer to the dynamically allocated array if the data is valid, nullptr otherwise.
int** Tilemap::ReadArrayVariable(std::ifstream& file, const std::string& name,
	unsigned columns, unsigned rows)
{
	std::string word;
	file >> word;

	// ensure that we are accessing the correct data
	if (word != name || columns < 1 || rows < 1)
		return nullptr;

	// dynamically allocate new 2D array
	int** tempArray = new int*[columns];
	for (unsigned i = 0; i < columns; i++)
	{
		tempArray[i] = new int[rows];
	}

	// copy data from file into new 2D array
	for (unsigned j = 0; j < rows; j++)
	{
		for (unsigned i = 0; i < columns; i++)
		{
			file >> tempArray[i][j];
			std::cout << tempArray[i][j] << " ";
		}
		std::cout << std::endl;
	}

	return tempArray;
}

//------------------------------------------------------------------------------
