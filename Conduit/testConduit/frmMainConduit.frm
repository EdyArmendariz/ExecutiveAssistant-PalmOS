VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Start 
      Caption         =   "Start Here"
      Height          =   735
      Left            =   240
      TabIndex        =   0
      Top             =   1200
      Width           =   2655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Private Sub Start_Click()
    On Error GoTo Err_handler
    
    Dim myQuery As New PDDirectLib.PDDatabaseQuery
    Dim dbinfo As PDDatabaseInfo
   ' Dim myDBInfo As New dmDatabaseInfo
    Dim Adapter As PDRecordAdapter
    Dim PDCondMgr As New PDCondMgr
    Dim CreatorID As Long
    Dim ConduitCount As Long
    Dim pdci As New PDConduitInfo
    Dim strA As String, strB As String
    Dim pdhi As PDHotsyncInfo
    Dim pdhsu As New PDstandardlib.PDHotSyncUtility
    Dim pdsa As New PDSystemAdapter
    Dim pdud As New PDUserData
    Dim pdeci As New PDDirectLib.PDExpansionCardInfo
    Dim luserid As Long
  '  Dim reccount As Long
    'Debug.Print hello

    If Not pdhsu.IsSyncInProgress Then
        MsgBox "HotSync not Running"
        Exit Sub
    End If

    CreatorID = PDCondMgr.StringToCreatorID("memo")
ConduitCount = PDCondMgr.GetConduitCount()
    Set pdci = PDCondMgr.GetConduitInfo(CreatorID)
         
   luserid = pdud.GetIDFromName("QA TEST")
   strB = pdud.GetUserDirectory(luserid)
   strA = pdud.GetRootDirectory
    
    'Set Value = myQuery.ReadDbNameList(True)
   ' On Error Resume Next
    Adapter = myQuery.OpenRecordDatabase("MemoDB", eRead Or eWrite)
    'reccount = Adapter.RecordCount
    
    
Exit Sub
Err_handler:
MsgBox Error(Err.Number)
'MsgBox "DOH!" & vbNewLine &  Err.Description & vbNewLine & "Number = " & Hex(Err.Number)

End Sub
