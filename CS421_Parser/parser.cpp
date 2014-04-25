#include "scanner.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;

Scanner scanner;
map<string, tokentype> symbolTable;
typedef bool (*fptr)(tokentype);


/***********************************************************************
 * Parse
 * ------------------------------------
 * @param parseList - list of Japanese strings representing valid sentences
 * @return valid 	- boolean representing a valid list
 ***********************************************************************/
bool Parse(vector<string> parseList);

bool Expected(bool(*compFunc[])(tokentype), vector<string> sublist);
bool Expected(bool(*compFunc)(tokentype), string compStr);
bool Expected(vector<fptr> compFunc, vector<string> sublist);
string FptrToString(fptr fp);

/***********************************************************************
 * Match
 * ------------------------------------------
 * @param compFunc(tokentype) - Allows us to pass a comparator function
 * @param type - tokentype we want to compare
 ***********************************************************************/
bool Match(fptr compFunc, tokentype type)
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
	return (type == WORD2);
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

int main()
{
	string fileName;
	string input;
	string strType;
	tokentype type;
	fstream file;
	vector<string> wordList;
	file.open("reservedWords.txt");//Opens the file
	if (file.is_open())//If the file is open
	{
		while (!file.eof())//While the file is not empty
		{
			file >> input;//Reads the string
			file >> strType;//Read the type
			type = scanner.StrToTokentype(strType);
			symbolTable.insert(pair<string, tokentype> (input, type));
		}
	}
	file.close();//Closes the input file
	file.clear();


	//example string list to test
	cout << "File Name: ";
	cin >> fileName;

	file.open(fileName.c_str());
	if(file.is_open())
	{
		while (!file.eof())//While the file is not empty
		{
			file >> input;
			wordList.push_back(input);
		}
	}
	file.close();

	bool good2go = Parse(wordList);
	cout << "Is this story in my language?: " ;
	 if(good2go)
		 cout << "Yes!";
	 else
		 cout << "No :(";
	 cout << endl;

	/* //Output entire symbol table
	for (map<string, tokentype>::iterator it = symbolTable.begin(); it
			!= symbolTable.end(); ++it)
	{
		cout <<  it->first << " " << scanner.TokenTypeStr(it->second) << endl;
	}*/
}

/*
 * <story> 	::= <s> [<s>]
 * <s> 		::= [CONNECTOR] <noun> SUBJECT (<s1> | <s2> | <s3> | <s4> | <s5>)
 * <s1> 	::= <verb> <tense> PERIOD
 * <s2>		::= <noun> <be> PERIOD
 * <s3>		::= <noun> DESTINATION <verb> <tense> PERIOD
 * <s4>		::= <noun> OBJECT <verb> <tense> PERIOD
 * <s5>		::= <noun> OBJECT <noun> DESTINATION <verb> <tense> PERIOD
 */
bool Parse(vector<string> parseList)
{
	//expresses whether or not you have a sentence
	bool valid = false;
	//expresses a valid part of a sentence
	bool isPart = true;
	//index in the parseList
	int index = 0;

	vector<vector<fptr> > ruleList;

	//define each rule with a vector representing it
	fptr s1[3] =
	{	&IsVerb, &IsTense, &IsPeriod};
	fptr s2[3] =
	{	&IsNoun, &IsBe, &IsPeriod};
	fptr s3[5] =
	{	&IsNoun, &IsDestination, &IsVerb, &IsTense, &IsPeriod};
	fptr s4[5] =
	{	&IsNoun, &IsObject, &IsVerb, &IsTense, &IsPeriod};
	fptr s5[7] =
	{	&IsNoun, &IsObject, &IsNoun, &IsDestination, &IsVerb, &IsTense, &IsPeriod};
	vector<fptr> v1(s1, s1 + sizeof(s1) / sizeof(s1[0]));
	vector<fptr> v2(s2, s2 + sizeof(s2) / sizeof(s2[0]));
	vector<fptr> v3(s3, s3 + sizeof(s3) / sizeof(s3[0]));
	vector<fptr> v4(s4, s4 + sizeof(s4) / sizeof(s4[0]));
	vector<fptr> v5(s5, s5 + sizeof(s5) / sizeof(s5[0]));
	ruleList.push_back(v1);
	ruleList.push_back(v2);
	ruleList.push_back(v3);
	ruleList.push_back(v4);
	ruleList.push_back(v5);


	if (parseList.empty())
	{
		cout << "ERROR: Nothing to parse\n" << endl;
		return false;
	}

	vector<string> sublist;
	//<story> 	::= <s> [<s>]
	cout << "Processing: <story>\n";
	do
	{
		valid = false;
		//<s> 		::= [CONNECTOR] <noun> SUBJECT (<s1> | <s2> | <s3> | <s4> | <s5>)
		cout << "Processing: <s>\n";
		if (isPart && index + 3 < (signed) parseList.size())
		{
			//optional connector

			if (Expected(&IsConnector, parseList[index]))
			{
				index++;
			}
			//check the noun
			bool (*compFunc[2])(tokentype) =
			{	&IsNoun, &IsSubject};

			sublist.assign(parseList.begin() + index, parseList.begin() + index
					+ sizeof(compFunc) / sizeof(compFunc[0]));

			if (isPart = Expected(compFunc, sublist))
			{
				index += sizeof(compFunc) / sizeof(compFunc[0]);
			}
		}

		int i = 0;
		while (isPart && !valid && i < (signed) ruleList.size())
		{
			if (ruleList[i].size() + index <= parseList.size())
			{
				cout << "Processing: <s" << i + 1 << ">\n";
				sublist.assign(parseList.begin() + index, parseList.begin()
						+ index + ruleList[i].size());
				if(valid = Expected(ruleList[i], sublist))
				{
					index += sublist.size();
				}
			}
			i++;
		}

	}
	while (index < (signed) parseList.size() && valid);


	return (valid);
}

bool Expected(bool(*compFunc[])(tokentype), vector<string> sublist)
{
	bool valid = true;

	int index = 0;
	while (valid && index < (signed) sublist.size())
	{
		valid = Expected(compFunc[index], sublist[index]);

		index++;
	}
	return valid;
}

bool Expected(vector<fptr> compFunc, vector<string> sublist)
{
	bool valid = true;

	//if the size of lists do not match
	if (sublist.size() != compFunc.size())
	{
		valid = false;
		cout << "\nERROR: not matching sizes (" << compFunc.size() << ":"
				<< sublist.size() << ")\n";
	}

	int index = 0;
	while (valid && index < (signed) sublist.size())
	{
		valid = Expected(compFunc[index], sublist[index]);

		/*
		if(!valid)
		{
			cout << "Expected: " << FptrToString(compFunc[index])
				<< " but found " << sublist[index] << endl;
		}*/
		index++;
	}


	return valid;
}

/***********************************************************************
 * Expected
 * ---------------------------------------------------------
 * Expected takes a comparator function and a string. It looks up the
 * the string in the symbol table and if it exists returns the type.
 * Otherwise it performs a scan on the string and determines it type.
 * Then it compares that type to the comparator functions and returns
 * true or false.
 * ---------------------------------------------------------
 * @param compFunc   - comparator function
 * @param compStr 	 - string to check
 * @return valid	 - If the string's tokentype is what is expected
 ***********************************************************************/
bool Expected(fptr compFunc, string compStr)
{
	bool valid;
	tokentype type;
	map<string, tokentype>::iterator iter;

	iter = symbolTable.find(compStr);
	//if the word is not in the lexicon
	if (iter == symbolTable.end())
	{
		//call the scanner for its type
		type = scanner.Scan(compStr);
		//insert it into the lexicon
		symbolTable.insert(pair<string, tokentype> (compStr, type));
	}
	//already inserted into lexicon look up its type
	else
	{
		type = iter->second;
	}

	//match expected results
	if(valid = Match(compFunc, type))
	{
		cout << "Matched " << FptrToString(compFunc) << endl;
	}
	else if(compFunc != &IsConnector)
	{
		cout << "Expected: " << FptrToString(compFunc)
			<< " but found " << compStr << endl;
	}

	return valid;
}

string FptrToString(fptr fp)
{
	string rtrString;
		if(fp == IsObject)
			rtrString = "OBJECT";
		else if(fp == IsDestination)
			rtrString = "DESTINATION";
		else if(fp == IsSubject)
			rtrString = "SUBJECT";
		else if(fp == IsConnector)
			rtrString = "CONNECTOR";
		else if(fp == IsPeriod)
			rtrString = "PERIOD";
		else if(fp == IsVerb)
			rtrString = "VERB";
		else if(fp == IsNoun)
			rtrString = "NOUN";
		else if(fp == IsBe)
			rtrString = "BE";
		else if(fp == IsTense)
			rtrString = "TENSE";
		else
			rtrString = "Undefined";
	return rtrString;
}

