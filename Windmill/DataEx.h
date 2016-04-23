#ifndef DATAEX_H
#define DATAEX_H

class DataEx
{
public:
	COLORREF C1;
	COLORREF C2;
	COLORREF C3;
	COLORREF C4;
	CString S;
	CString Acce;

	DataEx():C1 (RGB(0,0,0)),
	C2 (RGB(255,0,0)),
	C3 (RGB(0,255,0)),
	C4 (RGB(0,0,255)),
	S(""),
	Acce(""){}
};

#endif