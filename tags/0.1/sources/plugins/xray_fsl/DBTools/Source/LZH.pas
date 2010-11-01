{-----------------------------------------------------------------------------
 Unit Name: lzh
 Author:    Neo][
 Date:      27-май-2009
 Purpose:
 History:
-----------------------------------------------------------------------------}


unit lzh;
(*
 * LZHUF.C English version 1.0
 * Based on Japanese version 29-NOV-1988
 * LZSS coded by Haruhiko OKUMURA
 * Adaptive Huffman Coding coded by Haruyasu YOSHIZAKI
 * Edited and translated to English by Kenji RIKITAKE
 * Translated from C to Turbo Pascal by Douglas Webb   2/18/91
 *    Update and bug correction of TP version 4/29/91 (Sorry!!)
 *    Added Delphi exception handling may 09 1996 Danny Heijl
 *                                                Danny.Heijl@cevi.be
 * Added support for Delphi streams Aug. 05 1999
 *    Bruno Depero (bdepero@usa.net) and
 *    Kim Madsen (kbm@optical.dk)
 *)

{
     This unit allows the user to compress data using a combination of
   LZSS compression and adaptive Huffman coding, or conversely to decompress
   data that was previously compressed by this unit.

     There are a number of options as to where the data being compressed/
   decompressed is coming from/going to.

   In fact it requires that you pass the "LZHPack" procedure 2 procedural
  parameter of type 'GetProcType' and 'PutProcType' (declared below) which
  will accept 3 parameters and act in every way like a 'BlockRead'/
  'BlockWrite' procedure call. Your 'GetBytesProc' procedure should return
  the data to be compressed, and Your 'PutBytesProc' procedure should do
  something with the compressed data (ie., put it in a file).  In case you
  need to know (and you do if you want to decompress this data again) the
  number of bytes in the compressed data (original, not compressed size)
  is returned in 'Bytes_Written'.

  GetBytesProc = PROCEDURE(VAR DTA; NBytes:WORD; VAR Bytes_Got : WORD);

  DTA is the start of a memory location where the information returned
  should be.  NBytes is the number of bytes requested.  The actual number
  of bytes returned must be passed in Bytes_Got (if there is no more data
  then 0 should be returned).

  PutBytesProc = PROCEDURE(VAR DTA; NBytes:WORD; VAR Bytes_Got : WORD);

  As above except instead of asking for data the procedure is dumping out
  compressed data, do somthing with it.

   "LZHUnPack" is basically the same thing in reverse.  It requires
  procedural parameters of type 'PutProcType'/'GetProcType' which
  will act as above.  'GetProcType' must retrieve data compressed using
  "LZHPack" (above) and feed it to the unpacking routine as requested.
  'PutProcType' must accept the decompressed data and do something
  withit.  You must also pass in the original size of the decompressed data,
  failure to do so will have adverse results.

   Don't forget that as procedural parameters the 'GetProcType'/'PutProcType'
  procedures must be compiled in the 'F+' state to avoid a catastrophe.

}

{ Note: All the large data structures for these routines are allocated when
  needed from the heap, and deallocated when finished.  So when not in use
  memory requirements are minimal.  However, this unit uses about 34K of
  heap space, and 400 bytes of stack when in use. }

{$R-}                                   { NO range checking !! }

interface

uses Sysutils, Classes;

{$DEFINE WIN32}
{$IFDEF WIN32}
type
  Int16 = SmallInt;
{$ELSE}
type
  Int16 = Integer;
{$ENDIF}

  {.$DEFINE DEBUG}
{$IFDEF DEBUG}
{$D+}
{$ENDIF}

type

  ElzhException = class(Exception);

  TWriteProc = procedure(var DTA; NBytes: WORD; var Bytes_Put: WORD) of object;

  PutBytesProc = TwriteProc;
  {
   Your 'PutBytesProc' procedure should do something with the compressed
   data (ie., put it in a file).

   DTA is the start of a memory location where the information returned
   should be.  NBytes is the number of bytes requested.  The actual number
   of bytes put should be returned in Bytes_Got.

   Don't forget that as procedural parameters the 'GetProcType'/'PutProcType'
  procedures must be compiled in the 'F+' state to avoid a catastrophe.
  }

  TReadProc = procedure(var DTA; NBytes: WORD; var Bytes_Got: WORD) of object;
  GetBytesProc = TReadProc;
  {
   Your 'GetBytesProc' procedure should return the data to be compressed.
   In case you need to know (and you do if you want to decompress this
   data again) the number of bytes in the compressed data (original, not
   compressed size) is returned in 'Bytes_Written'.

   DTA is the start of a memory location where the information returned
   should be.  NBytes is the number of bytes requested.  The actual number
   of bytes returned must be passed in Bytes_Got (if there is no more data
   then 0 should be returned).

   Don't forget that as procedural parameters the 'GetProcType'/'PutProcType'
  procedures must be compiled in the 'F+' state to avoid a catastrophe.
  }

const
  EXIT_OK = 0;
  EXIT_FAILED = 1;
  { LZSS Parameters }
  N = 4096;                             { Size of string buffer }
  F = 60;                               { Size of look-ahead buffer }
  THRESHOLD = 2;
  NUL = N;                              { End of tree's node  }

  { Huffman coding parameters }
  N_CHAR = (256 - THRESHOLD + F);
  { character code (:= 0..N_CHAR-1) }
  T = (N_CHAR * 2 - 1);                 { Size of table }
  R = (T - 1);                          { root position }
  MAX_FREQ = $8000;
  { update when cumulative frequency }
  { reaches to this value }
{
* Tables FOR encoding/decoding upper 6 bits of
* sliding dictionary pointer
}
{ encoder table }
  p_len: array[0..63] of BYTE =
    ($03, $04, $04, $04, $05, $05, $05, $05,
    $05, $05, $05, $05, $06, $06, $06, $06,
    $06, $06, $06, $06, $06, $06, $06, $06,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $08, $08, $08, $08, $08, $08, $08, $08,
    $08, $08, $08, $08, $08, $08, $08, $08);

  p_code: array[0..63] of BYTE =
    ($00, $20, $30, $40, $50, $58, $60, $68,
    $70, $78, $80, $88, $90, $94, $98, $9C,
    $A0, $A4, $A8, $AC, $B0, $B4, $B8, $BC,
    $C0, $C2, $C4, $C6, $C8, $CA, $CC, $CE,
    $D0, $D2, $D4, $D6, $D8, $DA, $DC, $DE,
    $E0, $E2, $E4, $E6, $E8, $EA, $EC, $EE,
    $F0, $F1, $F2, $F3, $F4, $F5, $F6, $F7,
    $F8, $F9, $FA, $FB, $FC, $FD, $FE, $FF);

  { decoder table }
  d_code: array[0..255] of BYTE =
    ($00, $00, $00, $00, $00, $00, $00, $00,
    $00, $00, $00, $00, $00, $00, $00, $00,
    $00, $00, $00, $00, $00, $00, $00, $00,
    $00, $00, $00, $00, $00, $00, $00, $00,
    $01, $01, $01, $01, $01, $01, $01, $01,
    $01, $01, $01, $01, $01, $01, $01, $01,
    $02, $02, $02, $02, $02, $02, $02, $02,
    $02, $02, $02, $02, $02, $02, $02, $02,
    $03, $03, $03, $03, $03, $03, $03, $03,
    $03, $03, $03, $03, $03, $03, $03, $03,
    $04, $04, $04, $04, $04, $04, $04, $04,
    $05, $05, $05, $05, $05, $05, $05, $05,
    $06, $06, $06, $06, $06, $06, $06, $06,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $08, $08, $08, $08, $08, $08, $08, $08,
    $09, $09, $09, $09, $09, $09, $09, $09,
    $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A,
    $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B,
    $0C, $0C, $0C, $0C, $0D, $0D, $0D, $0D,
    $0E, $0E, $0E, $0E, $0F, $0F, $0F, $0F,
    $10, $10, $10, $10, $11, $11, $11, $11,
    $12, $12, $12, $12, $13, $13, $13, $13,
    $14, $14, $14, $14, $15, $15, $15, $15,
    $16, $16, $16, $16, $17, $17, $17, $17,
    $18, $18, $19, $19, $1A, $1A, $1B, $1B,
    $1C, $1C, $1D, $1D, $1E, $1E, $1F, $1F,
    $20, $20, $21, $21, $22, $22, $23, $23,
    $24, $24, $25, $25, $26, $26, $27, $27,
    $28, $28, $29, $29, $2A, $2A, $2B, $2B,
    $2C, $2C, $2D, $2D, $2E, $2E, $2F, $2F,
    $30, $31, $32, $33, $34, $35, $36, $37,
    $38, $39, $3A, $3B, $3C, $3D, $3E, $3F);

  d_len: array[0..255] of BYTE =
    ($03, $03, $03, $03, $03, $03, $03, $03,
    $03, $03, $03, $03, $03, $03, $03, $03,
    $03, $03, $03, $03, $03, $03, $03, $03,
    $03, $03, $03, $03, $03, $03, $03, $03,
    $04, $04, $04, $04, $04, $04, $04, $04,
    $04, $04, $04, $04, $04, $04, $04, $04,
    $04, $04, $04, $04, $04, $04, $04, $04,
    $04, $04, $04, $04, $04, $04, $04, $04,
    $04, $04, $04, $04, $04, $04, $04, $04,
    $04, $04, $04, $04, $04, $04, $04, $04,
    $05, $05, $05, $05, $05, $05, $05, $05,
    $05, $05, $05, $05, $05, $05, $05, $05,
    $05, $05, $05, $05, $05, $05, $05, $05,
    $05, $05, $05, $05, $05, $05, $05, $05,
    $05, $05, $05, $05, $05, $05, $05, $05,
    $05, $05, $05, $05, $05, $05, $05, $05,
    $05, $05, $05, $05, $05, $05, $05, $05,
    $05, $05, $05, $05, $05, $05, $05, $05,
    $06, $06, $06, $06, $06, $06, $06, $06,
    $06, $06, $06, $06, $06, $06, $06, $06,
    $06, $06, $06, $06, $06, $06, $06, $06,
    $06, $06, $06, $06, $06, $06, $06, $06,
    $06, $06, $06, $06, $06, $06, $06, $06,
    $06, $06, $06, $06, $06, $06, $06, $06,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $07, $07, $07, $07, $07, $07, $07, $07,
    $08, $08, $08, $08, $08, $08, $08, $08,
    $08, $08, $08, $08, $08, $08, $08, $08);

type
  Freqtype = array[0..T] of WORD;
  FreqPtr = ^freqtype;
  PntrType = array[0..PRED(T + N_Char)] of Int16;
  pntrPtr = ^pntrType;
  SonType = array[0..PRED(T)] of Int16;
  SonPtr = ^SonType;

  TextBufType = array[0..N + F - 2] of BYTE;
  TBufPtr = ^TextBufType;
  WordRay = array[0..N] of Int16;
  WordRayPtr = ^WordRay;
  BWordRay = array[0..N + 256] of Int16;
  BWordRayPtr = ^BWordRay;

  {PG 17/09/98}
  TLZH = class
  private
    code, len: WORD;
    procedure InitTree;                 { Initializing tree }
    procedure InsertNode(r: Int16);     { Inserting node to the tree }
    procedure DeleteNode(p: Int16);     { Deleting node from the tree }
    function GetBit(GetBytes: GetBytesProc): Int16; overload; { get one bit }
    function GetByte(GetBytes: GetBytesProc): Int16; overload; { get a byte }
    procedure update(c: Int16);
    procedure StartHuff;
    procedure Putcode(l: Int16; c: WORD; PutBytes: PutBytesProc); { output c bits }
    procedure reconst;
    procedure EncodeChar(c: WORD; PutBytes: PutBytesProc);
    procedure EncodePosition(c: WORD; PutBytes: PutBytesProc);
    procedure EncodeEnd(PutBytes: PutBytesProc);
    function DecodeChar(GetBytes: GetBytesProc): Int16; overload;
    function DecodePosition(GetBytes: GetBytesProc): WORD; overload;
    procedure InitLZH;
    procedure EndLZH;

  public
    StreamIn, StreamOut: TStream;

    getbuf: WORD;
    getlen: BYTE;
    putlen: BYTE;
    putbuf: WORD;
    textsize: longint;
    codesize: longINT;
    printcount: longint;
    match_position: Int16;
    match_length: Int16;

    text_buf: TBufPtr;
    lson, dad: WordRayPtr;
    rson: BWordRayPtr;
    freq: FreqPtr;                      { cumulative freq table }

    {
     * pointing parent nodes.
     * area [T..(T + N_CHAR - 1)] are pointers FOR leaves
     }
    prnt: PntrPtr;

    { pointing children nodes (son[], son[] + 1)}
    son: SonPtr;

    procedure LZHPack(var Bytes_Written: LongInt; GetBytes: GetBytesProc;
      PutBytes: PutBytesProc);
    {#XLZHUnPack}
    {
       This procedure allows the user to compress data using a combination of
     LZSS compression and adaptive Huffman coding.

       There are a number of options as to where the data being compressed
    is coming from.

     In fact it requires that you pass the "LZHPack" procedure 2 procedural
    parameter of type 'GetProcType' and 'PutProcType' (declared below) which
    will accept 3 parameters and act in every way like a 'BlockRead'/
    'BlockWrite' procedure call. Your 'GetBytesProc' procedure should return
    the data to be compressed, and Your 'PutBytesProc' procedure should do
    something with the compressed data (ie., put it in a file).  In case you
    need to know (and you do if you want to decompress this data again) the
    number of bytes in the compressed data (original, not compressed size)
    is returned in 'Bytes_Written'.

    DTA is the start of a memory location where the information returned
    should be.  NBytes is the number of bytes requested.  The actual number
    of bytes returned must be passed in Bytes_Got (if there is no more data
    then 0 should be returned).

    As above except instead of asking for data the procedure is dumping out
    compressed data, do somthing with it.

    }
    procedure LZHUnpack(TextSize: Longint; GetBytes: GetBytesProc;
      PutBytes: PutBytesProc);
    {#X LZHPack}
    {
      "LZHUnPack" is basically the same as LZHPack in reverse.  It requires
    procedural parameters of type 'PutProcType'/'GetProcType' which
    will act as above.  'GetProcType' must retrieve data compressed using
    "LZHPack" (above) and feed it to the unpacking routine as requested.
    'PutProcType' must accept the decompressed data and do something
    withit.  You must also pass in the original size of the decompressed data,
    failure to do so will have adverse results.
    }

    procedure GetBlockStream(var DTA; NBytes: Word; var Bytes_Got: Word);
    procedure PutBlockStream(var DTA; NBytes: Word; var Bytes_Got: Word);

    procedure GetBlockMem(var DTA; NBytes: Word; var Bytes_Got: Word);
    procedure PutBlockMem(var DTA; NBytes: Word; var Bytes_Got: Word);

    procedure Decompress(var aText: PByte; var aTextSize: Cardinal; const aCode: PByte; aCodeSize: Cardinal);

  private
    FTextSize: Cardinal;
    FCodeSize: Cardinal;

    FPDest: PByte;
    FDestPos: Cardinal;
    FDestLimit: Cardinal;

    FPSrc: PByte;
    FSrcPos: Cardinal;
    FSrcLimit: Cardinal;

    FGetBuff: Word;
    FPutBuff: Word;

    FGetLen: Byte;
    FPutLen: Byte;

    function DecodeChar: Int16; overload;
    function DecodePosition: Word; overload;

    function GetBit: Int16; overload;   { get one bit }
    function GetByte: Int16; overload;

    function GetC: Int16;
    procedure PutC(aVal: Int16);
  end;

  //procedure LZHD(Code: PByte; var CodeSize: Cardinal; Text: PByte; TextSize: Cardinal); cdecl;
  //procedure LZHC(Text: PByte; var TextSize: Cardinal; Code: PByte; CodeSize: Cardinal); cdecl;

var
  //void __cdecl lzh_compress(unsigned char * &,unsigned int &,unsigned char const *,unsigned int)
  lzh_compress: procedure(var Code: PByte; var CodeSize: Cardinal; const Text: PByte; TextSize: Cardinal); cdecl;
  lzh_decompress: procedure(var Text: PByte; var TextSize: Cardinal; const Code: PByte; CodeSize: Cardinal); cdecl;
  //lzh_compress: procedure(var _code: Pointer; var _codesize: Cardinal; _text: Pointer; _textsize: Cardinal); cdecl;
  //lzh_decompress: procedure(var _text: Pointer; var _textsize: Cardinal; _code: Pointer; _codesize: Cardinal); cdecl;

implementation

uses
  Windows;

var
  xrFSLLib: Cardinal = 0;

procedure TLZH.InitTree;                { Initializing tree }
var
  i: Int16;
begin
  for i := N + 1 to N + 256 do
    rson^[i] := NUL;                    { root }
  for i := 0 to N do
    dad^[i] := NUL;                     { node }
end;

procedure TLZH.InsertNode(r: Int16);    { Inserting node to the tree }
var
  tmp, i, p, cmp: Int16;
  key: TBufPtr;
  c: WORD;
begin
  cmp := 1;
  key := @text_buf^[r];
  p := SUCC(N) + key^[0];
  rson^[r] := NUL;
  lson^[r] := NUL;
  match_length := 0;
  while match_length < F do
  begin
    if (cmp >= 0) then
    begin
      if (rson^[p] <> NUL) then
      begin
        p := rson^[p]
      end
      else
      begin
        rson^[p] := r;
        dad^[r] := p;
        exit;
      end;
    end
    else
    begin
      if (lson^[p] <> NUL) then
      begin
        p := lson^[p]
      end
      else
      begin
        lson^[p] := r;
        dad^[r] := p;
        exit;
      end;
    end;
    i := 0;
    cmp := 0;
    while (i < F) and (cmp = 0) do
    begin
      inc(i);
      cmp := key^[i] - text_buf^[p + i];
    end;
    if (i > THRESHOLD) then
    begin
      tmp := PRED((r - p) and PRED(N));
      if (i > match_length) then
      begin
        match_position := tmp;
        match_length := i;
      end;
      if (match_length < F) and (i = match_length) then
      begin
        c := tmp;
        if (c < match_position) then
        begin
          match_position := c;
        end;
      end;
    end;                                { if i > threshold }
  end;                                  { WHILE match_length < F }
  dad^[r] := dad^[p];
  lson^[r] := lson^[p];
  rson^[r] := rson^[p];
  dad^[lson^[p]] := r;
  dad^[rson^[p]] := r;
  if (rson^[dad^[p]] = p) then
  begin
    rson^[dad^[p]] := r;
  end
  else
  begin
    lson^[dad^[p]] := r;
  end;
  dad^[p] := NUL;                       { remove p }
end;

procedure TLZH.DeleteNode(p: Int16);    { Deleting node from the tree }
var
  q: Int16;
begin
  if (dad^[p] = NUL) then
    exit;                               { unregistered }

  if (rson^[p] = NUL) then
  begin
    q := lson^[p];
  end
  else
  begin
    if (lson^[p] = NUL) then
    begin
      q := rson^[p];
    end
    else
    begin
      q := lson^[p];
      if (rson^[q] <> NUL) then
      begin
        repeat
          q := rson^[q];
        until (rson^[q] = NUL);
        rson^[dad^[q]] := lson^[q];
        dad^[lson^[q]] := dad^[q];
        lson^[q] := lson^[p];
        dad^[lson^[p]] := q;
      end;
      rson^[q] := rson^[p];
      dad^[rson^[p]] := q;
    end;
  end;
  dad^[q] := dad^[p];

  if (rson^[dad^[p]] = p) then
    rson^[dad^[p]] := q
  else
    lson^[dad^[p]] := q;

  dad^[p] := NUL;
end;

{ Huffman coding parameters }

function TLZH.GetBit(GetBytes: GetBytesProc): Int16; { get one bit }
var
  i: BYTE;
  i2: Int16;
  Wresult: Word;
begin
  while (getlen <= 8) do
  begin
    GetBytes(i, 1, Wresult);
    if Wresult = 1 then
      i2 := i
    else
      i2 := 0;

    getbuf := getbuf or (i2 shl (8 - getlen));
    INC(getlen, 8);
  end;
  i2 := getbuf;
  getbuf := getbuf shl 1;
  DEC(getlen);
  getbit := Int16((i2 < 0));

end;

function TLZH.GetByte(GetBytes: GetBytesProc): Int16; { get a byte }
var
  j: BYTE;
  i, Wresult: WORD;
begin
  while (getlen <= 8) do
  begin
    GetBytes(j, 1, Wresult);
    if Wresult = 1 then
      i := j
    else
      i := 0;

    getbuf := getbuf or (i shl (8 - getlen));
    INC(getlen, 8);
  end;
  i := getbuf;
  getbuf := getbuf shl 8;
  DEC(getlen, 8);
  getbyte := Int16(i shr 8);
end;

procedure TLZH.Putcode(l: Int16; c: WORD; PutBytes: PutBytesProc); { output c bits }
var
  Temp: BYTE;
  Got: WORD;
begin
  putbuf := putbuf or (c shr putlen);
  inc(putlen, l);
  if (putlen >= 8) then
  begin
    Temp := putbuf shr 8;
    PutBytes(Temp, 1, Got);
    DEC(putlen, 8);
    if (putlen >= 8) then
    begin
      Temp := Lo(PutBuf);
      PutBytes(Temp, 1, Got);
      INC(codesize, 2);
      DEC(putlen, 8);
      putbuf := c shl (l - putlen);
    end
    else
    begin
      putbuf := putbuf shl 8;
      INC(codesize);
    end;
  end;
end;

{ initialize freq tree }

procedure TLZH.StartHuff;
var
  i, j: Int16;
begin
  for i := 0 to N_CHAR - 1 do
  begin
    freq^[i] := 1;
    son^[i] := i + T;
    prnt^[i + T] := i;
  end;
  i := 0;
  j := N_CHAR;
  while (j <= R) do
  begin
    freq^[j] := freq^[i] + freq^[i + 1];
    son^[j] := i;
    prnt^[i] := j;
    prnt^[i + 1] := j;
    INC(i, 2);
    INC(j);
  end;
  freq^[T] := $FFFF;
  prnt^[R] := 0;
end;

{ reconstruct freq tree }

procedure TLZH.reconst;
var
  i, j, k, tmp: Int16;
  f, l: WORD;
begin
  { halven cumulative freq FOR leaf nodes }
  j := 0;
  for i := 0 to PRED(T) do
  begin
    if (son^[i] >= T) then
    begin
      freq^[j] := SUCC(freq^[i]) div 2; {@@ Bug Fix MOD -> DIV @@}
      son^[j] := son^[i];
      INC(j);
    end;
  end;
  { make a tree : first, connect children nodes }
  i := 0;
  j := N_CHAR;
  while (j < T) do
  begin
    k := SUCC(i);
    f := freq^[i] + freq^[k];
    freq^[j] := f;
    k := PRED(j);
    while f < freq^[k] do
      DEC(K);
    INC(k);
    l := (j - k) shl 1;
    tmp := SUCC(k);
    move(freq^[k], freq^[tmp], l);
    freq^[k] := f;
    move(son^[k], son^[tmp], l);
    son^[k] := i;
    INC(i, 2);
    INC(j);
  end;
  { connect parent nodes }
  for i := 0 to PRED(T) do
  begin
    k := son^[i];
    if (k >= T) then
    begin
      prnt^[k] := i;
    end
    else
    begin
      prnt^[k] := i;
      prnt^[SUCC(k)] := i;
    end;
  end;

end;

{ update freq tree }

procedure TLZH.update(c: Int16);
var
  i, j, k, l: Int16;
begin
  if (freq^[R] = MAX_FREQ) then
  begin
    reconst;
  end;
  c := prnt^[c + T];
  repeat
    INC(freq^[c]);
    k := freq^[c];

    { swap nodes to keep the tree freq-ordered }
    l := SUCC(C);
    if (k > freq^[l]) then
    begin
      while (k > freq^[l]) do
        INC(l);
      DEC(l);
      freq^[c] := freq^[l];
      freq^[l] := k;

      i := son^[c];
      prnt^[i] := l;
      if (i < T) then
        prnt^[SUCC(i)] := l;

      j := son^[l];
      son^[l] := i;

      prnt^[j] := c;
      if (j < T) then
        prnt^[SUCC(j)] := c;
      son^[c] := j;

      c := l;
    end;
    c := prnt^[c];
  until (c = 0);                        { REPEAT it until reaching the root }
end;

procedure TLZH.EncodeChar(c: WORD; PutBytes: PutBytesProc);
var
  i: WORD;
  j, k: Int16;
begin
  i := 0;
  j := 0;
  k := prnt^[c + T];

  { search connections from leaf node to the root }
  repeat
    i := i shr 1;

    {
    IF node's address is odd, output 1
    ELSE output 0
    }
    if BOOLEAN(k and 1) then
      INC(i, $8000);
    INC(j);
    k := prnt^[k];
  until (k = R);
  Putcode(j, i, PutBytes);
  code := i;
  len := j;
  update(c);
end;

procedure TLZH.EncodePosition(c: WORD; PutBytes: PutBytesProc);
var
  i, j: WORD;
begin
  { output upper 6 bits with encoding }
  i := c shr 6;
  j := p_code[i];
  Putcode(p_len[i], j shl 8, PutBytes);

  { output lower 6 bits directly }
  Putcode(6, (c and $3F) shl 10, PutBytes);
end;

procedure TLZH.EncodeEnd(PutBytes: PutBytesProc);
var
  Temp: BYTE;
  Got: WORD;
begin
  if BOOLEAN(putlen) then
  begin
    Temp := Lo(putbuf shr 8);
    PutBytes(Temp, 1, Got);
    INC(codesize);
  end;
end;

function TLZH.DecodeChar(GetBytes: GetBytesProc): Int16;
var
  c: WORD;
begin
  c := son^[R];

  {
   * start searching tree from the root to leaves.
   * choose node #(son[]) IF input bit = 0
   * ELSE choose #(son[]+1) (input bit = 1)
  }
  while (c < T) do
  begin
    c := c + GetBit(GetBytes);
    c := son^[c];
  end;
  c := c - T;
  update(c);
  Decodechar := Int16(c);
end;

function TLZH.DecodePosition(GetBytes: GetBytesProc): WORD;
var
  i, j, c: WORD;
begin
  { decode upper 6 bits from given table }
  i := GetByte(GetBytes);
  c := WORD(d_code[i] shl 6);
  j := d_len[i];

  { input lower 6 bits directly }
  DEC(j, 2);
  while j <> 0 do
  begin
    i := (i shl 1) + GetBit(GetBytes);
    DEC(J);
  end;
  DecodePosition := c or i and $3F;
end;

{ Compression }

procedure TLZH.InitLZH;
begin
  getbuf := 0;
  getlen := 0;
  putlen := 0;
  putbuf := 0;
  textsize := 0;
  codesize := 0;
  printcount := 0;
  match_position := 0;
  match_length := 0;
  try
    New(lson);
    New(dad);
    New(rson);
    New(text_buf);
    New(freq);
    New(prnt);
    New(son);
  except
    raise ElzhException.Create('LZH : Cannot get memory for dictionary tables');
  end;
end;

procedure TLZH.EndLZH;

begin
  try
    Dispose(son);
    Dispose(prnt);
    Dispose(freq);
    Dispose(text_buf);
    Dispose(rson);
    Dispose(dad);
    Dispose(lson);
  except
    raise ElzhException.Create('LZH : Error freeing memory for dictionary tables');
  end;
end;

procedure TLZH.LZHPack(var Bytes_Written: LongInt; GetBytes: GetBytesProc; PutBytes: PutBytesProc);
var
  ct: BYTE;
  i, len, r, s, last_match_length: Int16;
  Got: WORD;
begin

  InitLZH;

  try
    textsize := 0;                      { rewind and rescan }
    StartHuff;
    InitTree;
    s := 0;
    r := N - F;
    FillChar(Text_buf^[0], r, ' ');
    len := 0;
    Got := 1;
    while (len < F) and (Got <> 0) do
    begin
      GetBytes(ct, 1, Got);
      if Got <> 0 then
      begin
        text_buf^[r + len] := ct;
        INC(len);
      end;
    end;
    textsize := len;
    for i := 1 to F do
    begin
      InsertNode(r - i);
    end;
    InsertNode(r);
    repeat
      if (match_length > len) then
      begin
        match_length := len;
      end;
      if (match_length <= THRESHOLD) then
      begin
        match_length := 1;
        EncodeChar(text_buf^[r], PutBytes);
      end
      else
      begin
        EncodeChar(255 - THRESHOLD + match_length, PutBytes);
        EncodePosition(match_position, PutBytes);
      end;
      last_match_length := match_length;
      i := 0;
      Got := 1;
      while (i < last_match_length) and (Got <> 0) do
      begin
        GetBytes(ct, 1, Got);
        if Got <> 0 then
        begin
          DeleteNode(s);
          text_buf^[s] := ct;
          if (s < PRED(F)) then
          begin
            text_buf^[s + N] := ct;
          end;
          s := SUCC(s) and PRED(N);
          r := SUCC(r) and PRED(N);
          InsertNode(r);
          inc(i);
        end;
      end;                              { endwhile }
      INC(textsize, i);
      while (i < last_match_length) do
      begin
        INC(i);
        DeleteNode(s);
        s := SUCC(s) and PRED(N);
        r := SUCC(r) and PRED(N);
        DEC(len);
        if BOOLEAN(len) then
          InsertNode(r);
      end;                              { endwhile }
    until (len <= 0);                   { end repeat }
    EncodeEnd(PutBytes);

  finally
    EndLZH;
  end;

  Bytes_Written := TextSize;

end;

procedure TLZH.LZHUnpack(TextSize: Longint; GetBytes: GetBytesProc; PutBytes: PutBytesProc);
var
  c, i, j, k, r: Int16;
  c2: Byte;
  count: Longint;
  Put: Word;

begin

  InitLZH;

  try
    StartHuff;
    r := N - F;
    FillChar(text_buf^[0], r, ' ');
    Count := 0;
    while count < textsize do
    begin
      c := DecodeChar(GetBytes);
      if (c < 256) then
      begin
        c2 := Lo(c);
        PutBytes(c2, 1, Put);
        text_buf^[r] := c;
        INC(r);
        r := r and PRED(N);
        INC(count);
      end
      else
      begin                             {c >= 256 }
        i := (r - SUCC(DecodePosition(GetBytes))) and PRED(N);
        j := c - 255 + THRESHOLD;
        for k := 0 to PRED(j) do
        begin
          c := text_buf^[(i + k) and PRED(N)];
          c2 := Lo(c);
          PutBytes(c2, 1, Put);
          text_buf^[r] := c;
          INC(r);
          r := r and PRED(N);
          INC(count);
        end;                            { for }
      end;                              { if c < 256 }
    end;                                {endwhile count < textsize }

  finally
    ENDLZH;
  end;
end;

// Return as many bytes to the LZH compression buffer as requested.

procedure TLZH.GetBlockStream(var DTA; NBytes: Word; var Bytes_Got: Word);
begin
  //copy from stream into lzh compression buffer
  Bytes_Got := NBytes;
  if (StreamIn.Size - StreamIn.Position) < NBytes then
    Bytes_Got := StreamIn.Size - StreamIn.Position;
  StreamIn.ReadBuffer(DTA, Bytes_Got);
end;

procedure TLZH.PutBlockStream(var DTA; NBytes: Word; var Bytes_Got: Word);
begin
  //write from lzh decompression buffer to stream
  Bytes_Got := NBytes;
  StreamOut.WriteBuffer(DTA, Bytes_Got);
end;

procedure TLZH.PutC(aVal: Int16);
begin
  if (FDestPos >= FDestLimit) then
  begin
    FDestLimit := FDestPos * 2;
    FPDest := PByte(ReallocMemory(FPDest, FDestLimit));
  end;

  Assert(FDestPos < FDestLimit);
  (FPDest + FDestPos)^ := aVal;
  Inc(FDestPos);
end;

procedure TLZH.GetBlockMem(var DTA; NBytes: Word; var Bytes_Got: Word);
begin
  //copy from mem into lzh compression buffer

end;

procedure TLZH.PutBlockMem(var DTA; NBytes: Word; var Bytes_Got: Word);
begin

end;

{*------------------------------------------------------------------------------

  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TLZH.GetC: Int16;
begin
  if (FSrcPos < FSrcLimit) then
  begin
    Result := (FPSrc + FSrcPos)^;
    Inc(FSrcPos);
  end
  else
    Result := -1;
end;

{*------------------------------------------------------------------------------

  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TLZH.GetBit: Int16;
var
  i: Int16;
begin
  while FGetLen <= 8 do
  begin
    i := GetC();
    if i < 0 then
      i := 0;

    FGetBuff := FGetBuff or (i shl (8 - FGetLen));
    Inc(FGetLen, 8);
  end;

  i := FGetBuff;
  FGetBuff := FGetBuff shl 1;
  Dec(FGetLen);
  Result := Int16((i shr 15) and 1);
end;

{*------------------------------------------------------------------------------

  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TLZH.GetByte: Int16;
var
  j: BYTE;
  i: Word;
begin
  while (FGetLen <= 8) do
  begin
    i := GetC;

    if i < 0 then
      i := 0;

    FGetBuff := FGetBuff or (i shl (8 - FGetLen));
    Inc(FGetLen, 8);

    getbuf := getbuf or (i shl (8 - getlen));
    INC(getlen, 8);
  end;

  i := FGetBuff;
  FGetBuff := FGetBuff shl 8;
  Dec(FGetLen, 8);
  Result := Int16((i and $FF00) shr 8);
end;

{*------------------------------------------------------------------------------

  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TLZH.DecodeChar: Int16;
var
  c: Word;
begin
  c := son^[R];

  {
   * start searching tree from the root to leaves.
   * choose node #(son[]) IF input bit = 0
   * ELSE choose #(son[]+1) (input bit = 1)
  }
  while (c < T) do
  begin
    c := c + GetBit;
    c := son^[c];
  end;
  c := c - T;
  update(c);
  Result := Int16(c);
end;

{*------------------------------------------------------------------------------

  @return
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

function TLZH.DecodePosition: Word;
var
  i, j, c: WORD;
begin
  { decode upper 6 bits from given table }
  i := GetByte();
  c := WORD(d_code[i] shl 6);
  j := d_len[i];

  { input lower 6 bits directly }
  DEC(j, 2);
  while j <> 0 do
  begin
    i := (i shl 1) + GetBit();
    DEC(J);
  end;
  Result := c or (i and $3F);
end;

{*------------------------------------------------------------------------------

  @param aText
  @param aTextSize
  @param aCode
  @param aCodeSize
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TLZH.Decompress(var aText: PByte; var aTextSize: Cardinal;
  const aCode: PByte; aCodeSize: Cardinal);
var
  i, j, k, r, c: Int16;
  Count: Cardinal;
begin
  InitLZH;

  try
    FTextSize := PCardinal(aCode)^;
    FDestLimit := FTextSize;
    FPDest := PByte(AllocMem(FDestLimit));
    FDestPos := 0;

    FCodeSize := aCodeSize;
    FSrcLimit := FCodeSize;
    FPSrc := aCode;
    FSrcPos := 4;

    FGetBuff := 0;
    FGetLen := 0;

    StartHuff;

    r := N - F;
    FillChar(text_buf^[0], r, ' ');
    Count := 0;

    while Count < FTextSize do
    begin
      c := DecodeChar();
      if c < 256 then
      begin
        PutC(c);
        text_buf^[r] := c;
        Inc(r);
        r := r and (N - 1);
        Inc(count);
      end
      else
      begin
        i := (r - DecodePosition() - 1) and (N - 1);
        j := c - 255 + THRESHOLD;

        for k := 0 to J - 1 do
        begin
          c := text_buf^[(i + k) and (N - 1)];
          PutC(C);
          text_buf^[r] := c;
          Inc(r);

          r := r and (N - 1);
          Inc(Count);
        end;
      end;
    end;
  finally
    ENDLZH;
  end;

  Assert(FDestPos = FTextSize);
  aText := FPDest;
  aTextSize := FTextSize;
end;

initialization
  if xrFSLLib = 0 then
    xrFSLLib := LoadLibraryA(PAnsiChar('xrFSL_Core.dll'));

  if xrFSLLib <> 0 then
  begin
    @lzh_compress := GetProcAddress(xrFSLLib, PAnsiChar('?lzh_compress@@YAXPAEAAIPBEI@Z'));
    @lzh_decompress := GetProcAddress(xrFSLLib, PAnsiChar('?lzh_decompress@@YAXPAEAAIPBEI@Z'));
  end;

finalization
  if xrFSLLib <> 0 then
    FreeLibrary(xrFSLLib);

end.

