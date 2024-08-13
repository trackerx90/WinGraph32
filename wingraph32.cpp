//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("wingraph32.res");
USEFORM("Unit1.cpp", Form1);
USEUNIT("alloc.c");
USEUNIT("draw.c");
USEUNIT("drawlib.c");
USEUNIT("drawstr.c");
USEUNIT("fisheye.c");
USEUNIT("folding.c");
USEUNIT("grprint.c");
USEUNIT("grprint2.c");
USEUNIT("grprintstr.c");
USEUNIT("infobox.c");
USEUNIT("lex.yy.c");
USEUNIT("main.c");
USEUNIT("options.c");
USEUNIT("prepare.c");
USEUNIT("step0.c");
USEUNIT("step1.c");
USEUNIT("step2.c");
USEUNIT("step3.c");
USEUNIT("step4.c");
USEUNIT("timelim.c");
USEUNIT("tree.c");
USEUNIT("usrsignal.c");
USEUNIT("y.tab.c");
USEFORM("about.cpp", AboutForm);
USEFORM("help.cpp", HelpForm);
USEFORM("wait.cpp", WaitForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "WinGraph32";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TAboutForm), &AboutForm);
                 Application->CreateForm(__classid(THelpForm), &HelpForm);
                 Application->CreateForm(__classid(TWaitForm), &WaitForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------

