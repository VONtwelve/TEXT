
// TestView.cpp: CTestView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Test.h"
#endif

#include "TestDoc.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTestView 构造/析构

CTestView::CTestView() noexcept
{
	// TODO: 在此处添加构造代码

	OP_Str = _T("");
	SC_Pos = NULL;
	SC_Width = 0;
	str = _T("No more Humiliation 啊啊啊啊啊啊!!!");
}

CTestView::~CTestView()
{
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestView 绘图

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->TextOutW(500, 500, str);
}


// CTestView 打印

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTestView 诊断

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestView 消息处理程序


int CTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CreateSolidCaret(2, 20);
	ShowCaret();
	SetTimer(0, 500, NULL);
	return 0;
}


void CTestView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	if (nChar == 0x0d)
	{
		OP_Str = "";
		SC_Pos.y+= tm.tmHeight; 
	}
	else if (nChar == 0x08)
	{
		COLORREF clr = dc.SetTextColor(dc.GetBkColor());
		dc.TextOutW(SC_Pos.x, SC_Pos.y, OP_Str);
		OP_Str = OP_Str.Left(OP_Str.GetLength() - 1);
		dc.SetTextColor(clr);
	}
	else
	{
		OP_Str += (char)nChar;
	}
	POINT trans;
	trans.x = SC_Pos.x + dc.GetTextExtent(OP_Str).cx;
	trans.y = SC_Pos.y;
	SetCaretPos(trans);
	dc.TextOutW(SC_Pos.x, SC_Pos.y, OP_Str);
	
	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CTestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCaretPos(point);
	OP_Str = "";
	SC_Pos = point;
	CView::OnLButtonDown(nFlags, point);
}


void CTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	srand((unsigned)time(NULL));
	SC_Width += 5;
	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	
	CRect rect;
	rect.left = 500;
	rect.top = 500;
	rect.right = rect.left + SC_Width;
	rect.bottom = rect.top + tm.tmHeight;
	dc.SetTextColor(RGB(rand()/2, rand() / 2, rand() / 2));
	dc.DrawText(str, rect, DT_LEFT);

	if (SC_Width > dc.GetTextExtent(str).cx)
	{
		SC_Width = 0;
		dc.SetTextColor(RGB(rand(), rand(), rand()));
		dc.DrawText(str, rect, DT_LEFT);
	}
	
	CView::OnTimer(nIDEvent);
}
