unit Unit1;

interface

uses
 PDStandardLib_TLB, PDDirectLib_TLB, Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }


  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.Button1Click(Sender: TObject);
label  crash;
var

   pddbq: PDDatabaseQuery;
   pdra: IUnknown;
   pdsa: IPDSystemAdapter;
   pdhsu: PDHotSyncUtility;
   strName: WideString;
   strName2: WideString;
   action: TCloseAction;
begin
     Try
        pdhsu := PDStandardLib_TLB.CoPDHotSyncUtility.Create;

         if not pdhsu.IsSyncInProgress then
         begin
            ShowMessage( 'Start HotSync');
            GOTO crash;
         end;

        strName := 'MemoDB';
        strName2 := 'PDDirect.PDRecordAdapter';
        pdra := PDStandardLib_TLB.CoPDRecordAdapter.Create;

        pdsa := PDDirectLib_TLB.CoPDSystemAdapter.Create;



        pddbq := PDDirectLib_TLB.CoPDDatabaseQuery.Create;
        pdra :=  pddbq.OpenRecordDatabase(strName, strName2, 1 );

     crash:
     Except
           pdsa._Release;

     end;

     Application.Terminate;
end;

end.
 