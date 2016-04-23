
// WindmillView.cpp : CWindmillView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 				// ���������
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };					// ��������
GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };					//���淴���
GLfloat LightPosition[]= { 0.0f, 3.5f,9.5f, 1.0f };				 // ��Դλ��
GLfloat am[]={0.2f,0.2f,0.2f,1.0f};//���ʵĻ�������ɫ
GLfloat di[]={0.8f,0.8f,0.8f,1.0f};//���ʵ����������ɫ
GLfloat sp[]={1.0f,1.0f,1.0f,1.0f};//���ʵľ��淴�����ɫ
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

// CWindmillView ����/����
CWindmillView::CWindmillView()
{
	// TODO: �ڴ˴���ӹ������
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}
// CWindmillView ����
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
// CWindmillView ���
#ifdef _DEBUG
void CWindmillView::AssertValid() const
{
	CView::AssertValid();
}
void CWindmillView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CWindmillDoc* CWindmillView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWindmillDoc)));
	return (CWindmillDoc*)m_pDocument;
}
#endif //_DEBUG

//��ʼ������
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
//��ҶƬ
void CWindmillView::DrawFlabellum(COLORREF Color)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	GLfloat c1 =(GLfloat) GetRValue(Color)/255;
	GLfloat c2 =(GLfloat) GetGValue(Color)/255;
	GLfloat c3 =(GLfloat) GetBValue(Color)/255;
	glColor4f(c1,c2,c3,Alpha);	//����ҶƬ��ɫ
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
//���Ʊ���
void CWindmillView::Background()
{
	glColor3f(1.0,1.0,1.0);//ÿ��������֮ǰ����ɫ��ʼ����������һ�λ�ҶƬ����ɫ�Ḳ�ǵ�ǰ��ͼ��ɫ
	if(UseTexture == true)
	{
		glEnable(GL_TEXTURE_2D);
	}
	if(UseTexture == false)
	{
		glColor3f(0.3,0.2,0.4);
	}
	glBindTexture(GL_TEXTURE_2D, texture[0]); // ѡ������//������౳��
	glPushMatrix();
	glRotatef(-45.0,0.0,1.0,0.0);
	glNormal3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3i(0,0,BG_Width);// ������ı��ε�����
		glTexCoord2f(1.0f, 0.0f); glVertex3i(0,0,0); // ������ı��ε�����
		glTexCoord2f(1.0f, 1.0f); glVertex3i(0,BG_Width,0); // ������ı��ε�����
		glTexCoord2f(0.0f, 1.0f); glVertex3i(0,BG_Width,BG_Width);// ������ı��ε�����
	glEnd();
	glPopMatrix();

	if(UseTexture == false)
	{
		glColor3f(0.5,0.2,0.2);
	}
	glBindTexture(GL_TEXTURE_2D, texture[1]); // ѡ������//�����Ҳ౳��
	glPushMatrix();
	glRotatef(-45.0,0.0,1.0,0.0);
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3i(0,0,0); // ������ı��ε�����
		glTexCoord2f(1.0f, 0.0f); glVertex3i(BG_Width,0,0);// ������ı��ε�����
		glTexCoord2f(1.0f, 1.0f); glVertex3i(BG_Width,BG_Width,0); // ������ı��ε�����
		glTexCoord2f(0.0f, 1.0f); glVertex3i(0,BG_Width,0); // ������ı��ε�����
	glEnd();
	glPopMatrix();

	if(UseTexture == false)
	{
		glColor3f(0.3,0.5,0.7);
	}
	glBindTexture(GL_TEXTURE_2D, texture[2]); // ѡ������//���Ƶ��汳��
	glPushMatrix();
	glRotatef(-45.0,0.0,1.0,0.0);
	glNormal3f(0.0,1.0,0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3i(0,0,BG_Width);// ������ı��ε�����
		glTexCoord2f(1.0f, 0.0f); glVertex3i(BG_Width,0,BG_Width);// ������ı��ε�����
		glTexCoord2f(1.0f, 1.0f); glVertex3i(BG_Width,0,0); // ������ı��ε�����
		glTexCoord2f(0.0f, 1.0f); glVertex3i(0,0,0); // ������ı��ε�����
	glEnd();
	glPopMatrix();

	if(UseTexture == true)
	{
		glDisable(GL_TEXTURE_2D);
	}
}
//���Ƶ���
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
//���Ʒ糵�Լ���������
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
		if(MotionFlur == TRUE)//�����˶�ģ������ɫ��ʹ�ô���ʱ��Խ����Խ��
		{
			Alpha =((float)(i+1))/Accum*((float)(i+1))/Accum;
		}
		glPushMatrix();//���糵��һ��Ҷ
		glTranslatef(0.0,2.0,6.0);
		glRotatef(InitAngle-(Accum-i-1)*2,0.0,0.0,1.0);
		DrawFlabellum(Color1);
		glPopMatrix();
	
		glPushMatrix();//���糵�ڶ���Ҷ
		glTranslatef(0.0,2.0,6.0);
		glRotatef(InitAngle+90.0-(Accum-i-1)*2,0.0,0.0,1.0);//��ת֮��������仯�ˣ�x ��y�᷽�򶼱��˾�ʮ��
		DrawFlabellum(Color2);
		glPopMatrix();

		glPushMatrix();//���糵������Ҷ
		glTranslatef(0.0,2.0,6.0);
		glRotatef(InitAngle+180.0-(Accum-i-1)*2,0.0,0.0,1.0);
		DrawFlabellum(Color3);
		glPopMatrix();

		glPushMatrix();//���糵���ĸ�Ҷ
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
        0,                              // �����ṹ��
        0, 0, 0                         //��֧�ֽṹ��
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
	// TODO: �ڴ˴������Ϣ����������
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
    glMatrixMode(GL_PROJECTION);//ͶӰ�任
	glPushMatrix();
	glLoadIdentity();
    gluPerspective(65.0,aspect,4.0,20.0);

    glMatrixMode(GL_MODELVIEW);//ģ�ͣ���ͼ�任
	gluLookAt(0.0f,1.0f,4.0f,0.0,0.0,0.0,0.0f,1.0f,0.0f);

	UseLight();
	glColorMaterial(GL_FRONT,GL_DIFFUSE); 
    glEnable(GL_COLOR_MATERIAL); 
}
void CWindmillView::OnSetColor()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent == 1)//ÿ0.01���������¼�
	{	
		if(Ac == 0)//������ٶ�Ϊ�����
		{			
			InitAngle += Sp/100.0;//ÿ0.01��ת��Step
		}		
		if(Ac > 0)//������ٶ�Ϊ��
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
		if(Ac < 0)//������ٶ�Ϊ��
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
	// TODO: �ڴ���������������
	Ac = fabs(Ac);
	this->SetTimer(1,10,NULL);//ÿ0.01��ת��һ���Ƕ�
}
void CWindmillView::OnStop()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	MotionFlur = TRUE;
}
void CWindmillView::OnDisableMotionFlur()
{
	// TODO: �ڴ���������������
	MotionFlur = FALSE;
}
AUX_RGBImageRec* CWindmillView::LoadBMP(char *Filename)//����λͼͼ��
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
//��������
void CWindmillView::OnTexture()
{
	// TODO: �ڴ���������������
	UseTexture = !UseTexture;
	PostMessage(WM_PAINT);
}
//���ù�Դ
int CWindmillView::UseLight()
{
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);//�����Դ
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	
	glMaterialfv(GL_FRONT,GL_AMBIENT,am);//������ʣ�����ȫ��ɫ
	glMaterialfv(GL_FRONT,GL_DIFFUSE,di);
	glMaterialfv(GL_FRONT,GL_SPECULAR,sp);
	glMaterialfv(GL_FRONT,GL_SHININESS,&shininess);//����ָ��Խ�󣬸߹���ԽС,shininessΪ0ʱ����ֻ�ܿ������徵�淴�����

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
	// TODO: �ڴ���������������
}
