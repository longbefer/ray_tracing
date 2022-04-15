
// Temp1View.cpp: CTemp1View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Temp1.h"
#endif

#include "Temp1Doc.h"
#include "Temp1View.h"
#include <time.h>
#include <cstring>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTemp1View

IMPLEMENT_DYNCREATE(CTemp1View, CView)

BEGIN_MESSAGE_MAP(CTemp1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_SAVE, &CTemp1View::OnFileSave)
END_MESSAGE_MAP()

// CTemp1View 构造/析构

CTemp1View::CTemp1View() noexcept
{
	// TODO: 在此处添加构造代码
	img.GetImage(IDB_BITMAP2);
}

CTemp1View::~CTemp1View()
{
	img.DeleteObject();
}

BOOL CTemp1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTemp1View 绘图

void CTemp1View::OnDraw(CDC* pDC)
{
	CTemp1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect rect;
	GetClientRect(&rect);
	//pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	//pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	//pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区范围,x轴水平向右，y轴垂直向上
	//pDC->SetViewportOrg(0, rect.Height());//客户区中心为原点

	//CDC memDC;
	//memDC.CreateCompatibleDC(pDC);
	//CBitmap NewBitmap, * pOldBitmap;
	//NewBitmap.LoadBitmap(IDB_BITMAP1);//从资源中导入空心汉字的位图
	//pOldBitmap = memDC.SelectObject(&NewBitmap);

	//world.GetWindowsSize(rect.Width(), rect.Height());
	time_t start = time(nullptr);
	world.GetWindowsSize(800, 800);
	world.pDC = pDC;
	world.img = &img;
	world.bulid();
	//world.render_perspective();
	//world.render_sence();
	world.render_world();
	//img.DeleteObject();
	time_t end = time(nullptr);
	//char time[11];
	//_ltoa_s(end - start, time, 10);
	CString time;
	time.Format(L"%d s", int(end - start));
	pDC->TextOutW(0, 0, time);

	//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	//memDC.SelectObject(pOldBitmap);//从内存DC中释放位图
	//NewBitmap.DeleteObject();
	//ReleaseDC(&memDC);

}


// CTemp1View 打印

BOOL CTemp1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTemp1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTemp1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTemp1View 诊断

#ifdef _DEBUG
void CTemp1View::AssertValid() const
{
	CView::AssertValid();
}

void CTemp1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTemp1Doc* CTemp1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTemp1Doc)));
	return (CTemp1Doc*)m_pDocument;
}
#endif //_DEBUG


// CTemp1View 消息处理程序


void CTemp1View::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	CRect rect;
	GetClientRect(&rect);
	//将文件保存为命名
	CFileDialog dialog(FALSE, L"Bitmap File(*.bmp)|*.bmp|All Files (*.*)|*.*||", NULL, NULL, L"Bitmap File(*.bmp)|*.bmp|All Files (*.*)|*.*||", this);
	//False指定为保存文件，.bmp指定为保存文件扩展名
	CString pathName = 0/*, fileHead, fileName*/;
	if (dialog.DoModal() == IDOK) {
		pathName = dialog.GetPathName();//获取文件名（路径）
		CImage image;
		image.Create(rect.Width(), rect.Height(), 32);//图像的长和宽
		CDC* pDC = GetDC();
		CDC dstDC;
		dstDC.CreateCompatibleDC(pDC);
		dstDC.SelectObject(image);
		dstDC.StretchBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);
		image.Save(pathName);
	}
}
