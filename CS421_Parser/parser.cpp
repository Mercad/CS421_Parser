#include "scanner.h"
#include <iostream>
#include <vector>
using namespace std;


/***********************************************************************
 * Parse
 * ------------------------------------
 * @param parseList - list of Japanese strings representing valid sentences
 * @return valid 	- boolean representing a valid list
 ***********************************************************************/
bool Parse(vector<string> parseList);

bool Expected(bool (*compFunc[])(tokentype), vector<string> sublist);
bool Expected(bool (*compFunc)(tokentype), string compStr);

/***********************************************************************
 * Match
 * ------------------------------------------
 * @param compFunc(tokentype) - Allows us to pass a comparator function
 * @param type - tokentype we want to compare
 ***********************************************************************/
bool Match(bool (*compFunc)(tokentype), tokentype type)
{
	return compFunc(type);
}

bool IsObject(tokentype type)
{
	return (type == OBJECT);
}

bool IsDestination(tokentype type)
{
	return (type == DESTINATION);
}

bool IsSubject(tokentype type)
{
	return (type == SUBJECT);
}

bool IsConnector(tokentype type)
{
	return (type == CONNECTOR);
}

bool IsPeriod(tokentype type)
{
	return (type == PERIOD);
}

bool IsVerb(tokentype type)
{
	return (type == VERB);
}

bool IsNoun(tokentype type)
{
	return (type == WORD1 || type == WORD2 || type == PRONOUN);
}

bool IsBe(tokentype type)
{
	return (type == IS || type == WAS);
}

bool IsTense(tokentype type)
{
	return (type == VERBPAST || type == VERBPASTNEG || type == VERB || type
			== VERBNEG);
}

Scanner scanner;
//tokentype type;


int main()
{
	//tokentype type = VERBPASTNEG;
	//int index = 0;
	//bool (*compFunc)(tokentype) = &IsTense;

	//array of function pointers
	bool (*compFunc[3])(tokentype) = {&IsNoun, &IsPeriod, &IsNoun};

	//example string list to test
	vector<string> testList;
	testList.push_back("rika");
	testList.push_back(".");
	testList.push_back("masu");
	//array of expected inputs
	//tokentype type[3] = {CONNECTOR, WORD1, PERIOD};
	//bool matches = Match(compFunc, index, type);
	bool good2go = Expected(compFunc, testList);
    /*
	while(good2go && index < 3)
	{
		cout << "Index: " << index ;
		//tests whether the input fits an expected input
		good2go = Match(compFunc[index], type[index]);
		cout << " Matches: " << good2go << endl;
		index++;
	}*/

	cout << "Expected test: " << good2go;
	bool g2g2 = Expected(&IsNoun, "rika");
	cout << "\ng2g2: " << g2g2;
	//cout << "Sizeof compfunc: " << sizeof(compFunc)/sizeof(compFunc[0]);
	return 0;
}

bool Parse(vector<string> parseList)
{
	//expresses whether or not you have a sentence
	bool valid = false;
	//expresses a valid part of a sentence
	bool isPart = true;
	//index in the parseList
	int index = 0;
	//int state = 0;
	tokentype type;

	if(parseList.empty())
	{
		return false;
	}

	do
	{

		//s1


		bool (*compFunc[3])(tokentype) = {&IsConnector, &IsNoun, &IsSubject};
		if(parseList.size() < 3)
		{
			isPart = false;
		}
		while(isPart && index < 3)
		{
			isPart = Match(compFunc[index], type);
		}

		if(index + 1 < (signed)parseList.size() && IsVerb(type))
		{
			//s2
		}
		else if(index + 1 < (signed)parseList.size() && IsNoun(type))
		{
			//s3
		}
		else
		{
			valid = false;
		}

		//s2
		isPart = Match(&IsVerb, type);
		isPart = Match(&IsTense, type);

		//s3
		if(IsBe(type))
		{
			//s4
		}
		else if(IsDestination(type))
		{
			//s5
		}

		//s7
		isPart = IsVerb(type);
		isPart = (type == PERIOD);

		index++;
	}
	while(index < (signed)parseList.size() && valid);

	return(valid);
}

//TODO maybe make it a vector of comparator functions and not an array
/***********************************************************************
 * Expected
 * ---------------------------------------------------------
 * Expected takes 2 lists and compares whether the tokentypes of one list
 * are what they are supposed to be. If they are it returns true, otherwise
 * it returns false.
 * ---------------------------------------------------------
 * @param compFunc[] - Array of comparator functions
 * @param sublist 	 - List of strings to check
 * @return valid	 - If the strings tokentype are what they supposed to be
 ***********************************************************************/
bool Expected(bool (*compFunc[])(tokentype), vector<string> sublist)
{
	//Scanner scanner;
	bool valid = true;
	tokentype type;
	//if the size of lists do not match
	if( sublist.size() < ( sizeof(compFunc)/sizeof(compFunc[0]) ) )
	{
		valid = false;
	}

	int index = 0;
	while(valid && index < (signed)sublist.size())
	{
		//TODO if the word is not in the lexeme
		//call the scanner and if its a valid word put it in the lexeme
		type = scanner.Scan(sublist[index]);

		//get the type

		//try and fit the type into a valid sentence

		//match expected results
		valid = Match(compFunc[index], type);

		index++;
	}
	return valid;
}

bool Expected(bool (*compFunc)(tokentype), string compStr)
{
	bool (*funcs[1])(tokentype) = {compFunc};
	vector<string> list (1, compStr);
	return Expected(funcs, list);
}
