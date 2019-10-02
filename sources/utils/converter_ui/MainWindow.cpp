#include "pch.h"
#include "MainWindow.h"

#pragma unmanaged
#include "windows.h"
#include "converter.h"
#include "tools_base.h"
#include "dm_tools.h"
#include "ogf_tools.h"
//#include "level_tools.h"
#include "xrdemo_tools.h"
#include "xr_dm.h"
#include "xr_ogf_v4.h"
#pragma managed

#include "xr_file_system.h"
//#include "xr_ini_file.h"
#include "xr_log.h"

using namespace xray_re;
using namespace ConverterUI;

/* Help for understanding
Ogf options: +
	object
	skls
	skl
	bones

Omf options: +
	skls
	skl

Xrdemo options: +
	anm - one target

DM options: +
	object
	info -- not a format

Level options:
	mode:
		maya
		le
		le2
	with_lods
	fancy

DB options:
	pack:
		2947ru - release version format(SOC???)
		2947ww - world-wide release version and 3120 format
		
	unpack:
		11xx (1114/1154) - unpack only
		2215			 - unpack only
		2945 (2945/2939) - unpack only
		2947ru			 - release version format(SOC???)
		2947ww			 - world-wide release version and 3120 format

	other cmds:
		xdb		  - assume .xdb or .db archive format
		xdb_ud    - attach user data from file
		flt		  - extract only files, filtered by mask
		strip_thm - COMMENTED!!!!! - remove attached image in texture descriptors

OGG options:
	wav

DDS options:
	tga:
		with_solid
		with_bump
*/

MainWindow::MainWindow(array<String^>^ aArgs) {
	InitializeComponent();

	xr_file_system& fs = xr_file_system::instance();
	if (!fs.initialize(DEFAULT_FS_SPEC)) {
		// TODO
	} else { // initialize controls
		//mSoundsPathTextBox->Text = to_string(fs.resolve_path(PA_GAME_SOUNDS));		
	}
	xr_log::instance().init("converter");

	/*mIni = new xr_ini_file(CONVERTER_INI);
	if (!mIni->empty()) {
		u32 lcount = mIni->line_count("profiles");
		for (u32 i = 0; i < lcount; i++) {
			const char* lname;
			mIni->r_line("profiles", i, &lname, NULL);
			mProfilePicker->Items->Add(to_string(lname));
		}

		mProfilePicker->Enabled = true;
		mProfilePicker->SelectedIndex = 0;
	}*/

	mModePicker->SelectedIndex = 0;

	if (aArgs->Length > 0) {
		Prepare(aArgs[0]);
	}
}

MainWindow::~MainWindow() {
	if (components) {
		delete components;
	}
}

void MainWindow::OnBrowseFileButton_Click(Object^ sender, EventArgs^ e) {
	mFilePicker->FileName = "";  
	mFilePicker->ShowDialog();
}

void MainWindow::OnBrowseFolderButton_Click(Object^ sender, EventArgs^ e) {
	mFolderPicker->ShowDialog();
	
	if (mFolderPicker->SelectedPath->Length > 0) {
		mOutputFolderTextBox->Text = mFolderPicker->SelectedPath;
	}
}

void MainWindow::OnStartButton_Click(Object^ sender, EventArgs^ e) {
	char* sFilePath   = to_string(mFilePathTextBox->Text);
	char* sTargetPath = to_string(mOutputFolderTextBox->Text);
	char* sFormat	  = to_string(mFormatPicker->SelectedItem->ToString());

	switch (mTools) {
		case tools_base::TOOLS_OGF:
			{
				const char* sMotionName = mMotionPicker->SelectedItem ? to_string(mMotionPicker->SelectedItem->ToString()) : "";
				ogf_tools* tools = new ogf_tools;
				tools->process(sFilePath, sTargetPath, sFormat, sMotionName);
				delete tools;
			} break;
		case tools_base::TOOLS_OMF:
			{
				const char* sMotionName = mMotionPicker->SelectedItem ? to_string(mMotionPicker->SelectedItem->ToString()) : "";
				omf_tools* tools = new omf_tools;
				tools->process(sFilePath, sTargetPath, sFormat, sMotionName);
				delete tools;
			} break;
		case tools_base::TOOLS_DM:
			{
				dm_tools* tools = new dm_tools;
				tools->process(sFilePath, sTargetPath, sFormat);
				delete tools;
			} break;
		case tools_base::TOOLS_XRDEMO:
			{
				xrdemo_tools* tools = new xrdemo_tools;
				tools->process(sFilePath, sTargetPath);
				delete tools;
			} break;
	}
}

void MainWindow::OnStartDecompileLevelClick(Object^ sender, EventArgs^ e) {
	/*char* sProfile = to_string(mProfilePicker->SelectedItem->ToString());
	const char* sConfig = mIni->r_string("profiles", sProfile);
	std::string sLevelsPath = mIni->r_string(sConfig, PA_GAME_LEVELS);
*/

	//level_tools* tools = new level_tools;
	//tools->process(sConfig, to_string(mModePicker->SelectedItem->ToString()), to_string(mSceneNameTextBox->Text), to_string(mLevelPicker->SelectedItem->ToString()), mIni);
}

void MainWindow::OnFilePickerOk(Object^ sender, CancelEventArgs^ e) {
	Prepare(mFilePicker->FileName);
}

void MainWindow::OnDragDrop(Object^ sender, DragEventArgs^ e) {
	Prepare(((array<String^>^) e->Data->GetData(DataFormats::FileDrop))[0]);
}

void MainWindow::OnDragEnter(Object^ sender, DragEventArgs^ e) {
	if (e->Data->GetDataPresent(DataFormats::FileDrop)) {
		e->Effect = DragDropEffects::Copy;
	} else {
		e->Effect = DragDropEffects::None;
	}
}

void MainWindow::OnSelectedProfileChanged(Object^ sender, EventArgs^ e) {
	mLevelPicker->Items->Clear();

	char* sProfile = to_string(mProfilePicker->SelectedItem->ToString());
	const char* sConfig = mIni->r_string("profiles", sProfile);
	std::string sLevelsPath = mIni->r_string(sConfig, PA_GAME_LEVELS);

	WIN32_FIND_DATAA info;
	HANDLE h = FindFirstFile((sLevelsPath + '*').c_str(), &info);

	if (h != INVALID_HANDLE_VALUE) {
		do {
			if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (std::strcmp(info.cFileName, ".") != 0 && std::strcmp(info.cFileName, "..") != 0)
					mLevelPicker->Items->Add(to_string(info.cFileName));
			}
		} while (FindNextFile(h, &info));
		FindClose(h);
	}

	if (mLevelPicker->Items->Count > 0) {
		mLevelPicker->Enabled = true;
		mLevelPicker->SelectedIndex = 0;
	} else {
		mLevelPicker->Text = nullptr;
		mLevelPicker->Enabled = false;
	}
}

void MainWindow::OnSelectedLevelChanged(Object^ sender, EventArgs^ e) {
	mSceneNameTextBox->Text = mLevelPicker->SelectedItem->ToString();
}

void MainWindow::Prepare(String^ sPath) {
	char* sFilePath = to_string(sPath);

	msg("starting prepare...");

	std::string extension;
	xr_file_system::split_path(sFilePath, 0, 0, &extension);

	mTools = tools_base::TOOLS_NULL;

	if (extension == ".ogf")
		mTools |= tools_base::TOOLS_OGF;
	else if (extension == ".omf")
		mTools |= tools_base::TOOLS_OMF;
	else if (extension == ".dm")
		mTools |= tools_base::TOOLS_DM;
	else if (extension == ".xrdemo")
		mTools |= tools_base::TOOLS_XRDEMO;
	/*else if (db_tools::is_known(extension))
		format |= tools_base::TOOLS_DB;*/

	msg("tools %u", mTools);

	switch (mTools) {
		case tools_base::TOOLS_OGF:
		case tools_base::TOOLS_OMF:
		case tools_base::TOOLS_DM:
		case tools_base::TOOLS_XRDEMO:
			PrepareObjectTools(sFilePath);
			break;
	}
}

void MainWindow::PrepareObjectTools(char* sFilePath) {
	mMotionPicker->Text = nullptr;
	mMotionPicker->Enabled = false;
	mMotionPicker->Items->Clear();
	mFormatPicker->Items->Clear();
	mObjectInfoListBox->Items->Clear();

	mFilePathTextBox->Text = to_string(sFilePath);
	mStartButton->Enabled = true;

	switch (mTools) {
		case tools_base::TOOLS_OGF:
			{
				xr_ogf* ogf = xr_ogf::load_ogf(sFilePath);
				if (ogf) {
					mFormatPicker->Items->AddRange(mOGFFormats);

					if (!ogf->motions().empty()) {
						mFormatPicker->Items->AddRange(mOMFFormats);
						FillMotionComboBox(ogf->motions());

						mObjectInfoListBox->Items->Add("Motions count: " + ogf->motions().size());
						mObjectInfoListBox->Items->Add("");
					}

					const xr_ogf_vec& childrens = ogf->children();
					mObjectInfoListBox->Items->Add("Textures:");
					for (u32 i = 0; i < childrens.size(); i++) {
						String^ sValue = to_string(childrens[i]->texture().c_str());
						if (!mObjectInfoListBox->Items->Contains(sValue)) {
							mObjectInfoListBox->Items->Add(sValue);
						}
					}
					
					mObjectInfoListBox->Items->Add("");
					mObjectInfoListBox->Items->Add("Shaders:");
					for (u32 i = 0; i < childrens.size(); i++) {
						String^ sValue = to_string(childrens[i]->shader().c_str());
						if (!mObjectInfoListBox->Items->Contains(sValue)) {
							mObjectInfoListBox->Items->Add(sValue);
						}
					}
				} else {
					// TODO Message about can`t open ogf file
					msg("can`t load ogf");
					//return;
				}
				delete ogf;
			} break;
		case tools_base::TOOLS_OMF:
			{
				xr_ogf_v4* omf = new xr_ogf_v4;
				if (omf->load_omf(sFilePath)) {
					mFormatPicker->Items->AddRange(mOMFFormats);
					FillMotionComboBox(omf->motions());

					mObjectInfoListBox->Items->Add("Motions count: " + omf->motions().size());
				} else {
					// TODO Message about can`t open omf file				
					msg("can`t load omf");
					//return;
				}
				delete omf;
			} break;
		case tools_base::TOOLS_DM:
			{
				xr_dm* dm = new xr_dm;
				if (dm->load_dm(sFilePath)) {
					mFormatPicker->Items->AddRange(mDMFormats);

					mObjectInfoListBox->Items->Add("Texture: " + to_string(dm->texture().c_str()));
					mObjectInfoListBox->Items->Add("Shader: " + to_string(dm->shader().c_str()));
					mObjectInfoListBox->Items->Add("Scale (min/max): " + dm->min_scale() + "/" + dm->max_scale());
					mObjectInfoListBox->Items->Add("Flags: " + dm->flags());
				} else {
					// TODO Message about can`t open dm file				
					msg("can`t load dm");
					//return;
				}
			} break;
		case tools_base::TOOLS_XRDEMO:
			mFormatPicker->Items->AddRange(mXRDemoFormats);
			break;
	}

	mFormatPicker->Enabled = true;
	mFormatPicker->SelectedIndex = 0;
}

void MainWindow::FillMotionComboBox(xr_skl_motion_vec& motions) {
	if (!motions.empty()) {
		for (u32 i = 0; i < motions.size(); i++) {
			mMotionPicker->Items->Add(to_string(motions[i]->name().c_str()));
		}
		mMotionPicker->Enabled = true;
		mMotionPicker->SelectedIndex = 0;
	}
}