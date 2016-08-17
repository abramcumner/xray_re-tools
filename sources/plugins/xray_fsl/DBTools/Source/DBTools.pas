unit DBTools;

interface

type
  TDBFile = record
    function LessThan(a: TDBFile; b: TDBFile): Boolean;

    Path: string;
    Offset: Cardinal;
    RealSize: Cardinal;
    CompressedSize: Cardinal;
    CRC: Cardinal;
  end;

implementation

end.

