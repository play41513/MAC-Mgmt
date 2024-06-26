//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "ConstantString.h"
#include "StringProc.h"
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Registry.hpp>
#include <pngimage.hpp>
#include <Menus.hpp>
#include <fstream>
#include <string>
#include <stdio.h>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *plRight;
	TPanel *Panel32;
	TPanel *plWorkOrderNumber;
	TPanel *Panel44;
	TPanel *plEmployeeIDPanel;
	TPanel *Panel50;
	TPanel *plLastCode;
	TPanel *Panel56;
	TPanel *plStartCode;
	TPanel *plWorkMode;
	TPanel *Panel28;
	TImage *Image3;
	TPanel *plBarcode;
	TPanel *Panel1;
	TEdit *edtBarcodeValue;
	TPanel *Panel26;
	TPanel *plBurnMAC;
	TPanel *plResult;
	TLabel *lbErrorMsg;
	TEdit *plEmployeeID;
	TPopupMenu *PopupMenu;
	TMenuItem *popChangeID;
	TMenuItem *popBackWOInfo;
	TPanel *plCount;
	TImage *ImgDisk;
	void __fastcall edtBarcodeValueKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall popChangeIDClick(TObject *Sender);
	void __fastcall plEmployeeIDKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall popBackWOInfoClick(TObject *Sender);
	void __fastcall edtBarcodeValueClick(TObject *Sender);


private:	// User declarations
	void TfrmMain::ShowWorkOrderForm();
	void TfrmMain::FindLogFile();
	void TfrmMain::SetRegValue();
	void TfrmMain::ReadRegValue();
	bool TfrmMain::writeLOG(AnsiString Msg);
	bool TfrmMain::SaveUserLogLine(AnsiString FileName,AnsiString FileLine);
	DWORD	TfrmMain::CheckLogFile(bool bInit);
public:		// User declarations
	StringProc *StrProc;
	AnsiString LOGFilePath,LogDiskPath,RDLOGFilePath,RDLogDiskPath;
	bool bReWork;
	DWORD dwTestCount;
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
