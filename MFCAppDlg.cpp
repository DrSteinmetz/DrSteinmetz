//										*** BattleShips VS Submarines ***
// MFCAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCApp.h"
#include "MFCAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCAppDlg dialog



CMFCAppDlg::CMFCAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPP_DIALOG, pParent)
	, d_shipsLeft(SIZE)
	, d_subsLeft(SIZE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DESHIPS, d_shipsLeft);
	DDV_MinMaxInt(pDX, d_shipsLeft, 0, NETSIZE);
	DDX_Text(pDX, IDC_EDIT_DESUBS, d_subsLeft);
	DDV_MinMaxInt(pDX, d_subsLeft, 0, NETSIZE);
}

BEGIN_MESSAGE_MAP(CMFCAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_Undo, &CMFCAppDlg::OnBnClickedUndo)
	ON_BN_CLICKED(IDC_Redo, &CMFCAppDlg::OnBnClickedRedo)
	//ON_BN_CLICKED(IDC_Open, &CMFCAppDlg::OnBnClickedOpen)
	//ON_BN_CLICKED(IDC_SaveFile, &CMFCAppDlg::OnBnClickedSavefile)
	ON_BN_CLICKED(IDC_INSTRUCTIONS, &CMFCAppDlg::OnBnClickedInstructions)
END_MESSAGE_MAP()


// CMFCAppDlg message handlers

BOOL CMFCAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetWindowText(_T("BattleShips VS Submarines"));
	drawAreaBorders.CreatePen(PS_SOLID, 4, RGB(0, 0, 0));	//Modifies the drawing area borders color
	drawAreaBg.CreateSolidBrush(RGB(150, 255, 255));	//Modifies the drawing area back ground color
	netPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));	//Modifies the net's borders
	GetClientRect(&win);
	drawArea.SetRect(win.Width() / 5, 0, win.Width(), win.Height());	//Modifies the drawing area size

	/*int a = MessageBox(_T("          Congratiolations!\n       You won the game!!!\n\n\nWould you like to play again?"),
	_T("BattleShips VS Submarines"), 4); exit(0);*/

	//Initializing the enemy Battle Field here:
	srand((unsigned int)time(NULL));

	int rv = 0, i = 0;
	while (i < achi.getSize())
	{
		rv = (rand() % NETSIZE);
		if (submarines[rv] == 0)
		{
			submarines[rv]++;		//Puts a submarine in a random place of the submarines array
			i++;
		}
	}


	cube rc;
	int x1, y1 = 1, x2 = (drawArea.Width() / 10), y2 = (drawArea.Height() / 10);
	for (int k = 0, i = 0; i < 10; i++)	//Initializing the Battle Field:
	{
		x1 = (win.Width() - drawArea.Width()) + 1;
		for (int j = 0; j < 10; j++)
		{
			if (submarines[k++])
			{
				rc.containSubmarine = true;
				//Enable to reveal the enemy ships locations:--->ø
				//rc.color = RGB(255, 255, 0);	 //Yellow <------ð
			}									 //				 |
			else								 //				 |
			{									 //				 |
				rc.containSubmarine = false;	 //				 |
				//rc.color = RGB(100, 255, 255); //Cyan   <------ð
			}
			
			rc.rect.SetRect(x1, y1, x1 + x2, y1 + y2);
			net.cubeArr.push_back(rc);

			x1 += (drawArea.Width() / 10);
		}
		y1 += (drawArea.Height() / 10);
	}


	BattleShip bs;
	int x3 = 10, y3 = 20;
	int x4 = x3 + rc.rect.Width() - 50;
	int y4 = y3 + rc.rect.Height() - 4;
	for (i = 0; i < achi.getSize(); i++)	//Initializing the BattleShips:
	{
		bs.rect.SetRect(x3, y3, x4, y4);
		achi.shipsArr.push_back(bs);

		y3 += 65;
		y4 += 65;
		if ((i + 1) % (SIZE / 3) == 0)
		{
			x3 += 30;
			y3 = 20;
			x4 += 30;
			y4 = y3 + rc.rect.Height() - 4;
		}
	}


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	//Creating a white Rectangle for the drawing area.
	CClientDC dc(this);
	dc.SelectObject(&drawAreaBorders);
	dc.SelectObject(&drawAreaBg);
	dc.Rectangle(&drawArea);
	dc.SelectObject(&netPen);


	for (int i = 0; i < NETSIZE; i++)	//Drawing the Battle Field...
	{
		if (net.cubeArr[i].rect == singleNetRect)
		{
			net.cubeArr[i].selected = true;
			selCub.push_back(&net.cubeArr[i]);

			if (net.cubeArr[i].subDestroyed == true)
				net.cubeArr[i].color = RGB(0, 255, 0);
			else
				net.cubeArr[i].color = RGB(255, 0, 0);

			net.Draw(dc, i);

			if (net.cubeArr[i].subDestroyed == true)
				net.cubeArr[i].color = RGB(0, 255, 0);
			else
				net.cubeArr[i].color = RGB(255, 180, 180);
		}
		else
			net.Draw(dc, i);
	}

	for (int i = 0; i < achi.getSize(); i++)	//Drawing the BattleShips...
	{
		achi.Draw(dc, i);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//Added Events:

void CMFCAppDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (myTurn == true && gameStarted == true && drawArea.PtInRect(point))
	{
		for (int i = 0; i < NETSIZE; i++)
			if (net.cubeArr[i].rect.PtInRect(point) && net.cubeArr[i].selected == false)
			{
				if (net.cubeArr[i].containSubmarine == true && deShips < 6)
				{
					net.cubeArr[i].subDestroyed = true;
					deSubs++;
				}
				singleNetRect = net.cubeArr[i].rect;
				myTurn = false;
			}
		InvalidateRect(&singleNetRect);
		UpdateWindow();
	}


	//Counting how many ships have already been placed.
	if (gameStarted == false)
	{
		for (int i = 0; i < achi.getSize(); i++)
		{
			if (achi.shipsArr[i].rect.PtInRect(point) && achi.shipsArr[i].selected == false && achi.shipsArr[i].placed == false)
			{
				achi.shipsArr[i].oldLoc = achi.shipsArr[i].rect;
				achi.shipsArr[i].selected = true;
				someShipSel = true;
			}
			else if (drawArea.PtInRect(point) && achi.howManyShipsPlaced < achi.getSize())
			{
				MessageBox(_T("You can't start playing before you've placed all of your ships.\n\n\t\tPress OK to continue."),
					_T("BattleShips VS Submarines"));
				break;
			}
			InvalidateRect(&achi.shipsArr[i].rect);
			UpdateWindow();
		}

		if (achi.howManyShipsPlaced >= achi.getSize())
		{
			gameStarted = true;
			MessageBox(_T("All of your ships has been placed successfully.\n\n\tPress OK to start the game!"),
				_T("BattleShips VS Submarines"));
		}
	}

	isPress = true;

	d_shipsLeft = SIZE - deShips;
	d_subsLeft = SIZE - deSubs;
	UpdateData(FALSE);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCAppDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rectInv = singleNetRect;
	CRect oldRect;
	srand((unsigned int)time(NULL));

	if (deSubs >= SIZE)
	{
		int a = MessageBox(_T("          Congratiolations!\n       You won the game!!!\n\n\nWould you like to play again?"),
			_T("BattleShips VS Submarines"), 4);
		if (a == IDYES)
			restartGame();
		else if (a == IDNO)
			exit(0);
	}

	if (deShips >= SIZE)
	{
		int a = MessageBox(_T("        You lost the game...\n\n\nWould you like to play again?"),
			_T("BattleShips VS Submarines"), 4);
		if (a == IDYES)
			restartGame();
		else if (a == IDNO)
			exit(0);
	}

	if (gameStarted == false)
	{
		for (int i = 0; i < achi.getSize(); i++)
		{
			if (achi.shipsArr[i].selected == true && achi.shipsArr[i].placed == false)
			{
				for (int j = 0; j < NETSIZE; j++)
					if (net.cubeArr[j].rect.PtInRect(point) && net.cubeArr[j].containShip == false)
					{
						oldRect = achi.shipsArr[i].rect;
						someShipSel = false;

						shiPtr[i] = &(achi.shipsArr[i]);

						net.cubeArr[j].shipNum = i;
						achi.shipsArr[i].placed = true;
						achi.howManyShipsPlaced++;
						achi.shipsArr[i].locAtRectNum = j;
						net.cubeArr[j].containShip = true;
						net.cubeArr[j].color = RGB(0, 240, 255);

						int x1, y1;
						x1 = net.cubeArr[j].rect.TopLeft().x + 23;
						y1 = net.cubeArr[j].rect.TopLeft().y + 2;

						achi.shipsArr[i].rect.MoveToXY(x1, y1);
						selBtlSp[iSelBtlSp++] = &achi.shipsArr[i];	//backing up the selected ships for the 'Undo'

						InvalidateRect(&net.cubeArr[j].rect);
					}
					else if(achi.shipsArr[i].placed == false)//Puts the selected ship back in its place if not placed on a net's rect:
					{
						CRect bgnPlc = achi.shipsArr[i].oldLoc, emptyPlc = achi.shipsArr[i].rect;
						if (bgnPlc != nullptr)
							achi.shipsArr[i].rect.MoveToXY(bgnPlc.TopLeft().x, bgnPlc.TopLeft().y);
						someShipSel = false;
						InvalidateRect(&emptyPlc);
					}
				achi.shipsArr[i].selected = false;
			}
			oldRect.SetRect(oldRect.TopLeft().x - 1, oldRect.TopLeft().y - 1, oldRect.BottomRight().x + 1, oldRect.BottomRight().y + 1);
			InvalidateRect(&rectInv);
			InvalidateRect(&oldRect);
			UpdateWindow();
		}
	}

	if (achi.howManyShipsPlaced >= achi.getSize() && gameStarted == false)
	{
		MessageBox(_T("If you're done locating your ships, press OK and then press on any cube to start the game!\n\nIf you're not happy with the ships location, press 'Undo' and relocate your ships."),
			_T("BattleShips VS Submarines"));
	}

	if (myTurn == false && gameStarted == true && deSubs < achi.getSize())
	{
		int k = 0, rv = 0;
		myTurn = true;

		while (k == 0) //The computer guess where to attack
		{
			rv = (rand() % NETSIZE);
			if (subAttacks[rv] == 0)
			{
				subAttacks[rv]++;
				k++;
			}
		}
		
		if (net.cubeArr[rv].containShip == true)
		{
			deShips++;
			//Destroy the ship and change its color from grey to black:
			shiPtr[net.cubeArr[rv].shipNum]->color = RGB(0, 0, 0);
			shiPtr[net.cubeArr[rv].shipNum]->destroyed = true;
			net.cubeArr[rv].shipDestroyed = true;
		}
	}
	else
		myTurn = true;

	isPress = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCAppDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (isPress && someShipSel)
	{
		int i, xx, yy, x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		CRect invRect;

		xx = point.x - 10;
		yy = point.y - 23;

		for (i = 0; i < achi.getSize(); i++)
			if (achi.shipsArr[i].selected == true)
				break;

		x1 = achi.shipsArr[i].rect.TopLeft().x - 1;
		y1 = achi.shipsArr[i].rect.TopLeft().y - 1;
		x2 = achi.shipsArr[i].rect.BottomRight().x + 1;
		y2 = achi.shipsArr[i].rect.BottomRight().y + 1;
		invRect.SetRect(x1, y1, x2, y2);

		achi.shipsArr[i].rect.MoveToXY(xx, yy);

		InvalidateRect(&achi.shipsArr[i].rect);
		InvalidateRect(&invRect);
		UpdateWindow();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CMFCAppDlg::OnBnClickedUndo()
{
	if (gameStarted)
	{
		if (selCub.size() > 1)
		{
			cube *r = selCub.back();
			selCub.pop_back();

			r->color = RGB(100, 255, 255);
			r->selected = false;
			if (r->shipDestroyed)	//Only if the ship is in the last cube
			{
				r->shipDestroyed = false;
				achi.shipsArr[r->shipNum].destroyed = false;
				achi.shipsArr[r->shipNum].color = RGB(200, 200, 200);
				deShips--;
			}
			if (r->subDestroyed)	//Only if the sub is in the last cube
			{
				r->subDestroyed = false;
				deSubs--;
			}

			singleNetRect = NULL;
			myTurn = true;

			d_shipsLeft = SIZE - deShips;
			d_subsLeft = SIZE - deSubs;
			UpdateData(FALSE);
			InvalidateRect(&r->rect);
			UpdateWindow();
		}
		else
			MessageBox(_T("'Undo' Can't be done.\n\n\tPress OK to continue."),
				_T("BattleShips VS Submarines"), 0);
	}
	else
	{
		if (iSelBtlSp)
		{
			CRect preR;
			CClientDC dc(this);
			BattleShip *bs;

			bs = selBtlSp[--iSelBtlSp];
			selBtlSp[iSelBtlSp] = nullptr;
			bs->selected = false;
			bs->placed = false;
			preR = bs->rect;
			bs->rect.SetRect(bs->oldLoc.TopLeft(), bs->oldLoc.BottomRight());
			bs->oldLoc = preR;

			net.cubeArr[bs->locAtRectNum].containShip = false;
			net.cubeArr[bs->locAtRectNum].color = RGB(100, 255, 255);
			achi.howManyShipsPlaced--;

			InvalidateRect(&bs->oldLoc);
			InvalidateRect(&preR);
			UpdateWindow();
		}
		else
			MessageBox(_T("'Undo' Can't be done.\n\n\tPress OK to continue."),
				_T("BattleShips VS Submarines"), 0);
	}
}


void CMFCAppDlg::OnBnClickedRedo()
{
	MessageBox(_T("'Redo' Can't be done.\n\n\tPress OK to continue."),
		_T("BattleShips VS Submarines"), 0);
}


//void CMFCAppDlg::OnBnClickedOpen()
//{
//	//phone *p;
//	//p = &bzk;	p->printPhoneDetails();	p->printCallLog();
//	//cellCam * temp = dynamic_cast<cellCam*>(p); if (temp) temp->showPics();
//	
//	CFileDialog dlg(TRUE, _TEXT("dat"), _TEXT("*.dat"));
//	CString fname;
//	int res = dlg.DoModal();
//	if (res == IDOK)
//		fname = dlg.GetPathName();
//
//	// Load to file:
//	//CFile fOut;
//	//if (fOut.Open(fname, CFile::modeRead))
//	//{
//	//	CArchive ar(&fOut, CArchive::load);
//	//	ar >> LogicSize; //ar >> objects.size();
//	//	for (int i = 0; i < LogicSize; i++) {
//	//		int ts;
//	//		ar >> ts;
//	//		switch (ts) {
//	//		case 0:
//	//			shapes[i] = new mRectangle();
//	//			shapes[i]->tShape = RECTANGLE;
//	//			break;
//	//		case 1:
//	//			shapes[i] = new mEllipse();
//	//			shapes[i]->tShape = ELLIPSE;
//	//			break;
//	//		}
//	//		shapes[i]->Serialize(ar);
//	//	}
//	//	ar.Close();
//	//}
//	//fOut.Close();
//
//	MessageBox(_T("File could not be found.\n\n\tPress OK to continue."),
//		_T("BattleShips VS Submarines"), 0);
//}


//void CMFCAppDlg::OnBnClickedSavefile()
//{
//	//for (int i = 0; i < net.rectArr.size(); i++)
//	//{
//	//	objects.push_back(&net.rectArr[i]);
//	//}
//	//for (int i = 0; i < achi.shipsArr.size(); i++)
//	//{
//	//	objects.push_back(&achi.shipsArr[i]);
//	//}
//
//	CFileDialog dlg(FALSE, _TEXT("dat"), _TEXT("*.dat"));
//	CString fname;
//	int res = dlg.DoModal();
//	if (res == IDOK)
//		fname = dlg.GetPathName();
//
//	// Write to file:
//	//CFile fOut;
//	//if (fOut.Open(fname, CFile::modeCreate | CFile::modeWrite))
//	//{
//	//	CArchive ar(&fOut, CArchive::store);
//	//	ar << LogicSize; //ar << objects.size();
//	//	for (int i = 0; i < LogicSize; i++) {
//	//		//ar << shapes[i]->tShape;
//	//		shapes[i]->Serialize(ar);
//	//	}
//	//	ar.Close();
//	//}
//	//fOut.Close();
//
//	MessageBox(_T("The game has been successfully saved."),
//		_T("BattleShips VS Submarines"), 0);
//}


void CMFCAppDlg::OnBnClickedInstructions()
{
	MessageBox(_T("Game Instructions:\n)x( You can't start playing before you've been placed all of your ships.\n)x( When your ship gets hit it turns black.\n)x( When you hit a submarine the cube turns green.\n\n\t\tPress OK to continue."),
		_T("BattleShips VS Submarines"), 0);
}



void CMFCAppDlg::restartGame()
{
	srand((unsigned int)time(NULL));

	objects.clear();
	selCub.clear();
	isPress = false;
	gameStarted = false;
	myTurn = false;
	someShipSel = false;
	deSubs = 0;
	deShips = 0;
	iSelBtlSp = 0;
	achi.howManyShipsPlaced = 0;
	singleNetRect = NULL;

	int rv = 0, i = 0;
	for (i = 0; i < achi.getSize(); i++)
	{
		shiPtr[i] = nullptr;
		selBtlSp[i] = nullptr;
		achi.shipsArr[i].selected = false;
		achi.shipsArr[i].destroyed = false;
		achi.shipsArr[i].oldLoc = NULL;
		achi.shipsArr[i].placed = false;
		achi.shipsArr[i].locAtRectNum = -1;
		achi.shipsArr[i].color = RGB(200, 200, 200);
	}
	for (i = 0; i < NETSIZE; i++)
	{
		submarines[i] = 0;
		subAttacks[i] = 0;
		net.cubeArr[i].selected = false;
		net.cubeArr[i].containShip = false;
		net.cubeArr[i].containSubmarine = false;
		net.cubeArr[i].shipDestroyed = false;
		net.cubeArr[i].subDestroyed = false;
		net.cubeArr[i].shipNum = -1;
		net.cubeArr[i].color = RGB(100, 255, 255);
	}
	i = 0;
	while (i < achi.getSize())
	{
		rv = (rand() % NETSIZE);
		if (submarines[rv] == 0)
		{
			submarines[rv]++;		//Puts a submarine in a random place of the submarines array
			i++;
		}
	}

	for (int k = 0, i = 0; i < NETSIZE; i++)	//Restarting the Battle Field:
	{
		if (submarines[k++])
		{
			net.cubeArr[i].containSubmarine = true;
			//Enable to reveal the enemy ships locations:--------------->ø
			//net.cubeArr[i].color = RGB(255, 255, 0);	 //Yellow <------ð
		}												 //				 |
		else											 //				 |
		{												 //				 |
			net.cubeArr[i].containSubmarine = false;	 //				 |
			//net.cubeArr[i].color = RGB(100, 255, 255); //Cyan   <------ð
		}
	}

	CRect r;
	r.SetRect(0, 0, (drawArea.Width() / 10), (drawArea.Height() / 10));
	int x3 = 10, y3 = 20;
	int x4 = x3 + r.Width() - 50;
	int y4 = y3 + r.Height() - 4;
	for (i = 0; i < achi.getSize(); i++)	//Restarting the BattleShips:
	{
		achi.shipsArr[i].rect.SetRect(x3, y3, x4, y4);
		y3 += 65;
		y4 += 65;
		if ((i + 1) % (SIZE / 3) == 0)
		{
			x3 += 30;
			y3 = 20;
			x4 += 30;
			y4 = y3 + r.Height() - 4;
		}
	}
	Invalidate();
}