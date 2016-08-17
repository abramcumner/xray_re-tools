unit dbtOperationsThreads;

interface

uses
  xrFSLDBTools, Classes, Forms;

type
  {*------------------------------------------------------------------------------
    Класс базового потока для операций с прогрессом.
  -------------------------------------------------------------------------------}
  TBaseProgressThread = class(TThread)
  private
    FProgressForm: TForm;               /// Экземпляр формы прогресса
    FDBTools: TDBTools;                 /// Указатель на класс архива

    FDBToolsOwner: Boolean;             /// Флаг, создавали ли мы сами DBTools или взяли внешний

    FProgress: TDBProgress;             /// Прогресс(обновляется из обработчика)
    FProgressHandler: TDBProgressProc;  /// Лямбда метод обработчик прогресса

  protected
    procedure OnComplete(Sender: TObject);
  public
    constructor Create(const aProgressForm: TForm; const aDBTools: TDBTools = nil);
    destructor Destroy; override;
  end;

  {*------------------------------------------------------------------------------
    Класс потока, выполняющего открытие архива в отдельном потоке.
  -------------------------------------------------------------------------------}
  TOpenDBThread = class(TBaseProgressThread)
  private
    FFileName: string;                  /// Имя файла, который необходимо открыть
  public
    constructor Create(const aProgressForm: TForm; const aDBTools: TDBTools; const aFileName: string);
    destructor Destroy; override;

    procedure Execute; override;
  end;

implementation

uses
  formProgress, Controls, SysUtils;

{ TBaseProgressThread }

{*------------------------------------------------------------------------------

  @param aProgressForm
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TBaseProgressThread.Create(const aProgressForm: TForm;
  const aDBTools: TDBTools);
begin
  // Заполнение полей класса
  FProgressForm := aProgressForm;

  // назначение хандлера завершения потока
  OnTerminate := OnComplete;

  //--------------------------------------
  // Определение колбэк метода прогресса
  //--------------------------------------
  {(*}
  FProgressHandler := function(Sender: TDBTools; const aProgress: TDBProgress): Boolean
  var
    // лямбда процедура для синхронизированного вызова
    UpdateProc: TThreadProcedure;
  begin
    // Сохраняем данные прогресса
    FProgress := aProgress;

    // Определение лямбда процедуры для синхронизированного обновления прогресса
    UpdateProc := procedure
    begin
      TfrmProgress(FProgressForm).lbl1.Caption := FProgress.OperationName;
      TfrmProgress(FProgressForm).pbProgress.Position := FProgress.Progress;
      FProgress.Canceled := TfrmProgress(FProgressForm).Canceled;
    end;

    Synchronize(UpdateProc);
    Result := FProgress.Canceled;
  end;
  {*)}

  //------------------------
  // Создание класса архива
  //------------------------
  FDBToolsOwner := not Assigned(aDBTools);
  if FDBToolsOwner then
    FDBTools := TDBTools.Create
  else
    FDBTools := aDBTools;

  FDBTools.OnProgress := FProgressHandler;

  inherited Create(False);
end;

{*------------------------------------------------------------------------------

  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

destructor TBaseProgressThread.Destroy;
begin
  if FDBToolsOwner and Assigned(FDBTools) then
    FreeAndNil(FDBTools);

  inherited;
end;

{*------------------------------------------------------------------------------
  Закрытие окна прогресса.
  @param Sender
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TBaseProgressThread.OnComplete(Sender: TObject);
begin
  FProgressForm.ModalResult := mrOk;
end;

{ TOpenDBThread }

{*------------------------------------------------------------------------------

  @param aProgressForm
  @param aFileName
  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

constructor TOpenDBThread.Create(const aProgressForm: TForm;
  const aDBTools: TDBTools; const aFileName: string);
begin
  inherited Create(aProgressForm, aDBTools);

  FFileName := aFileName;
end;

{*------------------------------------------------------------------------------

  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

destructor TOpenDBThread.Destroy;
begin

  inherited;
end;

{*------------------------------------------------------------------------------

  @author Neo][
  @throws
  @todo
-------------------------------------------------------------------------------}

procedure TOpenDBThread.Execute;
begin
  inherited;

  FDBTools.Open(FFileName);
end;

end.

