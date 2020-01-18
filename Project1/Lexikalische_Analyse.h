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
enum lexstate { // lexstate ist ein Aufzählungtyp und beinhaltet die folgendnen "Zustände". Bsp: L_START = 0, L_INT = 1,...
	L_START, L_INT, L_IDENT, L_STRING, L_STRING2,
	L_COMMENT, L_TEXT_COMMENT, L_LINE_COMMENT, L_END_TEXT_COMMENT
};
const int STRING1 = 3;
const int IDENTIFIER = 4;
const int INTEGER1 = 5;
const int TOKENSTART = 300;
const int TIME1 = 301;
const int POSE1 = 302;
const int NEIGHBOURS1 = 303;
const int NAME1 = 304;

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
void CParser::Load_tokenentry(string str, int index) // Schreibt die in IP_init_token_table() erstellten token in sie beidne Token-Tabellen 
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

	Load_tokenentry("Time", TIME1);					//Eigens hinzugefügter Token für yyparce()
	Load_tokenentry("Pose", POSE1);					//Eigens hinzugefügter Token für yyparce()
	Load_tokenentry("Neighbours", NEIGHBOURS1);		//Eigens hinzugefügter Token für yyparce()
	Load_tokenentry("Name", NAME1);					//Eigens hinzugefügter Token für yyparce()



}
//------------------------------------------------------------------------
void CParser::pr_tokentable() // Ausgabe der Tokentabelle -> wird nicht verwendet
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
	vector<float> times;
	vector<float> poses;
	vector<string> neighbours;
	string NAME;
	int ind = 0;
	int tok;
	int PARSE_STATE = 0;
	if (prflag)fprintf(IP_List, "%5d ", (int)IP_LineNumber);

	while ((tok = yylex()) != 0)
	{
		//printf("%d ", tok);
		if (tok == INTEGER1)	// ist tok ein Integer? j/n
		{
			if (PARSE_STATE == TIME1) // wenn PARSE_STATE == TIME1 dann ist der Integer ein Zeit- bzw. Kostenfaktor
			{
				times.push_back(yylval.i);
			};

			if (PARSE_STATE == POSE1) // wenn PARSE_STATE == POSE1 dann ist der Integer eine Koordinate
			{
				poses.push_back(yylval.i);
			};
		}
		else
			if (tok == IDENTIFIER)		// ist tok ein Identifier (Wort)? j/n
			{
				if (PARSE_STATE == NAME1)		// wenn PARSE_STATE == NAME1 dann ist der Identiefier der Stadtname
				{
					NAME = yylval.s;

					Citys.insert(make_pair(ind, new City()));  // Erstellt Spalte in der Map: "All_Citys" in der zum einen Der Name der Stadt und dessen Instanz abgelegt sind
				};
				if (PARSE_STATE == NEIGHBOURS1) // wenn PARSE_STATE == NAME1 dann ist der Identifier ein Nachbar
				{
					neighbours.push_back(yylval.s);
				};
			}
			else
				if (tok >= TOKENSTART) // Wenn tok >= TOKENSTART, dann wird der ensprechnde Status(PARSE_STATE) gesetzt
				{
					PARSE_STATE = tok;
				}
				else
				{
					if (tok == '}')		// Nach ende eines Datenstzes, werden die Objektmethoden zum einlesen der Werte aufgerufen und die Vektoren Initialisiert(.clear)
					{
						Citys[ind]->set_City_Name(NAME);// Name der Stadt in das Objekt schreiben
						Citys[ind]->set_pos(poses);
						Citys[ind]->set_neighbours(neighbours);
						Citys[ind]->set_Time(times);
						ind++;
						times.clear();
						poses.clear();
						neighbours.clear();
					};
				}

	}
	return Citys; // Rückgabewert ist die Map mit allen Objekten der einzelnen Städte
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
int CParser::IP_MatchToken(string& tok)		// Vergleich zweier token auf ihre Übereinstimmung
{
	int retval;
	if (IP_Token_table.find(tok) != IP_Token_table.end()) {
		retval = (IP_Token_table[tok]);		// wenn gleich
	}
	else {
		retval = (0);						// Wenn ungleich
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
	for (s = L_START, yytext = ""; 1;) // s wird mit L_START und yytext (Buffer) mit "" initialisiert
	{
		c = Getc(IP_Input);	// holt sich den Ascii-Wert auf den der FILE-Pointer zeigt
		yytext = yytext + (char)c; // schriebt sich den character des Ascii-Wertes in den Buffer
		if (!ugetflag) { // Errorabfrage
			if (c != EOF)if (prflag)fprintf(IP_List, "%c", c);
		}
		else ugetflag = 0;
		switch (s) {
			//Starting state, look for something resembling a token.
		case L_START: // 1. Fall -> s = L_START: 
			
			if (isdigit(c)) {	// isdigit() überprüft "c" darauf, eine Ganzzahl zwischen 0 und 9 zu sein 
				s = L_INT;		// wenn ja dann wird s = L_INT 
			}
			else if (isalpha(c) || c == '\\') {		//isalpha() überprüft "c" darauf ein Buchstabe oder "\\" zu sein
				s = L_IDENT;						//wenn ja dann 				
			}
			else if (isspace(c)) {					//Überprüft "c" darauf ein Leerzeichen, Tab oder "enld" zu sein
				if (c == '\n') {					// wenn endl dann Zeilennummer erhöhen
					IP_LineNumber += 1;
					if (prflag)
						fprintf(IP_List, "%5d ", (int)IP_LineNumber);
				}
				yytext = ""; // Buffer leeren
			}
			else if (c == '/') {		//  wenn c == '/' dann wird s auf L_COMMENT gesetzt
				yytext = "";			// Buffer leeren
				s = L_COMMENT;
			}
			else if (c == '"') {		// wenn c == '"' dann... s = L_STRING
				s = L_STRING;
			}
			else if (c == EOF) {		// wenn c == EOF springen wir aus der FOR-Schleife und aus der methode yylex()
				return ('\0');
			}
			else {						// falls der case nicht klar ist wird der character zurückgegeben  und yylex() beendet
				return (c);	
			}
			break; // Ende erster Case


			
		case L_COMMENT://-------------------------------- Bginn des L_COMMENT Case
			if (c == '/')
				s = L_LINE_COMMENT;
			else if (c == '*')
				s = L_TEXT_COMMENT;
			else {
				Ungetc(c);
				return('/'); /* its the division operator not a comment */
			}
			break;
		case L_LINE_COMMENT://-------------------------------- Bginn des L_LINE_COMMENT Case
			if (c == '\n') {
				s = L_START;// zurück zum Start
				Ungetc(c);
			}
			yytext = "";
			break;
		case L_TEXT_COMMENT://-------------------------------- Bginn des L_TEXT_COMMENT Case
			if (c == '\n') {
				IP_LineNumber += 1;
			}
			else if (c == '*')
				s = L_END_TEXT_COMMENT;
			yytext = "";
			break;
		case L_END_TEXT_COMMENT://-------------------------------- Bginn des L_END_TETX_COMMENT Case
			if (c == '/') {
				s = L_START;// zurück zum Start
			}
			else {
				s = L_TEXT_COMMENT;
			}
			yytext = ""; // Buffer leeren
			break;
			/*
			* Suck up the integer digits.
			*/
		case L_INT: // 
			if (isdigit(c)) { // Wenn ein C ein interger dann wird er zu beginn der schleife yytext hinzugefügt
				break;
			}
			else {  
				Ungetc(c); // Wenn c kein Integer dann wird c dem Datenstrom zurückgegeben
				yylval.s = yytext.substr(0, yytext.size() - 1); // formt einen String aus den in yytext stehenden charactern der Länge yytext.size()-1
				yylval.i = atoi(yylval.s.c_str()); // Formt einen Integer aus dem String in yylval.s
				return (INTEGER1); // der Token INTEGER1 wird an yyparse() zurückgegeben und kann die Zahl in yylval aufgerufen werden
			}
			break;
			/*
* Grab an identifier, see if the current context enables
* it with a specific token value.
*/
		case L_IDENT:
			if (isalpha(c) || isdigit(c) || c == '_' || c == '-')  // Wenn ein C ein character (oder wie def.) dann wird er zu beginn der schleife yytext hinzugefügt
				break;
			Ungetc(c);
			yytext = yytext.substr(0, yytext.size() - 1); // formt einen String aus den in yytext stehenden charactern der Länge yytext.size()-1
			yylval.s = yytext; // Schreibt den String aus yytext in yylaval.s
			if (c = IP_MatchToken(yytext)) { // hier wird überprüft ob es sich dei dem IDENTIFIER um einen in der Tokentable definierten token handelt
				return (c);					// der dann an yyparse() zurückgegeben wird
			}
			else {
				return (IDENTIFIER);// IDENTIEFIER wird  an yyparse() zurückgegeben 
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
