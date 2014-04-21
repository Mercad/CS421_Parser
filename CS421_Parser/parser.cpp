#include "scanner.h"
#include <iostream>
#include <vector>
#include <map>
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
map<string, tokentype> lexicon;

//


int main()
{
	fstream file;
	string input;
	string strType;
	tokentype type;

	file.open("reservedWords.txt");//Opens the file
	if (file.is_open())//If the file is open
	{
		while (!file.eof())//While the file is not empty
		{
			file >> input;//Reads the string
			file >> strType;//Read the type
			type = scanner.StrToTokentype(strType);
			lexicon.insert(pair<string, tokentype> (input, type));
		}
	}
	file.close();//Closes the input file
	file.clear();

	//tokentype type = VERBPASTNEG;
	//int index = 0;
	//bool (*compFunc)(tokentype) = &IsTense;

	//array of function pointers
	bool (*compFunc[3])(tokentype) = {&IsNoun, &IsPeriod, &IsVerb};

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
	cout << "\nExpected test 2: " << g2g2;

	/*Outputting the entire lexicon
	cout << "\nLexicon: " << endl;
	for (map<string, tokentype>::iterator iter = lexicon.begin(); iter != lexicon.end(); ++iter)
	{
		cout << iter->first << " " << scanner.TokenTypeStr(iter->second) << endl;
	}
	return 0;
	*/
}

/*
 * s1 ::= [CONNECTOR] <noun> SUBJECT (<s2> | <s3>)
 * s2 ::= <verb> <tense> <s7>
 * s3 ::= <noun> (<s4> | <s5> | <s6>)
 * s4 ::= <be> <s7>
 * s5 ::= DESTINATION <s2>
 * s6 ::= OBJECT (<s2> | <noun> <s5>)
 * s7 ::= PERIOD [s1]
 */
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
		//optional connector
		if(Expected(&IsConnector, parseList[index]))
		{
			index++;
		}
		//check the noun
		bool (*compFunc[2])(tokentype) = {&IsNoun, &IsSubject};
		vector<string> sublist(parseList.begin() + index, parseList.begin() + sizeof(compFunc)/sizeof(compFunc[0]) );
		if(Expected(compFunc, sublist));
		{
			index += sizeof(compFunc)/sizeof(compFunc[0]);
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
	map<string, tokentype>::iterator iter;

	//if the size of lists do not match
	if( sublist.size() < ( sizeof(compFunc)/sizeof(compFunc[0]) ) )
	{
		valid = false;
	}

	int index = 0;
	while(valid && index < (signed)sublist.size())
	{
		//TODO modularize this
		iter = lexicon.find(sublist[index]);
		//if the word is not in the lexicon
		if (iter == lexicon.end())
		{
			//call the scanner for its type
			type = scanner.Scan(sublist[index]);
			//insert it into the lexicon
			lexicon.insert(pair<string, tokentype> (sublist[index], type));
		}
		//already inserted into lexicon look up its type
		else
		{
			type = iter->second;
		}

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
