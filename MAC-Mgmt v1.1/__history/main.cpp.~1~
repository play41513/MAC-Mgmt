//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "MSGBOX.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::edtBarcodeValueKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key == KEY_ENTER)
	{
		lbErrorMsg->Caption = "";
		if(edtBarcodeValue->Text.Length() == 12)
		{
			if(StrProc->CheckStringIsMAC(edtBarcodeValue->Text))
			{
				if(plBurnMAC->Caption == ""
				|| plResult->Color != clCream)
				{
					plBurnMAC->Caption = edtBarcodeValue->Text;
					plSN->Caption = "";
					plMAC->Caption = "";
					plBurnMAC->Font->Color = clBlue;
					plSN->Font->Color = clBlue;
					plMAC->Font->Color = clBlue;
					plResult->Caption = "Wait...";
					plResult->Color = clCream;
				}
				else if(plSN->Caption != "" && plMAC->Caption == "")
				{
					plMAC->Caption = edtBarcodeValue->Text;
					switch(CheckLogFile(false))
					{
						case KEYIN_PASS:
							writeLOG("PASS");
							break;
						case KEYIN_REPEAT_MAC:
							lbErrorMsg->Caption = "(!)MAC碼已重複";
							plBurnMAC->Font->Color = clRed;
							plMAC->Font->Color = clRed;
							break;
						case KEYIN_REPEAT_SN:
							lbErrorMsg->Caption = "(!)SN碼已重複";
							plSN->Font->Color = clRed;
							break;
						case KEYIN_OVERRANGE:
							lbErrorMsg->Caption = "(!)MAC碼超出範圍";
							plBurnMAC->Font->Color = clRed;
							plMAC->Font->Color = clRed;
							break;
						case KEYIN_ERROR:
							lbErrorMsg->Caption = "(!)MAC碼不符";
							plBurnMAC->Font->Color = clRed;
							plMAC->Font->Color = clRed;
							break;
						case KEYIN_SN_ERROR:
							lbErrorMsg->Caption = "(!)SN格式與LOG不符";
							plSN->Font->Color = clRed;
							break;
					}
					if(lbErrorMsg->Caption != "")
					{
						writeLOG(lbErrorMsg->Caption);
						plResult->Caption = "FAIL";
						plResult->Color = clRed;
					}
					else
					{
						if(ImgDisk->Visible)
						{
							plResult->Caption = "PASS";
							plResult->Color = clGreen;
						}
                    }
				}
				else if(plBurnMAC->Caption != "" && plSN->Caption == "")
				{
					plBurnMAC->Caption = edtBarcodeValue->Text;
					plSN->Caption = "";
					plMAC->Caption = "";
					plBurnMAC->Font->Color = clBlue;
					plSN->Font->Color = clBlue;
					plMAC->Font->Color = clBlue;
					plResult->Caption = "Wait...";
					plResult->Color = clCream;
                }
			}
			else lbErrorMsg->Caption = "(!)MAC 輸入格式錯誤";
		}
		else if(edtBarcodeValue->Text.Length() == 11)
		{
			if(plBurnMAC->Caption != "" && plSN->Caption == "" && plMAC->Caption == "")
			{
				if(StrProc->CheckSNFormat(edtBarcodeValue->Text,false))
				{
					plSN->Caption = edtBarcodeValue->Text;
				}
				else lbErrorMsg->Caption = "(!)SN 輸入格式錯誤";
			}
		}
		else lbErrorMsg->Caption = "(!)輸入格式錯誤";
		edtBarcodeValue->SelectAll();
		if(!ImgDisk->Visible)
			ShowWorkOrderForm();
    }
}
//---------------------------------------------------------------------------
void TfrmMain::ShowWorkOrderForm()
{
	if(frmMsg==NULL)  frmMsg = new TfrmMsg(Application);

	frmMsg->edtSetWorkOrderNumber->Text = plWorkOrderNumber->Caption;
	frmMsg->edtStartCode->Text = plStartCode->Caption;
	frmMsg->edtLastCode->Text = plLastCode->Caption;
	frmMsg->edtSetEmployeeID->Text = plEmployeeID->Text.Trim();

	if(plWorkMode->Caption.Pos("一般"))
	{
		frmMsg->plNormalMode->Color = clLime;
		frmMsg->plReworkMode->Color = clCream;
	}
	else
	{
		frmMsg->plNormalMode->Color = clCream;
		frmMsg->plReworkMode->Color = clLime;
	}

	if(frmMsg->ShowModal()== mrOk)
	{
		plWorkOrderNumber->Caption = frmMsg->edtSetWorkOrderNumber->Text.Trim();
		plEmployeeID->Text = frmMsg->edtSetEmployeeID->Text.Trim();
		plStartCode->Caption = frmMsg->edtStartCode->Text;
		plLastCode->Caption = frmMsg->edtLastCode->Text;
		//
		LOGFilePath = "C:\\ASMP\\log\\"+plWorkOrderNumber->Caption+"\\MAC-Mgmt(DS-UCC6)";
		LogDiskPath = StrProc->CheckDiskName(plWorkOrderNumber);
		//
		plWorkMode->Caption = frmMsg->plNormalMode->Color == clLime ? "一般模式":"重工模式";
		if(frmMsg->plNormalMode->Color != clLime)
		{   //重工模式
			bReWork = true;
			LOGFilePath += "\\ReWork";
			if(LogDiskPath != "")
				LogDiskPath += "\\ReWork";
		}
		else	bReWork = false;

		FindLogFile();
		SetRegValue();
		lbErrorMsg->Caption = "";
		plSN->Caption = "";
		plMAC->Caption = "";
		plBurnMAC->Caption = "";
		plBurnMAC->Font->Color = clBlue;
		plSN->Font->Color = clBlue;
		plMAC->Font->Color = clBlue;
		plResult->Caption = "Wait...";
		plResult->Color = clCream;
		edtBarcodeValue->Text = "";
		CheckLogFile(true);
		plCount->Caption = "數量 : "+AnsiString(dwTestCount);

		delete frmMsg;
		frmMsg = NULL;
	}
	else
	{
		delete frmMsg;
		frmMsg = NULL;
		Close();
	}
}
void TfrmMain::FindLogFile()
{
	//
	StrProc->NewFilePath(LOGFilePath);
	StrProc->NewFilePath(LogDiskPath);
	//
	TSearchRec Sr;
	AnsiString FileName = plStartCode->Caption+"-"+plLastCode->Caption+".csv";
	if(DirectoryExists(LogDiskPath.c_str()))
	{
		ImgDisk->Visible = true;
		if(FindFirst(LogDiskPath+"\\*.csv",faAnyFile,Sr)==0)
		{
			do
			{
				if(Sr.Name == FileName)
				{
					LogDiskPath += "\\"+Sr.Name;
					LOGFilePath += "\\"+Sr.Name;
					break;
				}
			}
			while(FindNext(Sr)==0);
			FindClose(Sr);
		}

		if(!LogDiskPath.Pos(".csv"))
		{
			LogDiskPath += "\\"+FileName;
			LOGFilePath += "\\"+FileName;
		}
		else//檔案已存在於USBDisk
		{
			if(!FileExists(LOGFilePath))
			{
				CopyFile(LogDiskPath.c_str(),LOGFilePath.c_str(),true);
			}
		}
	}
	else
	{
		ImgDisk->Visible = false;
		if(DirectoryExists(LOGFilePath.c_str()))
		{
			if(FindFirst(LOGFilePath+"\\*.csv",faAnyFile,Sr)==0)
			{
				do
				{
					if(Sr.Name == FileName)
					{
						LOGFilePath += "\\"+Sr.Name;
						break;
					}
				}
				while(FindNext(Sr)==0);
				FindClose(Sr);
			}
		}
		if(!LOGFilePath.Pos(".csv"))
		{
			LOGFilePath += "\\"+FileName;
		}
	}
}
void TfrmMain::SetRegValue()
{
	TRegistry *reg = new TRegistry;
	AnsiString SS;
	reg->RootKey = HKEY_CURRENT_USER;
	reg->OpenKey("SOFTWARE", false);
	bool bl = reg->OpenKey("MAC-Mgmt(DS-UCC6)", true);
	bl = reg->OpenKey("v1.0", true);
	if(bl)
	{
		reg->WriteString("WorkOrderNumber", plWorkOrderNumber->Caption );
		reg->WriteString("START_CODE", plStartCode->Caption );
		reg->WriteString("LAST_CODE", plLastCode->Caption );
	}
	delete reg;
}
void TfrmMain::ReadRegValue()
{
	AnsiString value;
	TRegistry *reg = new TRegistry();

	try
	{
		reg->RootKey = HKEY_CURRENT_USER;
		reg->OpenKey("SOFTWARE", false);
		bool bl = reg->OpenKey("MAC-Mgmt(DS-UCC6)", true);
		bl = reg->OpenKey("v1.0", false);
		if(bl)
		{
			if(reg->ValueExists("WorkOrderNumber")){
				value = reg->ReadString("WorkOrderNumber");
				plWorkOrderNumber->Caption = value;
			}
			//
			if(reg->ValueExists("START_CODE")){
				value = reg->ReadString("START_CODE");
				plStartCode->Caption = value;
			}
			else plStartCode->Caption = "000000000000A";

			if(reg->ValueExists("LAST_CODE")){
				value = reg->ReadString("LAST_CODE");
				plLastCode->Caption = value;
			}
			else plLastCode->Caption = "000000000000A";
		}
	}__finally {
		delete reg;
	}
}
void __fastcall TfrmMain::FormShow(TObject *Sender)
{
	frmMain->Caption = APP_TITLE;
	StrProc = new StringProc();
	dwTestCount = 0;
	ReadRegValue();
	StrProc->SetSNFormat("______A____");
	ShowWorkOrderForm();
	edtBarcodeValue->CharCase = ecUpperCase;
	plEmployeeID->CharCase = ecUpperCase;
}
//---------------------------------------------------------------------------
bool TfrmMain::writeLOG(AnsiString Msg)
{
	bool bPASS[2] = {true,true};
	AnsiString SS,Temp;
	AnsiString time = FormatDateTime("mm-dd-yyyy hh:nn:ss", Now());


	if(Msg=="PASS")
	{
		dwTestCount++;
		plCount->Caption = "數量 : "+AnsiString(dwTestCount);
	}
	AnsiString strTemp;
	strTemp.sprintf("%04d",dwTestCount);
	if(Msg.Pos("PASS"))
	{
		SS = strTemp+",[PASS],"+time
			+",[BurnMAC],"+plBurnMAC->Caption;
		SS+= ",[MAC],"+plMAC->Caption+",[SN],"+plSN->Caption;
		SS+= ",[StartCode],"+plStartCode->Caption+",[LastCode],"+plLastCode->Caption+",[WorkOrder],"+plWorkOrderNumber->Caption+",[EmployeeID],"+plEmployeeID->Text;
		SS += "\n";
	}
	else
	{
		SS =  "0000,[FAIL],"+time
			+",[BurnMAC],"+plBurnMAC->Caption;
		SS+= ",[MAC],"+plMAC->Caption+",[SN],"+plSN->Caption;
		SS+= ",[StartCode],"+plStartCode->Caption+",[LastCode],"+plLastCode->Caption+",[WorkOrder],"+plWorkOrderNumber->Caption	+",[EmployeeID],"+plEmployeeID->Text;
		SS += ",[ERROR],"+Msg+"\n";
	}

	if(LogDiskPath.Pos("csv") && ImgDisk->Visible )
	{
		bPASS[1] = SaveUserLogLine(LogDiskPath,SS);
		if(!bPASS[1])
		{
			ImgDisk->Visible = false;
			MessageBoxA(Handle,"偵測不到LOG隨身碟", "ERROR", MB_OK);
		}
		else bPASS[0] = SaveUserLogLine(LOGFilePath,SS);
	}
	for(int i = 0 ; i < 2 ; i++)
		if(!bPASS[i]) return false;

	return true;
}
bool  TfrmMain::SaveUserLogLine(AnsiString FileName,AnsiString FileLine)
{
	FILE * fp;
	fp = fopen(FileName.c_str(),"a+");
	if(NULL == fp)
		return false;
	fseek( fp, 0, SEEK_END);
	fwrite(FileLine.c_str(),FileLine.Length(),1,fp); //寫入一筆資料
	fclose(fp);
	return true;
}
DWORD	TfrmMain::CheckLogFile(bool bInit)
{
	if(plBurnMAC->Caption != plMAC->Caption && !bInit)
		return KEYIN_ERROR;
	if(!StrProc->CheckMACRange(plBurnMAC->Caption,plLastCode->Caption,plStartCode->Caption) && !bInit)
		return KEYIN_OVERRANGE;
	AnsiString LOG_MAC = "";
	AnsiString LOG_SN = "";
	if(bReWork) return KEYIN_PASS;
	if(FileExists(LOGFilePath))
	{
		ifstream lanfile(LOGFilePath.c_str());
		std::string filemsg;
		if (lanfile.is_open()) {
			while (!lanfile.eof())
			{
				getline(lanfile, filemsg);
				if (strstr(filemsg.c_str(), "PASS"))
				{
					LOG_MAC = AnsiString(filemsg.c_str());
					dwTestCount = LOG_MAC.SubString(1,4).ToInt();
					LOG_MAC = LOG_MAC.SubString(LOG_MAC.Pos("[MAC]")+6,LOG_MAC.Pos(",[SN]")-LOG_MAC.Pos("[MAC]")-6).Trim();
					if(plMAC->Caption == LOG_MAC && !bInit)
					{
						lanfile.close();
						return KEYIN_REPEAT_MAC;
					}
					LOG_SN = AnsiString(filemsg.c_str());
					LOG_SN = LOG_SN.SubString(LOG_SN.Pos("[SN]")+5,LOG_SN.Pos(",[StartCode]")-LOG_SN.Pos("[SN]")-5).Trim();
					if(plSN->Caption == LOG_SN && !bInit)
					{
						lanfile.close();
						return KEYIN_REPEAT_SN;
					}
					if(StrProc->ReturnSNFormat() == "______A____")
					{
						StrProc->SetSNFormat(LOG_SN.SubString(1,6)+"A____");
						if(!StrProc->CheckSNFormat(plSN->Caption,false) && !bInit)
						{
							lanfile.close();
							return KEYIN_SN_ERROR;
						}
					}
				}
			}
			lanfile.close();
		}
	}
	if(FileExists(LogDiskPath))
	{
		ifstream lanfile(LogDiskPath.c_str());
		std::string filemsg;
		if (lanfile.is_open()) {
			while (!lanfile.eof())
			{
				getline(lanfile, filemsg);
				if (strstr(filemsg.c_str(), "PASS"))
				{
					LOG_MAC = AnsiString(filemsg.c_str());
					dwTestCount = LOG_MAC.SubString(1,4).ToInt();
					LOG_MAC = LOG_MAC.SubString(LOG_MAC.Pos("[MAC]")+6,LOG_MAC.Pos(",[SN]")-LOG_MAC.Pos("[MAC]")-6).Trim();
					if(plMAC->Caption == LOG_MAC && !bInit)
					{
						lanfile.close();
						return KEYIN_REPEAT_MAC;
					}
					LOG_SN = AnsiString(filemsg.c_str());
					LOG_SN = LOG_SN.SubString(LOG_SN.Pos("[SN]")+5,LOG_SN.Pos(",[StartCode]")-LOG_SN.Pos("[SN]")-5).Trim();
					if(plSN->Caption == LOG_SN && !bInit)
					{
						lanfile.close();
						return KEYIN_REPEAT_SN;
					}
					if(StrProc->ReturnSNFormat() == "______A____")
					{
						StrProc->SetSNFormat(LOG_SN.SubString(1,6)+"A____");
						if(!StrProc->CheckSNFormat(plSN->Caption,false) && !bInit)
						{
							lanfile.close();
							return KEYIN_SN_ERROR;
						}
					}
				}

			}
			lanfile.close();
		}
	}
	return KEYIN_PASS;
}
void __fastcall TfrmMain::popChangeIDClick(TObject *Sender)
{
	lbErrorMsg->Caption = "";
	plSN->Caption = "";
	plMAC->Caption = "";
	plBurnMAC->Caption = "";
	plBurnMAC->Font->Color = clBlue;
	plSN->Font->Color = clBlue;
	plMAC->Font->Color = clBlue;
	plResult->Caption = "Wait...";
	plResult->Color = clCream;
	edtBarcodeValue->Text = "";
	plEmployeeIDPanel->Enabled = true;
	edtBarcodeValue->Enabled = false;
	plEmployeeID->SetFocus();
	plEmployeeID->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::plEmployeeIDKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key==13)
	{
		if(plEmployeeID->Text.Length()== 6)
		{
			if(plEmployeeID->Text.SubString(1,1).Pos("T"))
			{
				plEmployeeIDPanel->Enabled = false;
				edtBarcodeValue->Enabled = true;
				edtBarcodeValue->SetFocus();
				edtBarcodeValue->SelectAll();
			}
			else plEmployeeID->Text = "";
		}
		else if(plEmployeeID->Text.Length()== 7)
		{
			if(plEmployeeID->Text.SubString(1,2).Pos("SA")
				||plEmployeeID->Text.SubString(1,2).Pos("UA")
				||plEmployeeID->Text.SubString(1,2).Pos("UB")
				||plEmployeeID->Text.SubString(1,2).Pos("XB")
				||plEmployeeID->Text.SubString(1,2).Pos("XA"))
			{
				plEmployeeIDPanel->Enabled = false;
				edtBarcodeValue->Enabled = true;
				edtBarcodeValue->SetFocus();
				edtBarcodeValue->SelectAll();
			}
			else plEmployeeID->Text = "";
		}
		else plEmployeeID->Text = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::popBackWOInfoClick(TObject *Sender)
{
	ShowWorkOrderForm();
}
//---------------------------------------------------------------------------

