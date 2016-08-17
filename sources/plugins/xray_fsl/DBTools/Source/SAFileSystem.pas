{-----------------------------------------------------------------------------
 Unit Name: SAFileSystem
 Author:    Neo][
 Date:      24-май-2009
 Purpose:   Реализация виртуальной файловой системы, реализует различные
 методы для работы с файловой системой.
 History:
 ToDo:      -- Разобраться с предопределёнными алиасами
-----------------------------------------------------------------------------}

unit SAFileSystem;

interface

uses
  Generics.Collections, SysUtils, SAScrambler;

const
  //// Константы алиасов путей.

  PA_APP_DIR = '$app_dir$';             /// Алиас директории приложения

  CHUNK_COMPRESSED = $80000000;
  CHUNK_ID_MASK = not CHUNK_COMPRESSED;

type

  {*------------------------------------------------------------------------------
    Базовый класс для исключений файловой системы.
  -------------------------------------------------------------------------------}
  ESAFileSystem = class(Exception);

  {*------------------------------------------------------------------------------
    Класс исключения, возбуждаемого при ошибках парсинка файла спецификации
    путей.
  -------------------------------------------------------------------------------}
  ESAFSSpecFile = class(ESAFileSystem);

  {*------------------------------------------------------------------------------
    Структура алиаса пути.
  -------------------------------------------------------------------------------}
  TSAPathAlias = record
    Name,                               /// Имя алиаса
    Root,                               /// Путь алиаса
    Caption: string;                    /// Название(НЕ РЕАЛИЗОВАНО)
  end;

  {*------------------------------------------------------------------------------
    Базовый класс инкапсулирующий в себе функции чтения файла.
  -------------------------------------------------------------------------------}
  TSAReader = class
  private
    { setters/ getters }
    function GetSize: Cardinal;
    function GetData: Pointer;
    function GetEOF: Boolean;
  protected
    FData: PByte;                       /// Указатель на начало данных
    FNext: PByte;                       /// ???
    FEnd: PByte;                        /// Указатель на конец данных
    FCaret: record                      /// Указатель на текущую позицию
      case Integer of
        0: (chr: PAnsiChar);
        1: (u8: PByte);
        2: (s8: PShortInt);
        3: (u16: PWord);
        4: (s16: PSmallInt);
        5: (u32: PLongWord);
        6: (s32: PLongint);
        7: (f: PSingle);
    end;

  public
    (* constructors/destructors *)
    constructor Create; overload;
    constructor Create(aData: Pointer; aSize: Cardinal); overload;
    destructor Destroy; override;

    { chunks operations }
    function FindChunk(aID: Cardinal; var aCompressed: Boolean; aReset: Boolean = True): Cardinal; overload;
    function FindChunk(aID: Cardinal): Cardinal; overload;

    function OpenChunk(aID: Cardinal): TSAReader; overload;
    function OpenChunk(aID: Cardinal; const aScrambler: TSAScrambler): TSAReader; overload;
    procedure CloseChunk(var aReader: TSAReader);

    { reading }
    procedure r_raw(aDestination: Pointer; aDestSize: Cardinal);
    procedure r_sz(var aDest: string);
    function r_u32: LongWord;
    function r_s32: LongInt;
    function r_u24: Cardinal;
    function r_u16: Word;
    function r_s16: SmallInt;
    function r_u8: Byte;
    function r_s8: ShortInt;
    function r_bool: Boolean;

    { properties }
    property Size: Cardinal read GetSize;
    property Data: Pointer read GetData;
    property EOF: Boolean read GetEOF;
  end;

  {*------------------------------------------------------------------------------
    Класс ридера, который сам занимается управлением памятью.
  -------------------------------------------------------------------------------}
  TSATempReader = class(TSAReader)
  public
    destructor Destroy; override;
  end;

  {*------------------------------------------------------------------------------
    Класс реализующий чтение из файл мэппинга.
  -------------------------------------------------------------------------------}
  TSAMMapReader = class(TSAReader)
  private
    FHFile,                             /// Хэндл замапленного файла
    FHMMap: Cardinal;                   /// Хэндл memory mapping-a
  public
    //## constructors/destructors
    constructor Create; overload;
    constructor Create(aHFile, aHMMap: Cardinal; aData: Pointer; aSize: Cardinal); overload;
    destructor Destroy; override;

  end;

  {-------------------------------------------------------------------------------
   Класс виртуальной файловой системы, инкапсулирует в себе различные методы для
   работы с файловой системой.
   @Note После создания экземпляра класса, его необходимо проинициализировать
   вызовом метода Init. В качестве параметра которому передаётся имя файла
   спецификации путей. Файл спецификации путей содержит определения алиасов путей.

   Формат алиаса следующий:

   $идентификатор$ | root_path | add_path | caption
   -------------------------------------------------------------------------------}
  TSAFileSystem = class
  private
    FPathAliases: TList < TSAPathAlias > ; /// Список алиасов путей

    //## path aliases
    function AddPathAlias(const aName, aRoot, aAdd: string): Integer;
    function FindPathAlias(const aName: string): Integer;
    procedure ParseFSSpec(const aReader: TSAReader);
  public
    //## constructors/destructors
    constructor Create;
    destructor Destroy; override;

    //## path aliases
    function Init(const aFSSpecFileName: string): Boolean;

    //## read
    function ReadOpen(const aFileName: string): TSAReader;
    procedure ReadClose(var aReader: TSAReader);
  end;

resourcestring
  StrFSSpecFileParsingErr = 'Can''t parse FS specification file, line: %d';

implementation

uses
  Windows, Character, SAStringUtils, Classes, LZH;

function GetFileSizeEx(hFile: Cardinal; out lpFileSize: LARGE_INTEGER): BOOL; stdcall; external 'kernel32.dll';
//procedure lzh_decompress(Code: PByte; var CodeSize: Cardinal; const Text: PByte; TextSize: Cardinal); cdecl; external 'xrFSL_Core.dll';

{============================================}
{============= TSAFileSystem ================}
{============================================}

{*------------------------------------------------------------------------------
  Конструктор по умолчанию.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TSAFileSystem.Create;
begin
  FPathAliases := TList < TSAPathAlias > .Create;
end;

{*------------------------------------------------------------------------------
  Деструктор по умолчанию.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

destructor TSAFileSystem.Destroy;
begin
  FPathAliases.Free;

  inherited;
end;

{*------------------------------------------------------------------------------
  Добавление нового алиаса.
  @param aName Идентификатор алиаса.
  @param aRoot Путь алиаса.
  @param aAdd Дополнение в пути.
  @return Индекс добавленного алиаса в списке.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TSAFileSystem.AddPathAlias(const aName, aRoot, aAdd: string): Integer;
var
  Idx: Integer;
  PathAlias: TSAPathAlias;
begin
  Idx := FindPathAlias(aName);
  Assert(Idx = -1, Format('Such path alias(%s) alredy exists.', [aName]));

  if Idx <> -1 then
    Exit(-1);

  // Подготовим алиас
  PathAlias.Name := aName;

  // пробуем найти алиас рутовой директории
  Idx := FindPathAlias(aRoot);
  if Idx <> -1 then
    PathAlias.Root := FPathAliases[Idx].Root
  else
  begin
    PathAlias.Root := aRoot;
    TSAStringUtils.PathAppendSeparator(PathAlias.Root);
  end;

  // Если есть дополнение, тогда его тоже дописываем
  PathAlias.Root := PathAlias.Root + aAdd;
  TSAStringUtils.PathAppendSeparator(PathAlias.Root);

  // Добавим алиас в список
  Result := FPathAliases.Add(PathAlias);
end;

{*------------------------------------------------------------------------------
  Поиск алиаса по идентификатору.
  @param aName Идентификатор алиаса.
  @return Индекс в списке.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TSAFileSystem.FindPathAlias(const aName: string): Integer;
var
  I: Integer;
begin
  for I := 0 to FPathAliases.Count - 1 do
    if FPathAliases[i].Name = aName then
      Exit(i);

  Result := -1;
end;

{*------------------------------------------------------------------------------
  Разбор файла спецификации путей.
  @note Файл спецификации путей содержит определения алиасов путей.
  Формат алиаса следующий:
  $идентификатор$ | root_path | add_path | caption
  @param aReader Экземпляр ридера ассоциированного с файлом спецификации.
  @author Neo][
  @throws ESAFSSpecFile
  @todo
-------------------------------------------------------------------------------}

procedure TSAFileSystem.ParseFSSpec(const aReader: TSAReader);

// Переход на следующую строку в файле
  function NextLine(aPCaret, aPEnd: PAnsiChar): PAnsiChar;
  begin
    while (aPCaret < aPEnd) and (aPCaret^ <> #10) do
      Inc(aPCaret);

    Result := aPCaret + 1;
  end;

  // чтение имени алиаса
  function ReadAlias(aPCaret, aPEnd: PAnsiChar): PAnsiChar;
  begin
    if ((aPCaret >= aPEnd) or (aPCaret^ <> '$')) then
      Exit(nil);

    Inc(aPCaret);

    if ((aPCaret >= aPEnd) or
      (not TCharacter.IsLetterOrDigit(Char(aPCaret^)) and
      (aPCaret^ <> '_'))) then
      Exit(nil);

    while aPCaret < aPEnd do
    begin
      if aPCaret^ = '$' then
        Exit(aPCaret + 1)
      else if (not TCharacter.IsLetterOrDigit(Char(aPCaret^)) and
        (aPCaret^ <> '_')) then
        Break;

      Inc(aPCaret);
    end;

    Result := nil;
  end;

  // Пропуск специальных символов(пробел, табуляция, etc.)
  function SkipSS(aPCaret, aPEnd: PAnsiChar): PAnsiChar;
  begin
    while aPCaret < aPEnd do
    begin
      if (aPCaret^ <> ' ') and (aPCaret^ <> #9) then
        Break;

      Inc(aPCaret);
    end;
    Result := aPCaret;
  end;

  // чтение значения алиаса
  function ReadValue(aPCaret, aPEnd: PAnsiChar): PAnsiChar;
  var
    PLastSS: PAnsiChar;
  begin
    aPCaret := SkipSS(aPCaret, aPEnd);
    PLastSS := nil;

    while aPCaret < aPEnd do
    begin
      if (aPCaret^ = ' ') or (aPCaret^ = #9) then
      begin
        if PLastSS = nil then
          PLastSS := aPCaret;
      end
      else if (aPCaret^ = '#10') or (aPCaret^ = #13) or (aPCaret^ = '|') then
      begin
        if PLastSS = nil then
          PLastSS := aPCaret;

        Break;
      end
      else
        PLastSS := nil;

      Inc(aPCaret);
    end;

    if PLastSS <> nil then
      Result := PLastSS
    else
      Result := aPCaret;
  end;

  // Преобразование указателей в строку
  function ToString(aPCaret, aPEnd: PAnsiChar): string;
  var
    TmpAnsi: AnsiString;
    Len: Integer;
  begin
    { TODO -oNeo][ -c : Check bounds 12.04.2009 20:07:59 }
    Len := aPEnd - aPCaret;
    SetLength(TmpAnsi, Len);
    Move(aPCaret^, TmpAnsi[1], Len);
    Result := string(TmpAnsi);
  end;

  //---------- Main func impl ----------
var
  PCaret,                               // Указатель на текущую позицию в файле
  PEnd,                                 // Указатель на конец файла
  PLast: PAnsiChar;                     // Указатель на конец какого-либо токена при разборе и т.п.

  Line: Cardinal;                       // Строка в файле
  TmpAliasName: string;                 // имя алиаса
  TmpValues: array[0..2] of string;     // массив параметров алиаса
  I: Integer;
begin
  PCaret := aReader.FCaret.chr;
  PEnd := PCaret + aReader.Size;

  Line := 1;
  while PCaret < PEnd do
  begin
    // Встретили символ начала идентификатора алиаса
    if PCaret^ = '$' then
    begin
      // Получаем содержимое строки между $...$, включая оба $
      PLast := ReadAlias(PCaret, PEnd);
      if PLast = nil then
      begin
        raise ESAFSSpecFile.CreateFmt(StrFSSpecFileParsingErr, [Line]);
      end;

      // Записывем идентификатор алиаса
      TmpAliasName := ToString(PCaret, PLast);

      // Пропускаем специальные символы
      PCaret := SkipSS(PLast, PEnd);
      if (PCaret = PEnd) or ((PCaret)^ <> '|') then
      begin
        raise ESAFSSpecFile.CreateFmt(StrFSSpecFileParsingErr, [Line]);
      end;

      // Читаем параметры алиаса
      for I := 0 to 2 do
      begin
        // Пропускаем символ разделителя | и спец. символы
        Inc(PCaret);
        PCaret := SkipSS(PCaret, PEnd);

        // Читаем значение алиаса
        PLast := ReadValue(PCaret, PEnd);

        //if (PLast = PEnd) {or (SkipSS(PLast, PEnd)^ <> '|')} then
        //begin
        //  gCore.Log.LogFatal('[FS]Can''t parse FS specification file, line: %d', [Line]);
        //  Exit(False);
        //end;

        TmpValues[i] := ToString(PCaret, PLast);

        // Пропускаем спец. символы после значения.
        PCaret := SkipSS(PLast, PEnd);

        // Если после значения нет разделителя или достигли конца файла - завершаем цикл
        if (PCaret = PEnd) or (SkipSS(PLast, PEnd)^ <> '|') then
          Break;
      end;

      // Добавляем алиас в список
      if AddPathAlias(TmpAliasName, TmpValues[0], TmpValues[1]) = -1 then
      begin
        raise ESAFSSpecFile.CreateFmt(StrFSSpecFileParsingErr, [Line]);
      end;
    end
    else if (PCaret^ <> ';') and (PCaret^ <> ' ') then  { TODO : А, если вместо пробела пользуемся табуляцией!? Так что добавить ещё SkipSS }
    begin
      raise ESAFSSpecFile.CreateFmt(StrFSSpecFileParsingErr, [Line]);
    end;

    // Переходим на след. строку
    PCaret := NextLine(PCaret, PEnd);
    Inc(Line);
  end;
end;

{*------------------------------------------------------------------------------
  Инициализация файловой системы. Получение всех путей из файла спецификации.
  @param aFSSpecFileName Имя файла спецификации путей.
  @return True, если успешно.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TSAFileSystem.Init(const aFSSpecFileName: string): Boolean;
var
  R: TSAReader;
begin
  if aFSSpecFileName <> '' then
  begin
    // Открываем файл
    R := ReadOpen(aFSSpecFileName);
    if (R = nil) then
      Exit(False);

    // Если не смогли распарсить файл спецификации путей, тогда очистим список
    // алиасов, функция в итоге вернёт False
    try
      try
        ParseFSSpec(R);
      finally
        ReadClose(R);
      end;
    except
      FPathAliases.Clear;
      //raise;
    end;
  end;

  Result := FPathAliases.Count <> 0;
end;

{*------------------------------------------------------------------------------
  Открытие файла на чтение.
  @param aFileName Имя файла.
  @return Возращает указатель на экземпляр TSAReader, асигнованный с файлом.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TSAFileSystem.ReadOpen(const aFileName: string): TSAReader;
var
  hFile,                                // хэндл файла
  hMMap: Cardinal;                      // хэндл memory mapping-a
  size64: LARGE_INTEGER;                // 64х размер файла
  Len: Cardinal;
  SI: SYSTEM_INFO;
  Reader: TSAReader;                    // указатель на ридер
  Data: Pointer;                        // Указатель на данные файла
  Read: Cardinal;                       // Количество прочитанных байт из файла
begin
  Reader := nil;

  hFile := CreateFile(PChar(aFileName),
    GENERIC_READ,
    FILE_SHARE_READ,
    nil,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    0);

  if hFile = INVALID_HANDLE_VALUE then
    Exit(nil);

  //-------------------------
  // получаем размер файла
  //-------------------------
  if ((not GetFileSizeEx(hFile, size64)) or (size64.HighPart <> 0)) then
  begin
    CloseHandle(hFile);
    Exit(nil);
  end;

  Len := size64.LowPart;
  GetSystemInfo(SI);

  //-----------------------------------------------
  // Если файл помещается в страницу памяти(64кб),
  // тогда загрузим данные в кучу
  //-----------------------------------------------
  if Len < SI.dwAllocationGranularity then
  begin
    try
      // выделяем память под данные из файла
      Data := GetMemory(Len);           /// !!! Throw Exception
    except
      Data := nil
    end;

    // Если удалось выделить память - читаем данные из файла
    if Assigned(Data) then
      if (ReadFile(hFile, Data^, Len, Read, nil) and (Read = Len)) then
        Reader := TSAReader.Create(Data, Len)
      else
        FreeMem(Data);

    // Закрываем файл и возращаем экземпляр ридера
    CloseHandle(hFile);
    Exit(Reader);
  end;

  //----------------------
  // Иначе замапим файл
  //----------------------
  hMMap := CreateFileMapping(hFile, nil, PAGE_READONLY, 0, Len, nil);
  if hMMap = 0 then
  begin
    CloseHandle(hFile);
    Exit(nil);
  end;

  Data := MapViewOfFile(hMMap, FILE_MAP_READ, 0, 0, Len);
  if Data <> nil then
  begin
    Reader := TSAMMapReader.Create(hFile, hMMap, Data, Len);
    if Reader <> nil then
      Exit(Reader);

    UnmapViewOfFile(Data);
  end
  else if GetLastError() = ERROR_NOT_ENOUGH_MEMORY then
  begin
    // Если нехватает у нас памяти на загрузку файла

  end;

  CloseHandle(hMMap);
  CloseHandle(hFile);

  Result := nil;
end;

{*------------------------------------------------------------------------------
  Закрытие файла открытого на чтение.
  @param aReader Экземпляр ридера, который асигнован с открытым файлом.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TSAFileSystem.ReadClose(var aReader: TSAReader);
begin
  aReader.Free;
  aReader := nil;
end;

{============================================}
{================= TSAReader ================}
{============================================}

{*------------------------------------------------------------------------------
  Конструктор по умолчанию.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TSAReader.Create;
begin
  FData := nil;
  FNext := nil;
  FEnd := nil;
  FCaret.u8 := nil;
end;

{*------------------------------------------------------------------------------
  Конструктор с возможностью задания указателя на данные.
  @param aData Указатель на данные.
  @param aSize Размер данных.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TSAReader.Create(aData: Pointer; aSize: Cardinal);
begin
  FData := aData;
  FNext := aData;
  FCaret.u8 := aData;

  FEnd := FData + aSize;
end;

{*------------------------------------------------------------------------------
  Деструктор по умолчанию.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

destructor TSAReader.Destroy;
begin

  inherited;
end;

function TSAReader.FindChunk(aID: Cardinal; var aCompressed: Boolean;
  aReset: Boolean): Cardinal;
var
  ID, Size: Cardinal;
begin
  if aReset then
    FCaret.u8 := FData;

  while (FCaret.u8 < FEnd) do
  begin
    Assert(FCaret.u8 + 8 <= FEnd, 'Can''t inc pointer');

    ID := r_u32;
    Size := r_u32;

    Assert(FCaret.u8 + Size <= FEnd);

    if (aID = (ID and CHUNK_ID_MASK)) then
    begin
      aCompressed := (ID and CHUNK_COMPRESSED) <> 0;
      Exit(Size);
    end;

    Inc(FCaret.u8, Size);
  end;

  Result := 0;
end;

function TSAReader.FindChunk(aID: Cardinal): Cardinal;
var
  Compressed: Boolean;
begin
  Result := FindChunk(aID, Compressed);
end;

{*------------------------------------------------------------------------------

  @param aID
  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TSAReader.OpenChunk(aID: Cardinal): TSAReader;
var
  Compressed: Boolean;
  Size, RealSize: Cardinal;
  Data: PByte;
  LZH: TLZH;
begin
  Size := FindChunk(aID, Compressed);

  if Size = 0 then
    Exit(nil);

  if Compressed then
  begin
    LZH := TLZH.Create;
    try
      LZH.Decompress(Data, RealSize, FCaret.u8, Size);
      Result := TSATempReader.Create(Data, RealSize);
    finally
      LZH.Free;
    end;
  end
  else
  begin
    Result := TSAReader.Create(FCaret.u8, Size);
  end;
end;

{*------------------------------------------------------------------------------

  @param aID
  @param aScrambler
  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TSAReader.OpenChunk(aID: Cardinal;
  const aScrambler: TSAScrambler): TSAReader;
begin
  Result := nil;
end;

procedure TSAReader.CloseChunk(var aReader: TSAReader);
begin
  Assert((aReader = nil) or ((aReader <> Self) and (aReader.FCaret.u8 <= aReader.FEnd)));
  FreeAndNil(aReader);
end;

procedure TSAReader.r_raw(aDestination: Pointer; aDestSize: Cardinal);
begin
  Assert(FCaret.u8 + aDestSize <= FEnd);

  Move(FCaret.u8, aDestination, aDestSize);
  Inc(FCaret.u8, aDestSize);
end;

procedure TSAReader.r_sz(var aDest: string);
var
  p: PAnsiChar;
begin

  p := FCaret.chr;
  Assert(p < FEnd);

  while (p^ <> #0) do
  begin
    if p >= FEnd then
    begin
      aDest := TSAStringUtils.ToString(FCaret.chr, p);
      FCaret.chr := p;
      Exit;
    end;

    Inc(p);
  end;

  aDest := TSAStringUtils.ToString(FCaret.chr, p);
  FCaret.chr := p + 1;
end;

function TSAReader.r_s16: SmallInt;
begin
  Result := FCaret.s16^;
  Inc(FCaret.s16);
end;

function TSAReader.r_s32: LongInt;
begin
  Result := FCaret.s32^;
  Inc(FCaret.s32);
end;

function TSAReader.r_s8: ShortInt;
begin
  Result := FCaret.s8^;
  Inc(FCaret.s8);
end;

function TSAReader.r_u16: Word;
begin
  Result := FCaret.u16^;
  Inc(FCaret.u16);
end;

function TSAReader.r_u24: Cardinal;
begin
  Result := 0;
  r_raw(@Result, 3);
end;

function TSAReader.r_u32: LongWord;
begin
  Result := FCaret.u32^;
  Inc(FCaret.u32);
end;

function TSAReader.r_u8: Byte;
begin
  Result := FCaret.u8^;
  Inc(FCaret.u8);
end;

function TSAReader.r_bool: Boolean;
begin
  Result := FCaret.u8^ <> 0;
  Inc(FCaret.u8);
end;

{*------------------------------------------------------------------------------
  Метод свойства указателя на данные.
  @return Указатель на данные.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TSAReader.GetData: Pointer;
begin
  Result := Pointer(FData);
end;

{*------------------------------------------------------------------------------
  Геттер свойства EOF.
  @return True, если достигли конца файла.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TSAReader.GetEOF: Boolean;
begin
  Assert(FCaret.u8 <= FEnd);
  Result := FCaret.u8 = FEnd;
end;

{*------------------------------------------------------------------------------
  Метод свойства размер.
  @return Размер данных.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TSAReader.GetSize: Cardinal;
begin
  Assert(FData <= FEnd);
  Result := FEnd - FData;
end;

{============================================}
{============== TSAMMapReader ===============}
{============================================}

{*------------------------------------------------------------------------------
  Конструктор по умолчанию.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TSAMMapReader.Create;
begin
  FHFile := INVALID_HANDLE_VALUE;
  FHMMap := INVALID_HANDLE_VALUE;
end;

{*------------------------------------------------------------------------------
  Конструктор с возможностью ассоциации с файл мэппингом и соотв. данными.
  @param aHFile Хэндл замапленного файла.
  @param aHMMap Хэндл memory mapping-a.
  @param aData Указатель на данные.
  @param aSize Размер данных.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TSAMMapReader.Create(aHFile, aHMMap: Cardinal; aData: Pointer; aSize: Cardinal);
begin
  FHFile := aHFile;
  FHMMap := aHMMap;

  FData := aData;
  FNext := aData;
  FCaret.u8 := aData;

  FEnd := FData + aSize;
end;

{*------------------------------------------------------------------------------
  Деструктор по умолчанию.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

destructor TSAMMapReader.Destroy;
begin
  Assert(Assigned(FData));
  Assert(FHMMap <> INVALID_HANDLE_VALUE);
  Assert(FHFile <> INVALID_HANDLE_VALUE);

  UnmapViewOfFile(FData);

  CloseHandle(FHMMap);
  CloseHandle(FHFile);

  inherited;
end;

{ TSATempReader }

destructor TSATempReader.Destroy;
begin
  if FData <> nil then
    FreeMem(FData);

  FData := nil;

  inherited;
end;

end.

