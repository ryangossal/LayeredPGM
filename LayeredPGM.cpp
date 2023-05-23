#include <iostream>
#include <fstream>
#include <string>
using namespace std;
template <class T>
class PGM
{
protected:
	T** board;
	T numrows;
	T numcols;
	T max;
	string magic;
	string comment;
	void readInput(ifstream&);
public:
	class exceptionClass;
	// default constructor
	PGM();
	// one-arg constructor
	PGM(ifstream&);
	// destructor
	~PGM();
	// overloaded addition operator
	PGM operator+(const PGM<T>&);
	// copy constructor
	PGM(const PGM<T>&);
	// move constructor
	PGM(PGM<T>&&);
	// overloaded assignment operator
	PGM<T>& operator=(const PGM<T>&);
	// move assignment operator
	PGM<T>& operator=(PGM<T>&&);
	// template friend function for saving PGM object
	// file
	template <class T>
	friend void saveToPGM(const PGM<T>&, const char*);
};


template <typename T>
void PGM<T>::readInput(ifstream& infile)
{
	if (infile)
	{
		//This is parsing the header information in the pgm file
		getline(infile, magic);
		getline(infile, comment);
		infile >> numcols >> numrows;
		infile >> max;

		board = new T * [numrows];

		for (int i = 0; i < numrows; i++)
		{
			board[i] = new T[numcols];
		}

		for (int i = 0; i < numrows; i++)
		{
			for (int j = 0; j < numcols; j++)
			{
				infile >> board[i][j];
			}
		}

	}
	else if (!infile)
		throw exceptionClass("> ERROR!: input file could not be opened.");
}



template <typename T>
PGM<T>::PGM() : board(nullptr), numrows(0), numcols(0), max(0), magic(""), comment("")
{
	cout << "> No-arg PGM constructor called..." << endl;
}


template <typename T>
PGM<T>::PGM(ifstream& infile) : board(nullptr), numrows(0), numcols(0), max(0), magic(""), comment("")
{
	try
	{
		readInput(infile);
	}
	catch (exceptionClass ex)
	{
		throw ex;
	}

}


template <typename T>
PGM<T>:: ~PGM()
{
	for (int i = 0; i < numrows; i++)
	{
		delete[] board[i];
		board[i] = nullptr;
	}
	delete[] board;
	board = nullptr;
}


template <typename T>
PGM<T> PGM<T>:: operator+(const PGM<T>& p)
{
	cout << "> PGM addition operator called..." << endl;

	// Create a new PGM<T> object

	PGM<T> result;
	// set its header info based on the calling object
	result.comment = comment;
	result.magic = magic;
	result.max = max;
	result.numcols = numcols;
	result.numrows = numrows;
	// allocate memory for result
	result.board = new T * [p.numrows];

	for (unsigned int i = 0; i < p.numrows; i++)
	{
		result.board[i] = new T[p.numcols];
	}
	// use a nested for loop to set the values of
	for (int i = 0; i < numrows; i++)
	{
		for (int j = 0; j < numcols; j++)
		{
			result.board[i][j] = p.board[i][j] + board[i][j];
		}
	}

	return result;
}


template <typename T>
PGM<T>::PGM(const PGM<T>& p) : board(nullptr), numrows(p.numrows), numcols(p.numcols), magic(p.magic), comment(p.comment), max(p.max)
{
	board = new T * [p.numrows];

	for (unsigned int i = 0; i < p.numrows; i++)
	{
		board[i] = new T[p.numcols];
	}

	for (unsigned int i = 0; i < p.numrows; i++)
	{
		for (unsigned int j = 0; j < p.numcols; j++)
		{
			board[i][j] = p.board[i][j];
		}
	}
}


template <typename T>
PGM<T>::PGM(PGM<T>&& p) : board(p.board), numrows(p.numrows), numcols(p.numcols), magic(p.magic), comment(p.comment), max(p.max)
{
	cout << "> Move Constructor Called..." << endl;
	p.board = nullptr;
	p.numrows = 0;
	p.numcols = 0;
}


template <typename T>
PGM<T>& PGM<T> :: operator=(const PGM<T>& p)
{
	if (this != &p)
	{

		if (board != nullptr)
		{
			for (int i = 0; i < numrows; i++)
			{
				delete[] board[i];
				delete[] board;
			}
		}

		board = new T[p.numrows];

		for (unsigned int i = 0; i < p.numrows; i++)
		{
			for (unsigned int j = 0; j < p.numcols; j++)
			{
				board[i][j] = p.board[i][j];
			}
		}

		numrows = p.numrows;
		numcols = p.numcols;
		magic = p.magic;
		comment = p.comment;

	}
	return *this;
}


template <typename T>
PGM<T>& PGM<T>:: operator=(PGM<T>&& p)
{
	cout << "> Move assignment operater called..." << endl;
	if (this != &p)
	{
		swap(board, p.board);
		swap(numrows, p.numrows);
		swap(numcols, p.numcols);
		swap(magic, p.magic);
		swap(comment, p.comment);
		swap(max, p.max);
	}
	return *this;
}



template <typename T>
void saveToPGM(const PGM<T>& f, const char* fn)
{
	cout << "> Saving PGM object to ASCII file..." << endl;
	if (f.board != nullptr)
	{
		ofstream outputFile(fn);

		outputFile << f.magic << endl;
		outputFile << f.comment << endl;
		outputFile << f.numcols << " " << f.numrows << endl;
		outputFile << f.max << endl;

		for (int i = 0; i < f.numrows; i++)
		{
			for (int j = 0; j < f.numcols; j++)
			{
				outputFile << f.board[i][j] << " ";
			}
			outputFile << endl;
		}
		outputFile.close();
	}
	else
	{
		cout << "> Error: PGM object is uninitialized" << endl;
	}

}



template <typename T>
class PGM<T>::exceptionClass
{
private:
	const char* errorMessage;
public:
	exceptionClass(const char* e) : errorMessage(e) {}
	const char* returnError()
	{
		return errorMessage;
	}
};



int main()
{
	ifstream file1("secret.pgm", ios::in);
	ifstream file2("key.pgm", ios::in);
	PGM<short>* ptr1 = nullptr;
	PGM<short>* ptr2 = nullptr;
	PGM<short>* ptr3 = nullptr;
	try
	{
		ptr1 = new PGM<short>(file1);
		ptr2 = new PGM<short>(file2);
		ptr3 = new PGM<short>(*ptr1 + *ptr2);
	}
	catch (PGM<short>::exceptionClass e)
	{
		cout << e.returnError() << endl;
		return 0;
	}
	saveToPGM(*ptr3, "result.pgm");
	delete ptr1;
	ptr1 = nullptr;
	delete ptr2;
	ptr2 = nullptr;
	delete ptr3;
	ptr3 = nullptr;


	return 0;

}