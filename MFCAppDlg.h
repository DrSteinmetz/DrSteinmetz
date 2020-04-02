
// MFCAppDlg.h : header file
//

#pragma once
#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

#define SIZE 6		//How many ships are playing
#define NETSIZE 100	//How many cubes are in the net

enum shapeType { CUBE, CUBEARR, SHIP, SHIPARR };

class Shape// : public CObject
{
public:
	Shape() {}
	virtual ~Shape() {}

	shapeType shapeTp;
	COLORREF color;
	CRect rect;
	bool selected;

	virtual CPoint getPoint()const = 0;

	/*void Serialize(CArchive &ar) {
		CObject::Serialize(ar);
		int t = 0;
		if (ar.IsStoring()) {
			ar << t << p1 << p2 << color << bkcolor;
		}
		else {
			ar >> p1 >> p2 >> color >> bkcolor;
			border.CreatePen(PS_SOLID, 2, color);
			bkground.CreateSolidBrush(bkcolor);
		}
	}*/
};

class cube : public Shape
{
public:
	cube() { shapeTp = CUBE; }
	virtual ~cube() {}

	COLORREF color = RGB(100, 255, 255);
	CRect rect;
	int shipNum;
	bool selected{ false };
	bool shipDestroyed{ false };
	bool subDestroyed{ false };
	bool containShip{ false };
	bool containSubmarine{ false };

	void Draw(CClientDC &dc)
	{
		CBrush bk;
		bk.CreateSolidBrush(color);
		dc.SelectObject(&bk);
		dc.Rectangle(&rect);
	}

	virtual CPoint getPoint()const { return rect.CenterPoint(); }
};

class Net : public cube
{
public:
	Net() { shapeTp = CUBEARR; }
	~Net() { cubeArr.clear(); }

	vector<cube> cubeArr;
	vector<cube>& getRectArr() { return this->cubeArr; }

	void Draw(CClientDC &dc, int i) { cubeArr[i].Draw(dc); }

	virtual CPoint getPoint()const { return rect.CenterPoint(); }
};

class BattleShip : public Shape
{
private:
	COLORREF regCol = RGB(0, 0, 0);
	COLORREF colSel = RGB(255, 0, 0);
public:
	BattleShip() { shapeTp = SHIP; }
	~BattleShip() {}
	CRect rect;
	CRect oldLoc = nullptr;
	COLORREF color = RGB(200, 200, 200);
	int locAtRectNum;
	bool selected{ false };
	bool placed{ false };
	bool destroyed{ false };

	void Draw(CClientDC &dc)
	{
		CBrush bk;
		CPen br;
		bk.CreateSolidBrush(color);
		br.CreatePen(PS_SOLID, 2, selected ? colSel : regCol);
		dc.SelectObject(&bk);
		dc.SelectObject(&br);
		dc.Ellipse(&rect);
	}

	virtual CPoint getPoint()const { return rect.CenterPoint(); }
};

class Navy : public BattleShip
{
private:
	int size = SIZE;
public:
	Navy() { shapeTp = SHIPARR; }
	~Navy() { shipsArr.clear(); }

	int getSize() const { return size; }
	int howManyShipsPlaced = 0;

	vector<BattleShip> shipsArr;
	vector<BattleShip>& getShipsArr() { return this->shipsArr; }

	void Draw(CClientDC &dc, int i) { shipsArr[i].Draw(dc); }
	
	virtual CPoint getPoint()const { return rect.CenterPoint(); }
};

// CMFCAppDlg dialog
class CMFCAppDlg : public CDialogEx
{
	// Construction
public:
	CMFCAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPP_DIALOG };
#endif

	CRect win;
	CRect drawArea;
	CRect singleNetRect;

	CPen drawAreaBorders;
	CPen netPen;
	CBrush drawAreaBg;
	
	vector<Shape*> objects{ nullptr };	//For saving all of the program's objects using polymorphism
	vector<cube*> selCub{ nullptr };
	BattleShip *shiPtr[SIZE]{ nullptr };
	BattleShip *selBtlSp[SIZE]{ nullptr };
	Net net;
	Navy achi;
	
	bool isPress{ false };
	bool gameStarted{ false };
	bool myTurn{ false };
	bool someShipSel{ false };
	
	int submarines[100]{ 0 };
	int subAttacks[100]{ 0 };
	int deSubs = 0;
	int deShips = 0;
	int iSelBtlSp = 0;

	void restartGame();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedUndo();
	afx_msg void OnBnClickedRedo();
	//afx_msg void OnBnClickedOpen();
	//afx_msg void OnBnClickedSavefile();
	afx_msg void OnBnClickedInstructions();

	int d_shipsLeft;
	int d_subsLeft;
};