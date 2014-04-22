#include "scanner.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;

Scanner scanner;
map<string, tokentype> symbolTable;
typedef bool (*fptr)(tokentype);

//vector< vector<fptr> > ruleList;


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

/***********************************************************************
 * Match
 * ------------------------------------------
 * @param compFunc(tokentype) - Allows us to pass a comparator function
 * @param type - tokentype we want to compare
 ***********************************************************************/
bool Match(bool(*compFunc)(tokentype), tokentype type)
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
	return (type == VERB || type == WORD2);
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
	string input;
	string strType;
	tokentype type;
	fstream file;
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

	//tokentype type = VERBPASTNEG;
	//int index = 0;
	//bool (*compFunc)(tokentype) = &IsTense;

	//array of function pointers
	//fptr compFuncTest[3] = {&IsNoun, &IsPeriod, &IsVerb};
	//vector<fptr> compFunc (compFuncTest, compFuncTest + sizeof(compFuncTest)/sizeof(compFuncTest[0]));


	//example string list to test
	vector<string> testList;
	//mata rika wa masu mashita . rule 1 worked
	/*
	testList.push_back("mata");
	testList.push_back("rika");
	testList.push_back("wa");
	testList.push_back("masu");
	testList.push_back("mashita");
	testList.push_back(".");
*/

	//mata rika wa rika deshita . rule 2 worked
	/*
	testList.push_back("mata");
	testList.push_back("rika");
	testList.push_back("wa");
	testList.push_back("rika");
	testList.push_back("deshita");
	testList.push_back(".");
*/
	testList.push_back("mata");
	testList.push_back("rika");
	testList.push_back("wa");
	testList.push_back("rika");
	testList.push_back("deshita");
	testList.push_back(".");
	testList.push_back("mata");
	testList.push_back("rika");
	testList.push_back("wa");
	testList.push_back("rika");
	testList.push_back("ni");
	testList.push_back("wanai");
	testList.push_back("masendeshita");
	testList.push_back(".");

	//array of expected inputs
	//tokentype type[3] = {CONNECTOR, WORD1, PERIOD};
	//bool matches = Match(compFunc, index, type);
	//bool good2go = Expected(compFunc, testList);
	bool good2go = Parse(testList);
	/*
	 while(good2go && index < 3)
	 {
	 cout << "Index: " << index ;
	 //tests whether the input fits an expected input
	 good2go = Match(compFunc[index], type[index]);
	 cout << " Matches: " << good2go << endl;
	 index++;
	 }*/

	cout << "Expected test: " << good2go << endl;
	//bool g2g2 = Expected(&IsNoun, "rika");
	//cout << "\nExpected test 2: " << g2g2;

	//output all the symbols
	for (map<string, tokentype>::iterator it = symbolTable.begin(); it
			!= symbolTable.end(); ++it)
	{
		cout <<  it->first << " " << scanner.TokenTypeStr(it->second) << endl;
	}
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

	vector<vector<fptr> > ruleList;
	bool (*rule1[3])(tokentype) =
	{	&IsVerb, &IsTense, &IsPeriod};
	bool (*rule2[3])(tokentype) =
	{	&IsNoun, &IsBe, &IsPeriod};
	bool (*rule3[5])(tokentype) =
	{	&IsNoun, &IsDestination, &IsVerb, &IsTense, &IsPeriod};
	bool (*rule4[5])(tokentype) =
	{	&IsNoun, &IsObject, &IsVerb, &IsTense, &IsPeriod};
	bool (*rule5[7])(tokentype) =
	{	&IsNoun, &IsObject, &IsNoun, &IsDestination, &IsVerb, &IsTense, &IsPeriod};
	vector<fptr> v1(rule1, rule1 + sizeof(rule1) / sizeof(rule1[0]));
	vector<fptr> v2(rule2, rule2 + sizeof(rule2) / sizeof(rule2[0]));
	vector<fptr> v3(rule3, rule3 + sizeof(rule3) / sizeof(rule3[0]));
	vector<fptr> v4(rule4, rule4 + sizeof(rule4) / sizeof(rule4[0]));
	vector<fptr> v5(rule5, rule5 + sizeof(rule5) / sizeof(rule5[0]));
	ruleList.push_back(v1);
	ruleList.push_back(v2);
	ruleList.push_back(v3);
	ruleList.push_back(v4);
	ruleList.push_back(v5);

	if (parseList.empty())
	{
		return false;
	}

	vector<string> sublist;
	do
	{
		valid = false;
		//s1
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
				sublist.clear();
				sublist.assign(parseList.begin() + index, parseList.begin()
						+ index + ruleList[i].size());
				if(valid = Expected(ruleList[i], sublist))
				{
					cout << "Rule #:" << i+1 << endl;
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

	//cout << "\nArray size in func " << sizeof(compFunc)/sizeof(compFunc[0]) << endl;

	/*
	 //if the size of lists do not match
	 if (sublist.size() != (sizeof(compFunc) / sizeof(compFunc[0])))
	 {
	 valid = false;
	 cout << "\nERROR: not matching sizes (" << sublist.size() << ":"
	 << (sizeof(compFunc)/sizeof(compFunc[0])) << ")\n";
	 }*/

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
	tokentype type;
	map<string, tokentype>::iterator iter;

	//TODO modularize this
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
	return Match(compFunc, type);
}

