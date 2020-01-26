


static Boolean MyGadgetHandler( FormGadgetTypeInCallback * gadgetP,
	UInt16 cmd, void * paramP )
	{
		Boolean handled = false;
		Boolean isPenDown = true;
		EventPtr eventP = NULL;
		Coord crdOldScreenX = 0, crdOldScreenY = 0;  // Coord stores raw coordinates
		Coord crdNewScreenX = 0, crdNewScreenY = 0;
		RectangleType sdf, rct;
		int id = gadgetP->id;
		UInt32 romVersion = 0;
		UInt16 ticksPerSec = SysTicksPerSecond();
		FormType * myFTP = 0x0, * ptrForm = 0x0;
		FormLabelType * flt = 0x0, * newLabel = 0x0;
		RectangleType  dummyrect;
		RGBColorType rgbold, rgbnew;
		Boolean b = false;
		ControlType * ctButton = 0x0;
		MemHandle mh = 0x0;
		BitmapType * ptrbmp = 0x0;
		Err error = errNone;
		
		switch ( cmd ) // cmd is one of 4 values
		{
			case formGadgetHandleEventCmd:		// 2				
				eventP = ( EventPtr ) paramP;
					switch ( eventP->eType )
					{
						case frmGadgetEnterEvent:
						{
							crdOldScreenX = nodeManager.n[id].xpos;
							crdOldScreenY = nodeManager.n[id].ypos;
							dummyrect.topLeft.x = crdOldScreenX + 22;
 							dummyrect.topLeft.y = crdOldScreenY - 12;
 							dummyrect.extent.x = 50;
 							dummyrect.extent.y = 16;													
							
							while ( isPenDown )
							{
								EvtGetPen ( &crdNewScreenX, &crdNewScreenY, &isPenDown );
								SysTaskDelay( ticksPerSec / 10);
							}
							
							if ( RctPtInRectangle (crdNewScreenX, crdNewScreenY,
              										&(gadgetP->rect)) )
							{								
 								char * cForm = "Fix Printing";	
 								char * cLabel = "FormLabel";
 								char * cButton = "OK";
 															
 								WinPushDrawState();
 								if ( !isLabelShowing )
 								{
 									rgbnew.r = 255;
 									rgbnew.b = 255;
 									rgbnew.g = 255;
 									WinSetForeColorRGB( &rgbnew, &rgbold );
 									rgbnew.r = 0;
 									rgbnew.b = 0;
 									rgbnew.g = 0;
 									isLabelShowing = true;
 									if ( dummyrect.topLeft.x < 80 )
 										myNewForm = FrmNewForm( 69, cForm, dummyrect.topLeft.x, dummyrect.topLeft.y - 10,
 															80, 24, true, 0, 0, 0 );
 									else
 										myNewForm = FrmNewForm( 69, cForm, dummyrect.topLeft.x - 80, dummyrect.topLeft.y - 10 ,
 															80, 24, true, 0, 0, 0 );
 									b = FrmValidatePtr(  myNewForm );
 									b = b;				
 																		
 									void * formPP = myNewForm;
 									ctButton = CtlNewControl(  &formPP, 692, buttonCtl, cButton,60, 15, 20, 
 									10, stdFont, 0, false );
 									CtlSetEnabled( ctButton, true );
 									UInt16 myIndex = FrmGetObjectIndex( myNewForm, 692  );
 									FrmDoDialog( myNewForm );																								 
 								}
 								else
 								{										
 									isLabelShowing = false;
 									if ( FrmValidatePtr ( myNewForm ) )
 									{
 										UInt16 mIndy = FrmGetObjectIndex( myNewForm, 692 );
 										
 										FrmRemoveObject( &myNewForm , mIndy );
 									}
 								}
 								WinPopDrawState();
 								int iii = 0;
 								
							}
							else {
								
							if ( nodeManager.n[ id ].hasRightSib )
							{
								Node * buffNode = nodeManager.n[id].getRightSib();
								PointType buffdest = buffNode->getLeftConnector();
								PointType buffsrc = nodeManager.n[id].getRightConnector();
								WinEraseLine( buffsrc.x, buffsrc.y, buffdest.x, buffdest.y );							
							}
							
							if ( nodeManager.n[ id ].hasLeftSib )
							{
								Node * buffNode = nodeManager.n[id].getLeftSib();
								PointType buffdest = buffNode->getRightConnector();
								PointType buffsrc = nodeManager.n[id].getLeftConnector();
								WinEraseLine( buffsrc.x, buffsrc.y, buffdest.x, buffdest.y );							
							}
							
							if ( nodeManager.n[ id ].hasTopSib )
							{
								Node * buffNode = nodeManager.n[id].getTopSib();
								PointType buffdest = buffNode->getBottomConnector();
								PointType buffsrc = nodeManager.n[id].getTopConnector();
								WinEraseLine( buffsrc.x, buffsrc.y, buffdest.x, buffdest.y );							
							}
							
							if ( nodeManager.n[ id ].hasBottomSib )
							{
								Node * buffNode = nodeManager.n[id].getBottomSib();
								PointType buffdest = buffNode->getTopConnector();
								PointType buffsrc = nodeManager.n[id].getBottomConnector();
								WinEraseLine( buffsrc.x, buffsrc.y, buffdest.x, buffdest.y );							
							}
							
							gadgetP->rect.topLeft.x = crdNewScreenX;
  							gadgetP->rect.topLeft.y = crdNewScreenY;
							rct.topLeft.x = crdOldScreenX;
							rct.topLeft.y = crdOldScreenY;
							rct.extent.x = 20;
							rct.extent.y = 10;
							 WinEraseRectangleFrame ( rectangleFrame, &rct);
							WinEraseRectangle ( &rct, 0 );
							rct.topLeft.x = crdNewScreenX;
							rct.topLeft.y = crdNewScreenY;
							WinDrawRectangleFrame ( rectangleFrame, &rct	);
							
							if ( nodeManager.n[ id ].hasRightSib )
							{
								Node * buffNode = nodeManager.n[id].getRightSib();
								PointType buffdest = buffNode->getLeftConnector();
								PointType buffsrc = nodeManager.n[id].getRightConnector();
								WinDrawGrayLine( buffsrc.x, buffsrc.y, buffdest.x, buffdest.y );							
							}
							if ( nodeManager.n[ id ].hasLeftSib )
							{
								Node * buffNode = nodeManager.n[id].getLeftSib();
								PointType buffdest = buffNode->getRightConnector();
								PointType buffsrc = nodeManager.n[id].getLeftConnector();
								WinDrawGrayLine( buffsrc.x, buffsrc.y, buffdest.x, buffdest.y );							
							}
							if ( nodeManager.n[ id ].hasTopSib )
							{
								Node * buffNode = nodeManager.n[id].getTopSib();
								PointType buffdest = buffNode->getBottomConnector();
								PointType buffsrc = nodeManager.n[id].getTopConnector();
								WinDrawGrayLine( buffsrc.x, buffsrc.y, buffdest.x, buffdest.y );							
							}
							
							if ( nodeManager.n[ id ].hasBottomSib )
							{
								Node * buffNode = nodeManager.n[id].getBottomSib();
								PointType buffdest = buffNode->getTopConnector();
								PointType buffsrc = nodeManager.n[id].getBottomConnector();
								WinDrawGrayLine( buffsrc.x, buffsrc.y, buffdest.x, buffdest.y );							
							}
							
							if ( id == 0 )
								mh = DmGetResource( 'Tbmp', 1340  );
							if ( id == 1 )
								mh = DmGetResource( 'Tbmp', 1341  );
							if ( id == 2 )
								mh = DmGetResource( 'Tbmp', 1341  );
								
							if ( mh == NULL )
								error = DmGetLastErr();	
							ptrbmp = (BitmapType*)MemHandleLock( mh );
							WinPaintBitmap ( ptrbmp, crdNewScreenX, crdNewScreenY );
							error = MemHandleUnlock ( mh );
							}
							
						break;}
						case frmGadgetMiscEvent:
							isPenDown = isPenDown;
							break;
						default:
							break;	
					}
				
				break;
			case formGadgetDeleteCmd:
				break;
			case formGadgetDrawCmd:
				break;
			case formGadgetEraseCmd:
				break;
			default:
				break;
		}
		
		
		return handled;	
	}
	
	