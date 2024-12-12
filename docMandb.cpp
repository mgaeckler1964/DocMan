//---------------------------------------------------------------------------
#include <gak/logfile.h>

#include <vcl.h>
#pragma hdrstop
USERES("docMandb.res");
USEFORM("..\GAKLIB\Repository\dbConvertFrm.cpp", DbConvertForm);
USELIB("..\..\Object\gaklib\gaklib_bcb.lib");
USEFORM("DocManDBfrm.cpp", DocManDbConvertForm);
USEFORM("..\GAKLIB\Repository\Config.cpp", ConfigDataModule); /* TDataModule: File Type */
USEFORM("..\GAKLIB\Repository\LoginDlg.cpp", LoginForm);
USEFORM("..\GAKLIB\Repository\StatusFrm.cpp", StatusForm);
USELIB("..\..\Object\winlib\winlibBCB.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	doEnableLog();
	try
	{
		Application->Initialize();
		Application->Title = "DocMan Database Administration";
		Application->CreateForm(__classid(TDocManDbConvertForm), &DocManDbConvertForm);
		Application->CreateForm(__classid(TConfigDataModule), &ConfigDataModule);
		Application->CreateForm(__classid(TLoginForm), &LoginForm);
		Application->CreateForm(__classid(TStatusForm), &StatusForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------








