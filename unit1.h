//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TPopupMenu *PopupMenu1;
        TActionList *ActionList1;
        TAction *cmReload;
        TPanel *Panel1;
        TAction *cmClose;
        TAction *cmZoomin;
        TAction *cmZoomout;
        TMenuItem *cm1;
        TMenuItem *Zoomout1;
        TMenuItem *Exit1;
        TAction *cmPSCF;
        TToolBar *ToolBar1;
        TImageList *ImageList1;
        TToolButton *ToolButton1;
        TAction *cmZoomNormal;
        TAction *cmZoomAll;
        TAction *cmOrigin;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TMenuItem *Zoomnormal1;
        TMenuItem *Fitallgraph1;
        TMenuItem *File1;
        TMenuItem *Zoom1;
        TMenuItem *Exit2;
        TMenuItem *Zoomin1;
        TMenuItem *Zoomout2;
        TMenuItem *Zoomnormal2;
        TMenuItem *Fitallgraph2;
        TMenuItem *Move1;
        TMenuItem *Positiononorigin1;
        TScrollBar *hbar;
        TScrollBar *vbar;
        TAction *cmFPSCF;
        TAction *cmCSCF;
        TAction *cmFCSCF;
        TAction *cmCSNF;
        TMenuItem *View1;
        TMenuItem *Normalview1;
        TMenuItem *Polarfisheye1;
        TMenuItem *Fixedradiuspolarfisheye1;
        TMenuItem *Cartesianfisheye1;
        TMenuItem *Fixedradiuscartesianfisheye1;
        TMenuItem *View2;
        TMenuItem *Normalview2;
        TMenuItem *Polarfisheye2;
        TMenuItem *Fixedradiuspolarfisheye2;
        TMenuItem *Cartesianfisheye2;
        TMenuItem *Fixedradiuscartesianfisheye2;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TAction *cmStick;
        TMenuItem *Easygraphpanning1;
        TAction *cmAbout;
        TMenuItem *Help1;
        TMenuItem *cmAbout1;
        TAction *cmHelp;
        TMenuItem *Help2;
        TToolButton *ToolButton12;
        TToolButton *ToolButton13;
        TAction *cmEdgeLabels;
        TMenuItem *N1;
        TMenuItem *Displayedgelabels1;
        TStatusBar *StatusBar1;
        TRichEdit *RichEdit1;
        TTimer *Timer1;
        TAction *cmPrint;
        TPrintDialog *PrintDialog1;
        TMenuItem *N2;
        TMenuItem *Print1;
        TToolButton *ToolButton14;
        TToolButton *ToolButton15;
        TPrinterSetupDialog *PrinterSetupDialog1;
        TAction *cmSaveAs;
        TMenuItem *Saveas1;
        TSaveDialog *SaveDialog1;
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall cmReloadExecute(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall cmCloseExecute(TObject *Sender);
        void __fastcall cmZoominExecute(TObject *Sender);
        void __fastcall cmZoomoutExecute(TObject *Sender);
        void __fastcall cmPSCFExecute(TObject *Sender);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall cmZoomNormalExecute(TObject *Sender);
        void __fastcall cmZoomAllExecute(TObject *Sender);
        void __fastcall cmOriginExecute(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall vbarScroll(TObject *Sender, TScrollCode ScrollCode,
          int &ScrollPos);
        void __fastcall hbarScroll(TObject *Sender, TScrollCode ScrollCode,
          int &ScrollPos);
        void __fastcall cmFPSCFExecute(TObject *Sender);
        void __fastcall cmCSCFExecute(TObject *Sender);
        void __fastcall cmFCSCFExecute(TObject *Sender);
        void __fastcall cmCSNFExecute(TObject *Sender);
        void __fastcall cmStickExecute(TObject *Sender);
        void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
        void __fastcall cmAboutExecute(TObject *Sender);
        void __fastcall cmHelpExecute(TObject *Sender);
        void __fastcall cmEdgeLabelsExecute(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall cmPrintExecute(TObject *Sender);
        void __fastcall PrinterSetupDialog1Show(TObject *Sender);
        void __fastcall PrinterSetupDialog1Close(TObject *Sender);
        void __fastcall cmSaveAsExecute(TObject *Sender);
        void __fastcall cmSaveAsUpdate(TObject *Sender);
private:	// User declarations
        void save_registry_options(void);
        void restore_registry_options(void);
        int xpages, ypages;
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        void display_exit_button(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

