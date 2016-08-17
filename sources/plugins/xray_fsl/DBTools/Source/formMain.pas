{-----------------------------------------------------------------------------
 Unit Name: formMain
 Author:    Neo][
 Date:      26-май-2009
 Purpose:   Главная форма приложения.
 History:   26-май-2009   Initial revision.

 ToDo:

-----------------------------------------------------------------------------}

unit formMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ImgList, PlatformDefaultStyleActnCtrls, ActnList, ActnMan, ActnCtrls,
  ToolWin, ActnMenus, VirtualTrees, StdCtrls, xrFSLDBTools;

type
  {*------------------------------------------------------------------------------
    Класс главной формы приложения.
  -------------------------------------------------------------------------------}
  TfrmMain = class(TForm)
    actmgrMain: TActionManager;
    ilToolbar: TImageList;
    actmmbMain: TActionMainMenuBar;
    acttbMain: TActionToolBar;
    actCloseApp: TAction;
    actAbout: TAction;
    vstFiles: TVirtualStringTree;
    dlgOpen: TOpenDialog;
    actFileOpen: TAction;
    mmoLog: TMemo;
    procedure actCloseAppExecute(Sender: TObject);
    procedure actAboutExecute(Sender: TObject);
    procedure actFileOpenExecute(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure vstFilesGetText(Sender: TBaseVirtualTree; Node: PVirtualNode;
      Column: TColumnIndex; TextType: TVSTTextType; var CellText: string);
    procedure vstFilesFreeNode(Sender: TBaseVirtualTree; Node: PVirtualNode);
  private
    FDBTools: TDBTools;                 /// Экземпляр класса архива

    procedure OpenArchive(const aFileName: string);
  public
    { Public declarations }
  end;

var
  frmMain: TfrmMain;

implementation

uses
  formProgress, dbtOperationsThreads, dbtCommon;

{$R *.dfm}

procedure TfrmMain.actAboutExecute(Sender: TObject);
begin
  MessageDlg('xrFSL DBTools' + #13 + #10 + 'http://gameru.net/' + #13 + #10 + 'Neo][  2009(c)',
    mtInformation, [mbOK], 0);
end;

procedure TfrmMain.actCloseAppExecute(Sender: TObject);
begin
  Close;
end;

procedure TfrmMain.actFileOpenExecute(Sender: TObject);
begin
  mmoLog.Clear;

  if dlgOpen.Execute and FileExists(dlgOpen.FileName) then
    OpenArchive(dlgOpen.FileName);
end;

{*------------------------------------------------------------------------------
  Чтение содержимого архива в отдельном потоке и заполнение VST.
  @param aFileName Полный путь до файла архива.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TfrmMain.OpenArchive(const aFileName: string);
var
  ProgressForm: TfrmProgress;
  VSTHelper: TVSTHelper;

  OpenThread: TOpenDBThread;
begin
  //--------------------------
  // Очищаем временные данные
  //--------------------------
  vstFiles.Clear;
  FDBTools.Close;

  ProgressForm := TfrmProgress.Create(Self);
  try
    OpenThread := TOpenDBThread.Create(ProgressForm, FDBTools, dlgOpen.FileName);
    try
      // стопорим поток показом модального окна прогресса.
      // поток сам закроет окно и освободит главный поток, когда чтение
      // архива будет завершено
      ProgressForm.ShowModal;

      //------------------------------
      // Заполнение VST
      //-------------------------------
      VSTHelper := TVSTHelper.Create(vstFiles);
      try
        VSTHelper.FillTreeWithFiles(FDBTools.Files);
      finally
        FreeAndNil(VSTHelper);
      end;
    finally
      OpenThread.Free;
    end;
  finally
    ProgressForm.Free;
  end;
end;

{*------------------------------------------------------------------------------

  @param Sender
  @param Node
  @param Column
  @param TextType
  @param CellText
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TfrmMain.vstFilesGetText(Sender: TBaseVirtualTree; Node: PVirtualNode;
  Column: TColumnIndex; TextType: TVSTTextType; var CellText: string);
var
  pNodeData: PDBTViewerItem;
begin
  pNodeData := Sender.GetNodeData(Node);

  if Assigned(pNodeData) and (Assigned(pNodeData.Data)) then
    case Column of
      //0: CellText := pNodeData^.pDBFile^.Path;
      0:
        begin
          //Assert(pNodeData^.Index < FDBTools.Files.Count);
          CellText := pNodeData.Data.ItemName;
        end;
      //1: CellText := pNodeData.Folder;

    end;

end;

{*------------------------------------------------------------------------------

  @param Sender
  @param Node
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TfrmMain.vstFilesFreeNode(Sender: TBaseVirtualTree;
  Node: PVirtualNode);
var
  NodeData: PDBTViewerItem;
begin
  NodeData := Sender.GetNodeData(Node);

  if Assigned(NodeData) and Assigned(NodeData.Data) then
    FreeAndNil(NodeData.Data);
end;

{*------------------------------------------------------------------------------

  @param Sender
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TfrmMain.FormCreate(Sender: TObject);
begin
  // Сообщать об утечках памяти, если программа запущена из под IDE
{$WARNINGS OFF}
  //ReportMemoryLeaksOnShutdown := DebugHook <> 0;
{$WARNINGS ON}

  FDBTools := TDBTools.Create;

  // Настройка VST
  vstFiles.NodeDataSize := SizeOf(TDBTViewerItem);
end;

{*------------------------------------------------------------------------------

  @param Sender
  @param CanClose
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TfrmMain.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
  vstFiles.Clear;

  if Assigned(FDBTools) then
    FreeAndNil(FDBTools);
end;

end.

