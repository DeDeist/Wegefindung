#include "MyForm.h"
#include "iostream"
#include <iomanip>
#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
#include <chrono>

#include "Lexikalische_Analyse.h"
#include "Lex_Functions.h"
#include "astar_class.h"
#include "dijkstra_class.h"

using namespace System;
using namespace std;

[STAThreadAttribute]

int main()
{
	Windows::Forms::Application::EnableVisualStyles();
	Windows::Forms::Application::Run(gcnew Project1::MyForm());	
	return 0;
}