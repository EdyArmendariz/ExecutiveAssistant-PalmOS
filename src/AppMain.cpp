/******************************************************************************
 *
 * Copyright (c) 1999-2004 PalmSource, Inc. All rights reserved.
 *
 * File: AppMain.c
 *
 *****************************************************************************/

#include <PalmOS.h>  
#include <StringMgr.h>
#include <stdlib.h>
#include "AppResources.h"
#include "NodeManager.h"
#include "Object.h"
#include "Database.h"
#include "EventHandling.h"
#include "Sections.h"

/***********************************************************************
 *
 *	Entry Points
 *
 ***********************************************************************/


/***********************************************************************
 *
 *	Internal Constants
 *
 ***********************************************************************/
#define appFileCreator			'MND0'	// register your own at http://www.palmos.com/dev/creatorid/
#define appVersionNum			0x01
#define appPrefID				0x00
#define appPrefVersionNum		0x01


const Int16 FORM_EMPLOYEE = 1;
const Int16 FORM_PROJECTEMPLOYEE = 2;
const Int16 PRJEMPPOPUPMILESTONE = 1;
const Int16 PRJEMPPOPUPPROJECT   = 2;

Coord glox = 2;
RectangleType  TestRectangle;

UInt32 globalSelectedNodeID = 0;
structMilestone * globalSelectedMilestone = NULL;
Database myDatabase;
DmOpenRef globalOpenRefProject = NULL;
DmOpenRef globalOpenRefEmployee = NULL;

UInt16 EMPLOYEE_COUNT = 0;
 
LocalID dbID;
int globalProjectListRow = 0;    
Char ** globalProjectList = 0x0;
Int16 globalProjectListCount = 0;
Char * 	globalProjectListSelection = 0x0;
Int16 	globalProjectIDSelected = 0;

Char ** globalEmployeeList = 0x0;
Int16 globalEmployeeListCount = 0;
Char * globalEmployeeSelection;
Int16 globalEmployeeFORMSELECTED = 0; // Keep track of the referring form

Char ** globalListProjectEmployee = 0x0;
Int16 globalListProjectEmployeeCount = 0;

Char ** globalProjectEmployeeList = 0x0;
Int16 globalProjectEmployeeListCount = 0;
Int16 globalProjectEmployeePopup = 1;


Char * globalMilestoneName = 0x0;
Char * globalMilestoneDesc = 0x0;
UInt32 globalMilestoneID = 0; // Assigned a value of structMilestone.id from MyGadgetHandler
UInt16 globalMilestoneRecID = 0;
Char ** globalAchtungList = 0x0;
Int16 globalAchtungListCount = 0;
FieldType * globalMilestoneDescField = 0x0;
MemHandle memHandle = 0x0, globalMemHandle = 0x0;
NodeManager nodeManager;
bool isLabelShowing = false;
FormType  * frmBalloon = 0x0;

UInt32 globalEmpID [7] = { 0, 0, 0, 0, 0, 0, 0 };

void ChangeLabel(UInt16 labelID, const Char *newLabel);
void ChangeMyLabel( FormPtr frm, UInt16 labelID, const Char *newLabel);
bool isClickInRectangle( const RectangleType * rt, Coord _x, Coord _y );
static Boolean frmProjectEmployeeListHE( EventType * et ) CODESEGMENT1;
static Boolean frmProjectListHandleEvent( EventType * et ) CODESEGMENT1;
static Boolean frmProjectListTableSelectEvent( EventType *  et ) CODESEGMENT1;
static Boolean frmProjectMenuHandleEvent( EventType * et ) CODESEGMENT1;
static Boolean frmProjectNewCtlSelectEvent( EventPtr ep ) CODESEGMENT2;
static Boolean frmProjectEmployeeHandleEvent( EventPtr ep ) CODESEGMENT2;
static void assignEmployeeToMilestone( ) CODESEGMENT2;

/***********************************************************************
 *
 *	Internal Functions
 *
 ***********************************************************************/
 

 static Boolean frmDynamicFormHandleEvent ( EventPtr ep )
{
	Boolean handled = false;
	FormPtr ptrForm;
	
	switch( ep->eType )
	{
		case frmOpenEvent:
			ptrForm = FrmGetActiveForm();
			FrmDrawForm(ptrForm);
			handled = true;	
		break;	
		case ctlSelectEvent:
			
			switch ( ep->data.ctlSelect.controlID  )
			{
				case 692:
					FrmGotoForm(  frmGraph );
				break;	
			}
			
		break;
	}
	return handled;	
}
 
 /*
  * Handles the movement, selection, etc. of the Milestones in the graph.
  * 
  */
 static Boolean MyGadgetHandler( FormGadgetTypeInCallback * gadgetP,
	UInt16 cmd, void * paramP )
	{
		Err error = errNone;
		Boolean handled = false;
		Boolean isPenDown = true, uselessPenDown;
		EventPtr eventP = NULL;
		Coord crdOldScreenX = 0, crdOldScreenY = 0;  // Coord stores raw coordinates
		Coord crdNewScreenX = 0, crdNewScreenY = 0;
		Coord crdFirstClickX = 0, crdFirstClickY = 0;
		int id = gadgetP->id, id2 = 0;
		UInt32 romVersion = 0;
		UInt16 ticksPerSec = SysTicksPerSecond();
		UInt16 index = 0;
		FormType * myFTP = 0x0, * ptrForm = 0x0, * thisForm = FrmGetActiveForm();
		RectangleType  rct, rctBalloonMsg;
		ControlType * ctbtnOK = 0x0, * ctbtnEdit = 0x0;
		MemHandle mh = 0x0, h = 0x0;
		BitmapType * ptrbmp = 0x0;
		Node * ptrNode = 0x0, * buffNode = 0x0;
		Node myNode;
		structMilestone * stctMile, * ptrMile, saveMile, newMile;
		UInt16 rowcount = 0;
		Char * c;
		switch ( cmd ) // cmd is one of 4 values
		{	
			case formGadgetDrawCmd:
				// ... does nothing
			break;
			case formGadgetHandleEventCmd:		// 2				
				eventP = ( EventPtr ) paramP;
					switch ( eventP->eType )
					{
						case frmGadgetEnterEvent:
						{
							stctMile = (structMilestone*)FrmGetGadgetData( thisForm, FrmGetObjectIndex( thisForm, gadgetP->id ) );							//i
							globalSelectedMilestone = stctMile;
							id2 = stctMile->id;
							ptrMile = nodeManager.borrowNodeByUnique( stctMile->id );
							globalSelectedNodeID = ptrMile->id;
							saveMile.id = ptrMile->id;
							saveMile.pid = ptrMile->pid;
							saveMile.icon = ptrMile->icon;
							StrCopy ( saveMile.name, ptrMile->name );
							StrCopy ( saveMile.desc, ptrMile->desc );
							saveMile.xpos = ptrMile->xpos;
							saveMile.ypos = ptrMile->ypos;
							saveMile.intTopSib = ptrMile->intTopSib;
							saveMile.intBottomSib = ptrMile->intBottomSib;
							saveMile.intRightSib = ptrMile->intRightSib;
							saveMile.intLeftSib = ptrMile->intLeftSib;
							
							crdOldScreenX = ( * ptrMile ).xpos;
							crdOldScreenY = ( * ptrMile ).ypos;										
							
							EvtGetPen( &crdFirstClickX, &crdFirstClickY, &uselessPenDown );															
							
							while ( isPenDown )
							{
								EvtGetPen ( &crdNewScreenX, &crdNewScreenY, &isPenDown );
								SysTaskDelay( ticksPerSec / 10);
							}
							
							// Single Tap within the Node
							if ( RctPtInRectangle (crdNewScreenX, crdNewScreenY,
              										&(gadgetP->rect)) )
							{		
								char * cButton = "OK";
								char * cEdit = "Edit";																				
 								WinPushDrawState();
 								if ( !isLabelShowing )
 								{
 									isLabelShowing = true;					
 									rctBalloonMsg.extent.x = 50;
 									rctBalloonMsg.extent.y = 24;
 									
 									if ( crdOldScreenX < 54 )
 										rctBalloonMsg.topLeft.x = 05;
 									else if ( crdOldScreenX > 108 )
 										rctBalloonMsg.topLeft.x = 78;
 									else
 										rctBalloonMsg.topLeft.x = 40;
 									
 									if ( crdOldScreenY < 32 )
 										rctBalloonMsg.topLeft.y = crdOldScreenY + 14;
 									else	
 										rctBalloonMsg.topLeft.y = crdOldScreenY - 28;										
										
									if ( globalMilestoneName != NULL )
									{
										delete [] globalMilestoneName;	
										globalMilestoneName = NULL;
									}	
									if ( globalMilestoneDesc != NULL )
									{
										delete [] globalMilestoneDesc;	
										globalMilestoneDesc = NULL;
									}		
									globalMilestoneID = (* stctMile ).id;
									globalMilestoneName = new Char[ strlen( (* stctMile ).name ) + 1 ];								
 									globalMilestoneDesc =  new Char[ strlen( (* stctMile ).desc ) + 1 ];		
									globalMilestoneName = StrCopy( globalMilestoneName, (* stctMile ).name );
									c = globalMilestoneName;
									globalMilestoneDesc = StrCopy( globalMilestoneDesc, (* stctMile ).desc );
 									// Position the Balloon Message correctly
 									frmBalloon = FrmNewForm( 69, globalMilestoneName, rctBalloonMsg.topLeft.x, rctBalloonMsg.topLeft.y,
 															80, rctBalloonMsg.extent.y, true, 0, 0, 0 );
 																																			
 									void * formPP = frmBalloon;
 									ctbtnOK = CtlNewControl(  &formPP, 692, buttonCtl, cButton, 60, 15, 20, 
 									10, stdFont, 0, false );
 									ctbtnEdit = CtlNewControl(  &formPP, 693, buttonCtl, cEdit, 35, 15, 20, 
 									10, stdFont, 0, false );
 									CtlSetEnabled( ctbtnOK, true );
 									UInt16 editButton = FrmDoDialog( frmBalloon );
 									if ( editButton == 693 )
 									{
 										FrmGotoForm( frmMilestoneInfoDlg );
 									}
 								}
 								else
 								{										
 									isLabelShowing = false;
 									if ( FrmValidatePtr ( frmBalloon ) )
 									{
 										UInt16 mIndy = FrmGetObjectIndex( frmBalloon, 692 );									
 										FrmRemoveObject( &frmBalloon , mIndy );
 										FrmDeleteForm ( frmBalloon );
 									}
 								}
 								WinPopDrawState();					
							}
							else 
							{	
								// the Milestone was moved										
								nodeManager.eraseConnectorLines( &saveMile );		
								int distx = ( crdFirstClickX - crdOldScreenX );
								if ( distx < 0 )
									distx * (-1);
								int disty = ( crdFirstClickY - crdOldScreenY );	
								if ( disty < 0 )
									disty * (-1);
									
								crdNewScreenX = crdNewScreenX - distx;
								crdNewScreenY = crdNewScreenY - disty;
								
								gadgetP->rect.topLeft.x = crdNewScreenX ;
  								gadgetP->rect.topLeft.y = crdNewScreenY ;
  								saveMile.xpos = crdNewScreenX;
  								saveMile.ypos = crdNewScreenY;
								rct.topLeft.x = crdOldScreenX;
								rct.topLeft.y = crdOldScreenY;
								rct.extent.x = 20;
								rct.extent.y = 10;
								WinEraseRectangle ( &rct, 0 );			
								rct.topLeft.x = crdNewScreenX;
								rct.topLeft.y = crdNewScreenY;											
								nodeManager.drawConnectorLines ( &saveMile );												
								nodeManager.setNode ( saveMile, gadgetP->id );
								DmFindRecordByID( globalOpenRefMilestone,  saveMile.id, &index );
								h = DmQueryRecord( openRefMilestone, index  );
									stctMile = ( structMilestone * ) MemHandleLock( h );		
									newMile.id = (*stctMile).id;
									newMile.pid = (*stctMile).pid;
									StrCopy( newMile.name, (*stctMile).name );
									StrCopy( newMile.desc, (*stctMile).desc );
									newMile.xpos = (*stctMile).xpos;
									newMile.ypos = (*stctMile).ypos;
									newMile.icon = (*stctMile).icon;
								MemHandleUnlock( h );
								
								if ( ptrMile->icon == 1200 )
									mh = DmGetResource( 'Tbmp', 1200 );
								if ( ptrMile->icon == 1201 )
									mh = DmGetResource( 'Tbmp', 1201 );
								if ( ptrMile->icon == 1202 )
									mh = DmGetResource( 'Tbmp', 1202 );
								if ( ptrMile->icon == 1203 )
									mh = DmGetResource( 'Tbmp', 1203 );
								
								if ( mh == NULL )
									error = DmGetLastErr();	
								ptrbmp = (BitmapType*)MemHandleLock( mh );
								WinPaintBitmap ( ptrbmp, crdNewScreenX, crdNewScreenY);
								error = MemHandleUnlock ( mh );
							}						
						break;}
						case frmGadgetMiscEvent:
							index = 0;
						
							break;
						default:
							break;	
					}
				
				break;
			case formGadgetDeleteCmd:
				break;
			case formGadgetEraseCmd:
				break;
			default:
				break;
		}		
		return handled;	
	}
	
	
/**
 * !  ! !   !   !   !  !  !  !  !  !  !   !   !   !   !    !  !   !   !
 * ! !   ! ! ! !   CURRENTLY UNDER DEBUGGMENT	  !   !   !  !   !  ! ! 
 *  !  ! !   !   !   !  !  !  !  !  !  !   !   !   !   !    !  !   !   !
 */	
static Boolean frmEmployeeListTableSelectEvent	( EventType * et )
{
	Boolean handled = false;
	Char * c = 0x0;
	
	// which list item was selected?
	int i = et->data.lstSelect.selection;
	ListType * lt = et->data.lstSelect.pList;
	int j = LstGetSelection ( lt );	
	// 0 is the top of the list

	c = LstGetSelectionText( lt, j );
	if ( c == NULL )
		return handled;
	
	if ( globalEmployeeSelection )
	{
		delete [] globalEmployeeSelection;
		globalEmployeeSelection = NULL;
	}		
	
	globalEmployeeSelection = new Char[ strlen( c ) + 1] ;
	globalEmployeeSelection = StrCopy( globalEmployeeSelection, c );
	FrmGotoForm( frmProjectEmployee );
}
	

/**
 * What should a Project Employee selection do?
 * 
 */
static Boolean frmProjectEmployeeListTableSelectEvent( EventType *  et )
{
	Err error = 0;
	Boolean handled = false;
	Char * c = 0x0;
	// which list item was selected?
	int i = et->data.lstSelect.selection;
	ListType * lt = et->data.lstSelect.pList;
	int j = LstGetSelection ( lt );	
	c = LstGetSelectionText( lt, j );
	if ( c == NULL )
		return handled;
	
	if ( globalEmployeeSelection )
	{
		delete [] globalEmployeeSelection;
		globalEmployeeSelection = NULL;
	}		
	
	globalEmployeeSelection = new Char[ StrLen( c ) + 1] ;
	globalEmployeeSelection = StrCopy( globalEmployeeSelection, c );
	
	FrmGotoForm( frmProjectEmployee );
	handled = true;
	return handled;	
}
	
static Boolean frmProjectListTableSelectEvent( EventType *  et )
{
	Err error = 0;
	Boolean handled = false;
	Char * c = 0x0, * c2 = 0x0;
	structProject * stucp;
	Int16 tempVal = 0;
	MemHandle _handle;
	
	// which list item was selected?
	int i = et->data.lstSelect.selection;
	ListType * lt = et->data.lstSelect.pList;
	int j = LstGetSelection ( lt );	
	c = LstGetSelectionText( lt, j );
	
	if ( c == NULL )
		return handled;
	
	if ( globalProjectListSelection )
	{
		delete [] globalProjectListSelection;
		globalProjectListSelection = NULL;
	}		
	
	globalProjectListSelection = new Char[ StrLen( c ) + 1] ;
	globalProjectListSelection = StrCopy( globalProjectListSelection, c );
	globalProjectListRow = i;
	
	// search for the selection here
	for( int symbolicID = 0; symbolicID < globalProjectListCount; symbolicID++ )
	{
		_handle  = DmQueryRecord( globalOpenRefProject, symbolicID  );	
		
		if ( _handle )
		{
			stucp = (structProject*) MemHandleLock( _handle );
			if ( StrCompare( stucp->name, globalProjectListSelection ) == 0 )
				globalProjectIDSelected = stucp->pid;
			
			MemPtrUnlock( (MemPtr*)stucp );
		}
	}
	
	FrmGotoForm( frmProjectMenu );
	handled = true;
	return handled;	
}


static Boolean frmProjectListCtlSelectEvent( EventType * et )
{
	Boolean bHandled = false;
	
	switch( et -> data.ctlSelect.controlID )
	{		
		case btnProjectListBack:
		case btnProjectListBackButton:
			FrmGotoForm( frmMainMenu );
		break;				
	}

	return bHandled;
}


static Boolean frmEmployeeListCtlSelectEvent( EventType * et )
{
	Boolean bHandled = false;
	
	switch( et -> data.ctlSelect.controlID )
	{		
		case btnEmployeeListHome:
			FrmGotoForm( frmMainMenu );
		break;
		case btnEmployeeListBack:
			FrmGotoForm( frmMainMenu );
		break;				
	}

	return bHandled;
}


static Boolean frmAchtungListHandleEvent( EventType * et )
{
	Boolean handled = false;
	Err error = errNone;
	FormType * thisForm = FrmGetFormPtr( frmAchtungList );
	MemHandle _handle = NULL;
	Char ** ptrString;
	Char * projStr = globalProjectListSelection;
	Int16 projID = globalProjectIDSelected;
	UInt16 rowcount = 0;
	UInt16 idxList = 0;
	structMilestone * stctMilestone = NULL;
	ListType * ptrList;
	int listIndex = 0;
	
	FormType * ptrForm = FrmGetFormPtr( frmAchtungList );
	
	int i = et->eType;
	switch( et->eType )
	{
		case lstSelectEvent:  // 12
			frmAchtungListTableSelectEvent( et );	
			handled = true;		
		break;
		case frmOpenEvent:	// 24		
			
			/*
			 * for each milestone of project # p.
			 * get the status of the milestone.
			 * if the status is late, then add it to this list.
			 * 
			 */	
			 if ( globalAchtungListCount > 0 )
			 {					
				ptrString = (Char **) MemPtrNew( globalAchtungListCount * sizeof(Char*));
				globalAchtungListUInt32 = new UInt32[ globalAchtungListCount ];
				globalAchtungList = ptrString;			
				rowcount = DmNumRecords( openRefMilestone );		
			
				for( int index = 0; index < rowcount; index++ )
				{
					// for each node in the database
					_handle  = DmQueryRecord( openRefMilestone, index  );
					if ( _handle )
					{
						stctMilestone = ( structMilestone * ) MemHandleLock( _handle );
							
						if ( stctMilestone->pid != projID )
						{
							MemHandleUnlock( _handle );
						}
						else
						{
							if ( stctMilestone->icon == 1201 )
							{
								globalAchtungListUInt32[listIndex] = stctMilestone->id;
								ptrString[ listIndex ] = (Char*) MemPtrNew( StrLen( stctMilestone->name ) + 1 );
								StrCopy( ptrString[listIndex], stctMilestone->name );
								listIndex ++;
							}	
							MemHandleUnlock( _handle );
						}
					}
				}
				rowcount = globalAchtungListCount;
				idxList	= FrmGetObjectIndex( ptrForm, frmAchtungList );
				ptrList = ( ListType * )  FrmGetObjectPtr( ptrForm, idxList  );		
				LstSetListChoices( ptrList, ptrString, globalAchtungListCount );	
			 }
			 
			FrmDrawForm( thisForm );
			handled = true;	
		break;			
		case ctlSelectEvent:
			frmAchtungListCtlSelectEvent( et );
			handled = true;
		break;			
		case frmCloseEvent:
			/*
			 * for each item in this list, free it's memory 
			 */
			 if( globalAchtungList )
			{
				rowcount = globalAchtungListCount;
				for( int i = 0; i < globalAchtungListCount; i++ )
				{
					if ( globalAchtungList[i] != NULL )
						error = MemPtrFree((MemPtr) globalAchtungList[i]);
				}
          		error = MemPtrFree((MemPtr) globalAchtungList);
          		globalAchtungList = NULL;
          		globalAchtungListCount = 0;
			}
			
			handled = false;
		break;			
		default:  // 1, 2, 5, 7, 9, 11, 28, 
		break;		
	} 

	return handled;
}


static Boolean frmGraphHandleEvent(EventPtr pEvent)
{
	Boolean handled = false;
	FormType *thisForm = 0x0;
	EventType gadgetEvent;
	
	switch (pEvent->eType) {
		case frmOpenEvent:
			thisForm = FrmGetActiveForm();			
			FrmDrawForm( thisForm );
			handled = true;			
			nodeManager.drawNodes( thisForm, MyGadgetHandler, globalProjectIDSelected );	
			TestRectangle.topLeft.x = 80;
			TestRectangle.topLeft.y = 80;
			TestRectangle.extent.x = 10;
			TestRectangle.extent.y = 10;
			break;
		case ctlSelectEvent:
			switch (pEvent->data.ctlSelect.controlID ) 
			{
				case Form2DoneButton:
					FrmGotoForm( frmProjectMenu );
				break;
				case frmGraphGotoList:
					FrmGotoForm( frmProjectList );
				break;
				case frmGraphGotoHome:
					FrmGotoForm( frmMainMenu );
				break;
				case frmGraphUp:
					thisForm = FrmGetActiveForm();
					nodeManager.scrollNodes( thisForm, globalProjectIDSelected, 10, 0 );				
				break;
				case frmGraphDown:
					thisForm = FrmGetActiveForm();
					nodeManager.scrollNodes( thisForm, globalProjectIDSelected, -10, 0 );				
				break;
				
				case frmGraphLeft:
					thisForm = FrmGetActiveForm();
					nodeManager.scrollNodes( thisForm, globalProjectIDSelected, 0, 10 );			
				break;
				case frmGraphRight:
					thisForm = FrmGetActiveForm();
					nodeManager.scrollNodes( thisForm, globalProjectIDSelected, 0, -10 );	
				break;
			}	
			break;
		case frmUpdateEvent:
			FrmDrawForm( FrmGetActiveForm ( ) );
		break;		
		case penUpEvent:
			handled = false;	 			
		break;
		case penDownEvent:	
			// Allow the underlying gadget to handle this event
			handled = false;
		break;
		case frmCloseEvent:		
					
			thisForm = FrmGetActiveForm();
			nodeManager.eraseNodes( thisForm, globalProjectIDSelected );		
		break;
		case menuEvent:
		switch ( pEvent->data.menu.itemID ) 
			{
				case mnuGraphMakeNeat:
					nodeManager.straightenNode( globalSelectedMilestone );
				break;
			}	
		break;
		default:   break;
	}
	return handled;
}


/**
 * Displays more employee's information.
 * Linked from either Main > Select a Project > ProjectMenu > Select an Employee,
 * Or Linked from Main > Select an Employee.
 * 
 */
static Boolean frmProjectEmployeeHandleEvent( EventPtr ep )
{
	Err error = errNone;
	Boolean handled = false;
	FormPtr ptrForm;
	FieldType *field;
	AddrLookupParamsType addrparam;
	AddressLookupFields alf;
	Boolean clip;
	Char **ptrString;
	UInt32 * ptrUInt32;
	UInt16 idxList = 0, rowcount = 0, emprowcount = 0;
	UInt32 empid = 0;
	MemHandle _handle;
	structMilestone *stumi, *stume;
	structEmployee *stuem;
	ListType *ptrList;
	int listCount = 0;
	HostTraceInit();
	
	switch ( ep->eType )
	{
		case lstSelectEvent:  // 12
			HostTraceOutputTL(appErrorClass, "lstSelectEvent enter" );
			lstEmployeeTableSelectEvent( ep );	
			HostTraceOutputTL(appErrorClass, "lstSelectEvent enter" );
			handled = true;		
		break;
		case frmOpenEvent:			
			ptrForm = FrmGetActiveForm();		
					
			rowcount = DmNumRecords( globalOpenRefMilestone );
			emprowcount = DmNumRecords( globalOpenRefEmployee );
			
			FrmCopyTitle ( ptrForm, globalEmployeeSelection );	
			// H A N D L E   D I F F E R E N T   R E F E R R I N G   S O U R C E   F O R M S
			//if ( globalEmployeeFORMSELECTED != FORM_EMPLOYEE )
			//	FrmHideObject( ptrForm, FrmGetObjectIndex( ptrForm, popProjectEmployeePulldown ) );
			//else
			//	FrmShowObject( ptrForm, FrmGetObjectIndex( ptrForm, popProjectEmployeePulldown ) );		
			
			// Loop through the employee database and get the UniqueID.	
			rowcount = DmNumRecords( globalOpenRefEmployee );			
			for( int symbolicID = 0; symbolicID < rowcount; symbolicID++ )
			{
				_handle  = DmQueryRecord( globalOpenRefEmployee, symbolicID  );
				if ( _handle )
				{
					stuem = (structEmployee*) MemHandleLock( _handle );
					if ( StrCompare( stuem->fname, globalEmployeeSelection ) == 0 )
					{
						empid = stuem->id;		
						HostTraceOutputTL(appErrorClass, "uniqueID");			
					}

					MemPtrUnlock( (MemPtr*)stuem );
				}
			}
			
			// Count the number of Milestones assigned to the Employee
			rowcount = DmNumRecords( globalOpenRefMilestone );			
			for( int i = 0; i < rowcount; i++ )
			{
				_handle  = DmQueryRecord( globalOpenRefMilestone, i  );
				if ( _handle )
				{
					stumi = (structMilestone*) MemHandleLock( _handle );	
															
					if (  empid == stumi->emp1 )
					{
						HostTraceOutputTL(appErrorClass, "empid %lu", stumi->emp1 );
						listCount ++;
						HostTraceOutputTL(appErrorClass, "listCount %ui", listCount );
					}				
					error = MemPtrUnlock( (MemPtr*) stumi );						
				}
			}		
				
			// Display a Filtered  List to show only this employee's milestones...
			ptrString = (Char ** ) MemPtrNew( listCount * sizeof( Char* ));	
			ptrUInt32 = new UInt32[ listCount  ];
			HostTraceOutputTL(appErrorClass, "StrCopy" );								
			globalListProjectEmployee = ptrString;						
			globalListProjectEmployeeUInt32 = ptrUInt32;
			
			listCount = 0;
			for( int symbolicID = (rowcount - 1); symbolicID >= 0; symbolicID-- )
			{
				_handle  = DmQueryRecord( globalOpenRefMilestone, symbolicID  );
				if ( _handle )
				{
					stume = (structMilestone*) MemHandleLock( _handle );	
					// Loop through the milestone database and compare its emp1 to the UniqueID.			
					if( stume->emp1 == empid )	
					{							
						ptrUInt32[ listCount ] = stume->id;
						ptrString[ listCount ] = (Char*) MemPtrNew( StrLen( stume->name ) + 1 );								
						StrCopy( ptrString[listCount], stume->name ); 	
						HostTraceOutputTL(appErrorClass, "StrCopy" );	
						listCount++;	
					}			 											
						MemPtrUnlock( (MemPtr*)stume );
				}
			}					
			globalListProjectEmployeeCount = listCount;
			
				idxList	= FrmGetObjectIndex( ptrForm, lstProjectEmployee );
				ptrList = ( ListType * )  FrmGetObjectPtr( ptrForm, idxList  );
				LstSetListChoices( ptrList, ptrString, listCount );	
		
			// - - - - - - - - - - - - - - - - - - - 
			//HostTraceClose();	
			
			FrmDrawForm(ptrForm);
			handled = true;
		break;
		case popSelectEvent:
			ptrForm = FrmGetActiveForm();
			FrmShowObject( ptrForm, FrmGetObjectIndex( ptrForm, frmProjectEmployeeTestButton ) );
		break;
		case ctlSelectEvent:
			switch (ep->data.ctlSelect.controlID ) 
			{
				case frmProjectEmployeeBack:
					if ( globalEmployeeFORMSELECTED == FORM_PROJECTEMPLOYEE )
						FrmGotoForm( frmProjectEmployeeList );
					else
						FrmGotoForm( frmEmployeeList );
				break;
				case frmProjectEmployeeTestButton:
					ptrForm = FrmGetActiveForm();
					field = (FieldType * ) FrmGetObjectPtr( ptrForm, FrmGetObjectIndex( ptrForm, frmProjectEmployeeFieldType ) );
					PhoneNumberLookup( field );					
				break;
				case btnProjectEmployeeHome:
					FrmGotoForm( frmMainMenu );
				break;
				case btnProjectEmployeePrjList:
					FrmGotoForm( frmProjectList );
				break;
				case btnProjectEmployeePrjMenu:
					FrmGotoForm( frmProjectMenu );
				break;				
			}
		break;
		case frmCloseEvent:
			HostTraceOutputTL(appErrorClass, "frmCloseEvent begin" );
			if( globalListProjectEmployee )
			{
				UInt16 i;
				for( i = 0; i < globalListProjectEmployeeCount; i++ )
				{
					if ( globalListProjectEmployee[i] != NULL )
					{
						error = MemPtrFree((MemPtr) globalListProjectEmployee[i]);
					}
				}
          		error = MemPtrFree((MemPtr) globalListProjectEmployee);
          		globalListProjectEmployee = NULL;
          		globalListProjectEmployeeCount = 0;
          		
			}	
			
			if ( globalListProjectEmployeeUInt32 )
			{
				delete globalListProjectEmployeeUInt32;	
				globalListProjectEmployeeUInt32 = NULL;
			}
			HostTraceOutputTL(appErrorClass, "frmCloseEvent end" );
		break;
		default:
		break;
	}
	HostTraceClose();	
	return handled;
}


static Boolean frmMilestoneInfoHandleEvent( EventPtr ep )
{
	Err error = errNone;
	Boolean handled = false;
	FormPtr ptrForm;
	MemHandle h, h2, oldText, packedH;
	MemPtr memptr, newRecPtr;
	Char * charptr = 0x0, * c = 0x0;

	structMilestone * stctMile, newMile, * ptrMile;
	UInt16 recidx = globalMilestoneRecID;
	UInt16  riattb;
	UInt32 riunique, some32 = 0;
	LocalID rilocalid;
	
	//-----------------------------
	UInt16 length;
	UInt16 SelectedIndex = 0;
	UInt16 SelectedID = 0;
	
	ptrForm = FrmGetActiveForm();
	
	switch ( ep->eType )
	{
		case frmOpenEvent:					
			// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
			error = DmFindRecordByID( globalOpenRefMilestone, globalMilestoneID, &globalMilestoneRecID );
			packedH = DmGetRecord( globalOpenRefMilestone, globalMilestoneRecID );
			stctMile = ( structMilestone * ) MemHandleLock( packedH );
			
							newMile.id = (*stctMile).id;
							newMile.pid = (*stctMile).pid;
							StrCopy( newMile.name, (*stctMile).name );
							StrCopy( newMile.desc, (*stctMile).desc );
							newMile.xpos = (*stctMile).xpos;
							newMile.ypos = (*stctMile).ypos;
							newMile.icon = (*stctMile).icon;
			
			globalMilestoneDescField = (FieldType * ) FrmGetObjectPtr( ptrForm, FrmGetObjectIndex( ptrForm, txtMilestoneInfoDesc ) );
			ChangeLabel( lblMilestoneInfoDlgName, globalMilestoneName );
			FldSetText( globalMilestoneDescField, packedH, OffsetOf(structMilestone, desc), StrLen( (*stctMile).desc ) + 1 );
			MemHandleUnlock( packedH );
			// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
			
			
			FrmSetControlGroupSelection( ptrForm, 1, newMile.icon );
			
			FrmDrawForm(ptrForm);
			handled = true;
		break;
		case ctlSelectEvent:
			switch ( ep->data.ctlSelect.controlID  )
			{
				case btnMilestoneInfoOK:	
					FrmGotoForm( frmGraph );
				break;	
				case btnMilestoneInfoCancel:
				break;
				case 1202:
				//FrmSetControlGroupSelection( ptrForm, 1, 1202 );
				break;
				case 1203:
				//FrmSetControlGroupSelection( ptrForm, 1, 1201 );
				break;
				case 1201:
				//FrmSetControlGroupSelection( ptrForm, 1, 1202 );
				break;
				default:
					length = 99;
			}
		break;
		case frmCloseEvent:
				SelectedIndex = FrmGetControlGroupSelection ( ptrForm, 1 );
				SelectedID = FrmGetObjectId( ptrForm, SelectedIndex );
				FldCompactText( globalMilestoneDescField );					
				FldSetTextHandle( globalMilestoneDescField, NULL );
				DmReleaseRecord(globalOpenRefMilestone, globalMilestoneRecID, true );
				
				h = DmGetRecord( globalOpenRefMilestone, globalMilestoneRecID );
				memptr =  MemHandleLock( h );
				ptrMile = ( structMilestone * ) memptr;
				newMile.icon = SelectedID;
				error = DmWrite( memptr, OffsetOf( structMilestone, icon ), (& newMile.icon), sizeof( newMile.icon ) );
				error = DmReleaseRecord(globalOpenRefMilestone, globalMilestoneRecID, true );
				MemHandleUnlock( h );
		break;	
	}	
	return handled;	
}

/***********************************************************************
 * FUNCTION:    MainFormDoCommand
 * DESCRIPTION: This routine performs the menu command specified.
 * PARAMETERS:  command  - menu item id
 * RETURNED:    nothing
 ***********************************************************************/
static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;
	FormType * pForm;

	switch (command) {
		case MainOptionsAboutStarterApp:
			pForm = FrmInitForm(AboutForm);
			FrmDoDialog(pForm);					// Display the About Box.
			FrmDeleteForm(pForm);
			handled = true;
			break;
		case frmMainMenuResetDatabase:
			myDatabase.deleteNodeDB();
			globalOpenRefMilestone = myDatabase.createNodeDB( globalOpenRefMilestone, nodeManager, globalEmpID );
			break;
	}
	return handled;
}

/***********************************************************************
 * FUNCTION:    MainFormMenuEvent
 * DESCRIPTION: This routine performs Control handling.
 * PARAMETERS:  pEvent - the event to process.
 * RETURNED:    true if the event was handled.
 ***********************************************************************/
static Boolean MainFormCtlSelectEvent(EventType *pEvent)
{
	Boolean handled = false;
	Err error = errNone;
   	UInt16 versionP;
   	UInt32 crDateP;
   	UInt32 romSizeP;
   	UInt32 ramSizeP;
   	UInt32 freeBytesP;  	
   	LocalID localId;
   	Char * found = "Found";
   	Char * none = "None";

   	
   	// - - - - - - - - - - - - -
   	   	FormGadgetType * fgt = NULL;
   		UInt16 gadgetId = 99, gadgetIndex = 0;
   		Coord x = 0, y = 0, width = 10, height =10, movex = 0, movey = 0;
   		Coord dumx, dumy;
   		Boolean isPenDown = true;
   		RectangleType rectangle;
   	// - - - - - - - - - - - - - 
	
	switch (pEvent->data.ctlSelect.controlID ) {

		case btnViewAllProjects:	
			FrmGotoForm(  frmProjectList );
			handled = true;
			break;
			
		case btnViewAllEmployees:
			FrmGotoForm( frmEmployeeList );
			handled = true;
		break;
			
		case btnBuildNewProject:
				FrmGotoForm( frmProjectNew );	
				handled = true;		
			break;
	}
	return handled;
}

/***********************************************************************
 * FUNCTION:    MainFormHandleEvent
 * DESCRIPTION: This routine is the event handler for the 
 *              "MainForm" of this application.
 * PARAMETERS:  pEvent  - a pointer to an EventType structure
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 ***********************************************************************/
static Boolean MainFormHandleEvent(EventType* pEvent)
{
	Boolean handled = false;
	FormType * thisForm;	
	FormGadgetType * gadget = 0x0;
	int num = 0;

	switch (pEvent->eType) 
	{		
		case frmOpenEvent:
			thisForm = FrmGetActiveForm();
			FrmDrawForm(thisForm);
			handled = true;
			break;			
		case ctlSelectEvent:
			thisForm = FrmGetActiveForm();
			handled = MainFormCtlSelectEvent(pEvent);
			break;				
		case menuEvent:
			handled = MainFormDoCommand(pEvent->data.menu.itemID);	
			break;			
		case frmSaveEvent:
			break;				
		case frmCloseEvent:
			break;
		default:
			break;
	}
	
	return handled;
}

/**
 * Invoked from form Project Menu.
 */
static Boolean frmProjectEmployeeListHE( EventType * et )
{
	Err error = errNone;
	Char * errorChar = NULL;
	Boolean handled = false;
	FormType * ptrForm;	
	UInt16 idxList = 0, rowcount = 0;
	int listCount = 0, listCount2;
	Char ** ptrString;
	MemHandle _handle;
	structEmployee * stuce;
	ListType * ptrList;
	ptrForm = FrmGetFormPtr( frmProjectEmployeeList );	
	
	switch( et->eType )
	{
		case lstSelectEvent:  // 12
			// the user clicked on an employee's name.
			globalEmployeeFORMSELECTED = FORM_PROJECTEMPLOYEE;
			frmProjectEmployeeListTableSelectEvent( et );	
			handled = true;		
		break;
		case frmOpenEvent:
			HostTraceInit();
			ptrForm = FrmGetFormPtr( frmProjectEmployeeList );		
			rowcount = DmNumRecords( globalOpenRefEmployee );							
			
			for( int i = 0; i < rowcount; i++ )
			{
				_handle  = DmQueryRecord( globalOpenRefEmployee, i  );
				if ( _handle )
				{
					stuce = (structEmployee*) MemHandleLock( _handle );											
					if ( globalProjectIDSelected == stuce->pid )
					{
						listCount ++;
					}
					
					error = MemPtrUnlock( (MemPtr*)stuce );						
				}
			}
					
			ptrString = (Char ** ) MemPtrNew( listCount * sizeof( Char* ));								
			globalProjectEmployeeList = ptrString;						
			
			listCount = 0;
			for( int symbolicID = (rowcount - 1); symbolicID >= 0; symbolicID-- )
			{
				_handle  = DmQueryRecord( globalOpenRefEmployee, symbolicID  );
				if ( _handle )
				{
					stuce = (structEmployee*) MemHandleLock( _handle );				
					HostTraceOutputTL(appErrorClass, stuce->fname );
					if( stuce->pid == globalProjectIDSelected )	
					{							
						ptrString[ listCount ] = (Char*) MemPtrNew( StrLen( stuce->fname ) + 1 );								
						StrCopy( ptrString[listCount], stuce->fname ); 		

						listCount++;	
					}			 											
						MemPtrUnlock( (MemPtr*)stuce );
				}
			}					
			globalProjectEmployeeListCount = listCount;
			
				idxList	= FrmGetObjectIndex( ptrForm, frmProjectEmployeeListList );
				ptrList = ( ListType * )  FrmGetObjectPtr( ptrForm, idxList  );
				LstSetListChoices( ptrList, ptrString, listCount );				
						
			FrmDrawForm( ptrForm );
			handled = true;	
			HostTraceClose();
		break;
		case ctlSelectEvent:
			switch (et->data.ctlSelect.controlID ) 
			{
				case frmProjectEmployeeListBack:
					FrmGotoForm( frmProjectMenu );
				break;
				case frmProjectEmployeeListGotoList:
					FrmGotoForm( frmProjectList );
				break;
				case frmProjectEmployeeListHome:
					FrmGotoForm( frmMainMenu );
				break;
			}
		break;
		case frmCloseEvent:
			if( globalProjectEmployeeList )
			{
				UInt16 i;
				for( i = 0; i < globalProjectEmployeeListCount; i++ )
				{
					if ( globalProjectEmployeeList[i] != NULL )
						error = MemPtrFree((MemPtr) globalProjectEmployeeList[i]);
				}
          		error = MemPtrFree((MemPtr) globalProjectEmployeeList);
          		globalProjectEmployeeList = NULL;
          		globalProjectEmployeeListCount = 0;
			}
		break;
		
		default:
		break;	
	}
	return handled;
}


/**
 * Invoked from MainFormCtlSelectEvent( )
 */
static Boolean frmProjectListHandleEvent( EventType * et )
{
	Boolean handled = false;
	Err error = errNone;
	UInt16 idxList = 0, rowcount = 0;
	MemHandle _handle;
	ListType * ptrList;
	Char ** ptrString;
	structProject * stucp;
	
	FormType * ptrForm = FrmGetFormPtr( frmProjectList );	

	switch( et->eType )
	{
		case lstSelectEvent:  // 12
			frmProjectListTableSelectEvent( et );	
			handled = true;		
		break;
		case frmOpenEvent:	// 24	
			HostTraceInit( );
			
			rowcount = DmNumRecords( globalOpenRefProject );
			ptrString = (Char **) MemPtrNew( rowcount * sizeof(Char*) );
			
			// point to the pointer globally in order to delete later
			globalProjectList = ptrString;			
			globalProjectListCount = rowcount;
			for( int symbolicID = 0; symbolicID < rowcount; symbolicID++ )
			{
				_handle  = DmQueryRecord( globalOpenRefProject, symbolicID  );
				if ( _handle )
				{
					stucp = (structProject*) MemHandleLock( _handle );
					ptrString[ symbolicID ] = (Char*) MemPtrNew( StrLen( stucp->name ) + 1 );
					StrCopy( ptrString[symbolicID], stucp->name );
					MemPtrUnlock( (MemPtr*)stucp );
				}
			}
	
			idxList	= FrmGetObjectIndex( ptrForm, frmProjectListList );
			ptrList = ( ListType * )  FrmGetObjectPtr( ptrForm, idxList  );		
			LstSetListChoices( ptrList, ptrString, rowcount );		
			
			FrmDrawForm( ptrForm );
			handled = true;	
			HostTraceOutputTL(appErrorClass, " --- Project List OK  --- ");
			HostTraceClose();
		break;			
		case ctlSelectEvent:
			frmProjectListCtlSelectEvent( et );
			handled = true;
		break;			
		case frmCloseEvent:
			// free the memory allocated in frmOpenEvent

			if( globalProjectList )
			{
				UInt16 i;
				for( i = 0; i < globalProjectListCount; i++ )
				{
					if ( globalProjectList[i] != NULL )
						error = MemPtrFree((MemPtr) globalProjectList[i]);
				}
          		error = MemPtrFree((MemPtr) globalProjectList);
          		globalProjectList = NULL;
          		globalProjectListCount = 0;
			}
			
			handled = false;
		break;			
		default:  // 1, 2, 5, 11
		break;		
	} 

	return handled;
}



static Boolean frmEmployeeListHandleEvent( EventType * et )
{
	Err error = errNone;
	Boolean handled = false;
	FormType * ptrForm = FrmGetFormPtr( frmEmployeeList );
	UInt16 idxList = 0, rowcount = 0;
	Char ** ptrString;
	MemHandle _handle;
	structEmployee * stuce;
	ListType * ptrList;
	int listcount = 0;
	
	switch( et->eType )
	{
		case lstSelectEvent:  // 12
			globalEmployeeFORMSELECTED = FORM_EMPLOYEE;
			frmEmployeeListTableSelectEvent( et );	
			handled = true;		
		break;
		case frmOpenEvent:	// 24	
			rowcount = DmNumRecords( globalOpenRefEmployee );
			ptrString = (Char **) MemPtrNew( rowcount * sizeof(Char*));
			globalEmployeeList = ptrString;			
			globalEmployeeListCount = rowcount;
			listcount = 0;
			for( int symbolicID = 0; symbolicID < rowcount; symbolicID++ )
			{
				_handle  = DmQueryRecord( globalOpenRefEmployee, symbolicID  );
				if ( _handle )
				{
					stuce = (structEmployee*) MemHandleLock( _handle );
					ptrString[ symbolicID ] = (Char*) MemPtrNew( StrLen( stuce->fname ) + 1 );
					StrCopy( ptrString[symbolicID], stuce->fname );
					MemPtrUnlock( (MemPtr*)stuce );
				}
			}
	
			idxList	= FrmGetObjectIndex( ptrForm, frmEmployeeListList );
			ptrList = ( ListType * )  FrmGetObjectPtr( ptrForm, idxList  );		
			LstSetListChoices( ptrList, ptrString, rowcount );		
		
			FrmDrawForm( ptrForm );
			handled = true;	
		break;			
		case ctlSelectEvent:
			frmEmployeeListCtlSelectEvent( et );
			handled = true;
		break;			
		case frmCloseEvent:
			// free the memory allocated in frmOpenEvent
			if( globalEmployeeList )
			{
				UInt16 i;
				for( i = 0; i < globalEmployeeListCount; i++ )
				{
					if ( globalEmployeeList[i] != NULL )
						error = MemPtrFree((MemPtr) globalEmployeeList[i]);
				}
          		error = MemPtrFree((MemPtr) globalEmployeeList);
          		globalEmployeeList = NULL;
          		globalEmployeeListCount = 0;
			}
			
			handled = false;
		break;			
		default:  // 1, 2, 5, 11
		break;		
	} 

	return handled;
}


static Boolean frmProjectMenuHandleEvent( EventPtr  et )
{
	Boolean handled = false;
	FormType * ptrForm;
	FormType * thisForm;
	Int16 SELECTED_ROW = globalProjectListRow;
	UInt16 idxObject = 0;
	Char * dummyC = NULL;
	MemHandle mh = NULL, _handle = NULL;
	UInt16 rowcount = 0;
	structMilestone * stctMilestone = NULL;
	Int16 projID = globalProjectIDSelected;

	switch( et->eType )
	{
		case frmOpenEvent: //24
			ptrForm = FrmGetFormPtr( frmProjectMenu );			
			FrmCopyTitle ( ptrForm, globalProjectListSelection );
			rowcount = DmNumRecords( openRefMilestone ); 
			globalAchtungListCount = 0;
			
			// Search for any milestones tagged as Achtung.
			for( int symbolicID = 0; symbolicID < rowcount; symbolicID++ )
			{
				// for each node in the database
				_handle  = DmQueryRecord( openRefMilestone, symbolicID  );
				if ( _handle )
				{
					stctMilestone = ( structMilestone * ) MemHandleLock( _handle );
							
					if ( stctMilestone->pid == projID )					
						if ( stctMilestone->icon == 1201 )					
							globalAchtungListCount ++;	
																												
					MemHandleUnlock( _handle );				
				}
			}
			
			// Display the Achtung icon.
			if ( globalAchtungListCount == 0 )
				FrmHideObject( ptrForm, FrmGetObjectIndex( ptrForm, btnAchtungList ) );
			else
				FrmShowObject( ptrForm, FrmGetObjectIndex( ptrForm, btnAchtungList ) );
			
			FrmDrawForm ( ptrForm );
			handled = true;
		break;	
		case ctlSelectEvent:
			switch( et -> data.ctlSelect.controlID )
			{
				case btnProjectMenuBack:
					FrmGotoForm( frmProjectList );
					handled = true;
				break;				
				case btnProjectMenuStatus:
					FrmGotoForm( frmGraph );
					handled = true;
				break;
				case btnProjectMenuBudget:
					FrmGotoForm( frmBudgetMenu );
					handled = true;
				break;	
				case btnProjectMenuHome:
					FrmGotoForm( frmMainMenu );
					handled = true;
				break;
				case btnAchtungList:
					FrmGotoForm( frmAchtungList );
					handled = true;
				break;
				case btnProjectMenuEmployees:
					FrmGotoForm( frmProjectEmployeeList );
				break;
				default:
					break;
			} 	
		break;
		case penDownEvent:			
		break;
		case frmCloseEvent:
			idxObject = idxObject;					
		default: // 5
			break;		
	} 
	return handled;
}

static void assignEmployeeToMilestone()
{


	
}


/***********************************************************************
 * FUNCTION:    AppHandleEvent
 * DESCRIPTION: This routine loads form resources and set the event
 *              handler for the form loaded.
 * PARAMETERS:  event  - a pointer to an EventType structure
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 * REVISION HISTORY:
 ***********************************************************************/
static Boolean AppHandleEvent(EventType* pEvent)
{
	UInt16 		formId;
	FormType* 	pForm;
	Boolean		handled = false;

	if (pEvent->eType == frmLoadEvent) {
		// Load the form resource.
		formId = pEvent->data.frmLoad.formID;	
		pForm = FrmInitForm(formId);
		FrmSetActiveForm(pForm);

		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId) {
			case MainForm:
				FrmSetEventHandler( pForm, MainFormHandleEvent );
				break;
			case frmProjectList:			
				FrmSetEventHandler( pForm, frmProjectListHandleEvent );				
				break;
			case frmProjectEmployeeList:
				FrmSetEventHandler( pForm, frmProjectEmployeeListHE );
				break;
			case frmEmployeeList:			
				FrmSetEventHandler( pForm, frmEmployeeListHandleEvent );				
				break;
			case frmProjectMenu:
				FrmSetEventHandler( pForm, frmProjectMenuHandleEvent );
				break;
			case frmGraph:
				FrmSetEventHandler( pForm, frmGraphHandleEvent );					
				break;							
			case frmAchtungList:
				FrmSetEventHandler( pForm, frmAchtungListHandleEvent );
				break;
			case frmMilestoneInfoDlg:
				FrmSetEventHandler( pForm, frmMilestoneInfoHandleEvent );
				break;
			case frmProjectEmployee:
				FrmSetEventHandler( pForm, frmProjectEmployeeHandleEvent );
				break;
				
			default:
				break;
		}
		handled = true;
	}
	
	return handled;
}


/***********************************************************************
 * FUNCTION:     AppStart
 * DESCRIPTION:  Get the current application's preferences.
 * PARAMETERS:   nothing
 * RETURNED:     Err value errNone if nothing went wrong
 ***********************************************************************/
static Err AppStart(void)
{
	Err error = errNone;		
	LocalID localProjectID = 0;
	LocalID localMilestoneID = 0;
	LocalID localEmployeeID = 0;
	DmOpenRef doro = globalOpenRefMilestone;
	DmOpenRef dor = globalOpenRefMilestone;
	DmOpenRef dor3 = globalOpenRefEmployee;
	
	localProjectID = DmFindDatabase( 0, "projectMND0" );
	if ( localProjectID == 0 )
		globalOpenRefProject = myDatabase.createProjectDB( globalOpenRefProject );
	else
		globalOpenRefProject = myDatabase.openProjectDatabase( globalOpenRefProject );		

	localEmployeeID = DmFindDatabase( 0, "employeeMND0" );
	
	if ( localEmployeeID == 0 )
		globalOpenRefEmployee = myDatabase.createEmployeeDB( globalOpenRefEmployee, globalEmpID );
	else
		globalOpenRefEmployee = myDatabase.openEmployeeDatabase( globalOpenRefEmployee );


	localMilestoneID = DmFindDatabase( 0, "milestoneMND0" );
	if ( localMilestoneID == 0 )
		globalOpenRefMilestone = myDatabase.createNodeDB( globalOpenRefMilestone, nodeManager, globalEmpID );
	else
		globalOpenRefMilestone = myDatabase.openDatabase(globalOpenRefMilestone, nodeManager ); 
	

	FrmGotoForm(MainForm);
	return error;
}


/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppStop(void)
{
	Err error = errNone;
	
	if ( memHandle )
		error = MemHandleFree( memHandle );
	
	// Close all the open forms.
	FrmCloseAllForms();
	
	if ( globalMilestoneName != NULL )
	{
		delete [] globalMilestoneName;	
		globalMilestoneName = NULL;
	}	
									
	if ( globalMilestoneDesc != NULL )
	{
		delete [] globalMilestoneDesc;	
		globalMilestoneDesc = NULL;
	}
	
	if ( globalProjectListSelection )
	{
		delete [] globalProjectListSelection;
		globalProjectListSelection = NULL;
	}
	
	if ( globalEmployeeSelection )
	{
		delete [] globalEmployeeSelection;
		globalEmployeeSelection = NULL;
	}
	
	if ( globalListProjectEmployeeUInt32 )
	{
		HostTraceOutputTL(appErrorClass, "delete globalListProjectEmployeeUInt32" );
		delete [] globalListProjectEmployeeUInt32;
		globalListProjectEmployeeUInt32 = NULL;	
	}
	
	if ( globalAchtungListUInt32 )
	{
		delete [] globalAchtungListUInt32;
		globalAchtungListUInt32 = NULL;	
	}
	
	if ( globalOpenRefProject )
		error = DmCloseDatabase ( globalOpenRefProject );
		
	if ( globalOpenRefMilestone )	
		error = DmCloseDatabase ( globalOpenRefMilestone );
		
	if ( globalOpenRefEmployee )
		error = DmCloseDatabase ( globalOpenRefEmployee );
}


/***********************************************************************
 *
 * FUNCTION:    AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void AppEventLoop(void)
{
	Err			error;
	EventType	event;

	do {
		EvtGetEvent(&event, evtWaitForever);

		if (SysHandleEvent(&event))
			continue;
			
		if (MenuHandleEvent(0, &event, &error))
			continue;
			
		if (AppHandleEvent(&event))
			continue;

		FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}


/***********************************************************************
 * FUNCTION:    PilotMain
 * DESCRIPTION: This is the main entry point for the application.
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 * RETURNED:    Result of launch
 ***********************************************************************/
UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	Err error = errNone;

	switch (cmd) {
		case sysAppLaunchCmdNormalLaunch:
			if ((error = AppStart()) == 0) {			
				AppEventLoop();
				AppStop();
			}
			break;

		default:
			break;
	}
	
	return error;
}

void ChangeLabel(UInt16 labelID, const Char *newLabel)
{
	FormPtr frm = FrmGetActiveForm();
	UInt16	labelIndex = FrmGetObjectIndex(frm, labelID);
	
	//Hide the label first to erase old text
	FrmHideObject(frm, labelIndex);
	//Copy in the new string
	FrmCopyLabel(frm, labelID, newLabel);
	//Now force the redraw
	FrmShowObject(frm, labelIndex);
}

void ChangeMyLabel(FormPtr frm, UInt16 labelID, const Char *newLabel)
{
	UInt16	labelIndex = FrmGetObjectIndex(frm, labelID);
	
	//Hide the label first to erase old text
	FrmHideObject(frm, labelIndex);
	//Copy in the new string
	FrmCopyLabel(frm, labelID, newLabel);
	//Now force the redraw
	FrmShowObject(frm, labelIndex);
}

bool isClickInRectangle( const RectangleType * rt, Int16 _x, Int16 _y )
	{
		//These conditional statements verify the occupation of
		//the Bottom Right corner.
		if( _y < rt->topLeft.y )
			return false;
		if( _y > (rt->topLeft.y + rt->extent.y) )
			return false;
		if( _x < rt->topLeft.x )
			return false;
		if( _x > (rt->topLeft.x + rt->extent.x) )
			return false;
		return true;	
	}


