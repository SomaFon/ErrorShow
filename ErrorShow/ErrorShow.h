#pragma once
#include <Windowsx.h> 
#include <windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;


INT_PTR CALLBACK CallbackProcedure(HWND Dialog,  // handle to dialog box
	UINT Message,     // message
	WPARAM ParameterData1, // first message parameter
	LPARAM ParameterData2  // second message parameter
);
