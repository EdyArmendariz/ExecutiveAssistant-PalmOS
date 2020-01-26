
#ifndef Database_h
#define Database_h
#include "Task.h"

typedef struct DBRecordType
{
	int recordnumber;
	
}DBRecordType;

class Database
{
	private:
	DmOpenRef openRef;
	LocalID dbProjectID;
	LocalID dbID;
	
	public:
	Database()
	{
		
	}
	
	/**
	 * Opens the Application's Project database.
	 * Returns true if the database was opened succesfully,
	 * false otherwise.
	 */
	DmOpenRef openProjectDatabase(  DmOpenRef dor )
	{
		bool b = false;
		openRefProject = dor;
		DmOpenRef ptrTemp;
		
		dbProjectID = DmFindDatabase(0,"projectMND0");
		
		if( dbProjectID < 0 )
		{
			b = false;
		}
		else 
		{	
			ptrTemp = DmOpenDatabase( 0, dbProjectID, dmModeReadWrite );	
		
			if( ptrTemp == NULL )
			{
				if( DmGetLastErr() == dmErrDatabaseOpen )
					b = true;
				else
				{
					b = false;
				}
			}
			else
			{
				openRefMilestone = ptrTemp;
				b = true;
			}								
		}
		return openRefMilestone;
	}
	
	/**
	 * Opens the Application database.
	 * Returns true if the database was opened succesfully,
	 * false otherwise.
	 */
	DmOpenRef openDatabase(  DmOpenRef dor, NodeManager nm )
	{
		bool b = false;
		openRefMilestone = dor;
		DmOpenRef ptrTemp;
		
		dbID = DmFindDatabase(0,"milestoneMND0");
		
		if( dbID < 0 )
		{
			b = false;
		}
		else 
		{	
			ptrTemp = DmOpenDatabase( 0, dbID, dmModeReadWrite );	
		
			if( ptrTemp == NULL )
			{
				if( DmGetLastErr() == dmErrDatabaseOpen )
					b = true;
				else
				{
					b = false;
				}
			}
			else
			{
				this->openRef = ptrTemp;
				openRefMilestone = ptrTemp;
				b = true;
			}								
		}
		return openRefMilestone;
	}
	
	DmOpenRef openEmployeeDatabase(  DmOpenRef dor )
	{
		bool b = false;
		openRefEmployee = dor;
		DmOpenRef ptrTemp;
		
		dbID = DmFindDatabase(0,"employeeMND0");
		
		if( dbID < 0 )
		{
			b = false;
		}
		else 
		{	
			ptrTemp = DmOpenDatabase( 0, dbID, dmModeReadWrite );	
		
			if( ptrTemp == NULL )
			{
				if( DmGetLastErr() == dmErrDatabaseOpen )
					b = true;
				else
				{
					b = false;
				}
			}
			else
			{
				this->openRef = ptrTemp;
				openRefEmployee = ptrTemp;
				b = true;
			}								
		}
		return openRefEmployee;
	}
	
	
	DmOpenRef createProjectDB( DmOpenRef dor )
	{
		Err err;
		Err error = errNone;
		MemHandle h;
		MemPtr p;
		UInt16 index = 0;
		UInt32 uniqueID = 0;
		int PROJECTCOUNT = 5;
		structProject sp;
		Char * projectName [] = {"EP Scheduling", "EP Budgeting", "Global Vista",
								  "Cash Flow", "Wedding"  };
		Int16 projectID [] = { 1, 2, 3, 4, 5 };		
		HostTraceInit();
		
		error = DmCreateDatabase( 0, "projectMND0", 'MND0', 'DATA', false );	
		openRefProject = DmOpenDatabase(0, DmFindDatabase( 0, "projectMND0" ), dmModeReadWrite );

		DefaultAppInfoInit( openRefProject );

		
		// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		for ( int i = 0; i < PROJECTCOUNT; i++ )
		{
			h = DmNewRecord( openRefProject, &index, sizeof( sp ) );
			error = DmRecordInfo( openRefProject, index, NULL, &uniqueID, NULL );
			
			HostTraceOutputTL(appErrorClass, "<project>");
			HostTraceOutputTL(appErrorClass, "    <pid>%hd</pid>", projectID[i] );
			HostTraceOutputTL(appErrorClass, "    <uid>%lu</uid>", uniqueID );
			HostTraceOutputTL(appErrorClass, "    <familiarname>%s</familiarname>", projectName[i]); 
			HostTraceOutputTL(appErrorClass, "</project>" );
		
			
			sp.pid = projectID[i];
			StrCopy( sp.name, projectName[i] );
		
			if ( h ) 
			{ // could fail due to insufficient memory!
        		p = MemHandleLock( h );
        		err = DmWrite( p, 0, &sp, sizeof ( sp ) );
        		MemPtrUnlock(p);
        		// index received from DmNewRecord
        		DmReleaseRecord( openRefProject, index, true );
    		}	
		}
    	
    	HostTraceClose();
    	return openRefProject;
	}
	
	Err DefaultAppInfoInit( DmOpenRef dor )
	{
		// App Info Block
		UInt16  cardNo = 0;
		LocalID  dbID, appInfoID;
		MemHandle hAppInfo;
		DefaultAppInfoType * appInfo;
		// # # # # # # # # # # # # # # # #
		//   A P P    I N F O   B L O C K
		if ( DmOpenDatabaseInfo( dor, &dbID, NULL, NULL, &cardNo, NULL ))
			{
				HostTraceOutputTL( appErrorClass, " DmOpenDatabaseInfo() = %lu", dmErrInvalidParam );
			}
		if ( DmDatabaseInfo( cardNo, dbID, NULL, NULL, NULL,
			NULL, NULL, NULL, NULL, &appInfoID, NULL, NULL, NULL ) )
			{
				HostTraceOutputTL( appErrorClass, " DmDatabaseInfo() = %lu", dmErrInvalidParam );				
			}
		
		if ( appInfoID == NULL )
		{
			hAppInfo = DmNewHandle ( openRefMilestone, sizeof( DefaultAppInfoType ) );
			if ( !hAppInfo )			
				HostTraceOutputTL( appErrorClass, "  DmNewHandle()=%lu", dmErrMemError );	
							
			appInfoID = MemHandleToLocalID( hAppInfo );
			DmSetDatabaseInfo( cardNo, dbID, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &appInfoID, NULL, NULL, NULL );
		}
		
		appInfo = (DefaultAppInfoType*) MemLocalIDToLockedPtr( appInfoID, cardNo );		
		DmSet( appInfo, 0, sizeof( DefaultAppInfoType ), 0 );		
		MemPtrUnlock( appInfo );
	}
	
	DmOpenRef createNodeDB( DmOpenRef dor, NodeManager nm, UInt32 empIDs[] )
	{		
		// Database Stuff
		openRefMilestone = dor;
		Err error = errNone;
		MemHandle h;
		UInt16 index = 0;
		UInt32 uniqueID = 0;
		structMilestone  sM1, sM2, sM3, smgv1, smgv2, smgv3, smgv4;
		structMilestone wedding1, wedding2, wedding3, wedding4, 
						wedding5, wedding6, wedding7;
		
		error = DmCreateDatabase(0, "milestoneMND0", 'MND0', 'DATA', false);
		openRefMilestone = DmOpenDatabase(0, DmFindDatabase(0,"milestoneMND0"), dmModeReadWrite);
		
		DefaultAppInfoInit( openRefMilestone );
		
		// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		//                  C  A  S  H       F  L  O  W 
		// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		h = DmNewRecord( openRefMilestone, &index, sizeof( sM2 ) );
		error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );	

		Char * bbb = "Design";
		Char * bbb2 = "Do some Design.";		
		
		sM2.id = uniqueID;
		sM2.pid = 4;
		sM2.icon = nm.ICONGoodCmplt;
		StrCopy( sM2.name, bbb );
		StrCopy( sM2.desc, bbb2 );
		sM2.xpos = 40;
		sM2.ypos = 80;
		sM2.intTopSib = NO_SIBLING;
		sM2.intBottomSib = NO_SIBLING;
		sM2.intLeftSib = NO_SIBLING;
		sM2.intRightSib = NO_SIBLING;
		
		if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock( h );
        	Err err = DmWrite( p, 0, &sM2, sizeof ( sM2 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	h = DmNewRecord( openRefMilestone, &index, sizeof( sM3 ) );
    	error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );
    	Char * ccc = "Validate and Verify";
    	Char * ccc2 = "Do Val and Verify.";
		
		sM3.id = uniqueID;
		sM3.pid = 4;
		sM3.icon = nm.ICONGoodCmplt;
		StrCopy( sM3.name, ccc );
		StrCopy( sM3.desc, ccc2 );
		sM3.xpos = 70;
		sM3.ypos = 80;
		sM3.emp1 = empIDs[5];
		sM3.intTopSib = NO_SIBLING;
		sM3.intBottomSib = NO_SIBLING;
		sM3.intLeftSib = NO_SIBLING;
		sM3.intRightSib = NO_SIBLING;
		
    	if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock(h);
        	Err err = DmWrite( p, 0, &sM3, sizeof ( sM3 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	h = DmNewRecord( openRefMilestone, &index, sizeof( sM1 ) );
    	error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );
    	Char * aaa = "Gather Reqs.";
    	Char * aaa2 = "Do Gather.";   	
		
		sM1.id = uniqueID;
		sM1.pid = 4;
		sM1.icon = nm.ICONAchtung;
		StrCopy( sM1.name, aaa );
		StrCopy( sM1.desc, aaa2 );
		sM1.xpos = 40;
		sM1.ypos = 100;
		sM1.intTopSib = NO_SIBLING;
		sM1.intBottomSib = NO_SIBLING;
		sM1.intLeftSib = NO_SIBLING;
		sM1.intRightSib = NO_SIBLING;
		
    	if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock(h);
        	Err err = DmWrite( p, 0, &sM1, sizeof ( sM1 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	  	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	nm.setTopSibling( sM1.id, sM2.id );
    	nm.setBottomSibling( sM2.id, sM1.id );
    	nm.setLeftSibling( sM3.id, sM2.id );
    	nm.setRightSibling( sM2.id, sM3.id );
    	
    	// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		//          G  L  O  B  A  L        V  I  S  T  A
		// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		
    	h = DmNewRecord( openRefMilestone, &index, sizeof( smgv1 ) );
		error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );	

		Char * smgv1a = "Server Install";
		Char * smgv1aa = "Install and configure new servers.";		
		
		smgv1.id = uniqueID;
		smgv1.pid = 3;
		//sM2.icon = ICON_GOODCMPLT;
		smgv1.icon = nm.ICONNone;
		StrCopy( smgv1.name, smgv1a );
		StrCopy( smgv1.desc, smgv1aa );
		smgv1.xpos = 40;
		smgv1.ypos = 80;
		smgv1.intTopSib = NO_SIBLING;
		smgv1.intBottomSib = NO_SIBLING;
		smgv1.intLeftSib = NO_SIBLING;
		smgv1.intRightSib = NO_SIBLING;
		
		if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock( h );
        	Err err = DmWrite( p, 0, &smgv1, sizeof ( smgv1 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	h = DmNewRecord( openRefMilestone, &index, sizeof( smgv2 ) );
    	error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );
    	Char * smgv2b = "Data Migration";
    	Char * smgv2bb = "Prepare data mapping UML.";
		
		smgv2.id = uniqueID;
		smgv2.pid = 3;
		smgv2.icon = nm.ICONInProgress;
		StrCopy( smgv2.name, smgv2b );
		StrCopy( smgv2.desc, smgv2bb );
		smgv2.xpos = 70;
		smgv2.ypos = 80;
		smgv2.emp1 = empIDs[6];
		smgv2.intTopSib = NO_SIBLING;
		smgv2.intBottomSib = NO_SIBLING;
		smgv2.intLeftSib = NO_SIBLING;
		smgv2.intRightSib = NO_SIBLING;
		
    	if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock(h);
        	Err err = DmWrite( p, 0, &smgv2, sizeof ( smgv2 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	
    	h = DmNewRecord( openRefMilestone, &index, sizeof( smgv3 ) );
    	error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );
    	Char * smgv3b = "10-99";
    	Char * smgv3bb = "Fix this mess.";   	
		
		smgv3.id = uniqueID;
		smgv3.pid = 3;
		smgv3.icon = nm.ICONAchtung;
		StrCopy( smgv3.name, smgv3b );
		StrCopy( smgv3.desc, smgv3bb );
		smgv3.xpos = 40;
		smgv3.ypos = 100;
		smgv3.emp1 = empIDs[3];
		smgv3.intTopSib = NO_SIBLING;
		smgv3.intBottomSib = NO_SIBLING;
		smgv3.intLeftSib = NO_SIBLING;
		smgv3.intRightSib = NO_SIBLING;
		
    	if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock(h);
        	Err err = DmWrite( p, 0, &smgv3, sizeof ( smgv3 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	nm.setTopSibling( smgv3.id, smgv1.id );
    	nm.setBottomSibling( smgv1.id, smgv3.id );
    	nm.setLeftSibling( smgv2.id, smgv1.id );
    	nm.setRightSibling( smgv1.id, smgv2.id );
    	
    	
    	// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
		//          W  E  D  D  I  N  G
		// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
    	h = DmNewRecord( openRefMilestone, &index, sizeof( wedding1 ) );
		error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );	

		Char * wedding1a = "Guest List";
		Char * wedding1aa = "Prepare the Guest List.";		
		wedding1.id = uniqueID;
		wedding1.pid = 5;
		wedding1.icon = nm.ICONNone;
		StrCopy( wedding1.name, wedding1a );
		StrCopy( wedding1.desc, wedding1aa );
		wedding1.xpos = 30;
		wedding1.ypos = 100;
		wedding1.intTopSib = NO_SIBLING;
		wedding1.intBottomSib = NO_SIBLING;
		wedding1.intLeftSib = NO_SIBLING;
		wedding1.intRightSib = NO_SIBLING;
		
		if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock(h);
        	Err err = DmWrite( p, 0, &wedding1, sizeof ( wedding1 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	
    	h = DmNewRecord( openRefMilestone, &index, sizeof( wedding2 ) );
		error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );	

		Char * wedding2a = "Budget";
		Char * wedding2aa = "Set the Budget Limitations.";		
		wedding2.id = uniqueID;
		wedding2.pid = 5;
		wedding2.icon = nm.ICONNone;
		StrCopy( wedding2.name, wedding2a );
		StrCopy( wedding2.desc, wedding2aa );
		wedding2.xpos = 60;
		wedding2.ypos = 100;
		wedding2.intTopSib = NO_SIBLING;
		wedding2.intBottomSib = NO_SIBLING;
		wedding2.intLeftSib = NO_SIBLING;
		wedding2.intRightSib = NO_SIBLING;
		
		if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock(h);
        	Err err = DmWrite( p, 0, &wedding2, sizeof ( wedding2 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	
    	h = DmNewRecord( openRefMilestone, &index, sizeof( wedding3 ) );
		error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );	

		Char * wedding3a = "Reception";
		Char * wedding3aa = "Locate and Book the Reception Site.";		
		wedding3.id = uniqueID;
		wedding3.pid = 5;
		wedding3.icon = nm.ICONNone;
		StrCopy( wedding3.name, wedding3a );
		StrCopy( wedding3.desc, wedding3aa );
		wedding3.xpos = 30;
		wedding3.ypos = 80;
		wedding3.intTopSib = NO_SIBLING;
		wedding3.intBottomSib = NO_SIBLING;
		wedding3.intLeftSib = NO_SIBLING;
		wedding3.intRightSib = NO_SIBLING;
		
		if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock(h);
        	Err err = DmWrite( p, 0, &wedding3, sizeof ( wedding3 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	
    	h = DmNewRecord( openRefMilestone, &index, sizeof( wedding4 ) );
		error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );	

		Char * wedding4a = "Catering";
		Char * wedding4aa = "Select a Catering service.";		
		wedding4.id = uniqueID;
		wedding4.pid = 5;
		wedding4.icon = nm.ICONNone;
		StrCopy( wedding4.name, wedding4a );
		StrCopy( wedding4.desc, wedding4aa );
		wedding4.xpos = 60;
		wedding4.ypos = 80;
		wedding4.intTopSib = NO_SIBLING;
		wedding4.intBottomSib = NO_SIBLING;
		wedding4.intLeftSib = NO_SIBLING;
		wedding4.intRightSib = NO_SIBLING;
		
		if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock(h);
        	Err err = DmWrite( p, 0, &wedding4, sizeof ( wedding4 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    	
    	h = DmNewRecord( openRefMilestone, &index, sizeof( wedding5 ) );
		error = DmRecordInfo( openRefMilestone, index, NULL, &uniqueID, NULL );	

		Char * wedding5a = "Ceremony";
		Char * wedding5aa = "Reserve a Ceremony location.";		
		wedding5.id = uniqueID;
		wedding5.pid = 5;
		wedding5.icon = nm.ICONNone;
		StrCopy( wedding5.name, wedding5a );
		StrCopy( wedding5.desc, wedding5aa );
		wedding5.xpos = 140;
		wedding5.ypos = 140;
		wedding5.intTopSib = NO_SIBLING;
		wedding5.intBottomSib = NO_SIBLING;
		wedding5.intLeftSib = NO_SIBLING;
		wedding5.intRightSib = NO_SIBLING;
		
		if ( h ) 
		{ // could fail due to insufficient memory!
        	MemPtr p = MemHandleLock(h);
        	Err err = DmWrite( p, 0, &wedding5, sizeof ( wedding5 ) );
        	MemPtrUnlock(p);
        	// index received from DmNewRecord
        	DmReleaseRecord( openRefMilestone, index, true );
    	}
    	
    	// Clipped Code Here
    	
    	return openRefMilestone;
	}
	
	
	DmOpenRef createEmployeeDB( DmOpenRef dor, UInt32 empID [] )
	{
		openRefEmployee = dor;
		Err error = errNone;
		MemHandle h;
		UInt16 index = 0;
		UInt32 uniqueID = 0;
		structEmployee emp;
		structEmployee emp1, emp2, emp3, emp4, emp5, emp6;
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		int EMPCOUNT = 7;
		Char * fnameArray [] = { "Mubarak", "Ed", "Orlando", "Yuri", "Hieu", "David", "Kevin" };
		Char * lnameArray [] = { "Noohu", "Armendariz", "Epting", "Cuervo", "Dang", "Schanker", "Kolpien" };
		Int16 pidArray [] = { 1, 1, 1, 3, 2, 4, 3 }; 
		MemPtr p;
		Err err = errNone;
		
		error = DmCreateDatabase(0, "employeeMND0", 'MND0', 'DATA', false);
		openRefEmployee = DmOpenDatabase(0, DmFindDatabase(0,"employeeMND0"), dmModeReadWrite);
		HostTraceInit();
		
		DefaultAppInfoInit( openRefEmployee );
		HostTraceOutputTL(appErrorClass, "sizeof(structEmployee)=%lu", sizeof(emp));
		HostTraceOutputTL(appErrorClass, "sizeof(UInt32)=%lu", sizeof(UInt32));
		HostTraceOutputTL(appErrorClass, "sizeof(Int16)=%lu", sizeof(Int16));
		HostTraceOutputTL(appErrorClass, "sizeof(Char)=%lu", sizeof(Char));
		//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .		
		for ( int i = 0; i < EMPCOUNT; i++ )
		{
			h = DmNewRecord( openRefEmployee, &index, sizeof( emp ) );
			error = DmRecordInfo( openRefEmployee, index, NULL, &uniqueID, NULL );	
			empID[i] = uniqueID;
			emp.id = uniqueID;
			emp.pid = pidArray[i];
			StrCopy( emp.fname, fnameArray[i] );
			StrCopy( emp.lname, lnameArray[i] );
		
			if ( h ) 
			{
        		p = MemHandleLock(h);
        		err = DmWrite( p, 0, &emp, sizeof ( emp ) );
        		MemPtrUnlock(p);
        		DmReleaseRecord( openRefEmployee, index, true );
    		}	
		}
		
		HostTraceClose();
		return openRefEmployee;   	
	}
	
	Err deleteNodeDB( )
	{
		Err error = errNone;
		LocalID lid;
		lid = DmFindDatabase ( 0, "milestoneMND0" );
		if ( lid != 0 )
			error = DmDeleteDatabase( 0, lid );
		else
			error = DmGetLastErr();
			
		return error;
	}
	
};


#endif // Database_h

