
// Temp1View.h: CTemp1View 类的接口
//

#pragma once
#include "World.h"

class CTemp1View : public CView
{
protected: // 仅从序列化创建
	CTemp1View() noexcept;
	DECLARE_DYNCREATE(CTemp1View)

// 特性
public:
	CTemp1Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CTemp1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	World world;
	Texture img;
// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSave();
};

#ifndef _DEBUG  // Temp1View.cpp 中的调试版本
inline CTemp1Doc* CTemp1View::GetDocument() const
   { return reinterpret_cast<CTemp1Doc*>(m_pDocument); }
#endif

