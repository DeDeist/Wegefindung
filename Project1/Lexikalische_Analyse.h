#pragma once
// k7scan1.cpp //

#pragma warning(disable:4786)
#include <iostream>
#include <stdio.h> 
#include <string>
#include <map>
#include "Lex_Citys.h"

using namespace std;
#define Getc(s) getc(s)
#define Ungetc(c) {ungetc(c,IP_Input); ugetflag=1;}
/*
* Lexical analyzer states.
*/
enum lexstate {
	L_START, L_INT, L_IDENT, L_STRING, L_STRING2,
	L_COMMENT, L_TEXT_COMMENT, L_LINE_COMMENT, L_END_TEXT_COMMENT
};
const int STRING1 = 3;
const int IDENTIFIER = 4;
const int INTEGER1 = 5;
const int TOKENSTART = 300;
const int TIME1 = 299;
const int POSE1 = 298;
const int NEIGHBOURS1 = 297;
const int NAME1 = 296;

class CParser
{
public:
	string yytext; //input buffer
	struct tyylval { //value return structure
		string s; //string
		int i; //integer
	}yylval;
	FILE* IP_Input; //Input File
	FILE* IP_Error; //Error Output
	FILE* IP_List; //List Output
	int IP_LineNumber; //Line counter
	int ugetflag; //checks ungets
	int prflag; //controls printing
	map<string, int> IP_Token_table; //Tokendefinitions
	map<int, string> IP_revToken_table; //reverse Tokendefinitions
	int yylex(); //lexical analyser
	void yyerror(char* ers); //error reporter
	int IP_MatchToken(string& tok); //checks the token
	void InitParse(FILE* inp, FILE* err, FILE* lst); //Sets the File pointers

	void pr_tokentable(); //test output for tokens
	map<int, City*> yyparse();
	void IP_init_token_table(); //loads the tokens
	void Load_tokenentry(string str, int index); //load one token
	void PushString(char c); //Used for dtring assembly
	CParser() { IP_LineNumber = 1; ugetflag = 0; prflag = 0; }; //Constructor
};
//------------------------------------------------------------------------
// Adds a character to the string value
void CParser::PushString(char c)
{
	yylval.s += c;
}
//------------------------------------------------------------------------
void CParser::Load_tokenentry(string str, int index)
{
	IP_Token_table[str] = index;
	IP_revToken_table[index] = str;
}
void CParser::IP_init_token_table()
{
	Load_tokenentry("STRING1", 3);
	Load_tokenentry("IDENTIFIER", 4);
	Load_tokenentry("INTEGER1", 5);

	int ii = TOKENSTART;

	Load_tokenentry("Time", TIME1);
	Load_tokenentry("Pose", POSE1);
	Load_tokenentry("Neighbours", NEIGHBOURS1);
	Load_tokenentry("Name", NAME1);



}
//------------------------------------------------------------------------
void CParser::pr_tokentable()
{
	typedef map<string, int>::const_iterator CI;
	const char* buf;
	printf("Symbol Table ---------------------------------------------\n");
	for (CI p = IP_Token_table.begin(); p != IP_Token_table.end(); ++p) {
		buf = p->first.c_str();
		printf(" key:%s val:%d\n ", buf, p->second);
	}
}
//------------------------------------------------------------------------

/*
Funktion zur Erstellung der Map im Format: map<std::string, City*>

-> Kein übergabeparameter

-- Funktion wertet die von yylex() übergebenen Token anhand der ASCII-Tabelle oder den in: IP_init_token_table() definierten Token aus
   und ruft dann die entsprechende Methode des erstellten Objekts "Citys" auf die lediglich die übergebenen Vektoren  und Strings abspeichert
-- Erstellt wird dabei eine Map mit Objekten vom Typ: map<std::string, City*> auf die über die Städtenamen zugegriffen werden kann.


<- gibt erstellte Map zurück
*/
map<int, City*> CParser::yyparse()
{
	map<int, City*> Citys;
	string NAME;
	int ind = 0;
	int tok;
	if (prflag)fprintf(IP_List, "%5d ", (int)IP_LineNumber);

	tok = yylex();

	while (tok != 0)
	{


		if (tok == NAME1)
		{
			tok = yylex();

			while (tok != IDENTIFIER)
			{
				tok = yylex();

				if (tok == IDENTIFIER)
				{
					NAME = yylval.s;

					Citys.insert(make_pair(ind, new City()));  // Erstellt Spalte in der Map: "All_Citys" in der zum einen Der Name der Stadt und dessen Instanz abgelegt sind
					Citys[ind]->set_City_Name(NAME);// Name der Stadt in das Objekt schreiben

					while (tok != 0 && tok != 125)
					{
						tok = yylex();

						if (tok == POSE1) // Einlesen der Position
						{
							vector<float> poses;
							while (tok != 93)
							{
								tok = yylex();
								if (tok == INTEGER1)
								{

									poses.push_back(yylval.i);

								}
							}
							Citys[ind]->set_pos(poses);

						}
						else if (tok == TIME1)// Einlesen der Zeiten von Ort zu Ort
						{
							vector<float> times;
							while (tok != 93)
							{

								tok = yylex();
								if (tok == INTEGER1)
								{
									times.push_back(yylval.i);
								}
							}
							Citys[ind]->set_Time(times);
						}
						else if (tok == NEIGHBOURS1)
						{
							vector<string> neighbours;
							while (tok != 93)
							{

								tok = yylex();
								if (tok == IDENTIFIER)
								{
									neighbours.push_back(yylval.s);
								}
							}
							Citys[ind]->set_neighbours(neighbours);
						}
						else if (tok == 125) break;

					};
					ind++;
					break;

				}
			};
		}

		tok = yylex();
	}
	return Citys;
}
//------------------------------------------------------------------------
/*
* Parse File:
*
*/
void CParser::InitParse(FILE* inp, FILE* err, FILE* lst)
{
	/*
	* Set up the file state to something useful.
	*/
	IP_Input = inp;
	IP_Error = err;
	IP_List = lst;
	IP_LineNumber = 1;
	ugetflag = 0;
	/*
	* Define both the enabled token and keyword strings.
	*/
	IP_init_token_table();
}
//------------------------------------------------------------------------
int CParser::IP_MatchToken(string& tok)
{
	int retval;
	if (IP_Token_table.find(tok) != IP_Token_table.end()) {
		retval = (IP_Token_table[tok]);
	}
	else {
		retval = (0);
	}
	return retval;
}
//------------------------------------------------------------------------
/*
* yylex:
*
*/
int CParser::yylex()
{
	//Locals
	int c;
	lexstate s;
	/*
	* Keep on sucking up characters until we find something which
	* explicitly forces us out of this function.
	*/
	for (s = L_START, yytext = ""; 1;) {
		c = Getc(IP_Input);
		yytext = yytext + (char)c;
		if (!ugetflag) {
			if (c != EOF)if (prflag)fprintf(IP_List, "%c", c);
		}
		else ugetflag = 0;
		switch (s) {
			//Starting state, look for something resembling a token.
		case L_START:
			if (isdigit(c)) {
				s = L_INT;
			}
			else if (isalpha(c) || c == '\\') {
				s = L_IDENT;
			}
			else if (isspace(c)) {
				if (c == '\n') {
					IP_LineNumber += 1;
					if (prflag)
						fprintf(IP_List, "%5d ", (int)IP_LineNumber);
				}
				yytext = "";
			}
			else if (c == '/') {
				yytext = "";
				s = L_COMMENT;
			}
			else if (c == '"') {
				s = L_STRING;
			}
			else if (c == EOF) {
				return ('\0');
			}
			else {
				return (c);
			}
			break;
		case L_COMMENT:
			if (c == '/')
				s = L_LINE_COMMENT;
			else if (c == '*')
				s = L_TEXT_COMMENT;
			else {
				Ungetc(c);
				return('/'); /* its the division operator not a comment */
			}
			break;
		case L_LINE_COMMENT:
			if (c == '\n') {
				s = L_START;
				Ungetc(c);
			}
			yytext = "";
			break;
		case L_TEXT_COMMENT:
			if (c == '\n') {
				IP_LineNumber += 1;
			}
			else if (c == '*')
				s = L_END_TEXT_COMMENT;
			yytext = "";
			break;
		case L_END_TEXT_COMMENT:
			if (c == '/') {
				s = L_START;
			}
			else {
				s = L_TEXT_COMMENT;
			}
			yytext = "";
			break;
			/*
			* Suck up the integer digits.
			*/
		case L_INT:
			if (isdigit(c)) {
				break;
			}
			else {
				Ungetc(c);
				yylval.s = yytext.substr(0, yytext.size() - 1);
				yylval.i = atoi(yylval.s.c_str());
				return (INTEGER1);
			}
			break;
			/*
* Grab an identifier, see if the current context enables
* it with a specific token value.
*/
		case L_IDENT:
			if (isalpha(c) || isdigit(c) || c == '_')
				break;
			Ungetc(c);
			yytext = yytext.substr(0, yytext.size() - 1);
			yylval.s = yytext;
			if (c = IP_MatchToken(yytext)) {
				return (c);
			}
			else {
				return (IDENTIFIER);
			}
			/*
			* Suck up string characters but once resolved they should
			* be deposited in the string bucket because they can be
			* arbitrarily long.
			*/
		case L_STRING2:
			s = L_STRING;
			if (c == '"') {
				PushString((char)c);
			}
			else {
				if (c == '\\') {
					PushString((char)c);
				}
				else {
					PushString((char)'\\');
					PushString((char)c);
				}
			}
			break;
		case L_STRING:
			if (c == '\n')
				IP_LineNumber += 1;
			else if (c == '\r')
				;
			else if (c == '"' || c == EOF) {
				return (STRING1);
			}
			else if (c == '\\') {
				s = L_STRING2;
			}
			else
				PushString((char)c);
			break;
		default: printf("***Fatal Error*** Wrong case label in yylex\n");
		}
	}
}
/*
//------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	FILE* inf;
	errno_t err;
	char fistr[100];
	printf("Enter filename:\n");
	gets_s(fistr);
	err = fopen_s(&inf, fistr, "r");
	if (err != NULL) {
		printf("Cannot open input file %s\n", fistr);
		return 0;
	}
	CParser obj;
	obj.InitParse(inf, stderr, stdout);
	// obj.pr_tokentable();
	obj.yyparse();

	cout << "\n";

	cout << "\n";

	cout << "\n";
	for (map<string, int> ::const_iterator it = obj.IP_Token_table.begin();
		it != obj.IP_Token_table.end(); ++it)
	{
		cout << it->first << "\t" << it->second << endl;
	}
	return 0;
}*/
