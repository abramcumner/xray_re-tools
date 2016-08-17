{-----------------------------------------------------------------------------
 Unit Name: dbtCommon
 Author:    Neo][
 Date:      27-май-2009
 Purpose:
 History:
-----------------------------------------------------------------------------}


unit dbtCommon;

interface

uses
  xrFSLDBTools, VirtualTrees, Classes;

type
  // Forward decl
  TDBTBaseViewerItemData = class;

  {*------------------------------------------------------------------------------
    Структура кастом данных VST.
  -------------------------------------------------------------------------------}
  PDBTViewerItem = ^TDBTViewerItem;
  TDBTViewerItem = record
    Data: TDBTBaseViewerItemData;       /// Указатель на экземпляр класса данных
  end;

  {*------------------------------------------------------------------------------
    Базовый класс для данных итема VST.
  -------------------------------------------------------------------------------}
  TDBTBaseViewerItemData = class
  private
    FItemName: string;                  /// имя итема
    FIsDir: Boolean;                    /// Флаг является ли итем директорией

    FPath: string;                      /// Путь до папки с файлом
    FOffset: Cardinal;                  /// Смещение начала файла
    FRealSize: Cardinal;                /// Реальный размер файла
    FCompressedSize: Cardinal;          /// Размер сжатых данных
    FCRC: Cardinal;                     /// CRC32 файла

  public
    { constructors/destructord }
    constructor Create; overload;
    constructor Create(const aName: string; const aFileInfo: TDBFile); overload;
    destructor Destroy; override;

    { properties }
    property CompressedSize: Cardinal read FCompressedSize write FCompressedSize;
    property CRC: Cardinal read FCRC write FCRC;
    property IsDir: Boolean read FIsDir write FIsDir;
    property ItemName: string read FItemName write FItemName;
    property Offset: Cardinal read FOffset write FOffset;
    property Path: string read FPath write FPath;
    property RealSize: Cardinal read FRealSize write FRealSize;
  end;

  {*------------------------------------------------------------------------------
    Вспомогательный класс для работы с VST
  -------------------------------------------------------------------------------}
  TVSTHelper = class
  private
    { common }
    FVST: TBaseVirtualTree;             /// Указатель на дерево с которым работаем

    { Fill tree with files }
    FCachedStrs: TStringList;           /// ???

    procedure AddItem(aLevel: Integer; aParentNode: PVirtualNode;
      const aFileInfo: TDBFile; aPathElIndex: Integer = -1);
    function FindNodeWithText(aParent: PVirtualNode; const aText: string): PVirtualNode;
  public
    constructor Create(const aVST: TBaseVirtualTree);

    procedure FillTreeWithFiles(const aFiles: TDBFileList);
  end;

implementation

uses
  SysUtils;

{ TVSTHelper }

{*------------------------------------------------------------------------------
  Конструктор по умолчанию.
  @param aVST
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TVSTHelper.Create(const aVST: TBaseVirtualTree);
begin
  FVST := aVST;
end;

{*------------------------------------------------------------------------------
  Построение дерева каталогов по списку файлов.
  @param aFiles
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TVSTHelper.FillTreeWithFiles(const aFiles: TDBFileList);
var
  i: Integer;
begin
  FCachedStrs := TStringList.Create;
  FCachedStrs.Duplicates := dupIgnore;
  FCachedStrs.Sorted := True;
  try
    FVST.BeginUpdate;

    for i := 0 to aFiles.Count - 1 do
    begin
      AddItem(0, nil, aFiles.Items[i]);
    end;
  finally
    FVST.EndUpdate;
    FCachedStrs.Free;
  end;
end;

{*------------------------------------------------------------------------------

  @param aParent
  @param aText
  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TVSTHelper.FindNodeWithText(aParent: PVirtualNode;
  const aText: string): PVirtualNode;
var
  K: Integer;
  fStr: string;
  tmpNode: PVirtualNode;
  NodeData: PDBTViewerItem;
begin
  Result := nil;

  fStr := aText + IntToStr(Integer(AParent));
  K := FCachedStrs.IndexOf(fStr);

  if K > -1 then
    Result := Pointer(FCachedStrs.Objects[K])
  else
  begin
    //    if aParent <> nil then
    //      tmpNode := aParent.FirstChild
    //    else
    tmpNode := FVST.GetFirstChild(aParent);

    while tmpNode <> nil do
    begin
      NodeData := FVST.GetNodeData(tmpNode);

      if Assigned(NodeData) and (Assigned(NodeData.Data)) then
        if CompareText(NodeData.Data.ItemName, aText) = 0 then
        begin
          Result := tmpNode;
          FCachedStrs.AddObject(fStr, Pointer(tmpNode));
          Break;
        end;

      tmpNode := FVST.GetNextSibling(tmpNode);
    end;
  end
end;

{*------------------------------------------------------------------------------

  @param aLevel
  @param aParentNode
  @param aFileInfo
  @param aPathElIndex
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TVSTHelper.AddItem(aLevel: Integer; aParentNode: PVirtualNode;
  const aFileInfo: TDBFile; aPathElIndex: Integer = -1);
var
  prefix, RestPath: string;
  ID: Integer;
  Node: PVirtualNode;
  NodeData: PDBTViewerItem;
begin
  if aFileInfo.Path = '' then
    Exit;

  if aPathElIndex = -1 then
  begin
    RestPath := aFileInfo.Path;
    aPathElIndex := 1;
  end
  else
  begin
    RestPath := Copy(aFileInfo.Path, aPathElIndex, Length(aFileInfo.Path));
    //Inc(aPathElIndex);                  // Пропуск слэша на след. рекурсии
  end;

  ID := Pos('\', RestPath);
  prefix := '';

  if ID > 0 then
    prefix := Copy(RestPath, 1, ID - 1)
  else
  begin
    prefix := RestPath;
    RestPath := '';
  end;

  Node := FindNodeWithText(aParentNode, prefix);

  if Node = nil then
  begin
    Node := FVST.AddChild(aParentNode);
    NodeData := FVST.GetNodeData(Node);

    if Assigned(NodeData) then
    begin
      NodeData.Data := TDBTBaseViewerItemData.Create(prefix, aFileInfo);
      NodeData.Data.IsDir := RestPath <> '';
    end;
  end;

  if RestPath <> '' then
    AddItem(aLevel + 1, Node, aFileInfo, aPathElIndex + ID);
end;

{ TDBTBaseViewerItemData }

{*------------------------------------------------------------------------------
  Конструктор по умолчанию. Инициализирует все поля пустыми/нулевыми значениями.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TDBTBaseViewerItemData.Create;
begin
  FItemName := '';
  FPath := '';
  FOffset := 0;
  FRealSize := 0;
  FCompressedSize := 0;
  FCRC := 0;
end;

{*------------------------------------------------------------------------------
  Конструктор, заполняющий поля из структуры с информацией о файле.
  @param aFileInfo структура с информацией о файле.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TDBTBaseViewerItemData.Create(const aName: string;
  const aFileInfo: TDBFile);
begin
  FItemName := aName;
  FIsDir := False;

  FPath := aFileInfo.Path;
  FOffset := aFileInfo.Offset;
  FRealSize := aFileInfo.RealSize;
  FCompressedSize := aFileInfo.CompressedSize;
  FCRC := aFileInfo.CRC;
end;

{*------------------------------------------------------------------------------

  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

destructor TDBTBaseViewerItemData.Destroy;
begin

  inherited;
end;

end.

