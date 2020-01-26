program Project1;

uses
  Forms,
  uMain in 'uMain.pas' {frmMain},
  uFrmMilestoneInfo in 'uFrmMilestoneInfo.pas' {frmMilestoneInfo};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TfrmMilestoneInfo, frmMilestoneInfo);
  Application.Run;
end.
