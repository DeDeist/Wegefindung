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
	Boolean map_existing=FALSE;

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
		}

	private:
		void drawpoint(int x, int y)
		{
			this->map_area_graph ->FillEllipse(red_point, x, y, 10, 10);
		}

		void drawpoint_blue(int x, int y)
		{
			this->map_area_graph->FillEllipse(blue_point, x, y, 10, 10);
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

		void drawnumber(int number, int x, int y)
		{
			System::String^ sys_str_number = gcnew String(System::Convert::ToString(number+1));
			map_area_graph->DrawString(sys_str_number, this->lable_font, this->text_brush, float(x-5), float(y + 10));
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
			Boolean nomatch = TRUE;
			int i = 0;
			for (i = 0; nomatch && i<City_Map.size(); i++ )
			{
					nomatch = strncmp(City_Map[i]->City_Name.c_str(), City.c_str(), City.size());
			}
			i--;
			if (nomatch == TRUE) { i++;}
			
			return i;
		}

		void drawmap()
		{
			if(map_existing)
			{ 
				map_area_graph->Clear(System::Drawing::Color::White);
				for (int i = 0; i < all_citys.size(); i++)
				{
				
					for (int ii = 0; ii < all_citys[i]->Neighbours.size(); ii++)
					{
						int  Neigh = this->get_index(all_citys, all_citys[i]->Neighbours[ii]);
						if (Neigh >= all_citys.size())
						{
							std::cout << "Nachbarstadt " << all_citys[i]->Neighbours[ii] << " von " << all_citys[i]->City_Name << " wurde nicht gefunden" << "\n";
						}
						else
						{
							Boolean nomatch = TRUE;
							for (int iii = 0; iii < all_citys[Neigh]->Neighbours.size() && nomatch; iii++)
							{
								nomatch = strncmp(all_citys[i]->City_Name.c_str(), all_citys[Neigh]->Neighbours[iii].c_str(), all_citys[i]->City_Name.size());
							}
							if (nomatch) { cout << all_citys[Neigh]->City_Name << Neigh << " ist Nachbar von " << all_citys[i]->City_Name << i << " aber nicht umgekehrt" << endl; }
							this->connect_city(all_citys[i], all_citys[Neigh]);
						}
					}
				}
				//draw all citys names and dots
				for (int i = 0; i < all_citys.size(); i++)
				{ 
				drawpointandlabel(all_citys[i]->pos[0], all_citys[i]->pos[1], all_citys[i]->City_Name);
				}
			}

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

	private: System::Windows::Forms::LinkLabel^ linkLabel1;
	private: System::Windows::Forms::Button^ newMapButton;


	private: System::Windows::Forms::ComboBox^ EmptyBox;
	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	private: Graphics^ map_area_graph;
	private: Pen^ black_line;
	private: Pen^ green_line;
	private: Pen^ blue_line;
	private: Pen^ red_line;
	private: SolidBrush^ red_point;
	private: SolidBrush^ blue_point;
	private: SolidBrush^ text_brush;
	private: System::Drawing::Font^ lable_font;
	private: System::Drawing::StringFormat^ text_format;
	private: System::Windows::Forms::Button^ clear_map_button;
	private: System::Windows::Forms::Button^ show_path_button;
	private: System::Windows::Forms::CheckBox^ time_median_check_box;
	private: System::Windows::Forms::CheckBox^ pathbox;
	private: System::Windows::Forms::OpenFileDialog^ NewMapDialog;

	private: System::Windows::Forms::Label^ astar_nodes;
	private: System::Windows::Forms::Label^ astar_time;
	private: System::Windows::Forms::Label^ dijk_time;
	private: System::Windows::Forms::Label^ dijk_nodes;

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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->MapArea = (gcnew System::Windows::Forms::Panel());
			this->AsternCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->DijkstraCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->StartButton = (gcnew System::Windows::Forms::Button());
			this->StartBox = (gcnew System::Windows::Forms::ComboBox());
			this->DestinationBox = (gcnew System::Windows::Forms::ComboBox());
			this->FromLabel = (gcnew System::Windows::Forms::Label());
			this->ToLabel = (gcnew System::Windows::Forms::Label());
			this->linkLabel1 = (gcnew System::Windows::Forms::LinkLabel());
			this->newMapButton = (gcnew System::Windows::Forms::Button());
			this->EmptyBox = (gcnew System::Windows::Forms::ComboBox());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->show_path_button = (gcnew System::Windows::Forms::Button());
			this->clear_map_button = (gcnew System::Windows::Forms::Button());
			this->time_median_check_box = (gcnew System::Windows::Forms::CheckBox());
			this->pathbox = (gcnew System::Windows::Forms::CheckBox());
			this->NewMapDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->astar_nodes = (gcnew System::Windows::Forms::Label());
			this->astar_time = (gcnew System::Windows::Forms::Label());
			this->dijk_time = (gcnew System::Windows::Forms::Label());
			this->dijk_nodes = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// MapArea
			// 
			this->MapArea->BackColor = System::Drawing::SystemColors::Window;
			this->MapArea->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->MapArea->Location = System::Drawing::Point(12, 12);
			this->MapArea->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->MapArea->Name = L"MapArea";
			this->MapArea->Size = System::Drawing::Size(963, 1081);
			this->MapArea->TabIndex = 0;
			this->MapArea->Click += gcnew System::EventHandler(this, &MyForm::MapArea_Click);
			this->MapArea->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MapArea_Paint);
			this->MapArea->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MapArea_MouseDown_1);
			// 
			// AsternCheckBox
			// 
			this->AsternCheckBox->AutoSize = true;
			this->AsternCheckBox->Location = System::Drawing::Point(993, 178);
			this->AsternCheckBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->AsternCheckBox->Name = L"AsternCheckBox";
			this->AsternCheckBox->Size = System::Drawing::Size(71, 20);
			this->AsternCheckBox->TabIndex = 1;
			this->AsternCheckBox->Text = L"A-Stern";
			this->AsternCheckBox->UseVisualStyleBackColor = true;
			// 
			// DijkstraCheckBox
			// 
			this->DijkstraCheckBox->AutoSize = true;
			this->DijkstraCheckBox->Location = System::Drawing::Point(1124, 177);
			this->DijkstraCheckBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->DijkstraCheckBox->Name = L"DijkstraCheckBox";
			this->DijkstraCheckBox->Size = System::Drawing::Size(72, 20);
			this->DijkstraCheckBox->TabIndex = 2;
			this->DijkstraCheckBox->Text = L"Dijkstra";
			this->DijkstraCheckBox->UseVisualStyleBackColor = true;
			// 
			// StartButton
			// 
			this->StartButton->BackColor = System::Drawing::SystemColors::Control;
			this->StartButton->Location = System::Drawing::Point(992, 273);
			this->StartButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->StartButton->Name = L"StartButton";
			this->StartButton->Size = System::Drawing::Size(236, 59);
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
			this->StartBox->Location = System::Drawing::Point(993, 57);
			this->StartBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->StartBox->Name = L"StartBox";
			this->StartBox->Size = System::Drawing::Size(236, 24);
			this->StartBox->TabIndex = 4;
			this->StartBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::StartBox_SelectedIndexChanged);
			// 
			// DestinationBox
			// 
			this->DestinationBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->DestinationBox->FormattingEnabled = true;
			this->DestinationBox->Location = System::Drawing::Point(993, 128);
			this->DestinationBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->DestinationBox->Name = L"DestinationBox";
			this->DestinationBox->Size = System::Drawing::Size(236, 24);
			this->DestinationBox->TabIndex = 5;
			// 
			// FromLabel
			// 
			this->FromLabel->AutoSize = true;
			this->FromLabel->BackColor = System::Drawing::Color::Transparent;
			this->FromLabel->Location = System::Drawing::Point(989, 26);
			this->FromLabel->Name = L"FromLabel";
			this->FromLabel->Size = System::Drawing::Size(42, 16);
			this->FromLabel->TabIndex = 6;
			this->FromLabel->Text = L"From:";
			// 
			// ToLabel
			// 
			this->ToLabel->AutoSize = true;
			this->ToLabel->Location = System::Drawing::Point(989, 98);
			this->ToLabel->Name = L"ToLabel";
			this->ToLabel->Size = System::Drawing::Size(28, 16);
			this->ToLabel->TabIndex = 7;
			this->ToLabel->Text = L"To:";
			// 
			// linkLabel1
			// 
			this->linkLabel1->AutoSize = true;
			this->linkLabel1->Location = System::Drawing::Point(1159, 404);
			this->linkLabel1->Name = L"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(0, 16);
			this->linkLabel1->TabIndex = 8;
			// 
			// newMapButton
			// 
			this->newMapButton->BackColor = System::Drawing::SystemColors::Control;
			this->newMapButton->Location = System::Drawing::Point(992, 1029);
			this->newMapButton->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->newMapButton->Name = L"newMapButton";
			this->newMapButton->Size = System::Drawing::Size(241, 64);
			this->newMapButton->TabIndex = 9;
			this->newMapButton->Text = L"New Map";
			this->newMapButton->UseVisualStyleBackColor = false;
			this->newMapButton->Click += gcnew System::EventHandler(this, &MyForm::newMapButton_Click);
			// 
			// EmptyBox
			// 
			this->EmptyBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::Simple;
			this->EmptyBox->FormattingEnabled = true;
			this->EmptyBox->Items->AddRange(gcnew cli::array< System::Object^  >(1) { L"Berlin" });
			this->EmptyBox->Location = System::Drawing::Point(2549, 1099);
			this->EmptyBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->EmptyBox->Name = L"EmptyBox";
			this->EmptyBox->Size = System::Drawing::Size(12, 25);
			this->EmptyBox->TabIndex = 10;
			this->EmptyBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// richTextBox1
			// 
			this->richTextBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->richTextBox1->Location = System::Drawing::Point(993, 466);
			this->richTextBox1->Margin = System::Windows::Forms::Padding(4);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(236, 129);
			this->richTextBox1->TabIndex = 12;
			this->richTextBox1->Text = L"A-Stern:\nKnoten durchlaufen:\t\nBenöritgte Zeit [ms]\t\t\n\nDjkstra:\nKnoten durchlaufen"
				L":\t\nBenöritgte Zeit [ms]\t\t";
			this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::richTextBox1_TextChanged);
			// 
			// show_path_button
			// 
			this->show_path_button->BackColor = System::Drawing::SystemColors::Control;
			this->show_path_button->Location = System::Drawing::Point(993, 401);
			this->show_path_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->show_path_button->Name = L"show_path_button";
			this->show_path_button->Size = System::Drawing::Size(236, 59);
			this->show_path_button->TabIndex = 13;
			this->show_path_button->Text = L"Find Divers Routes";
			this->show_path_button->UseVisualStyleBackColor = false;
			this->show_path_button->Click += gcnew System::EventHandler(this, &MyForm::show_path_button_Click);
			// 
			// clear_map_button
			// 
			this->clear_map_button->BackColor = System::Drawing::SystemColors::Control;
			this->clear_map_button->Location = System::Drawing::Point(992, 337);
			this->clear_map_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->clear_map_button->Name = L"clear_map_button";
			this->clear_map_button->Size = System::Drawing::Size(236, 59);
			this->clear_map_button->TabIndex = 14;
			this->clear_map_button->Text = L"Clear Map";
			this->clear_map_button->UseVisualStyleBackColor = false;
			this->clear_map_button->Click += gcnew System::EventHandler(this, &MyForm::clear_map_button_Click);
			// 
			// time_median_check_box
			// 
			this->time_median_check_box->AutoSize = true;
			this->time_median_check_box->Location = System::Drawing::Point(993, 203);
			this->time_median_check_box->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->time_median_check_box->Name = L"time_median_check_box";
			this->time_median_check_box->Size = System::Drawing::Size(131, 20);
			this->time_median_check_box->TabIndex = 15;
			this->time_median_check_box->Text = L"Use Timemedian";
			this->time_median_check_box->UseVisualStyleBackColor = true;
			// 
			// pathbox
			// 
			this->pathbox->AutoSize = true;
			this->pathbox->Location = System::Drawing::Point(993, 229);
			this->pathbox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pathbox->Name = L"pathbox";
			this->pathbox->Size = System::Drawing::Size(90, 20);
			this->pathbox->TabIndex = 16;
			this->pathbox->Text = L"Show Path";
			this->pathbox->UseVisualStyleBackColor = true;
			// 
			// NewMapDialog
			// 
			this->NewMapDialog->FileName = L"NewMapDialog";
			this->NewMapDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::NewMapDialog_FileOk);
			// 
			// astar_nodes
			// 
			this->astar_nodes->AutoSize = true;
			this->astar_nodes->BackColor = System::Drawing::Color::White;
			this->astar_nodes->Location = System::Drawing::Point(1137, 484);
			this->astar_nodes->Name = L"astar_nodes";
			this->astar_nodes->Size = System::Drawing::Size(15, 16);
			this->astar_nodes->TabIndex = 18;
			this->astar_nodes->Text = L"0";
			// 
			// astar_time
			// 
			this->astar_time->AutoSize = true;
			this->astar_time->BackColor = System::Drawing::Color::White;
			this->astar_time->Location = System::Drawing::Point(1137, 500);
			this->astar_time->Name = L"astar_time";
			this->astar_time->Size = System::Drawing::Size(15, 16);
			this->astar_time->TabIndex = 19;
			this->astar_time->Text = L"0";
			// 
			// dijk_time
			// 
			this->dijk_time->AutoSize = true;
			this->dijk_time->BackColor = System::Drawing::Color::White;
			this->dijk_time->Location = System::Drawing::Point(1137, 566);
			this->dijk_time->Name = L"dijk_time";
			this->dijk_time->Size = System::Drawing::Size(15, 16);
			this->dijk_time->TabIndex = 20;
			this->dijk_time->Text = L"0";
			// 
			// dijk_nodes
			// 
			this->dijk_nodes->AutoSize = true;
			this->dijk_nodes->BackColor = System::Drawing::Color::White;
			this->dijk_nodes->Location = System::Drawing::Point(1137, 550);
			this->dijk_nodes->Name = L"dijk_nodes";
			this->dijk_nodes->Size = System::Drawing::Size(15, 16);
			this->dijk_nodes->TabIndex = 21;
			this->dijk_nodes->Text = L"0";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1292, 1141);
			this->Controls->Add(this->dijk_time);
			this->Controls->Add(this->dijk_nodes);
			this->Controls->Add(this->astar_time);
			this->Controls->Add(this->astar_nodes);
			this->Controls->Add(this->pathbox);
			this->Controls->Add(this->time_median_check_box);
			this->Controls->Add(this->clear_map_button);
			this->Controls->Add(this->show_path_button);
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
			this->Controls->Add(this->richTextBox1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"MyForm";
			this->Text = L"Wegefindung";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		void InitializeDrwatools(void)
		{
			this->map_area_graph = MapArea->CreateGraphics();
			this->black_line = gcnew Pen(Color::Black);
			this->green_line = gcnew Pen(Color::Green,5);
			this->blue_line = gcnew Pen(Color::Blue,3);
			this->red_line = gcnew Pen(Color::Red,2);
			this->red_point = gcnew SolidBrush(Color::Red);
			this->blue_point = gcnew SolidBrush(Color::Blue);
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
			MessageBox::Show("Wählen Sie einen Startort aus.");
			//cout << "Wählen Sie einen Startort aus." << endl;
			return;
		}
		if (DestinationBox->SelectedIndex == -1)
		{
			MessageBox::Show("Wählen Sie einen Zielort aus.");
			//cout << "Wählen Sie einen Zielort aus." << endl;
			return;
		}

		if (!(DijkstraCheckBox->Checked ) && (AsternCheckBox->Checked==0))
		{
			MessageBox::Show("Wählen Sie einen Algorithmus aus.");
			//cout << "Wählen Sie einen Zielort aus." << endl;
			return;

		}

		drawmap();
		if (DijkstraCheckBox->Checked)
		{
			//cout << "Dijkstra ausgewählt" << endl;
			dijkstra_algo dijk;
			//cout << "DIJKSTRA:\n";
			//bool median_dijkstra = false;
			vector<int> dijk_path = dijk.dijkstra(StartBox->SelectedIndex, DestinationBox->SelectedIndex, adjazenzmatrix, time_median_check_box->Checked);	//Ausführen des Dijkstra, Übergabe: Start, Ziel, Verbindungsmatrix. Rückgabe: Vektor mit Weg
			//cout << time_median_check_box->Checked << endl;
			//for (int ij = 0; ij < dijk_path.size(); ij++)
			//	cout << dijk_path[ij] << " ";				//dijk_path[]  Weg Ziel -> Start
			//cout << "\nCycles: " << dijk.cycles << "\n";	//dijk.cycles Anzahl Durchläufe
			//cout << "Elapsed time: " << dijk.elapsed_time.count() << " s\n";	//dijk.elapsesd_time.count() benötigte Zeit
			
			for (int i = 1; i < dijk_path.size(); i++)
			{
				connect_city_dij(all_citys[dijk_path[i - 1]], all_citys[dijk_path[i]]);
			}
			for (int i = 0; i < dijk.checked_nodes.size() && pathbox->Checked; i++)
			{
				drawnumber(i, all_citys[dijk.checked_nodes[i]]->pos[0], all_citys[dijk.checked_nodes[i]]->pos[1]);
			}

			System::String^ nodes = gcnew String(System::Convert::ToString(dijk.cycles));
			System::String^ time = gcnew String(System::Convert::ToString((dijk.elapsed_time.count()*1000)));
			this->dijk_time->Text = time;
			this->dijk_nodes->Text = nodes;
		}
		else
		{
			this->dijk_time->Text = L"0";
			this->dijk_nodes->Text = L"0";
		}
		
		if (AsternCheckBox->Checked)
		{
			//cout << "A-Stern ausgewählt" << endl;
			astar_algo a_star;
			//cout << "A_STAR:\n";
			//bool median_aster = true;
			vector<int> astar_path = a_star.astar(StartBox->SelectedIndex, DestinationBox->SelectedIndex, adjazenzmatrix, city_positions, time_median_check_box->Checked);	//Ausführen des A*, Übergabe: Start, Ziel, Verbindungsmatrix, Koordinaten. Rückgabe ist der Vektor mit dem gefunden Weg
			
			//for (int ij = 0; ij < astar_path.size(); ij++)
				//cout << astar_path[ij] << " ";					//astar_path[] enthält jetzt in Umgekehrter Folge den Weg. Ziel -> Start
			//cout << "\nCycles: " << a_star.cycles << "\n";		//a_star.cycles ist die Anzahl von Durchläufen bis der Weg gefunden wurde
			//cout << "Elapsed time: " << a_star.elapsed_time.count() << " s\n"; //a_star.elapsed_time.count() enthält die dafür benötigte Zeit in sec.

			for (int i = 1; i < astar_path.size(); i++)
			{
				connect_city_astern(all_citys[astar_path[i - 1]], all_citys[astar_path[i]]);

			}

			for (int i = 0; i < a_star.checked_nodes.size() && pathbox->Checked; i++)
			{
				drawnumber(i, all_citys[a_star.checked_nodes[i]]->pos[0], all_citys[a_star.checked_nodes[i]]->pos[1]);
			}

			System::String^ nodes = gcnew String(System::Convert::ToString(a_star.cycles));
			System::String^ time = gcnew String(System::Convert::ToString((a_star.elapsed_time.count()*1000)));
			this->astar_time->Text = time;
			this->astar_nodes->Text = nodes;
			
		}
		else
		{
			this->astar_time->Text = L"0";
			this->astar_nodes->Text = L"0";
		}

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


	
		//Lex Analyse
		CParser obj;
		map<int, City*> LEX_ALL_CITYS;
		vector<vector<float>> LEX_ADJAZENZMATRIX;
		vector<vector<float>> LEX_ALL_POSITIONS;

		obj.InitParse(inf, stderr, stdout);
		LEX_ALL_CITYS = obj.yyparse();
		LEX_ADJAZENZMATRIX = Lex_Adjazenzmatrix(LEX_ALL_CITYS);
		LEX_ALL_POSITIONS = Lex_Positions(LEX_ALL_CITYS);

		//save data to use in diffrent eventhandler
		this->set_citys_map(LEX_ALL_CITYS);
		this->set_city_positions(LEX_ALL_POSITIONS);
		this->set_adjazenzmatrix(LEX_ADJAZENZMATRIX);
		map_existing = TRUE;
		//draw map and add all citys to dropdown
		drawmap();
		this->StartBox->Items->Clear();
		this->DestinationBox->Items->Clear();
		for (int i = 0; i < all_citys.size(); i++)
		{
			this->addCity(all_citys[i]);
		}
		//for (int i = 0; i < all_citys.size(); i++)
		//{
		//	adddropdown(all_citys[i]->City_Name);
		//}
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
		//Point mouse_position = MapArea->PointToClient(Cursor->Position);
		//std::cout <<mouse_position.X <<","<< mouse_position.Y<< "\n";
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
	private: System::Void show_path_button_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		dijkstra_algo dijk;
		astar_algo a_star;
		Boolean nomatch = TRUE;
		vector<int> dijk_path;
		vector<int> astar_path;

		for (int i = 0; i < all_citys.size(); i++)

			for (int ii = 0; ii < all_citys.size(); ii++)
			{
				if (i == ii)
					ii++;
				if (ii == all_citys.size())
					break;
				dijk_path = dijk.dijkstra(i, ii, adjazenzmatrix,0);
				astar_path = a_star.astar(i, ii, adjazenzmatrix, city_positions,0);
				for (int iii = 0; iii < dijk_path.size() && nomatch; iii++)
				{
					nomatch = (dijk_path[iii] == astar_path[iii]);
				}

				if (nomatch == FALSE)
				{
					cout << "Weg von " << all_citys[0]->City_Name << " nach " << all_citys[i]->City_Name << " unterscheidet sich" << endl;
					cout << dijk_path.size() << endl;

					for (int ij = 0; ij < astar_path.size(); ij++)
						cout << astar_path[ij] << " ";
					cout << endl;

					for (int ij = 0; ij < dijk_path.size(); ij++)
						cout << dijk_path[ij] << " ";
					cout << endl;
					nomatch = TRUE;
				}
			}
	}
	private: System::Void clear_map_button_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	drawmap();
	}
	private: System::Void MapArea_MouseDown_1(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) 
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Right)
		{
			BOOLEAN nomatch = TRUE;
			Point mouse_position = MapArea->PointToClient(Cursor->Position);
			int i = 0;
			for ( i = 0; i < all_citys.size() && nomatch; i++)
			{
				//cout << i << endl;
				if ((all_citys[i]->pos[0]-5) < mouse_position.X && mouse_position.X < (all_citys[i]->pos[0] + 5))
				{
					//cout << all_citys[i]->pos[0] - 5 <<"  "<< mouse_position.X<<"  "<< all_citys[i]->pos[0] + 5 << endl;
					if (all_citys[i]->pos[1] - 5 < mouse_position.Y && mouse_position.Y < (all_citys[i]->pos[1] + 5))
					{
						//cout << i << endl;
						nomatch = FALSE;
					}
				}
			}
			if (all_citys.size() == i)
				return;
			this->DestinationBox->SelectedIndex = (i-1);
		}
		if (e->Button == System::Windows::Forms::MouseButtons::Left)
		{
			BOOLEAN nomatch = TRUE;
			Point mouse_position = MapArea->PointToClient(Cursor->Position);
			int i = 0;
			for (i = 0; i < all_citys.size() && nomatch; i++)
			{
				//cout << i << endl;
				if ((all_citys[i]->pos[0] - 5) < mouse_position.X && mouse_position.X < (all_citys[i]->pos[0] + 5))
				{
					//cout << all_citys[i]->pos[0] - 5 << "  " << mouse_position.X << "  " << all_citys[i]->pos[0] + 5 << endl;
					if (all_citys[i]->pos[1] - 5 < mouse_position.Y && mouse_position.Y < (all_citys[i]->pos[1] + 5))
					{
						//cout << i << endl;
						nomatch = FALSE;
					}
				}
			}
			if (all_citys.size() == i)
				return;
			this->StartBox->SelectedIndex = (i - 1);
		}
	}
};
}