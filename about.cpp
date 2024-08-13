//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "about.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutForm *AboutForm;
//---------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::Label1Click(TObject *Sender)
{
  ShellExecute(Application->Handle, "open", Label1->Caption.c_str(),
                        NULL, NULL, SW_SHOWDEFAULT);
}
//---------------------------------------------------------------------------

