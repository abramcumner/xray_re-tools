; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgWayObjects
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GraphViewer.h"

ClassCount=7
Class1=CAppGraphViewer
Class2=CDlgGraphViewer

ResourceCount=9
Resource2=IDD_GRAPHVIEWER_DIALOG
Resource1=IDR_MAINFRAME
Resource3=IDD_SETTINGS
Class3=CDlgLoad
Resource4=IDD_INFO
Class4=CDlgInfo
Resource5=IDD_LOAD
Class5=CView2D
Resource6=IDR_CONTEXTMENU
Resource7=IDD_GRAPHVIEWER
Class6=CDlgSettings
Resource8=IDD_WAYOBJECTS
Class7=CDlgWayObjects
Resource9=IDM_MAINMENU

[CLS:CAppGraphViewer]
Type=0
HeaderFile=GraphViewer.h
ImplementationFile=GraphViewer.cpp
Filter=N

[CLS:CDlgGraphViewer]
Type=0
HeaderFile=DlgGraphViewer.h
ImplementationFile=DlgGraphViewer.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CDlgGraphViewer



[DLG:IDD_GRAPHVIEWER_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CDlgGraphViewer

[DLG:IDD_GRAPHVIEWER]
Type=1
Class=?
ControlCount=0

[DLG:IDD_LOAD]
Type=1
Class=CDlgLoad
ControlCount=10
Control1=IDC_LOAD_SELECT,button,1342242817
Control2=IDC_STATIC,static,1342308352
Control3=IDC_LOAD_FILE,edit,1350633600
Control4=IDC_LOAD_BROWS,button,1342242816
Control5=IDC_LOAD_LEVELS,listbox,1352728833
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_LOAD_HEADER_INFO,static,1342312449
Control9=IDC_STATIC,static,1342308352
Control10=IDC_LOAD_LEVEL_INFO,static,1342312449

[CLS:CDlgLoad]
Type=0
HeaderFile=DlgLoad.h
ImplementationFile=DlgLoad.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgLoad

[DLG:IDD_INFO]
Type=1
Class=CDlgInfo
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_INFO_POSITION,edit,1350633600
Control3=IDC_STATIC,static,1342308352
Control4=IDC_INFO_SEL_TEXT,edit,1352730756
Control5=IDC_INFO_SEL,combobox,1344339971
Control6=IDC_INFO_SELEX,combobox,1478557699

[CLS:CDlgInfo]
Type=0
HeaderFile=DlgInfo.h
ImplementationFile=DlgInfo.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_INFO_SELEX

[MNU:IDR_CONTEXTMENU]
Type=1
Class=?
Command1=IDM_LOAD
Command2=IDM_SHOW_INFO
Command3=IDM_HIDE_INFO
CommandCount=3

[CLS:CView2D]
Type=0
HeaderFile=View2D.h
ImplementationFile=View2D.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CView2D

[MNU:IDM_MAINMENU]
Type=1
Class=?
Command1=IDM_OPEN
Command2=IDM_CLOSE
Command3=IDM_SHOW_SETTINGS
Command4=IDM_SHOW_INFO
Command5=IDM_SHOW_WAYS
CommandCount=5

[DLG:IDD_SETTINGS]
Type=1
Class=CDlgSettings
ControlCount=18
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_SETTINGS_ZOOM,combobox,1344339971
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_SETTINGS_RANGE,msctls_trackbar32,1342242848
Control7=IDC_SETTINGS_SETMIN,button,1342242816
Control8=IDC_SETTINGS_SETMAX,button,1342242816
Control9=IDC_STATIC,static,1342308864
Control10=IDC_STATIC,static,1342308864
Control11=IDC_SETTINGS_MIN,edit,1350633600
Control12=IDC_SETTINGS_CURMIN,edit,1350633600
Control13=IDC_SETTINGS_MAX,edit,1350633600
Control14=IDC_SETTINGS_CURMAX,edit,1350633600
Control15=IDC_STATIC,static,1342308865
Control16=IDC_STATIC,static,1342308865
Control17=IDC_STATIC,button,1342177287
Control18=IDC_SETTINGS_COVER,combobox,1344339971

[CLS:CDlgSettings]
Type=0
HeaderFile=DlgSettings.h
ImplementationFile=DlgSettings.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_SETTINGS_COVER

[DLG:IDD_WAYOBJECTS]
Type=1
Class=CDlgWayObjects
ControlCount=9
Control1=IDC_WAYOBJECT_FILETITLE,static,1342308352
Control2=IDC_WAYOBJECTS_FILE,edit,1350633600
Control3=IDC_WAYOBJECTS_BROWS,button,1342242816
Control4=IDC_WAYOBJECTS_LIST,listbox,1352728835
Control5=IDC_WAYOBJECTS_INFO,edit,1352730756
Control6=IDC_WAYOBJECTS_SELECTED,listbox,1352728833
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[CLS:CDlgWayObjects]
Type=0
HeaderFile=DlgWayObjects.h
ImplementationFile=DlgWayObjects.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_WAYOBJECTS_LIST

