program Project1;

uses
  Forms,
  Unit1 in 'Unit1.pas' {Form1},
  PDStandardLib_TLB in '..\..\..\..\..\..\Borland\Delphi4\Imports\PDStandardLib_TLB.pas',
  PDDirectLib_TLB in '..\..\..\..\..\..\Borland\Delphi4\Imports\PDDirectLib_TLB.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
