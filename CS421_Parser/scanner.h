#ifndef SCANNER_H_
#define SCANNER_H_

#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>

using namespace std;

typedef enum tokentype
{
	ERROR, WORD1, WORD2, PERIOD, VERB,
	VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS,
	OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR,
	EOFM
};

class Scanner
{
public:
	tokentype StrToTokentype(string str);
	bool IsWord(string parseStr);
	bool IsPeriod(string parseStr);
	string TokenTypeStr(tokentype t);
	tokentype Scan(string s);
	tokentype WordType(string parsedStr);
};


//State: No Bugs
//By:Corey Paxton
tokentype Scanner::StrToTokentype(string str)
{
	tokentype returnType;
	if(str.compare("WORD1") == 0)
	{
		returnType = WORD1;
	}
	else if(str.compare("WORD2") == 0)
	{
		returnType = WORD2;
	}
	else if(str.compare("PERIOD") == 0)
	{
		returnType = PERIOD;
	}
	else if(str.compare("VERB") == 0)
	{
		returnType = VERB;
	}
	else if(str.compare("VERBNEG") == 0)
	{
		returnType = VERBNEG;
	}
	else if(str.compare("VERBPAST") == 0)
	{
		returnType = VERBPAST;
	}
	else if(str.compare("VERBPASTNEG") == 0)
	{
		returnType = VERBPASTNEG;
	}
	else if(str.compare("IS") == 0)
	{
		returnType = IS;
	}
	else if(str.compare("WAS") == 0)
	{
		returnType = WAS;
	}
	else if(str.compare("OBJECT") == 0)
	{
		returnType = OBJECT;
	}
	else if(str.compare("SUBJECT") == 0)
	{
		returnType = SUBJECT;
	}
	else if(str.compare("DESTINATION") == 0)
	{
		returnType = DESTINATION;
	}
	else if(str.compare("PRONOUN") == 0)
	{
		returnType = PRONOUN;
	}
	else if(str.compare("CONNECTOR") == 0)
	{
		returnType = CONNECTOR;
	}
	else if(str.compare("EOFM") == 0)
	{
		returnType = EOFM;
	}
	else
	{
		returnType = ERROR;
	}

	return returnType;
}

//State: No Bugs
//By:Rowelle Jay Tiburcio
string Scanner::TokenTypeStr(tokentype t)
{
	string toString;
	switch (t)
	{
	case ERROR:
		toString = "ERROR";
		break;
	case WORD1:
		toString = "WORD1";
		break;
	case WORD2:
		toString = "WORD2";
		break;
	case PERIOD:
		toString = "PERIOD";
		break;
	case VERB:
		toString = "VERB";
		break;
	case VERBNEG:
		toString = "VERBNEG";
		break;
	case VERBPAST:
		toString = "VERBPAST";
		break;
	case VERBPASTNEG:
		toString = "VERBPASTNEG";
		break;
	case IS:
		toString = "IS";
		break;
	case WAS:
		toString = "WAS";
		break;
	case OBJECT:
		toString = "OBJECT";
		break;
	case SUBJECT:
		toString = "SUBJECT";
		break;
	case DESTINATION:
		toString = "DESTINATION";
		break;
	case PRONOUN:
		toString = "PRONOUN";
		break;
	case CONNECTOR:
		toString = "CONNECTOR";
		break;
	case EOFM:
		toString = "EOFM";
		break;
	default:
		toString = "Not possible";
		break;
	}
	return toString;
}
//SCANNER
//State: No Bugs
//By:Corey Paxton
//REGULAR EXPRESSION:
//(vowel | vowel n | consonant vowel | consonant vowel n |
//consonant-pair vowel | consonant-pair vowel n)^+
//if it ends in a | u | o | n WORD1
//if it ends in i | e         WORD2
tokentype Scanner::Scan(string s)
{
	//  cout << "Scanner was called..." << endl;
	tokentype type;
	if (IsPeriod(s))
	{
		type = PERIOD;
	}
	else if (IsWord(s))
	{
		type = WordType(s);
	}
	else
	{
		//cout << "Lexical Error: The string is not in my language" << endl;
		type = ERROR;
	}

	return type;
}
//if it ends in a | u | o | n WORD1
//if it ends in i | e         WORD2
//State: No Bugs
//By:Rowelle Jay Tiburcio
tokentype Scanner::WordType(string parsedStr)
{
	tokentype type;
	//wasn't really a parsed str if this fails, but check for boundary
	if (parsedStr.length() > 1)
	{
		switch (parsedStr[parsedStr.length() - 1])
		{
		case 'a':
		case 'u':
		case 'o':
		case 'n':
			type = WORD1;
			break;
		case 'i':
		case 'e':
			type = WORD2;
			break;
		default:
			type = ERROR;
			break;
		}
	}
	else
	{
		type = ERROR;
	}

	return type;
}
//By:Rowelle Jay Tiburcio
bool Scanner::IsPeriod(string parseStr)
{
	bool isPeriod = false;
	if (parseStr.length() == 1)
	{
		if (parseStr[0] == '.')
		{
			isPeriod = true;
		}
	}

	return isPeriod;
}
//REGULAR EXPRESSION:
//(vowel | vowel n | consonant vowel | consonant vowel n |
//consonant-pair vowel | consonant-pair vowel n)^+
//WORD DFA
//State: No Bugs
//By:Dennis Zheng, Corey Paxton, Rowelle Jay Tiburcio
bool Scanner::IsWord(string parseStr)
{
	bool isWord = true;//
	int state = 0;//Inital State
	int pos = 0;//Initial Position

	//position of the string and valid char
	while (pos < (signed) parseStr.length() && isWord)
	{
		//cout << "current state: " << state << endl;//This current state is displayed
		//cout << "character: " << parseStr[pos];//The current chracter is displayed
		//cout << parseStr[pos];
		switch (parseStr[pos])
		{
		case 'a':
		case 'i':
		case 'u':
		case 'e':
		case 'o'://If it is a vowel
			//every state but c
			if (state != 5)
			{
				state = 1;
			}
			else
			{
				isWord = false;
			}

			break;

		case 'n'://If it is an 'n'
			if (state == 0 || state == 7)
			{
				state = 3;
			}
			else if (state == 1)
			{
				state = 7;
			}
			else
			{
				isWord = false;
			}
			break;

		case 'd':
		case 'j':
		case 'w':
		case 'z'://If it is 'd,j,w,z'
			if (state == 0 || state == 1 || state == 7)
			{
				state = 2;
			}
			else
			{
				isWord = false;
			}
			break;

		case 'b':
		case 'g':
		case 'k':
		case 'm':
		case 'p':
		case 'r'://If it is 'b,h,k,m,p,r'
			if (state == 0 || state == 1 || state == 7)
			{
				state = 3;
			}
			else
			{
				isWord = false;
			}
			break;

		case 't'://If it is 't'
			if (state == 0 || state == 1 || state == 7)
			{
				state = 4;
			}
			else
			{
				isWord = false;
			}
			break;

		case 'c'://If it is 'c'
			if (state == 0 || state == 1 || state == 7)
			{
				state = 5;
			}
			else
			{
				isWord = false;
			}
			break;

		case 's'://If it is 's'
			if (state == 0 || state == 1 || state == 7)
			{
				state = 6;
			}
			//ts
			else if (state == 4)
			{
				state = 6;
			}
			else
			{
				isWord = false;
			}
			break;

		case 'h'://If it is 'h'
			if (state == 0 || state == 1 || state == 7)
			{
				state = 3;
			}
			//ch sh
			else if (state == 5 || state == 6)
			{
				state = 8;
			}
			else
			{
				isWord = false;
			}
			break;

		case 'y'://If it is 'y'
			if (state == 0 || state == 1 || state == 7)
			{
				state = 2;
			}
			//by gy hy ky my ny py ry
			else if (state == 3)
			{
				state = 9;
			}
			else
			{
				isWord = false;
			}
			break;

		default:
			//invalid character
			//cout << "I am stuck in state" << state << endl;
			isWord = false;
			break;
		}

		pos++;
	}

	//one of the end states
	if (state != 1 && state != 7)
	{
		isWord = false;//If it does not end at oen fo the end states
	}
	return isWord;//Returns the word it is ends at an end state
}

#endif /* SCANNER_H_ */
