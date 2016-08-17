program DBTools;

uses
  ExceptionLog,
  Forms,
  formMain in 'Source\formMain.pas' {frmMain},
  SAFileSystem in 'Source\SAFileSystem.pas',
  xrFSLDBTools in 'Source\xrFSLDBTools.pas',
  SAStringUtils in 'Source\SAStringUtils.pas',
  LZH in 'Source\LZH.pas',
  formProgress in 'Source\formProgress.pas' {frmProgress},
  dbtOperationsThreads in 'Source\dbtOperationsThreads.pas',
  dbtCommon in 'Source\dbtCommon.pas',
  SAScrambler in 'Source\SAScrambler.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.
