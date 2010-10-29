unit SAStringUtils;

interface

const
  { *
    * Sizes for buffers used by the _makepath() and _splitpath()functions.
    * note that the sizes include space for 0 - terminator
    * }
  cMAX_PATH = 260;                      /// Максимальная длина полного пути до файла
  cMAX_DRIVE = 3;                       /// максимальная длина дисковой компоненты пути
  cMAX_DIR = 256;                       /// Максимальная длина пути до папки
  cMAX_FNAME = 256;                     /// Максимальная длина имени файла
  cMAX_EXT = 256;                       /// Максимальная длина расширения файла

type
  {*------------------------------------------------------------------------------
    Вспомогательный класс для работы со строками.
  -------------------------------------------------------------------------------}
  TSAStringUtils = class
  public
    { file/folder/paths }
    class procedure PathAppendSeparator(var aPath: string);
    class procedure PathRemoveSeparator(var aPath: string);
    class procedure PathSplit(const aPath: string; aFolder: PString = nil;
      aName: PString = nil; aExtension: PString = nil);

    { converts }
    class function ToString(aPCaret, aPEnd: PChar): string; overload;
    class function ToString(aPCaret, aPEnd: PAnsiChar): string; overload;
    class function SizeToStr(aSize: LongInt): string;
    class function MsToStr(aTime: Cardinal): string;

    { searching }
    class function PosText(const aSubStr, aStr: string;
      aFromPos: Cardinal = 1; aToPos: Cardinal = MaxInt): Integer;

    class function PosPChar(aSubStr, aStr: PChar;
      aSubStrLen: Cardinal = 0;         // 0 -> StrLen is called internally
      aStrLen: Cardinal = 0;
      aIgnoreCase: boolean = False;
      aFromPos: Cardinal = 0;
      aToPos: Cardinal = MaxInt): Integer;

  end;

implementation

uses
  SysUtils, Windows;

var
  /// Глобальная переменная, символ десятичного разделителя в соотв. с региональными настройками
  gDecSep: Char = #0;
  //------

{*------------------------------------------------------------------------------
  Функция получения символа десятичного разделителя.
  @return Символ десятичного разделителя.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function DecSep: Char;
var
  buf: array[0..1] of Char;
begin
  if gDecSep = #0 then
    if GetLocaleInfo(GetThreadLocale, LOCALE_SDECIMAL, buf, 2) > 0 then
      gDecSep := buf[0]
    else
      gDecSep := ',';

  Result := gDecSep;
end;

{ TSAStringUtils }

{*------------------------------------------------------------------------------
  Присоединение обратного слэша(разделитель в пути) к строке, если необходимо.
  @param aPath Указатель на строку к которой необходимо присоединить разделитель.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

class procedure TSAStringUtils.PathAppendSeparator(var aPath: string);
begin
  if (aPath <> '') and (aPath[Length(aPath)] <> '\') then
    aPath := aPath + '\';
end;

{*------------------------------------------------------------------------------
  Удаление обратного слэша(разделитель пути) в конце пути.
  @param aPath Указатель на строку, которая является путём.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

class procedure TSAStringUtils.PathRemoveSeparator(var aPath: string);
begin
  aPath := ExcludeTrailingPathDelimiter(aPath);
end;

{*------------------------------------------------------------------------------
  Преобразование времени(мс) в строку.
  Например:
  15          ->  '15 ms'
  1000        ->  '1 s'
  60*1000     ->  '1 min'
  60*60*1000  ->  '1 h'
  @param aTime Время в миллисекундах.
  @return Время в виде строки.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

class function TSAStringUtils.MsToStr(aTime: Cardinal): string;
begin
  if aTime >= 1000 then
  begin
    if aTime >= 1000 * 60 then
    begin
      if aTime >= 1000 * 60 * 60 then
      begin
        aTime := aTime div (1000 * 60);
        Result := IntToStr(aTime mod 60);
        if Length(Result) = 1 then
          Result := '0' + Result;
        Result := IntToStr(aTime div 60) + ':' + Result + ' h';
      end
      else
      begin
        aTime := aTime div 1000;
        Result := IntToStr(aTime mod 60);
        if Length(Result) = 1 then
          Result := '0' + Result;
        Result := IntToStr(aTime div 60) + ':' + Result + ' min';
      end;
    end
    else
    begin
      Result := IntToStr(aTime mod 1000 div 10);
      if Length(Result) = 1 then
        Result := '0' + Result;
      Result := IntToStr(aTime div 1000) + DecSep + Result + ' s';
    end;
  end
  else
    Result := IntToStr(aTime) + ' ms';
end;

{*------------------------------------------------------------------------------
  Функция для преобразования размера файла в строку.
  Например:
  500         ->  '500 Bytes'
  1024        ->  '1 KB'
  1024*1024   ->  '1 MB'
  @param aSize Размер в байтах.
  @return Размер файла в виде строки.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

class function TSAStringUtils.SizeToStr(aSize: Integer): string;
begin
  if abs(aSize) >= 1024 then
  begin
    if abs(aSize) >= 1024 * 1024 then
    begin
      if abs(aSize) >= 1024 * 1024 * 1024 then
      begin
        Result := IntToStr(abs(aSize div 1024 div 1024 * 100 div 1024)) + ' GB';
        Insert(DecSep, Result, Length(Result) - 4);
      end
      else
      begin
        Result := IntToStr(abs(aSize div 1024 * 100 div 1024)) + ' MB';
        Insert(DecSep, Result, Length(Result) - 4);
      end;
    end
    else
    begin
      Result := IntToStr(abs(aSize * 100 div 1024)) + ' KB';
      Insert(DecSep, Result, Length(Result) - 4);
    end;
  end
  else
    Result := IntToStr(abs(aSize)) + ' Bytes';
end;

{*------------------------------------------------------------------------------
  Разбиение пути до файла на составляющие.
  @param aPath Путь до файла.
  @param aFolder Путь до папки.
  @param aName Имя файла.
  @param aExtension Расширение файла
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

class procedure TSAStringUtils.PathSplit(const aPath: string; aFolder: PString = nil;
  aName: PString = nil; aExtension: PString = nil);
var
  Drive, Path, FileName, Ext: string;
begin
  {
    Drive := ExtractFileDrive(aPath);
    Path := ExtractFilePath(aPath);

    // Path includes drive so remove that
    if Drive <> '' then
      Delete(Path, 1, Length(Drive));

    // add/remove separators
    PathAppendSeparator(Drive);
    PathRemoveSeparator(Path);

    if (Path <> '') and (Path[1] = DirDelimiter) then
      Delete(Path, 1, 1);

    // and extract the remaining elements
    FileName := PathExtractFileNameNoExt(aPath);
    Ext := ExtractFileExt(aPath);

    if Assigned(aFolder) then
    begin
      aFolder^ := aFolder^ + szDrive;
      aFolder^ := aFolder^ + szDir;
    end;

    if Assigned(aName) then
    begin
      aName^ := aName^ + szName;
    end;

    if Assigned(aExtension) then
    begin
      aExtension^ := aExtension^ + szExt;
    end;
    }
end;

{*------------------------------------------------------------------------------
  Аналог функции SysUtils.Pos, с расширенным функционалом. Поиск ведётся только
  в рамках aFromPos..aToPos, если aFromPos > aToPos, тогда поиск идёт в обратном
  направлении.
  @param aSubStr Подстрока для поиска.
  @param aStr Строка в которой производится поиск.
  @param fromPos Индекс стартового символа.
  @param toPos Индекс финишного символа.
  @return Позицию начала подстроки в строке.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

class function TSAStringUtils.PosText(const aSubStr, aStr: string; aFromPos,
  aToPos: cardinal): integer;
begin
  if (aFromPos > 0) and (aToPos > 0) then
    result := PosPChar(pchar(aSubStr), pchar(aStr), Length(aSubStr), Length(aStr), true, aFromPos - 1, aToPos - 1) + 1
  else
    result := 0;
end;

{*------------------------------------------------------------------------------

  @param aSubStr
  @param aStr
  @param aSubStrLen
  @param aStrLen
  @param aIgnoreCase
  @param aFromPos
  @param aToPos
  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

class function TSAStringUtils.PosPChar(aSubStr, aStr: PChar; aSubStrLen,
  aStrLen: Cardinal; aIgnoreCase: boolean; aFromPos, aToPos: Cardinal): Integer;

  function GetPCharLen(const pc: pchar): cardinal; assembler;
  asm
    MOV     EDX,EDI
    MOV     EDI,EAX
    MOV     ECX,0FFFFFFFFH
    XOR     AL,AL
    REPNE   SCASB
    MOV     EAX,0FFFFFFFEH
    SUB     EAX,ECX
    MOV     EDI,EDX
  end;

var
  pc1, pc2, pc3, pc4, pc5, pc6: pchar;
  c1: cardinal;
  ch1: char;
begin
  result := -1;
  if (aSubStr <> nil) and ((aSubStrLen <> 0) or (aSubStr^ <> #0)) and
    (aStr <> nil) and ((aStrLen <> 0) or (aStr^ <> #0)) then
  begin
    if aSubStrLen = 0 then
      aSubStrLen := GetPCharLen(aSubStr);
    if aStrLen = 0 then
      aStrLen := GetPCharLen(aStr);
    if aStrLen >= aSubStrLen then
    begin
      c1 := aStrLen - aSubStrLen;
      if aIgnoreCase then
        ch1 := StrLower(aSubStr)^
          //ch1 := lowCharTable[aSubStr^]
      else
        ch1 := aSubStr^;
      if aFromPos > aToPos then
      begin
        if aToPos <= c1 then
        begin
          if aFromPos > c1 then
            aFromPos := c1;
          pc1 := aStr + aFromPos;
          pc2 := aStr + aToPos;
          pc3 := aSubStr + 1;
          pc4 := aSubStr + aSubStrLen - 1;
          pc6 := pc3;
          if aIgnoreCase then
          begin
            while pc1 >= pc2 do
              //if lowCharTable[pc1^] = ch1 then
              if StrLower(pc1)^ = ch1 then
              begin
                inc(pc1);
                pc5 := pc1;
                while (pc3 <= pc4) and (StrLower(pc1)^ = StrLower(pc3)^) do
                begin
                  inc(pc1);
                  inc(pc3);
                end;
                if pc3 > pc4 then
                begin
                  result := pc5 - pchar(aStr) - 1;
                  break;
                end;
                pc3 := pc6;
                pc1 := pc5 - 2;
              end
              else
                dec(pc1);
          end
          else
            while pc1 >= pc2 do
              if pc1^ = ch1 then
              begin
                inc(pc1);
                pc5 := pc1;
                while (pc3 <= pc4) and (pc1^ = pc3^) do
                begin
                  inc(pc1);
                  inc(pc3);
                end;
                if pc3 > pc4 then
                begin
                  result := pc5 - pchar(aStr) - 1;
                  break;
                end;
                pc3 := pc6;
                pc1 := pc5 - 2;
              end
              else
                dec(pc1);
        end;
      end
      else if aFromPos <= c1 then
      begin
        if aToPos > c1 then
          aToPos := c1;
        pc1 := aStr + aFromPos;
        pc2 := aStr + aToPos;
        pc3 := aSubStr + 1;
        pc4 := aSubStr + aSubStrLen - 1;
        pc6 := pc3;
        if aIgnoreCase then
        begin
          while pc1 <= pc2 do
            if StrLower(pc1)^ = ch1 then
            begin
              inc(pc1);
              pc5 := pc1;
              while (pc3 <= pc4) and (StrLower(pc1)^ = StrLower(pc3)^) do
              begin
                inc(pc1);
                inc(pc3);
              end;
              if pc3 > pc4 then
              begin
                result := pc5 - pchar(aStr) - 1;
                break;
              end;
              pc3 := pc6;
              pc1 := pc5;
            end
            else
              inc(pc1);
        end
        else
          while pc1 <= pc2 do
            if pc1^ = ch1 then
            begin
              inc(pc1);
              pc5 := pc1;
              while (pc3 <= pc4) and (pc1^ = pc3^) do
              begin
                inc(pc1);
                inc(pc3);
              end;
              if pc3 > pc4 then
              begin
                result := pc5 - pchar(aStr) - 1;
                break;
              end;
              pc3 := pc6;
              pc1 := pc5;
            end
            else
              inc(pc1);
      end;
    end;
  end;
end;

{*------------------------------------------------------------------------------
  Преобразование указателей в строку.
  @param aPCaret указатель на начало строки.
  @param aPEnd указатель на конец строки.
  @return Строка полученная копированием с соотв. позиций.
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

class function TSAStringUtils.ToString(aPCaret, aPEnd: PChar): string;
var
  TmpAnsi: string;
  Len: Integer;
begin
  { TODO -oNeo][ -c : Check bounds 12.04.2009 20:07:59 }
  Len := aPEnd - aPCaret;
  SetLength(TmpAnsi, Len);
  Move(aPCaret^, TmpAnsi[1], Len);
  Result := string(TmpAnsi);
end;

{*------------------------------------------------------------------------------

  @param aPCaret
  @param aPEnd
  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

class function TSAStringUtils.ToString(aPCaret, aPEnd: PAnsiChar): string;
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

end.

