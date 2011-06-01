{-----------------------------------------------------------------------------
 Unit Name: SAScrambler
 Author:    Neo][
 Date:      27-май-2009
 Purpose:
 History:
-----------------------------------------------------------------------------}

unit SAScrambler;

interface

const
  ///
  SBOX_SIZE = 256;

type
  TSACipherConfig = record
    Seed: Integer;
    Seed0: Integer;
    SizeMult: Integer;
  end;

  ///
  TSASBoxesArr = array[0..255] of Byte;

  {*------------------------------------------------------------------------------

  -------------------------------------------------------------------------------}
  TSAScrambler = class
  private
    FSeed: Integer;

    FEncryptSBox: TSASBoxesArr;
    FDecryptSBox: TSASBoxesArr;

    procedure InitSBoxes(aSeed0, aSizeMult: Integer);
  public
    constructor Create; overload;
    constructor Create(const aCipherCfg: TSACipherConfig); overload;

    procedure Init(const aCipherCfg: TSACipherConfig);

    procedure Encrypt(aDest: PByte; const aSrc: PByte; aSize: Cardinal);
    procedure Decrypt(aDest: PByte; const aSrc: PByte; aSize: Cardinal);
  end;

implementation

{ TSAScrambler }

constructor TSAScrambler.Create;
begin

end;

constructor TSAScrambler.Create(const aCipherCfg: TSACipherConfig);
begin
  Init(aCipherCfg);
end;

procedure TSAScrambler.Decrypt(aDest: PByte; const aSrc: PByte;
  aSize: Cardinal);
begin

end;

procedure TSAScrambler.Encrypt(aDest: PByte; const aSrc: PByte;
  aSize: Cardinal);
begin

end;

procedure TSAScrambler.Init(const aCipherCfg: TSACipherConfig);
begin
  FSeed := aCipherCfg.Seed;
  InitSBoxes(aCipherCfg.Seed0, aCipherCfg.SizeMult);
end;

procedure TSAScrambler.InitSBoxes(aSeed0, aSizeMult: Integer);
var
  i: Integer;
begin
  for I := SBOX_SIZE - 1 downto 0 do
    FEncryptSBox[i] := Byte(i);
end;

end.

