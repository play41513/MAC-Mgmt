//---------------------------------------------------------------------------

#ifndef StringProcH
#define StringProcH
#include <tchar.h>
#include <direct.h>
//---------------------------------------------------------------------------
class StringProc
{
public:
	void SetSNFormat(AnsiString String);
	void NewFilePath(AnsiString Path);
	int  GetSNNumber(AnsiString SN);
	bool CheckStringNumber(AnsiString String);
	bool CheckStringNotNumber(AnsiString String);
	bool CheckSNFormat(AnsiString str,bool bChangeFormat);
	bool CheckValueIsRange(AnsiString SNKeyIn,AnsiString StartCode,AnsiString LastCode);
	bool CheckStringIsMAC(AnsiString String);
	bool CheckMACRange(AnsiString nowMAC,AnsiString maxMAC,AnsiString minMAC);
	AnsiString ReturnSNFormat();
	int  HexToInt(AnsiString HexStr);
	AnsiString GetSNString(int Number);
	AnsiString CheckDiskName(TPanel* plWorkOrderNumber);
	AnsiString SNFormat;
private:
};
#endif
