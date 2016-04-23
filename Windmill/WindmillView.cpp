
// WindmillView.cpp : CWindmillView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Windmill.h"
#endif

#include "WindmillDoc.h"
#include "WindmillView.h"
#include "SetColorDlg.h"
#include "RotateSpeed.h"
#include <math.h>
#define MAX_CHAR        128
#define BG_Width			 8
GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 				// 环境光参数
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };					// 漫射光参数
GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };					//镜面反射光
GLfloat LightPosition[]= { 0.0f, 3.5f,9.5f, 1.0f };				 // 光源位置
GLfloat am[]={0.2f,0.2f,0.2f,1.0f};//材质的环境光颜色
GLfloat di[]={0.8f,0.8f,0.8f,1.0f};//材质的漫反射光颜色
GLfloat sp[]={1.0f,1.0f,1.0f,1.0f};//材质的镜面反射光颜色
GLfloat shininess = 100.0;
extern DataEx Data;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef RGBA
#define RGBA(r,g,b,a)        ((COLORREF)( (((DWORD)(BYTE)(a))<<24) |     RGB(r,g,b) ))
#endif

// CWindmillView

IMPLEMENT_DYNCREATE(CWindmillView, CView)
BEGIN_MESSAGE_MAP(CWindmillView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_32773, &CWindmillView::OnSetColor)
	ON_WM_TIMER()
	ON_COMMAND(ID_32771, &CWindmillView::OnRotate)
	ON_COMMAND(ID_32772, &CWindmillView::OnStop)
	ON_COMMAND(ID_32774, &CWindmillView::OnRotateSpeed)
	ON_COMMAND(ID_32776, &CWindmillView::OnEnableMotionFlur)
	ON_COMMAND(ID_32777, &CWindmillView::OnDisableMotionFlur)
	ON_COMMAND(ID_32779, &CWindmillView::OnTexture)
	ON_COMMAND(ID_32780, &CWindmillView::OnChangeVane)
END_MESSAGE_MAP()

// CWindmillView 构造/析构
CWindmillView::CWindmillView()
{
	// TODO: 在此处添加构造代码
	Color1 = RGBA(0,0,0,0);
	Color2 = RGBA(255,0,0,0);
	Color3 = RGBA(0,255,0,0);
	Color4 = RGBA(0,0,255,0);
	

	m_hGLContext = NULL;
	InitAngle = 0.0;
	Sp = 100.0;
	MotionFlur = FALSE;
	TempFPSTime = 0;
	Step = 1.0;
	Ac= 0;
	T = 0;
	IsAccStop = false;
	UseTexture = false;
	VaneKind = 1;
	Alpha = 1.0;
}
CWindmillView::~CWindmillView()
{
}
BOOL CWindmillView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}
// CWindmillView 绘制
void CWindmillView::OnDraw(CDC* pDC)
{
	CWindmillDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	DrawWindmill();
	glPopMatrix();
	SwapBuffers( m_pDC->GetSafeHdc());
}
// CWindmillView 诊断
#ifdef _DEBUG
void CWindmillView::AssertValid() const
{
	CView::AssertValid();
}
void CWindmillView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CWindmillDoc* CWindmillView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWindmillDoc)));
	return (CWindmillDoc*)m_pDocument;
}
#endif //_DEBUG

//初始化函数
BOOL CWindmillView::OpenGLInit()
{
	//Get a DC for the Client Area
	m_pDC = new CClientDC(this);
	//Failure to Get DC
	if(m_pDC == NULL)
	{
		MessageBox("Error Obtaining DC");
		return FALSE;
	}
	//Failure to set the pixel format
	if(!SetWindowPixelFormat())
	{
		return FALSE;
	}
	//Create Rendering Context
	m_hGLContext = ::wglCreateContext (m_pDC->GetSafeHdc ());
	//Failure to Create Rendering Context
	if(m_hGLContext == 0)
	{
		MessageBox("Error Creating RC");
		return FALSE;
	}
	//Make the RC Current
	if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hGLContext)==FALSE)
	{
		MessageBox("Error making RC Current");
		return FALSE;
	}
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glClearColor(0.5,0.0,1.0,1.0);
	glClear (GL_COLOR_BUFFER_BIT);
	LoadGLTextures();
	return TRUE;
}
//画叶片
void CWindmillView::DrawFlabellum(COLORREF Color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	GLfloat c1 =(GLfloat) GetRValue(Color)/255;
	GLfloat c2 =(GLfloat) GetGValue(Color)/255;
	GLfloat c3 =(GLfloat) GetBValue(Color)/255;
	glColor4f(c1,c2,c3,Alpha);	//设置叶片颜色
	float P1[3] = {0.4,0.4,0.0};
	float P2[3] = {0.4,2.8,0.0};
	float P3[3] = {-0.4,2.8,0.0};
	float P4[3] = {-0.4,0.4,0.0};

	float P5[3] = {0.0,0.0,0.0};
	float P6[3] = {0.4,2.8,0.0};
	float P7[3] = {-0.4,1.0,0.0};
	glNormal3f(0.0,0.0,1.0);
	switch(VaneKind)
	{
	case 1:		
		glBegin(GL_QUADS);
			glVertex3fv (P1);
			glVertex3fv (P2);
			glVertex3fv (P3);
			glVertex3fv (P4);
		glEnd();
		break;
	case 2:
		glBegin(GL_TRIANGLES);
			glVertex3fv (P5);
			glVertex3fv (P6);
			glVertex3fv (P7);
		glEnd();
		break;
	default:
		break;
	}
	glDisable(GL_BLEND);
}
//绘制背景
void CWindmillView::Background()
{
	glColor3f(1.0,1.0,1.0);//每次贴纹理之前将颜色初始化，否则上一次画叶片的颜色会覆盖当前绘图颜色
	if(UseTexture == true)
	{
		glEnable(GL_TEXTURE_2D);
	}
	if(UseTexture == false)
	{
		glColor3f(0.3,0.2,0.4);
	}
	glBindTexture(GL_TEXTURE_2D, texture[0]); // 选择纹理//绘制左侧背景
	glPushMatrix();
	glRotatef(-45.0,0.0,1.0,0.0);
	glNormal3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3i(0,0,BG_Width);// 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3i(0,0,0); // 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3i(0,BG_Width,0); // 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3i(0,BG_Width,BG_Width);// 纹理和四边形的左上
	glEnd();
	glPopMatrix();

	if(UseTexture == false)
	{
		glColor3f(0.5,0.2,0.2);
	}
	glBindTexture(GL_TEXTURE_2D, texture[1]); // 选择纹理//绘制右侧背景
	glPushMatrix();
	glRotatef(-45.0,0.0,1.0,0.0);
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3i(0,0,0); // 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3i(BG_Width,0,0);// 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3i(BG_Width,BG_Width,0); // 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3i(0,BG_Width,0); // 纹理和四边形的左上
	glEnd();
	glPopMatrix();

	if(UseTexture == false)
	{
		glColor3f(0.3,0.5,0.7);
	}
	glBindTexture(GL_TEXTURE_2D, texture[2]); // 选择纹理//绘制底面背景
	glPushMatrix();
	glRotatef(-45.0,0.0,1.0,0.0);
	glNormal3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3i(0,0,BG_Width);// 纹理和四边形的左下
		glTexCoord2f(1.0f, 0.0f); glVertex3i(BG_Width,0,BG_Width);// 纹理和四边形的右下
		glTexCoord2f(1.0f, 1.0f); glVertex3i(BG_Width,0,0); // 纹理和四边形的右上
		glTexCoord2f(0.0f, 1.0f); glVertex3i(0,0,0); // 纹理和四边形的左上
	glEnd();
	glPopMatrix();

	if(UseTexture == true)
	{
		glDisable(GL_TEXTURE_2D);
	}
}
//绘制底座
void CWindmillView::DrawBase()
{
	GLUquadricObj *g_text; 
	g_text = gluNewQuadric(); 
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
	glTranslatef(0.0,0.0,6.6);

	glRectf(-0.35,0.0,0.35,3.45);

	glBegin(GL_POLYGON);
	glVertex3f(-0.35,3.45,0.0);
	glVertex3f(0.35,3.45,0.0);
	glVertex3f(0.35,3.45,-0.5);
	glVertex3f(-0.35,3.45,-0.5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(0.35,3.45,0.0);
	glVertex3f(0.35,0.0,0.0);
	glVertex3f(0.35,0.0,-0.5);
	glVertex3f(0.35,3.45,-0.5);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,3.0,6.6);
	gluCylinder(g_text,0.38f,0.0f,1.0f,32.0f,16.0f);
	glPopMatrix();
}
//绘制风车以及整个场景
void CWindmillView::DrawWindmill()
{
	glClear (GL_COLOR_BUFFER_BIT);
	Background();
	DrawBase();

	int Accum = 1;
	if(MotionFlur == TRUE)
	{
		Accum = Sp/20+1;
	}
	for(int i=0;i!=Accum;i++)
	{
		if(MotionFlur == TRUE)//操作运动模糊的颜色，使得存留时间越长的越淡
		{
			Alpha =((float)(i+1))/Accum*((float)(i+1))/Accum;
		}
		glPushMatrix();//画风车第一个叶
		glTranslatef(0.0,2.0,6.0);
		glRotatef(InitAngle-(Accum-i-1)*2,0.0,0.0,1.0);
		DrawFlabellum(Color1);
		glPopMatrix();
	
		glPushMatrix();//画风车第二个叶
		glTranslatef(0.0,2.0,6.0);
		glRotatef(InitAngle+90.0-(Accum-i-1)*2,0.0,0.0,1.0);//旋转之后坐标轴变化了，x ，y轴方向都变了九十度
		DrawFlabellum(Color2);
		glPopMatrix();

		glPushMatrix();//画风车第三个叶
		glTranslatef(0.0,2.0,6.0);
		glRotatef(InitAngle+180.0-(Accum-i-1)*2,0.0,0.0,1.0);
		DrawFlabellum(Color3);
		glPopMatrix();

		glPushMatrix();//画风车第四个叶
		glTranslatef(0.0,2.0,6.0);
		glRotatef(InitAngle+270.0-(Accum-i-1)*2,0.0,0.0,1.0);
		DrawFlabellum(Color4);
		glPopMatrix();
	}
	if(MotionFlur ==TRUE)
	{
		Alpha = 1.0;
	}
	glFinish();
}
bool CWindmillView::SetWindowPixelFormat()
{
	 static PIXELFORMATDESCRIPTOR pfd = 
	{
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
        PFD_SUPPORT_OPENGL |		    // support OpenGL
        PFD_DOUBLEBUFFER,				// double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        32,                             // 32-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,0, 0, 0, 0,                         // no accumulation buffer                 
        32,                             // 32-bit z-buffer
        0,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // 保留结构数
        0, 0, 0                         //不支持结构数
    };
    
    int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	if ( m_nPixelFormat == 0 )
	{
		m_nPixelFormat = 1;
        if(DescribePixelFormat(m_pDC->GetSafeHdc(),m_nPixelFormat,sizeof(PIXELFORMATDESCRIPTOR),&pfd)==0)
        {
            return FALSE;
        }
	}

    if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
    {
       return FALSE;
	}
    return TRUE;
}
int CWindmillView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	OpenGLInit();
	return 0;
}
void CWindmillView::OnDestroy()
{
	CView::OnDestroy();
	if(wglGetCurrentContext()!=NULL)
    {
        wglMakeCurrent(NULL,NULL);
    }
    if(this->m_hGLContext!=NULL)
    {
        wglDeleteContext(this->m_hGLContext);
        this->m_hGLContext = NULL;
    }
	// TODO: 在此处添加消息处理程序代码
}
void CWindmillView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	GLsizei width,height;
    GLdouble aspect;
    width = cx;
    height = cy;
    if(cy==0)
    {
        aspect = (GLdouble)width;
    }
    else
    {
        aspect = (GLdouble)width/(GLdouble)height;
    }
	glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);//投影变换
	glPushMatrix();
	glLoadIdentity();
    gluPerspective(65.0,aspect,4.0,20.0);

    glMatrixMode(GL_MODELVIEW);//模型，视图变换
	gluLookAt(0.0f,1.0f,4.0f,0.0,0.0,0.0,0.0f,1.0f,0.0f);

	UseLight();
	glColorMaterial(GL_FRONT,GL_DIFFUSE); 
    glEnable(GL_COLOR_MATERIAL); 
}
void CWindmillView::OnSetColor()
{
	// TODO: 在此添加命令处理程序代码
	SetColorDlg ColorDlg;
	if(ColorDlg.DoModal()== TRUE)
	{
		Color1 = Data.C1;
		Color2 = Data.C2;
		Color3 = Data.C3;
		Color4 = Data.C4;
	}
	PostMessage(WM_PAINT);
}
void CWindmillView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 1)//每0.01秒启动的事件
	{	
		if(Ac == 0)//如果加速度为无穷大
		{			
			InitAngle += Sp/100.0;//每0.01秒转动Step
		}		
		if(Ac > 0)//如果加速度为正
		{
			if(T < Sp / Ac)
			{
				InitAngle+= Ac/100.0*T;
				T++;
			}
			else
			{
				InitAngle+= Ac/100.0*T;
			}
		}
		if(Ac < 0)//如果加速度为负
		{
			if(T<(Sp/(-Ac))*100)
			{
				InitAngle+=Sp/100+Ac/10000.0*T;
				T++;
			}
			else
			{
				this->KillTimer(1);
			}
		}
		PostMessage(WM_PAINT);
	}
	CView::OnTimer(nIDEvent);
}
void CWindmillView::OnRotate()
{
	// TODO: 在此添加命令处理程序代码
	Ac = fabs(Ac);
	this->SetTimer(1,10,NULL);//每0.01秒转动一个角度
}
void CWindmillView::OnStop()
{
	// TODO: 在此添加命令处理程序代码
	if(Ac == 0)
	{
		this->KillTimer(1);
	}
	else
	{
		Ac = -fabs(Ac);
		T =0;
	}
}
void CWindmillView::OnRotateSpeed()
{
	// TODO: 在此添加命令处理程序代码
	RotateSpeed Dlg;
	if(Dlg.DoModal() == IDOK)
	{
		T = 0;
		Sp = atof(Data.S.GetBuffer(0));
		Ac = atof(Data.Acce.GetBuffer(0));
	}
	this->KillTimer(1);
	this->KillTimer(2);
	OnRotate();
}
void CWindmillView::OnEnableMotionFlur()
{
	// TODO: 在此添加命令处理程序代码
	MotionFlur = TRUE;
}
void CWindmillView::OnDisableMotionFlur()
{
	// TODO: 在此添加命令处理程序代码
	MotionFlur = FALSE;
}
AUX_RGBImageRec* CWindmillView::LoadBMP(char *Filename)//载入位图图像
{
	FILE *File=NULL;
	if(!Filename)
	{
		return NULL;
	}
	File=fopen(Filename,"r");
	if(File)
	{
		fclose(File);
		return auxDIBImageLoad(Filename);
	}
	return NULL;
}
int CWindmillView::LoadGLTextures()
{
	int Status=FALSE;
	AUX_RGBImageRec *TextureImage[3];
	memset(TextureImage,0,sizeof(void*)*3);
	if((TextureImage[0]=LoadBMP("res\\BGleft.bmp"))&&
		(TextureImage[1]=LoadBMP("res\\BGright.bmp"))&&
		(TextureImage[2]=LoadBMP("res\\BGbottom.bmp")))
	{
		Status=TRUE;
		for(int i=0;i!=3;i++)
		{
			glGenTextures(1,&texture[i]);
			glBindTexture(GL_TEXTURE_2D,texture[i]);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,TextureImage[i]->sizeX,
			TextureImage[i]->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,TextureImage[i]->data);
			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		}
	}

	for(int i=0;i!=3;i++)
	{
		if(TextureImage[i])
	   {
			if(TextureImage[i]->data)
			{
				free(TextureImage[i]->data);
			}
			free(TextureImage[i]);
	    }
	}
	return Status;
}
//启用纹理
void CWindmillView::OnTexture()
{
	// TODO: 在此添加命令处理程序代码
	UseTexture = !UseTexture;
	PostMessage(WM_PAINT);
}
//启用光源
int CWindmillView::UseLight()
{
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);//定义光源
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	
	glMaterialfv(GL_FRONT,GL_AMBIENT,am);//定义材质，否则全灰色
	glMaterialfv(GL_FRONT,GL_DIFFUSE,di);
	glMaterialfv(GL_FRONT,GL_SPECULAR,sp);
	glMaterialfv(GL_FRONT,GL_SHININESS,&shininess);//镜面指数越大，高光区越小,shininess为0时基本只能看到物体镜面反射光了

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,1.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT1);
	return 1;
}
void CWindmillView::OnChangeVane()
{
	VaneKind = VaneKind%2+1;
	PostMessage(WM_PAINT);
	// TODO: 在此添加命令处理程序代码
}
