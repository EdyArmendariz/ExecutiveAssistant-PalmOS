

#ifndef NODEMANAGER_H
#define NODEMANAGER_H
#include <PalmOS.h>


DmOpenRef openRefMilestone;

class Line
{
	public:
	int lx, ly, rx, ry;
	
	PointType ptLeft;
	PointType ptRight;	
};

int 	NO_SIBLING = -1;
int 	NODE_WIDTH = 20;
int 	NODE_HEIGHT = 10; 
Int16	ICON_NONE = 1200;
Int16	ICON_GOODCMPLT = 1202;
Int16	ICON_ACHTUNG = 1201;
Int16   ICON_INPROGRESS = 1203;

class Node
{
	public:
	UInt32 id;
	Char * name;
	Char * desc;
	Coord xpos;
	Coord ypos;
	UInt32 intRightSib;
	UInt32 intLeftSib;
	UInt32 intTopSib;
	UInt32 intBottomSib;
	Int8 icon;
	
	Node():xpos(0), ypos(0), intRightSib( NO_SIBLING ), intLeftSib( NO_SIBLING ),
			intTopSib( NO_SIBLING ), intBottomSib( NO_SIBLING ), name( NULL ),
			desc( NULL ), icon( ICON_NONE )
	{
	}
	
	PointType getRightConnector()
	{
		PointType pt;	
		Coord bufmyrx = xpos + NODE_WIDTH;
		Coord bufmyry = ypos + ( NODE_HEIGHT / 2 );	
		pt.x = bufmyrx;
		pt.y = bufmyry;
		return pt;
	}
	
	PointType getRightConnector( structMilestone * sm )
	{
		PointType pt;	
		Coord bufmyrx = sm->xpos + NODE_WIDTH;
		Coord bufmyry = sm->ypos + ( NODE_HEIGHT / 2 );	
		pt.x = bufmyrx;
		pt.y = bufmyry;
		return pt;
	}
	
	PointType getLeftConnector()
	{
		PointType pt;		
		Coord bufmyrx = xpos;
		Coord bufmyry = ypos +  ( NODE_HEIGHT / 2 );
		pt.x = bufmyrx;
		pt.y = bufmyry;	
		return pt;		
	}
	
	PointType getLeftConnector( structMilestone * sm )
	{
		PointType pt;		
		Coord bufmyrx = sm->xpos;
		Coord bufmyry = sm->ypos +  ( NODE_HEIGHT / 2 );
		pt.x = bufmyrx;
		pt.y = bufmyry;	
		return pt;		
	}
	
	PointType getTopConnector( )
	{
		PointType pt;	
		Coord bufmyrx = xpos + ( NODE_WIDTH / 2 );
		Coord bufmyry = ypos;
		pt.x = bufmyrx;
		pt.y = bufmyry;	
		return pt;			
	}
	
	PointType getTopConnector( structMilestone * sm )
	{
		PointType pt;	
		Coord bufmyrx = sm->xpos + ( NODE_WIDTH / 2 );
		Coord bufmyry = sm->ypos;
		pt.x = bufmyrx;
		pt.y = bufmyry;	
		return pt;	
	}
	
	PointType getBottomConnector()
	{
		PointType pt;	
		Coord bufmyrx = xpos + 10;// ( NODE_WIDTH / 2 );
		Coord bufmyry = ypos + NODE_HEIGHT;
		pt.x = bufmyrx;
		pt.y = bufmyry;
		return pt;			
	}
	
	PointType getBottomConnector( structMilestone * sm )
	{
		PointType pt;	
		Coord bufmyrx = sm->xpos + 10;// ( NODE_WIDTH / 2 );
		Coord bufmyry = sm->ypos + NODE_HEIGHT;
		pt.x = bufmyrx;
		pt.y = bufmyry;
		return pt;			
	}
	
	void setRightSib( Node * n )
	{
		if ( n != 0x0 )
		{
			intRightSib = n->id;
			//rightSib = n;	
		}
	}
	
	void setLeftSib( Node * n )
	{
		if ( n != 0x0 )
		{
			intLeftSib = n->id;
		}	
	}
	
	void setTopSib ( Node * n )
	{
		if ( n != 0x0 )
		{
			intTopSib = n->id;
		}	
	}
	
	
	void setBottomSib ( Node * n )
	{
		if ( n != 0x0 )
		{
			intBottomSib = n->id;
		}	
	}
	
	void removeRightSib( )
	{
		intRightSib = NO_SIBLING;
	}
	
	void removeLeftSib()
	{
		intLeftSib = NO_SIBLING;
	}
	
	void copyData( Node & n )
	{
		name = new Char[ strlen( n.name ) + 1 ];
		desc = new Char[ strlen( n.desc ) + 1 ];
		
		id = n.id;
		xpos = n.xpos;
		ypos = n.ypos;
		name = StrCopy ( name, n.name );
		desc = StrCopy ( desc, n.desc );
		intTopSib = n.intTopSib;
		intBottomSib = n.intBottomSib;
		intLeftSib = n.intLeftSib;
		intRightSib = n.intRightSib;	
	}
	
	void setDescription( Char * c )
	{	
		Char * tempdesc = desc;
		if ( desc)
		{
			int size = StrLen( desc );
			if ( size > 0 )
				delete [] desc;
				
			desc = new Char[ strlen( c ) + 1 ];	
			desc = StrCopy ( desc, c );
		}	
	}
	
	void free()
	{
		
	}
	
};

class NodeManager
{	 
	private:
	
	void setNodePosition( UInt32 unique, Coord x, Coord y )
	{
		Err error = errNone;
		MemHandle mh;
		MemPtr p;
		structMilestone * m;
		UInt16 index = 0;
		error = DmFindRecordByID ( openRefMilestone, unique, &index );
		mh = DmGetRecord( openRefMilestone, index );
		p = MemHandleLock( mh );
		error = DmWrite( p, OffsetOf( structMilestone, xpos ), &x, sizeof( x ) );
		MemPtrUnlock( p );
		DmReleaseRecord( openRefMilestone, index, true );
		
		mh = DmGetRecord( openRefMilestone, index );
		p = MemHandleLock( mh );
		error = DmWrite( p, OffsetOf( structMilestone, ypos ), &y, sizeof( y ) );
		MemPtrUnlock( p );
		DmReleaseRecord( openRefMilestone, index, true );
	}
	
	public:
		Int16 ICONNone; //	=						1200;
		Int16 ICONAchtung;//	=					1201;
		Int16 ICONGoodCmplt;	//	=				1202;
		Int16 ICONInProgress;//	=				1203;

	 NodeManager()
	 {
	 	ICONNone = 1200;
	 	ICONAchtung	= 1201;
	 	ICONGoodCmplt =	1202;
	 	ICONInProgress = 1203;
	 }
	 
	 NodeManager( DmOpenRef dor )
	 {
	 	openRefMilestone = dor; 	
	 	ICONNone = 1200;
	 	ICONAchtung	= 1201;
	 	ICONGoodCmplt =	1202;
	 	ICONInProgress = 1203;
	 }
	
	
	structMilestone * borrowNodeByIndex( UInt16 index )
	{
		MemHandle h;
		h  = DmGetRecord( openRefMilestone, index  );
		structMilestone * m = ( structMilestone * ) MemHandleLock( h );
		DmReleaseRecord( openRefMilestone, index, true );
		MemHandleUnlock( h );
		return m;	
	}
	
	
	structMilestone * borrowNodeByUnique ( UInt32 unique )
	{
		Err error = errNone;
		structMilestone * m;
		UInt16 index = 0;
		error = DmFindRecordByID ( openRefMilestone, unique, &index );
		m = borrowNodeByIndex( index );
		return m;	
	}
	
	Err setNode( structMilestone  n, UInt16 index )
	{
		Err error = errNone;
		MemHandle h;
		MemPtr p;
		h = DmQueryRecord( openRefMilestone, index  );		
		p = MemHandleLock( h );
		error = DmWrite( p, 0, &n, sizeof( n ) );
		MemPtrUnlock( p );
		return error;	
	}
	
	Err setTopSibling( UInt32 source, UInt32 dest )
	{
		Err error = errNone;
		MemHandle h;
		MemPtr p;
		UInt16 index = 0;
		
		error = DmFindRecordByID ( openRefMilestone, source, &index );
		h = DmGetRecord( openRefMilestone, index );
		p = MemHandleLock( h );
		error = DmWrite( p, OffsetOf( structMilestone, intTopSib ), &dest, sizeof( dest ) );
		MemPtrUnlock( p );
		DmReleaseRecord( openRefMilestone, index, true );
		return error;	
	}
	
	Err setBottomSibling(  UInt32 source, UInt32 dest )
	{
		Err error = errNone;
		MemHandle h;
		MemPtr p;
		UInt16 index = 0;
		
		error = DmFindRecordByID ( openRefMilestone, source, &index );
		h = DmGetRecord( openRefMilestone, index );
		p = MemHandleLock( h );
		error = DmWrite( p, OffsetOf( structMilestone, intBottomSib ), &dest, sizeof( dest ) );
		MemPtrUnlock( p );
		DmReleaseRecord( openRefMilestone, index, true );
		return error;	
	}
	
	Err setLeftSibling (  UInt32 source, UInt32 dest )
	{
		Err error = errNone;
		MemHandle h;
		MemPtr p;
		UInt16 index = 0;
		
		error = DmFindRecordByID ( openRefMilestone, source, &index );
		h = DmGetRecord( openRefMilestone, index );
		p = MemHandleLock( h );
		error = DmWrite( p, OffsetOf( structMilestone, intLeftSib ), &dest, sizeof( dest ) );
		MemPtrUnlock( p );
		DmReleaseRecord( openRefMilestone, index, true );
		return error;	
	}

	Err setRightSibling (  UInt32 source, UInt32 dest )
	{	
		Err error = errNone;
		MemHandle h;
		MemPtr p;
		UInt16 index = 0;
		
		error = DmFindRecordByID ( openRefMilestone, source, &index );
		h = DmGetRecord( openRefMilestone, index );
		p = MemHandleLock( h );
		error = DmWrite( p, OffsetOf( structMilestone, intRightSib ), &dest, sizeof( dest ) );
		MemPtrUnlock( p );
		DmReleaseRecord( openRefMilestone, index, true );
		return error;	
	}


	/**
	 * Draw connector lines between thisNode and all of its siblings.
	 * thisNode is queried for its siblings Top, Right, Bottom and Left.
	 * 
	 * Should be invoked after eraseConnectorLines( ).
	 */
	void drawConnectorLines( structMilestone * thisNode )
	{
		Node statNode;
		if ( thisNode->intTopSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intTopSib );
			PointType start = statNode.getTopConnector( thisNode );
			PointType finish = statNode.getBottomConnector( sibling );
			WinDrawLine( start.x, start.y, finish.x, finish.y );
		}	
		
		if ( thisNode->intBottomSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intBottomSib );
			PointType start = statNode.getBottomConnector( thisNode );
			PointType finish = statNode.getTopConnector( sibling );
			WinDrawLine( start.x, start.y, finish.x, finish.y );
		}
		
		if ( thisNode->intLeftSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intLeftSib );
			PointType start = statNode.getLeftConnector( thisNode );
			PointType finish = statNode.getRightConnector( sibling );
			WinDrawLine( start.x, start.y, finish.x, finish.y );
		}
		
		if ( thisNode->intRightSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intRightSib );
			PointType start = statNode.getRightConnector( thisNode );
			PointType finish = statNode.getLeftConnector( sibling );
			WinDrawLine( start.x, start.y, finish.x, finish.y );
		}
	}
	
	void straightenNode( structMilestone * thisNode )
	{
		Node statNode;
		structMilestone * top = NULL;
		structMilestone * right = NULL;
		structMilestone * bottom = NULL;
		structMilestone * left = NULL;
		
		if ( thisNode->intRightSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intRightSib );
			right = sibling;
			PointType start = statNode.getRightConnector( thisNode );
			PointType finish = statNode.getLeftConnector( sibling );
			WinEraseLine( start.x, start.y, finish.x, finish.y );
			setNodePosition( sibling->id, start.x + 10, start.y - 5 );
		}	
		
		if ( thisNode->intTopSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intTopSib );
			
			PointType start = statNode.getTopConnector( thisNode );
			PointType finish = statNode.getBottomConnector( sibling );
			WinEraseLine( start.x, start.y, finish.x, finish.y );
			setNodePosition( sibling->id, start.x - 10 , start.y - 15 );
			
		}
			
	}
	
	void eraseConnectorLines( structMilestone * thisNode )
	{
		Node statNode;
		if ( thisNode->intTopSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intTopSib );
			PointType start = statNode.getTopConnector( thisNode );
			PointType finish = statNode.getBottomConnector( sibling );
			WinEraseLine( start.x, start.y, finish.x, finish.y );
		}	
		
		if ( thisNode->intBottomSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intBottomSib );
			PointType start = statNode.getBottomConnector( thisNode );
			PointType finish = statNode.getTopConnector( sibling );
			WinEraseLine( start.x, start.y, finish.x, finish.y );
		}
		if ( thisNode->intLeftSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intLeftSib );
			PointType start = statNode.getLeftConnector( thisNode );
			PointType finish = statNode.getRightConnector( sibling );
			WinEraseLine( start.x, start.y, finish.x, finish.y );
		}
		
		if ( thisNode->intRightSib != NO_SIBLING )
		{
			structMilestone * sibling = borrowNodeByUnique( thisNode->intRightSib );
			PointType start = statNode.getRightConnector( thisNode );
			PointType finish = statNode.getLeftConnector( sibling );
			WinEraseLine( start.x, start.y, finish.x, finish.y );
		}
	}
	
	 void drawNodes( FormType * ft, FormGadgetHandlerType * fght, Int16 intProjSelect )
	{
		Err error = errNone;
		Int16 _icon = 0;
		UInt16 rowcount = 0;
		Node * ptrNode = 0x0;
		structMilestone * stctMilestone, myStone;
		MemHandle _handle, mh, memHandle;
		FormGadgetType * fgt = 0x0;		
		UInt16 gadgetIndex = 0;
		RectangleType bounds;
		BitmapType * ptrbmp;
		
		rowcount = DmNumRecords( openRefMilestone );
		for( int symbolicID = 0; symbolicID < rowcount; symbolicID++ )
		{
			// for each node in the database
			_handle  = DmQueryRecord( openRefMilestone, symbolicID  );
			
			if ( _handle )
			{
				stctMilestone = ( structMilestone * ) MemHandleLock( _handle );
				
				if ( stctMilestone->pid != intProjSelect )
				{
					MemHandleUnlock( _handle );
				}
				else
				{
					fgt = FrmNewGadget( &ft, symbolicID, stctMilestone->xpos , stctMilestone->ypos, NODE_WIDTH, NODE_HEIGHT );							
					FrmSetGadgetHandler( ft, FrmGetObjectIndex( ft, symbolicID ), fght );
					FrmSetGadgetData( ft, FrmGetObjectIndex( ft, symbolicID ), stctMilestone );
					MemHandleUnlock( _handle );
					gadgetIndex = FrmGetObjectIndex ( ft, symbolicID );
					FrmGetObjectBounds ( ft, gadgetIndex, &bounds );
					WinEraseRectangle ( &bounds, 0 );
				
					_icon = stctMilestone->icon;
					
					if ( stctMilestone->icon == ICON_GOODCMPLT )
						mh = DmGetResource( 'Tbmp', ICONGoodCmplt  );
					if ( stctMilestone->icon == ICON_ACHTUNG )
						mh = DmGetResource( 'Tbmp', ICONAchtung );
					if ( stctMilestone->icon == ICON_INPROGRESS )
						mh = DmGetResource( 'Tbmp', ICONInProgress );
					if ( stctMilestone->icon == ICON_NONE )
						mh = DmGetResource( 'Tbmp', ICONNone );
								
					if ( mh == NULL )
						error = DmGetLastErr();
					else
					{	
						ptrbmp = (BitmapType*)MemHandleLock( mh );
						WinPaintBitmap ( ptrbmp, stctMilestone->xpos, stctMilestone->ypos );
						error = MemHandleUnlock ( mh );
					}		
						
					drawConnectorLines( stctMilestone );		
				}	
			}
		}
	}
	
	void scrollNodes (  FormType * ft, Int16 intProjSelect, Coord vert, Coord horz )
	{
		Err error = errNone;
		UInt16 rowcount = 0;
		MemHandle _handle, mh;
		Int16 _icon = 0;
		structMilestone * stctMilestone;
		UInt16 gadgetIndex = 0;
		RectangleType bounds;
		BitmapType * ptrbmp;
		MemPtr p;
		Coord xtemp = 0, ytemp = 0;
		
		rowcount = DmNumRecords( openRefMilestone );
		for( int symbolicID = 0; symbolicID < rowcount; symbolicID++ )
		{
			_handle  = DmQueryRecord( openRefMilestone, symbolicID  );
			
			if ( _handle )
			{
				stctMilestone = ( structMilestone * ) MemHandleLock( _handle );
				
				if ( stctMilestone->pid != intProjSelect )
				{
					MemHandleUnlock( _handle );
				}
				else
				{				
					MemHandleUnlock( _handle );
					
					_handle  = DmGetRecord( openRefMilestone, symbolicID  );
					p = MemHandleLock( _handle );
					stctMilestone = ( structMilestone * ) p;
					eraseConnectorLines ( stctMilestone );
					xtemp = stctMilestone->xpos;
					xtemp += horz;
					error = DmWrite( p, OffsetOf( structMilestone, xpos ), &xtemp , sizeof( xtemp ) );
					MemPtrUnlock( p );
					DmReleaseRecord( openRefMilestone, symbolicID, true );
					
					_handle  = DmGetRecord( openRefMilestone, symbolicID  );
					p = MemHandleLock( _handle );
					stctMilestone = ( structMilestone * ) p;
					ytemp = stctMilestone->ypos;
					ytemp += vert;
					error = DmWrite( p, OffsetOf( structMilestone, ypos ), &ytemp , sizeof( ytemp ) );
					MemPtrUnlock( p );
					DmReleaseRecord( openRefMilestone, symbolicID, true );
					
					
					gadgetIndex = FrmGetObjectIndex ( ft, symbolicID );
					FrmGetObjectBounds ( ft, gadgetIndex, &bounds );
					WinEraseRectangle ( &bounds, 0 );
					bounds.topLeft.x = xtemp;
					bounds.topLeft.y = ytemp;				
					FrmSetObjectBounds ( ft, gadgetIndex, &bounds );
					
					_icon = stctMilestone->icon;
					
					if ( stctMilestone->icon == ICON_GOODCMPLT )
						mh = DmGetResource( 'Tbmp', ICONGoodCmplt  );
					if ( stctMilestone->icon == ICON_ACHTUNG )
						mh = DmGetResource( 'Tbmp', ICONAchtung );
					if ( stctMilestone->icon == ICON_INPROGRESS )
						mh = DmGetResource( 'Tbmp', ICONInProgress );
					if ( stctMilestone->icon == ICON_NONE )
						mh = DmGetResource( 'Tbmp', ICONNone );
						
					if ( mh == NULL )
						error = DmGetLastErr();
					else
					{	
						ptrbmp = (BitmapType*)MemHandleLock( mh );
						WinPaintBitmap ( ptrbmp, stctMilestone->xpos, stctMilestone->ypos );
						error = MemHandleUnlock ( mh );
					}
					drawConnectorLines( stctMilestone );		
				}
			}
		}
	}
	
	
	Err eraseNodes( FormType * ft, Int16 intProjSelect )
	{
		UInt16 rowcount = 0;
		structMilestone * ptrMile, ptrMile2;
		MemHandle h;		
		
		rowcount = DmNumRecords( openRefMilestone );
		
		for( int symbolicID = 0; symbolicID < rowcount; symbolicID++ )
		{
			h  = DmQueryRecord( openRefMilestone, symbolicID  );
			if ( h )
			{
				ptrMile = ( structMilestone * ) MemHandleLock( h );
				
				if ( ptrMile->pid != intProjSelect )
				{
					MemHandleUnlock( h );
				}
				else
				{
					UInt16  objidx = FrmGetObjectIndex ( ft, symbolicID );
					if ( objidx != frmInvalidObjectId )
					{
						FrmRemoveObject ( &ft, objidx );			
					}	
					
					MemHandleUnlock( h );
				}
			}
			
		}
		return errNone;
	}
	
	DmOpenRef getRef()
	{
		return openRefMilestone;	
	}
	
	void setDB( DmOpenRef dor )
	{
		openRefMilestone = dor;	
	}
	
	int getNumRecords()
	{
		DmNumRecords( openRefMilestone );	
	}
	
};

#endif // NODEMANAGER_H



