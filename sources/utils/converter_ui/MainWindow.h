#pragma once

#pragma unmanaged
#include "xr_skl_motion.h"
#pragma managed

using namespace xray_re;

namespace ConverterUI {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(array<String^>^ aArgs);

	protected:
		~MainWindow();

	private:
		System::ComponentModel::Container^	components;

		System::Windows::Forms::OpenFileDialog^ mFilePicker;
		System::Windows::Forms::TabPage^	mLocationTab;

		System::Windows::Forms::TabPage^	mMainTab;
		System::Windows::Forms::GroupBox^	groupBox2;
		System::Windows::Forms::TextBox^	mFilePathTextBox;
		System::Windows::Forms::Button^		mBrowseFileButton;

		System::Windows::Forms::GroupBox^	groupBox3;
		System::Windows::Forms::TextBox^	mOutputFolderTextBox;


		System::Windows::Forms::Button^		mBrowseDirectoryButton;
		System::Windows::Forms::ComboBox^	mMotionPicker;

		System::Windows::Forms::TabControl^ mTabControl;
		System::Windows::Forms::GroupBox^   groupBox4;
		System::Windows::Forms::ComboBox^   mFormatPicker;

		System::Windows::Forms::GroupBox^   groupBox5;
		System::Windows::Forms::Button^		mStartButton;

		System::Windows::Forms::TabPage^	mArchivesTab;
		System::Windows::Forms::TabPage^	mOtherTab;
		System::Windows::Forms::TabPage^	mSettingsTab;
		System::Windows::Forms::GroupBox^   groupBox1;
		System::Windows::Forms::TextBox^	textBox2;
		System::Windows::Forms::Button^		button2;
		System::Windows::Forms::TextBox^	textBox1;
		System::Windows::Forms::Button^		button1;
		System::Windows::Forms::GroupBox^	groupBox6;
		System::Windows::Forms::Button^		button5;
		System::Windows::Forms::TextBox^	textBox3;
		System::Windows::Forms::Button^		button3;
		System::Windows::Forms::Button^		button4;

		System::Windows::Forms::FolderBrowserDialog^  mFolderPicker;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Обязательный метод для поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent()
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainWindow::typeid));
			this->mFilePicker = (gcnew System::Windows::Forms::OpenFileDialog());
			this->mFolderPicker = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->mLocationTab = (gcnew System::Windows::Forms::TabPage());
			this->mMainTab = (gcnew System::Windows::Forms::TabPage());
			this->mStartButton = (gcnew System::Windows::Forms::Button());
			this->groupBox5 = (gcnew System::Windows::Forms::GroupBox());
			this->mMotionPicker = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->mOutputFolderTextBox = (gcnew System::Windows::Forms::TextBox());
			this->mBrowseDirectoryButton = (gcnew System::Windows::Forms::Button());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->mFormatPicker = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->mFilePathTextBox = (gcnew System::Windows::Forms::TextBox());
			this->mBrowseFileButton = (gcnew System::Windows::Forms::Button());
			this->mTabControl = (gcnew System::Windows::Forms::TabControl());
			this->mArchivesTab = (gcnew System::Windows::Forms::TabPage());
			this->groupBox6 = (gcnew System::Windows::Forms::GroupBox());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->mOtherTab = (gcnew System::Windows::Forms::TabPage());
			this->mSettingsTab = (gcnew System::Windows::Forms::TabPage());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->mMainTab->SuspendLayout();
			this->groupBox5->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->mTabControl->SuspendLayout();
			this->mArchivesTab->SuspendLayout();
			this->groupBox6->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// mFilePicker
			// 
			this->mFilePicker->Filter = L"OGF (*.ogf)|*.ogf|OMF (*.omf)|*.omf|DM (*.dm)|*.dm";
			this->mFilePicker->Title = L"Укажите путь к файлу";
			this->mFilePicker->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MainWindow::OnFilePickerOk);
			// 
			// mFolderPicker
			// 
			this->mFolderPicker->Description = L"Выберите папку";
			// 
			// mLocationTab
			// 
			this->mLocationTab->BackColor = System::Drawing::SystemColors::Control;
			this->mLocationTab->Location = System::Drawing::Point(4, 22);
			this->mLocationTab->Name = L"mLocationTab";
			this->mLocationTab->Padding = System::Windows::Forms::Padding(3);
			this->mLocationTab->Size = System::Drawing::Size(424, 151);
			this->mLocationTab->TabIndex = 1;
			this->mLocationTab->Text = L"Локации";
			// 
			// mMainTab
			// 
			this->mMainTab->BackColor = System::Drawing::Color::Transparent;
			this->mMainTab->Controls->Add(this->mStartButton);
			this->mMainTab->Controls->Add(this->groupBox5);
			this->mMainTab->Controls->Add(this->groupBox3);
			this->mMainTab->Controls->Add(this->groupBox4);
			this->mMainTab->Controls->Add(this->groupBox2);
			this->mMainTab->Location = System::Drawing::Point(4, 22);
			this->mMainTab->Name = L"mMainTab";
			this->mMainTab->Padding = System::Windows::Forms::Padding(3);
			this->mMainTab->Size = System::Drawing::Size(424, 151);
			this->mMainTab->TabIndex = 0;
			this->mMainTab->Text = L"Основное";
			// 
			// mStartButton
			// 
			this->mStartButton->Enabled = false;
			this->mStartButton->Location = System::Drawing::Point(385, 105);
			this->mStartButton->Name = L"mStartButton";
			this->mStartButton->Size = System::Drawing::Size(36, 43);
			this->mStartButton->TabIndex = 10;
			this->mStartButton->UseVisualStyleBackColor = true;
			this->mStartButton->Click += gcnew System::EventHandler(this, &MainWindow::OnStartButton_Click);
			// 
			// groupBox5
			// 
			this->groupBox5->Controls->Add(this->mMotionPicker);
			this->groupBox5->Location = System::Drawing::Point(125, 99);
			this->groupBox5->Name = L"groupBox5";
			this->groupBox5->Size = System::Drawing::Size(257, 49);
			this->groupBox5->TabIndex = 9;
			this->groupBox5->TabStop = false;
			this->groupBox5->Text = L"Анимации";
			// 
			// mMotionPicker
			// 
			this->mMotionPicker->AutoCompleteMode = System::Windows::Forms::AutoCompleteMode::SuggestAppend;
			this->mMotionPicker->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::ListItems;
			this->mMotionPicker->Enabled = false;
			this->mMotionPicker->FormattingEnabled = true;
			this->mMotionPicker->Location = System::Drawing::Point(6, 20);
			this->mMotionPicker->Name = L"mMotionPicker";
			this->mMotionPicker->Size = System::Drawing::Size(245, 21);
			this->mMotionPicker->Sorted = true;
			this->mMotionPicker->TabIndex = 7;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->mOutputFolderTextBox);
			this->groupBox3->Controls->Add(this->mBrowseDirectoryButton);
			this->groupBox3->Location = System::Drawing::Point(0, 51);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(421, 46);
			this->groupBox3->TabIndex = 10;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Выходной каталог";
			// 
			// mOutputFolderTextBox
			// 
			this->mOutputFolderTextBox->Location = System::Drawing::Point(6, 19);
			this->mOutputFolderTextBox->Name = L"mOutputFolderTextBox";
			this->mOutputFolderTextBox->Size = System::Drawing::Size(376, 20);
			this->mOutputFolderTextBox->TabIndex = 5;
			// 
			// mBrowseDirectoryButton
			// 
			this->mBrowseDirectoryButton->Location = System::Drawing::Point(388, 16);
			this->mBrowseDirectoryButton->Name = L"mBrowseDirectoryButton";
			this->mBrowseDirectoryButton->Size = System::Drawing::Size(27, 23);
			this->mBrowseDirectoryButton->TabIndex = 6;
			this->mBrowseDirectoryButton->Text = L"...";
			this->mBrowseDirectoryButton->UseVisualStyleBackColor = true;
			this->mBrowseDirectoryButton->Click += gcnew System::EventHandler(this, &MainWindow::OnBrowseFolderButton_Click);
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->mFormatPicker);
			this->groupBox4->Location = System::Drawing::Point(0, 99);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(119, 49);
			this->groupBox4->TabIndex = 8;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Выходной формат";
			// 
			// mFormatPicker
			// 
			this->mFormatPicker->Enabled = false;
			this->mFormatPicker->FormattingEnabled = true;
			this->mFormatPicker->Location = System::Drawing::Point(7, 20);
			this->mFormatPicker->Name = L"mFormatPicker";
			this->mFormatPicker->Size = System::Drawing::Size(105, 21);
			this->mFormatPicker->TabIndex = 0;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->mFilePathTextBox);
			this->groupBox2->Controls->Add(this->mBrowseFileButton);
			this->groupBox2->Location = System::Drawing::Point(0, 0);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(421, 48);
			this->groupBox2->TabIndex = 11;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Путь к файлу";
			// 
			// mFilePathTextBox
			// 
			this->mFilePathTextBox->Location = System::Drawing::Point(6, 19);
			this->mFilePathTextBox->Name = L"mFilePathTextBox";
			this->mFilePathTextBox->ReadOnly = true;
			this->mFilePathTextBox->Size = System::Drawing::Size(376, 20);
			this->mFilePathTextBox->TabIndex = 2;
			// 
			// mBrowseFileButton
			// 
			this->mBrowseFileButton->Location = System::Drawing::Point(388, 17);
			this->mBrowseFileButton->Name = L"mBrowseFileButton";
			this->mBrowseFileButton->Size = System::Drawing::Size(27, 23);
			this->mBrowseFileButton->TabIndex = 4;
			this->mBrowseFileButton->Text = L"...";
			this->mBrowseFileButton->UseVisualStyleBackColor = true;
			this->mBrowseFileButton->Click += gcnew System::EventHandler(this, &MainWindow::OnBrowseFileButton_Click);
			// 
			// mTabControl
			// 
			this->mTabControl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->mTabControl->Controls->Add(this->mMainTab);
			this->mTabControl->Controls->Add(this->mLocationTab);
			this->mTabControl->Controls->Add(this->mArchivesTab);
			this->mTabControl->Controls->Add(this->mOtherTab);
			this->mTabControl->Controls->Add(this->mSettingsTab);
			this->mTabControl->Location = System::Drawing::Point(-1, -1);
			this->mTabControl->Name = L"mTabControl";
			this->mTabControl->SelectedIndex = 0;
			this->mTabControl->Size = System::Drawing::Size(432, 177);
			this->mTabControl->TabIndex = 10;
			// 
			// mArchivesTab
			// 
			this->mArchivesTab->BackColor = System::Drawing::SystemColors::Control;
			this->mArchivesTab->Controls->Add(this->groupBox6);
			this->mArchivesTab->Controls->Add(this->groupBox1);
			this->mArchivesTab->Location = System::Drawing::Point(4, 22);
			this->mArchivesTab->Name = L"mArchivesTab";
			this->mArchivesTab->Size = System::Drawing::Size(424, 151);
			this->mArchivesTab->TabIndex = 2;
			this->mArchivesTab->Text = L"Архивы";
			// 
			// groupBox6
			// 
			this->groupBox6->Controls->Add(this->button5);
			this->groupBox6->Controls->Add(this->textBox3);
			this->groupBox6->Controls->Add(this->button3);
			this->groupBox6->Location = System::Drawing::Point(0, 76);
			this->groupBox6->Name = L"groupBox6";
			this->groupBox6->Size = System::Drawing::Size(421, 72);
			this->groupBox6->TabIndex = 13;
			this->groupBox6->TabStop = false;
			this->groupBox6->Text = L"Путь к игровому архиву";
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(340, 43);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 6;
			this->button5->Text = L"Начать";
			this->button5->UseVisualStyleBackColor = true;
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(6, 19);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(376, 20);
			this->textBox3->TabIndex = 2;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(388, 17);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(27, 23);
			this->button3->TabIndex = 4;
			this->button3->Text = L"...";
			this->button3->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button4);
			this->groupBox1->Controls->Add(this->textBox2);
			this->groupBox1->Controls->Add(this->button2);
			this->groupBox1->Location = System::Drawing::Point(0, 0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(421, 72);
			this->groupBox1->TabIndex = 12;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Путь к папке архивации";
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(340, 43);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 5;
			this->button4->Text = L"Начать";
			this->button4->UseVisualStyleBackColor = true;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(6, 19);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(376, 20);
			this->textBox2->TabIndex = 2;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(388, 17);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(27, 23);
			this->button2->TabIndex = 4;
			this->button2->Text = L"...";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// mOtherTab
			// 
			this->mOtherTab->BackColor = System::Drawing::SystemColors::Control;
			this->mOtherTab->Location = System::Drawing::Point(4, 22);
			this->mOtherTab->Name = L"mOtherTab";
			this->mOtherTab->Size = System::Drawing::Size(424, 151);
			this->mOtherTab->TabIndex = 3;
			this->mOtherTab->Text = L"Текстуры/звуки";
			// 
			// mSettingsTab
			// 
			this->mSettingsTab->BackColor = System::Drawing::SystemColors::Control;
			this->mSettingsTab->Location = System::Drawing::Point(4, 22);
			this->mSettingsTab->Name = L"mSettingsTab";
			this->mSettingsTab->Size = System::Drawing::Size(424, 151);
			this->mSettingsTab->TabIndex = 4;
			this->mSettingsTab->Text = L"Настройки";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(6, 19);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(376, 20);
			this->textBox1->TabIndex = 2;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(388, 17);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(27, 23);
			this->button1->TabIndex = 4;
			this->button1->Text = L"...";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// MainWindow
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(427, 171);
			this->Controls->Add(this->mTabControl);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"MainWindow";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Converter";
			this->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &MainWindow::OnDragDrop);
			this->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &MainWindow::OnDragEnter);
			this->mMainTab->ResumeLayout(false);
			this->groupBox5->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->mTabControl->ResumeLayout(false);
			this->mArchivesTab->ResumeLayout(false);
			this->groupBox6->ResumeLayout(false);
			this->groupBox6->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
		unsigned mTools;

		static array<String^>^ mOGFFormats = {"object", "bones"};
		static array<String^>^ mOMFFormats = {"skls",   "skl"};
		static array<String^>^ mDMFormats  = {"object", "info"};

		void OnBrowseFileButton_Click(Object^ sender, EventArgs^ e);

		void OnBrowseFolderButton_Click(Object^ sender, EventArgs^ e);

		void OnStartButton_Click(Object^ sender, EventArgs^ e);

		void OnFilePickerOk(Object^ sender, CancelEventArgs^ e);

		void OnDragDrop(Object^ sender, DragEventArgs^ e);

		void OnDragEnter(Object^ sender, DragEventArgs^ e);

		void Prepare(String^ sPath);

		void PrepareObjectTools(LPSTR sFilePath);

		void FillMotionComboBox(xr_skl_motion_vec& motions);
};
}

