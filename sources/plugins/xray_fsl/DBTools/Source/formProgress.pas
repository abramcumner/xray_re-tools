unit formProgress;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls;

type
  TfrmProgress = class(TForm)
    pbProgress: TProgressBar;
    lbl1: TLabel;
    btnCancel: TButton;
    procedure btnCancelClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    FCanceled: Boolean;
  public
    property Canceled: Boolean read FCanceled;
  end;

var
  frmProgress: TfrmProgress;

implementation

{$R *.dfm}

procedure TfrmProgress.btnCancelClick(Sender: TObject);
begin
  FCanceled := True;
  ModalResult := mrCancel;
end;

procedure TfrmProgress.FormCreate(Sender: TObject);
begin
  FCanceled := False;
end;

end.

