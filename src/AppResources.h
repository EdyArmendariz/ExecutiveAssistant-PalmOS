//
//	Palm App Name:   		"Beta"
//
//	Palm App Version:		"1.0"


//	Resource: tFRM 1000
#define MainForm                                  1000	//(Left Origin = 0, Top Origin = 0, Width = 160, Height = 160, Usable = 1, Modal = 0, Save Behind = 0, Help ID = 0, Menu Bar ID = 1000, Default Button ID = 0)
#define frmMainMenu					1000
#define btnViewAllProjects			1001
#define btnViewAllEmployees			1002
#define btnBuildNewProject			1003

//	Resource: tFRM 1100
#define AboutForm                                 1100	//(Left Origin = 2, Top Origin = 2, Width = 156, Height = 156, Usable = 1, Modal = 1, Save Behind = 1, Help ID = 0, Menu Bar ID = 0, Default Button ID = 0)
#define AboutOKButton                             1105	//(Left Origin = 58, Top Origin = 139, Width = 40, Height = 12, Usable = 1, Anchor Left = 1, Frame = 1, Non-bold Frame = 1, Font = Standard)
#define AboutTitleLabel                           1102	//(Left Origin = 54, Top Origin = 25, Usable = 1, Font = Bold 12)
#define AboutText1Label                           1103	//(Left Origin = 23, Top Origin = 54, Usable = 1, Font = Standard)
#define AboutText2Label                           1104	//(Left Origin = 50, Top Origin = 104, Usable = 1, Font = Bold)

//	Resource: tFRM 1300
#define frmGraph                            1300	//(Left Origin = 0, Top Origin = 0, Width = 160, Height = 160, Usable = 1, Modal = 0, Save Behind = 0, Help ID = 0, Menu Bar ID = 0, Default Button ID = 0)
#define mnuGraphMakeNeat					1300
#define Form2DoneButton                     1301	//(Left Origin = 1, Top Origin = 147, Width = 36, Height = 12, Usable = 1, Anchor Left = 1, Frame = 1, Non-bold Frame = 1, Font = Standard)
#define Form2SmileyGadget					1303
#define frmGraphGotoList					1304
#define frmGraphGotoHome					1305
#define frmGraphUp							1306
#define frmGraphDown						1307
#define frmGraphLeft						1308
#define frmGraphRight						1309


#define frmNewMilestoneDialog				1310
#define btnNewMilestoneOK					1311
#define btnNewMilestoneCancel				1312
#define txtNewMilestoneName					1313


#define frmMilestoneInfoDlg					1320
#define btnMilestoneInfoOK					1321
#define btnMilestoneInfoCancel				1322
#define lblMilestoneInfoDlgName				1323
#define txtMilestoneInfoDesc				1324

// Resource: Create a New Project
#define frmProjectNew				1350 // 1320
#define txtProjectNewName			1352 // 1322
#define btnProjectNewDesign			1353
#define btnProjectNewOK				1354 // 1324

// Resource: Project List
#define frmProjectList				1400
#define btnProjectListBack			1401		
#define frmProjectListTable			1402
#define frmProjectListList			1402	
#define frmProjectListScroll		1403	
#define btnProjectListBackButton	1404

#define frmEmployeeList				1420
#define frmEmployeeListList			1421
#define btnEmployeeListHome			1422
#define btnEmployeeListBack			1424
#define dlgEmployeeMilestoneInfo	1430
#define lblEmployeeMilestoneInfo	1431

// Resource: Project Menu
#define frmProjectMenu				1500
#define btnProjectMenuStatus		1501
#define btnProjectMenuBudget		1502
#define btnProjectMenuMilestones	1503
#define btnProjectMenuEmployees		1504
#define lblProjectMenuBudget		1506
#define txtProjectMenuMilestones	1507
#define txtProjectMenuEmployees		1508
#define btnProjectMenuBack			1509
#define lblProjectMenuTitle			1510
#define lblProjectMenuStatus		1511
#define btnAchtungList				1512
#define btnProjectMenuHome			1513

// From ProjectMenu ...
#define frmProjectEmployeeList		1530
#define frmProjectEmployeeListList	1531
#define frmProjectEmployeeListHome	1532
#define frmProjectEmployeeListBack	1533
#define frmProjectEmployeeListGotoList		1534

#define frmProjectEmployee			1560
#define frmProjectEmployeeBack		1561
#define frmProjectEmployeeFieldType	1564
#define frmProjectEmployeeTestButton 1565
#define btnProjectEmployeeHome			1566
#define btnProjectEmployeePrjList		1567
#define btnProjectEmployeePrjMenu		1568
#define popProjectEmployeePulldown		1569
#define lstProjectEmployee				1570


// Resource: Budget Menu
#define frmBudgetMenu				1600
#define btnBudgetMenuRunningTotal	1601
#define btnBudgetMenuProjectedCost	1602
#define lblBudgetMenuRunningTotal	1603
#define lblBudgetMenuProjectedCost	1604
#define btnBudgetMenuBack			1605

// Resource: Running Total
#define frmRunningTotal				1700
#define tblRunningTotal				1701
#define btnRunningTotalBack			1702

// Resource: Projected Cost
#define frmProjectedCost			1800
#define tblProjectedCost			1801
#define btnProjectedCostBack		1802


// Resource: Achtung List
#define frmAchtungList				1900
#define btnAchtungListBack			1901
#define btnAchtungListSelect		1902
#define btnAchtungListMainMenu		1903
#define dlgAchtungList				1920
#define lblDialogAchtung			1921



//	Resource: Talt 1001
#define RomIncompatibleAlert                      1001
#define RomIncompatibleOK                         0


//	Resource: MBAR 1000
#define MainFormMenuBar                           1000


//	Resource: MENU 1000
#define MainOptionsMenu                           1000
#define MainOptionsAboutStarterApp                1000
#define frmMainMenuResetDatabase				1002


//	Resource: PICT 1001
#define Bitmap                                    1001

//	Resource: PICT 1002
#define Bitmap2                                   1002

//	Resource: PICT 1008
#define Bitmap3                                   1008

//	Resource: PICT 1011
#define Bitmap4                                   1011

//	Resource: PICT 1012
#define Bitmap5                                   1012

//	Resource: PICT 1018
#define Bitmap6                                   1018


//	Resource: taif 1000
#define Largeicons12and8bitsAppIconFamily         1000

//	Resource: taif 1001
#define Smallicons12and8bitsAppIconFamily         1001

// AppInfoBlock
typedef struct
{
	UInt16  renamedCategories;
	char categoryLabels[dmRecNumCategories][dmCategoryLength];
	UInt8 categoryUniqIDs[dmRecNumCategories];
	UInt8 lastUniqID;
	UInt8 reserved1;
	UInt16 reserved2;
}
DefaultAppInfoType;


typedef struct{
	UInt32	id;				// the record's unique id within the database
	Int16	pid;
	Char	fname[20];
	Char	lname[30];
} structEmployee;

typedef struct {
	UInt32	id;				// this Project's Unique Record ID
	Int16	pid;			// this Project's referential key ID
	Char	name[20];		// this Project's name
	// Char		description [ 40 ];
} structProject;


typedef struct {
	UInt32 	id;				// this Milestone's ID from the DB row
	Int16	pid;			// the Project ID that this Milestone belongs to
	Coord 	xpos;
	Coord 	ypos;
	UInt32 	intRightSib;
	UInt32 	intLeftSib;
	UInt32 	intTopSib;
	UInt32 	intBottomSib;
	Int16	icon;
	UInt32	emp1;			// UniqueID of this employee's DB record.
	UInt32	emp2;
	UInt32	emp3;
	UInt32	emp4; 
	Char 	name [20];		// the Milestone ID
	Char 	desc [40];		// the Milestone description
} structMilestone;


// D E C L A R E   G L O B A L    V A R I A B L E S    H E R E 
// Declared here and used in AppMain.cpp
DmOpenRef openRefProject;
// Declared here and used in AppMain.cpp
DmOpenRef openRefEmployee;

DmOpenRef globalOpenRefMilestone = NULL;

UInt32 *globalListProjectEmployeeUInt32;

UInt32 *globalAchtungListUInt32;





