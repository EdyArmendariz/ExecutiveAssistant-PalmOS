
#ifndef _EVENT_HANDLING_
#define _EVENT_HANDLING_



#ifdef __GNUC__
#include "Sections.h"
#endif

void ChangeMyLabel( FormPtr frm, UInt16 labelID, const Char *newLabel);
static Boolean frmAchtungListCtlSelectEvent( EventType * et ) CODESEGMENT2;
static Boolean frmProjectEmployeeListHE( EventType * et );
static Boolean lstEmployeeTableSelectEvent( EventType * et );
static Boolean lstProjectEmployeeTableSelectEvent( EventType * et );


 
/**
 * 
 * 
 */
static Boolean frmAchtungListTableSelectEvent( EventType * et )
{
	Err error = 0;
	FormType * dlgForm;
	Boolean handled = false;
	UInt16 recordid;
	MemHandle packedH;
	structMilestone * stctMile;

	dlgForm = FrmInitForm( dlgAchtungList );

	// which list item was selected?
	int i = et->data.lstSelect.selection;
	ListType * lt = et->data.lstSelect.pList;
	int j = LstGetSelection ( lt );		
	// - - - - - SEARCH DB HERE - - - - - - - - - - 
	error = DmFindRecordByID(  globalOpenRefMilestone,
								 globalAchtungListUInt32[j], &recordid );
	packedH = DmQueryRecord( globalOpenRefMilestone, recordid );
			stctMile = ( structMilestone * ) MemHandleLock( packedH );
	
	// - - - - - - - - - - - - - - - - - - - - - - 
	ChangeMyLabel( dlgForm, lblDialogAchtung, (*stctMile).desc );
	FrmDoDialog( dlgForm );
	FrmDeleteForm( dlgForm );
	MemHandleUnlock( packedH );
}

static Boolean lstEmployeeTableSelectEvent( EventType * et )
{
	Err error = 0;
	FormType * dlgForm;
	Boolean handled = false;
	UInt16 recordid;
	MemHandle packedH;
	structMilestone * stctMile;
	HostTraceInit();
	HostTraceOutputTL(appErrorClass, "lstEmployeeTableSelectEvent enter" );
	
	dlgForm = FrmInitForm( dlgEmployeeMilestoneInfo );

	// which list item was selected?
	int i = et->data.lstSelect.selection;
	ListType * lt = et->data.lstSelect.pList;
	int j = LstGetSelection ( lt );
	Char * c = LstGetSelectionText( lt, j );	
	// - - - - - SEARCH DB HERE - - - - - - - - - - 
	error = DmFindRecordByID(  globalOpenRefMilestone,
								 globalListProjectEmployeeUInt32[j], &recordid );
	packedH = DmQueryRecord( globalOpenRefMilestone, recordid );
			stctMile = ( structMilestone * ) MemHandleLock( packedH );
	
	// - - - - - - - - - - - - - - - - - - - - - - 
	ChangeMyLabel( dlgForm, lblEmployeeMilestoneInfo,  (*stctMile).desc );
	FrmDoDialog( dlgForm );
	FrmDeleteForm( dlgForm );
	MemHandleUnlock( packedH );
	
	HostTraceOutputTL(appErrorClass, "lstEmployeeTableSelectEvent exit" );
	HostTraceClose();
}

static Boolean lstProjectEmployeeTableSelectEvent( EventType * et )
{
	Err error = 0;
	FormType * dlgForm;
	Boolean handled = false;

	dlgForm = FrmInitForm( dlgAchtungList );

	// which list item was selected?
	int i = et->data.lstSelect.selection;
	ListType * lt = et->data.lstSelect.pList;
	int j = LstGetSelection ( lt );
	Char * c = LstGetSelectionText( lt, j );	
	ChangeMyLabel( dlgForm, lblDialogAchtung,  c );
	FrmDoDialog( dlgForm );
	FrmDeleteForm( dlgForm );
}

static Boolean frmAchtungListCtlSelectEvent( EventType * et )
{
	Boolean bHandled = false;
	if(  globalOpenRefMilestone  );
	
	switch( et -> data.ctlSelect.controlID )
	{		
		case btnAchtungListBack:
			FrmGotoForm( frmProjectMenu );
		break;			
		case btnAchtungListSelect:
			FrmGotoForm( frmProjectList );
			
		break;
		case btnAchtungListMainMenu:
			FrmGotoForm( frmMainMenu );
		break;	
	}

	return bHandled;
}



#endif

