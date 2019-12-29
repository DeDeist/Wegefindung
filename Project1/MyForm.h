#pragma once
#include "string"
#include "GUI.h"
#include "iostream"
#include <msclr/marshal_cppstd.h>
#include "Lexikalische_Analyse.h"
#include "Lex_Functions.h"
#include "astar_class.h"
#include "dijkstra_class.h"

namespace Project1 {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Text;

	vector<vector<float>> city_positions;
	vector<vector<float>> adjazenzmatrix;
	map<int, City*> all_citys;


	/// <summary>
	/// Zusammenfassung für MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm()
		{
			
			InitializeComponent();
			InitializeDrwatools();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			
		}

	private:
		void drawpoint(int x, int y)
		{
			this->map_area_graph ->FillEllipse(red_point, x, y, 10, 10);
		}

		void set_citys_map(map<int, City*> set_map)
		{
			all_citys = set_map;
		}

		void set_adjazenzmatrix(vector<vector<float>> set_adja)
		{
			adjazenzmatrix = set_adja;
		}

		void set_city_positions(vector<vector<float>> set_city_pos)
		{
			city_positions = set_city_pos;
		}

		void drawpointandlabel(int x, int y, std::string lable_name)
		{
			System::String^ sys_str_lable_name = gcnew String(lable_name.c_str());
			this->drawpoint(x-5, y-5);			
			map_area_graph->DrawString(sys_str_lable_name, this->lable_font, this->text_brush,float (x+5),float (y-6));
		}

		void adddropdown(std::string new_drop)
		{
			System::String^ sys_str = gcnew String(new_drop.c_str());
			this->StartBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { sys_str });
			this->DestinationBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { sys_str });
		}

		void addCity(City* new_city)
		{
			drawpointandlabel(new_city->pos[0], new_city->pos[1], new_city->City_Name);
			adddropdown(new_city->City_Name);
		}

		void connect_city(City* one, City* two)
		{
			map_area_graph->DrawLine(black_line, one->pos[0], one->pos[1], two->pos[0], two->pos[1]);
		}
		
		void connect_city_astern(City* one, City* two)
		{
			map_area_graph->DrawLine(red_line, one->pos[0], one->pos[1], two->pos[0], two->pos[1]);
		}

		void connect_city_dij(City* one, City* two)
		{
			map_area_graph->DrawLine(green_line, one->pos[0], one->pos[1], two->pos[0], two->pos[1]);
		}

		int get_index(map<int, City*> City_Map, std::string City)
		{
			Boolean match = TRUE;
			int i = 0;
			for (i = 0; match && i<City_Map.size(); i++ )
			{
					match = strncmp(City_Map[i]->City_Name.c_str(), City.c_str(), City.size());
			}
			i--;
			if (match == TRUE) { i++;}
			
			return i;
		}


	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Panel^ MapArea;
	private: System::Windows::Forms::CheckBox^ AsternCheckBox;
	private: System::Windows::Forms::CheckBox^ DijkstraCheckBox;
	private: System::Windows::Forms::Button^ StartButton;
	private: System::Windows::Forms::ComboBox^ StartBox;
	private: System::Windows::Forms::ComboBox^ DestinationBox;
	private: System::Windows::Forms::Label^ FromLabel;
	private: System::Windows::Forms::Label^ ToLabel;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::LinkLabel^ linkLabel1;
	private: System::Windows::Forms::Button^ newMapButton;
	private: System::Windows::Forms::OpenFileDialog^ NewMapDialog;
	private: System::Windows::Forms::ErrorProvider^ errorProvider1;
	private: System::Windows::Forms::ComboBox^ EmptyBox;
	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	private: Graphics^ map_area_graph;
	private: Pen^ black_line;
	private: Pen^ green_line;
	private: Pen^ blue_line;
	private: Pen^ red_line;
	private: SolidBrush^ red_point;
	private: SolidBrush^ text_brush;
	private: System::Drawing::Font^ lable_font;
	private: System::Drawing::StringFormat^ text_format;
	private: System::ComponentModel::IContainer^ components;

	protected:

	public:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->MapArea = (gcnew System::Windows::Forms::Panel());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->AsternCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->DijkstraCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->StartButton = (gcnew System::Windows::Forms::Button());
			this->StartBox = (gcnew System::Windows::Forms::ComboBox());
			this->DestinationBox = (gcnew System::Windows::Forms::ComboBox());
			this->FromLabel = (gcnew System::Windows::Forms::Label());
			this->ToLabel = (gcnew System::Windows::Forms::Label());
			this->linkLabel1 = (gcnew System::Windows::Forms::LinkLabel());
			this->newMapButton = (gcnew System::Windows::Forms::Button());
			this->NewMapDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->errorProvider1 = (gcnew System::Windows::Forms::ErrorProvider(this->components));
			this->EmptyBox = (gcnew System::Windows::Forms::ComboBox());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->MapArea->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errorProvider1))->BeginInit();
			this->SuspendLayout();
			// 
			// MapArea
			// 
			this->MapArea->BackColor = System::Drawing::SystemColors::Window;
			this->MapArea->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"MapArea.BackgroundImage")));
			this->MapArea->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->MapArea->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->MapArea->Controls->Add(this->label3);
			this->MapArea->Location = System::Drawing::Point(9, 10);
			this->MapArea->Margin = System::Windows::Forms::Padding(2);
			this->MapArea->Name = L"MapArea";
			this->MapArea->Size = System::Drawing::Size(723, 879);
			this->MapArea->TabIndex = 0;
			this->MapArea->Click += gcnew System::EventHandler(this, &MyForm::MapArea_Click);
			this->MapArea->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MapArea_Paint);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(639, 408);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(0, 13);
			this->label3->TabIndex = 8;
			// 
			// AsternCheckBox
			// 
			this->AsternCheckBox->AutoSize = true;
			this->AsternCheckBox->Location = System::Drawing::Point(745, 145);
			this->AsternCheckBox->Margin = System::Windows::Forms::Padding(2);
			this->AsternCheckBox->Name = L"AsternCheckBox";
			this->AsternCheckBox->Size = System::Drawing::Size(61, 17);
			this->AsternCheckBox->TabIndex = 1;
			this->AsternCheckBox->Text = L"A-Stern";
			this->AsternCheckBox->UseVisualStyleBackColor = true;
			// 
			// DijkstraCheckBox
			// 
			this->DijkstraCheckBox->AutoSize = true;
			this->DijkstraCheckBox->Location = System::Drawing::Point(824, 145);
			this->DijkstraCheckBox->Margin = System::Windows::Forms::Padding(2);
			this->DijkstraCheckBox->Name = L"DijkstraCheckBox";
			this->DijkstraCheckBox->Size = System::Drawing::Size(61, 17);
			this->DijkstraCheckBox->TabIndex = 2;
			this->DijkstraCheckBox->Text = L"Dijkstra";
			this->DijkstraCheckBox->UseVisualStyleBackColor = true;
			// 
			// StartButton
			// 
			this->StartButton->BackColor = System::Drawing::SystemColors::Control;
			this->StartButton->Location = System::Drawing::Point(745, 167);
			this->StartButton->Margin = System::Windows::Forms::Padding(2);
			this->StartButton->Name = L"StartButton";
			this->StartButton->Size = System::Drawing::Size(177, 48);
			this->StartButton->TabIndex = 3;
			this->StartButton->Text = L"Start";
			this->StartButton->UseVisualStyleBackColor = false;
			this->StartButton->Click += gcnew System::EventHandler(this, &MyForm::StartButton_Click);
			this->StartButton->MouseLeave += gcnew System::EventHandler(this, &MyForm::StartButton_MouseLeave);
			this->StartButton->MouseHover += gcnew System::EventHandler(this, &MyForm::StartButton_MouseHover);
			// 
			// StartBox
			// 
			this->StartBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->StartBox->FormattingEnabled = true;
			this->StartBox->Location = System::Drawing::Point(745, 46);
			this->StartBox->Margin = System::Windows::Forms::Padding(2);
			this->StartBox->Name = L"StartBox";
			this->StartBox->Size = System::Drawing::Size(178, 21);
			this->StartBox->TabIndex = 4;
			this->StartBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::StartBox_SelectedIndexChanged);
			// 
			// DestinationBox
			// 
			this->DestinationBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->DestinationBox->FormattingEnabled = true;
			this->DestinationBox->Location = System::Drawing::Point(745, 104);
			this->DestinationBox->Margin = System::Windows::Forms::Padding(2);
			this->DestinationBox->Name = L"DestinationBox";
			this->DestinationBox->Size = System::Drawing::Size(178, 21);
			this->DestinationBox->TabIndex = 5;
			// 
			// FromLabel
			// 
			this->FromLabel->AutoSize = true;
			this->FromLabel->BackColor = System::Drawing::Color::Transparent;
			this->FromLabel->Location = System::Drawing::Point(742, 21);
			this->FromLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->FromLabel->Name = L"FromLabel";
			this->FromLabel->Size = System::Drawing::Size(33, 13);
			this->FromLabel->TabIndex = 6;
			this->FromLabel->Text = L"From:";
			// 
			// ToLabel
			// 
			this->ToLabel->AutoSize = true;
			this->ToLabel->Location = System::Drawing::Point(742, 80);
			this->ToLabel->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->ToLabel->Name = L"ToLabel";
			this->ToLabel->Size = System::Drawing::Size(23, 13);
			this->ToLabel->TabIndex = 7;
			this->ToLabel->Text = L"To:";
			// 
			// linkLabel1
			// 
			this->linkLabel1->AutoSize = true;
			this->linkLabel1->Location = System::Drawing::Point(869, 328);
			this->linkLabel1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->linkLabel1->Name = L"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(0, 13);
			this->linkLabel1->TabIndex = 8;
			// 
			// newMapButton
			// 
			this->newMapButton->BackColor = System::Drawing::SystemColors::Control;
			this->newMapButton->Location = System::Drawing::Point(745, 838);
			this->newMapButton->Margin = System::Windows::Forms::Padding(2);
			this->newMapButton->Name = L"newMapButton";
			this->newMapButton->Size = System::Drawing::Size(181, 52);
			this->newMapButton->TabIndex = 9;
			this->newMapButton->Text = L"New Map";
			this->newMapButton->UseVisualStyleBackColor = false;
			this->newMapButton->Click += gcnew System::EventHandler(this, &MyForm::newMapButton_Click);
			// 
			// NewMapDialog
			// 
			this->NewMapDialog->FileName = L"NewMapDialog";
			this->NewMapDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::NewMapDialog_FileOk);
			// 
			// errorProvider1
			// 
			this->errorProvider1->ContainerControl = this;
			// 
			// EmptyBox
			// 
			this->EmptyBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::Simple;
			this->EmptyBox->FormattingEnabled = true;
			this->EmptyBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Berlin" });
			this->EmptyBox->Location = System::Drawing::Point(1912, 893);
			this->EmptyBox->Margin = System::Windows::Forms::Padding(2);
			this->EmptyBox->Name = L"EmptyBox";
			this->EmptyBox->Size = System::Drawing::Size(10, 21);
			this->EmptyBox->TabIndex = 10;
			this->EmptyBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// richTextBox1
			// 
			this->richTextBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->richTextBox1->Location = System::Drawing::Point(745, 571);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(179, 262);
			this->richTextBox1->TabIndex = 12;
			this->richTextBox1->Text = L"A-Stern:\nKnoten durchlaufen:\t0\nBenöritgte Zeit\t\t0\n\nDjkstra:\nKnoten durchlaufen:\t0"
				L"\nBenöritgte Zeit\t\t0";
			this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::richTextBox1_TextChanged);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(985, 927);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->EmptyBox);
			this->Controls->Add(this->newMapButton);
			this->Controls->Add(this->linkLabel1);
			this->Controls->Add(this->ToLabel);
			this->Controls->Add(this->FromLabel);
			this->Controls->Add(this->DestinationBox);
			this->Controls->Add(this->StartBox);
			this->Controls->Add(this->StartButton);
			this->Controls->Add(this->DijkstraCheckBox);
			this->Controls->Add(this->AsternCheckBox);
			this->Controls->Add(this->MapArea);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"MyForm";
			this->Text = L"Wegefindung";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->MapArea->ResumeLayout(false);
			this->MapArea->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->errorProvider1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		void InitializeDrwatools(void)
		{
			this->map_area_graph = MapArea->CreateGraphics();
			this->black_line = gcnew Pen(Color::Black);
			this->green_line = gcnew Pen(Color::Green,6);
			this->blue_line = gcnew Pen(Color::Blue,3);
			this->red_line = gcnew Pen(Color::Red, 2);
			this->red_point = gcnew SolidBrush(Color::Red);
			this->lable_font = gcnew System::Drawing::Font("Arial", 10);
			this->text_brush = gcnew SolidBrush(Color::Blue);
			this->text_format = gcnew System::Drawing::StringFormat();
		}
		
	private: System::Void MapArea_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) 
	{
		
	}
	private: System::Void StartButton_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (StartBox->SelectedIndex == -1)
		{
			cout << "Wählen Sie einen Startort aus." << endl;
			return;
		}
		if (DestinationBox->SelectedIndex == -1)
		{
			cout << "Wählen Sie einen Zielort aus." << endl;
			return;
		}




		if (DijkstraCheckBox->Checked)
		{
			cout << "Dijkstra ausgewählt" << endl;
			dijkstra_algo dijk;
			cout << "DIJKSTRA:\n";
			vector<int> dijk_path = dijk.dijkstra(StartBox->SelectedIndex, DestinationBox->SelectedIndex, adjazenzmatrix);	//Ausführen des Dijkstra, Übergabe: Start, Ziel, Verbindungsmatrix. Rückgabe: Vektor mit Weg
			
			for (int ij = 0; ij < dijk_path.size(); ij++)
				cout << dijk_path[ij] << " ";				//dijk_path[]  Weg Ziel -> Start
			cout << "\nCycles: " << dijk.cycles << "\n";	//dijk.cycles Anzahl Durchläufe
			cout << "Elapsed time: " << dijk.elapsed_time.count() << " s\n";	//dijk.elapsesd_time.count() benötigte Zeit
			for (int i = 1; i < dijk_path.size(); i++)
			{
				connect_city_dij(all_citys[dijk_path[i - 1]], all_citys[dijk_path[i]]);

			}
		}	
		
		if (AsternCheckBox->Checked)
		{
			cout << "A-Stern ausgewählt" << endl;
			astar_algo a_star;
			cout << "A_STAR:\n";
			vector<int> astar_path = a_star.astar(StartBox->SelectedIndex, DestinationBox->SelectedIndex, adjazenzmatrix, city_positions);	//Ausführen des A*, Übergabe: Start, Ziel, Verbindungsmatrix, Koordinaten. Rückgabe ist der Vektor mit dem gefunden Weg
			
			for (int ij = 0; ij < astar_path.size(); ij++)
				cout << astar_path[ij] << " ";					//astar_path[] enthält jetzt in Umgekehrter Folge den Weg. Ziel -> Start
			cout << "\nCycles: " << a_star.cycles << "\n";		//a_star.cycles ist die Anzahl von Durchläufen bis der Weg gefunden wurde
			cout << "Elapsed time: " << a_star.elapsed_time.count() << " s\n"; //a_star.elapsed_time.count() enthält die dafür benötigte Zeit in sec.

			for (int i = 1; i < astar_path.size(); i++)
			{
				connect_city_astern(all_citys[astar_path[i - 1]], all_citys[astar_path[i]]);

			}
		}
			//		
			//#include <iostream>
			//#include <string>
			//#include <stdio.h>
			//#include <math.h>
			//#include <vector>
			//#include <algorithm>
			//#include <chrono>		//Bib für Zeitmessung, die Zeit ist aber sehr kurz unterschiede sind nicht wirklich gut erkennbar da die Zeit auch gut schwankt
			//
			//
			//#define xxx 10000.0		//def für "Unendlich" in der Verbindungsmatrix
			//		using namespace std;
			//
			//#include "astar_class.h"		//Algorithmen
			//#include "dijkstra_class.h"
			//
			//		void main()
			//		{
			//			int city_index_start, city_index_dest;		//index für Start und Ziel Stadt, sollte dann über GUI kommen
			//			city_index_start = 4;
			//			city_index_dest = 3;
			//
			//			vector<vector<float>> distance_matr =							//Matrix mit Verbindungsinfos, kommt von Matze
			//			{
			//			{ 0,xxx,xxx,205,xxx,xxx,284,282,xxx,179,xxx,xxx},
			//			{xxx, 0,233,xxx,xxx,xxx,119,125,xxx,xxx,xxx,xxx},
			//			{xxx,233, 0,xxx, 63,264,xxx,208, 83,xxx,xxx,xxx},
			//			{205,xxx,xxx, 0,xxx,xxx,xxx,xxx,xxx,108,xxx,xxx},
			//			{xxx,xxx, 63,xxx, 0,xxx,xxx,xxx, 47,xxx,xxx,xxx},
			//			{xxx,xxx,264,xxx,xxx, 0,xxx,352,189,395,400,217},
			//			{284,119,xxx,xxx,xxx,xxx, 0,154,xxx,xxx,xxx,xxx},
			//			{282,125,208,xxx,xxx,352,154, 0,xxx,256,xxx,xxx},
			//			{xxx,xxx, 83,xxx, 47,189,xxx,xxx, 0,xxx,xxx,xxx},
			//			{179,xxx,xxx,108,xxx,395,xxx,256,xxx, 0,425,xxx},
			//			{xxx,xxx,xxx,xxx,xxx,400,xxx,xxx,xxx,425, 0,220},
			//			{xxx,xxx,xxx,xxx,xxx,217,xxx,xxx,xxx,xxx,220, 0},
			//			};
			//
			//			vector<vector<float>> city_coords =								//Koordinaten der Städte, kommt von Matze
			//			{
			//			{115, 110},
			//			{20, 120},
			//			{15, 85},
			//			{130, 70},
			//			{5, 80},
			//			{35,45},
			//			{55, 130},
			//			{55, 105},
			//			{15, 65},
			//			{90,75},
			//			{90,5},
			//			{45, 10}
			//			};
			//
			//			astar_algo a_star;			//Objekt des A* Algorithmus anlegen
			//			dijkstra_algo dijk;			//Objekt des Dijkstra Algorithmus anlegen
			//
			//			cout << "A_STAR:\n";
			//			vector<int> astar_path = a_star.astar(city_index_start, city_index_dest, distance_matr, city_coords);	//Ausführen des A*, Übergabe: Start, Ziel, Verbindungsmatrix, Koordinaten. Rückgabe ist der Vektor mit dem gefunden Weg
			//
			//			for (int ij = 0; ij < astar_path.size(); ij++)
			//				cout << astar_path[ij] << " ";					//astar_path[] enthält jetzt in Umgekehrter Folge den Weg. Ziel -> Start
			//			cout << "\nCycles: " << a_star.cycles << "\n";		//a_star.cycles ist die Anzahl von Durchläufen bis der Weg gefunden wurde
			//			cout << "Elapsed time: " << a_star.elapsed_time.count() << " s\n"; //a_star.elapsed_time.count() enthält die dafür benötigte Zeit in sec.
			//
			//
			//
			//			cout << "DIJKSTRA:\n";
			//			vector<int> dijk_path = dijk.dijkstra(city_index_start, city_index_dest, distance_matr);	//Ausführen des Dijkstra, Übergabe: Start, Ziel, Verbindungsmatrix. Rückgabe: Vektor mit Weg
			//
			//			for (int ij = 0; ij < dijk_path.size(); ij++)
			//				cout << dijk_path[ij] << " ";				//dijk_path[]  Weg Ziel -> Start
			//			cout << "\nCycles: " << dijk.cycles << "\n";	//dijk.cycles Anzahl Durchläufe
			//			cout << "Elapsed time: " << dijk.elapsed_time.count() << " s\n";	//dijk.elapsesd_time.count() benötigte Zeit
	}

	private: System::Void StartBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) 
	{

	}

	private: System::Void NewMapDialog_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) 
	{

	}
	private: System::Void newMapButton_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		NewMapDialog->Filter = "Txt Files (*.txt)|*.txt";
		NewMapDialog->ShowDialog();
		String^ Filename = NewMapDialog->FileName;
		std::string Filenmae_std = msclr::interop::marshal_as< std::string >(Filename); //Convert System.String to Std.String
		FILE* inf;
		errno_t err;
		err = fopen_s(&inf, Filenmae_std.c_str(), "r");

		if (err != NULL) {
			printf("Cannot open input file %s\n", Filenmae_std.c_str());
			return;
		}

		//clear map area and dropdown menues
		map_area_graph->Clear(System::Drawing::Color::White);
		this->StartBox->Items->Clear();
		this->DestinationBox->Items->Clear();

	
		//Lex Analyse
		CParser obj;
		map<int, City*> LEX_ALL_CITYS;
		vector<vector<float>> LEX_ADJAZENZMATRIX;
		vector<vector<float>> LEX_ALL_POSITIONS;

		obj.InitParse(inf, stderr, stdout);
		LEX_ALL_CITYS = obj.yyparse();
		LEX_ADJAZENZMATRIX = Lex_Adjazenzmatrix(LEX_ALL_CITYS);
		LEX_ALL_POSITIONS = Lex_Positions(LEX_ALL_CITYS);

		//save data too use in diffrent eventhandler
		this->set_citys_map(LEX_ALL_CITYS);
		this->set_city_positions(LEX_ALL_POSITIONS);
		this->set_adjazenzmatrix(LEX_ADJAZENZMATRIX);

		//draw map and add all citys to dropdown

		for (int i = 0; i < LEX_ALL_CITYS.size(); i++)
		{
			
			//cout << LEX_ALL_CITYS[i]->City_Name << i << "\n" << "\n";
			for (int ii = 0; ii < LEX_ALL_CITYS[i]->Neighbours.size(); ii++)
			{
				int  Neigh = this->get_index(LEX_ALL_CITYS, LEX_ALL_CITYS[i]->Neighbours[ii]);
				if (Neigh >= LEX_ALL_CITYS.size())
				{
					std::cout <<"Nachbarstadt "<< LEX_ALL_CITYS[i]->Neighbours[ii] << " von " << LEX_ALL_CITYS[i]->City_Name <<" wurde nicht gefunden"<< "\n";
				}
				else
				{
					Boolean nomatch = TRUE;
					for (int iii = 0; iii < LEX_ALL_CITYS[Neigh]->Neighbours.size() && nomatch; iii++)
					{
						nomatch = strncmp( LEX_ALL_CITYS[i]->City_Name.c_str() , LEX_ALL_CITYS[Neigh]->Neighbours[iii].c_str(), LEX_ALL_CITYS[i]->City_Name.size());
						//cout << LEX_ALL_CITYS[i]->City_Name << " " << LEX_ALL_CITYS[Neigh]->Neighbours[iii] << " " << iii << " "<< nomatch <<endl;
					}
					if (nomatch) { cout << LEX_ALL_CITYS[Neigh]->City_Name<< Neigh<<" ist Nachbar von " <<   LEX_ALL_CITYS[i]->City_Name << i << " aber nicht umgekehrt" << endl; }
					this->connect_city(LEX_ALL_CITYS[i], LEX_ALL_CITYS[Neigh]);
				}
			}
		}		
		//draw all citys names and dots
		for (int i = 0; i < LEX_ALL_CITYS.size(); i++)
		{
			this->addCity(LEX_ALL_CITYS[i]);
		}
	}

	private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) 
	{

	}
	private: System::Void richTextBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) 
	{

	}
	private: System::Void MapArea_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		Point mouse_position = MapArea->PointToClient(Cursor->Position);
		std::cout <<mouse_position.X <<","<< mouse_position.Y<< "\n";
	}
	private: System::Void StartButton_MouseHover(System::Object^ sender, System::EventArgs^ e) 
	{
	this->StartButton->BackColor = System::Drawing::SystemColors::ActiveCaption;
	}
	private: System::Void StartButton_MouseLeave(System::Object^ sender, System::EventArgs^ e) 
	{
	this->StartButton->BackColor = System::Drawing::SystemColors::Control;
	}
	private: System::Void MapArea_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) 
	{
	
	}
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) 
	{
	}
};
}

