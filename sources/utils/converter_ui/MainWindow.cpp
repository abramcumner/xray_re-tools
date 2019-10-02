#include "pch.h"
#include "MainWindow.h"

#pragma unmanaged
#include "dm_tools.h"
#include "ogf_tools.h"
#include "xr_ogf_v4.h"
#pragma managed

#include "xr_file_system.h"
#include "xr_log.h"

using namespace xray_re;
using namespace ConverterUI;

/* Help for understanding
Ogf options:
	object
	skls
	skl
	bones

Omf options:
	skls
	skl

Xrdemo options:
	anm - one target

DM options:
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
*/

MainWindow::MainWindow(array<String^>^ aArgs) {
	xr_file_system& fs = xr_file_system::instance();
	if (!fs.initialize(DEFAULT_FS_SPEC)) {
		msg("can't initialize the file system");
	}
	xr_log::instance().init("converter");

	InitializeComponent();

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
	LPSTR sFilePath   = to_string(mFilePathTextBox->Text);
	LPSTR sTargetPath = to_string(mOutputFolderTextBox->Text);
	LPSTR sFormat	  = to_string(mFormatPicker->SelectedItem->ToString());

	switch (mTools) {
		case object_tools::TOOLS_OGF: // OGF
			{
				LPCSTR sMotionName = mMotionPicker->SelectedItem ? to_string(mMotionPicker->SelectedItem->ToString()) : "";
				ogf_tools* tools = new ogf_tools;
				tools->process(sFilePath, sTargetPath, sFormat, sMotionName);
			} break;
		case object_tools::TOOLS_OMF: // OMF
			{
				LPCSTR sMotionName = mMotionPicker->SelectedItem ? to_string(mMotionPicker->SelectedItem->ToString()) : "";
				omf_tools* tools = new omf_tools;
				tools->process(sFilePath, sTargetPath, sFormat, sMotionName);
				delete tools;
			} break;
		case object_tools::TOOLS_DM: // DM
			{
				dm_tools* tools = new dm_tools;
				tools->process(sFilePath, sTargetPath, sFormat);
				delete tools;
			} break;
	}
}

void MainWindow::OnFilePickerOk(Object^ sender, CancelEventArgs^ e) {
	Prepare(mFilePicker->FileName);
}

void MainWindow::OnDragDrop(Object^ sender, DragEventArgs^ e) {
	array<String^>^ objs = (array<String^>^) e->Data->GetData(DataFormats::FileDrop);
	Prepare(objs[0]);
}

void MainWindow::OnDragEnter(Object^ sender, DragEventArgs^ e) {
	if (e->Data->GetDataPresent(DataFormats::FileDrop)) {
		e->Effect = DragDropEffects::Copy;
	} else {
		e->Effect = DragDropEffects::None;
	}
}

void MainWindow::Prepare(String^ sPath) {
	LPSTR sFilePath = to_string(sPath);

	msg("starting prepare...");

	std::string extension;
	xr_file_system::split_path(sFilePath, 0, 0, &extension);

	mTools = object_tools::TOOLS_NULL;

	if (extension == ".ogf")
		mTools = object_tools::TOOLS_OGF;
	else if (extension == ".omf")
		mTools = object_tools::TOOLS_OMF;
	else if (extension == ".dm")
		mTools = object_tools::TOOLS_DM;
	/*else if (extension == ".xrdemo")
		mTools |= object_tools::TOOLS_XRDEMO;
	else if (db_tools::is_known(extension))
		format |= object_tools::TOOLS_DB;*/

	msg("tools %u", mTools);

	switch (mTools) {
		case object_tools::TOOLS_OGF:
		case object_tools::TOOLS_OMF:
		case object_tools::TOOLS_DM:
		//case object_tools::TOOLS_XRDEMO:
			PrepareObjectTools(sFilePath);
			break;
	}
}

void MainWindow::PrepareObjectTools(LPSTR sFilePath) {
	mMotionPicker->Text = nullptr;
	mMotionPicker->Enabled = false;
	mMotionPicker->Items->Clear();
	mFormatPicker->Items->Clear();

	mStartButton->Enabled = true;
	mFilePathTextBox->Text = to_string(sFilePath);

	switch (mTools) {
		case object_tools::TOOLS_OGF:
			{
				msg("load ogf");

				xr_ogf* ogf = xr_ogf::load_ogf(sFilePath);
				if (ogf) {
					msg("file loaded");

					mFormatPicker->Items->AddRange(mOGFFormats);

					if (!ogf->motions().empty()) {
						msg("file has motions");
						mFormatPicker->Items->AddRange(mOMFFormats);
						FillMotionComboBox(ogf->motions());
					}
				} else {
					// TODO Message about can`t open ogf file
					msg("can`t load file");
				}
				delete ogf;
			} break;
		case object_tools::TOOLS_OMF:
			{
				msg("load omf");

				xr_ogf_v4* omf = new xr_ogf_v4;
				if (omf->load_omf(sFilePath)) {
					msg("file loaded");

					mFormatPicker->Items->AddRange(mOMFFormats);
					FillMotionComboBox(omf->motions());
				} else {
					// TODO Message about can`t open omf file				
					msg("can`t load file");
				}
				delete omf;
			} break;
		case object_tools::TOOLS_DM:
			mFormatPicker->Items->AddRange(mDMFormats);
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