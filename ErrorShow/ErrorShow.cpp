#include "ErrorShow.h"
#include "resource.h"

void ZoDialogIcon(HWND Dialog, int IconIdentify);
INT WINAPI _tWinMain(
	_In_ HINSTANCE Instance,
	_In_opt_ HINSTANCE PrevInstance,
	_In_ PTSTR CmdLine,
	_In_ INT CmdShow
)
{  

	//生成一个对话框
	//NULL 系统默认
	DialogBoxParam(Instance, MAKEINTRESOURCE(IDD_ERROR_SHOW), //ID_DIALOG
			NULL, CallbackProcedure, NULL);

	//窗口回调函数---》处理窗口上的消息

	return(0);
}
INT_PTR CALLBACK CallbackProcedure(HWND Dialog,  // handle to dialog box
	UINT Message,     // message
	WPARAM ParameterData1, // first message parameter
	LPARAM ParameterData2  // second message parameter
)
{


	switch (Message)
	{
	case WM_INITDIALOG:
	{

		ZoDialogIcon(Dialog, IDI_ERROR_SHOW);

		HWND Edit = GetDlgItem(Dialog, IDC_ERROR_CODE);  //获得一个控件的句柄
		Edit_LimitText(Edit, 8);   //限制输入字符

		break;
	}
	case WM_COMMAND:
	{
		int ControlIdentify = LOWORD(ParameterData1);

		switch (ControlIdentify)
		{
		case IDOK:
		{
		
			// Get the error code
			DWORD ErrorCode = GetDlgItemInt(Dialog, IDC_ERROR_CODE, NULL, FALSE);

			HLOCAL LocalHandle = NULL;   // Buffer that gets the error message string

			// Use the default system locale since we look for Windows messages.
			// Note: this MAKELANGID combination has 0 as value
			DWORD SystemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);

			// Get the error code's textual description
			BOOL IsOk = FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				NULL, ErrorCode, SystemLocale,
				(PTSTR)&LocalHandle, 0, NULL);

			if (!IsOk) {
				// Is it a network-related error?
				HMODULE Module = LoadLibraryEx(_T("netmsg.dll"), NULL,
					DONT_RESOLVE_DLL_REFERENCES);

				if (Module != NULL) {
					IsOk = FormatMessage(
						FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
						FORMAT_MESSAGE_ALLOCATE_BUFFER,
						Module, ErrorCode, SystemLocale,
						(PTSTR)&LocalHandle, 0, NULL);
					FreeLibrary(Module);
				}
			}

			if (IsOk && (LocalHandle != NULL)) {
				SetDlgItemText(Dialog, IDC_ERROR_TEXT, (PCTSTR)LocalLock(LocalHandle));
				LocalFree(LocalHandle);
			}
			else {
				SetDlgItemText(Dialog, IDC_ERROR_TEXT,
					_T("没有找到对应的错误码."));
			}

			break;

		}
		case IDCANCEL:
		{
			EndDialog(Dialog, 0);  //关闭对话框
			break;
		}
		}


		break;
	}
	case WM_CLOSE:
	{

	
		EndDialog(Dialog, 0);   //关闭对话框
		break;
	}
	}



	return 0;
}
void ZoDialogIcon(HWND Dialog, int IconIdentify)
{
	//
	HINSTANCE Instance = (HINSTANCE)GetWindowLongPtr(Dialog, GWLP_HINSTANCE);  //获取进程

	HICON Icon = LoadIcon(Instance,MAKEINTRESOURCE(IconIdentify));//加载图标到进程中
	SendMessage(Dialog, WM_SETICON, ICON_BIG, (LPARAM)Icon);
	SendMessage(Dialog, WM_SETICON, ICON_SMALL, (LPARAM)Icon);
}