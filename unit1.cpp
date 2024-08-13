/*
 *      WinGraph32 - Graph Visualization Program
 *      Version 1.0
 *      The WIN32 interface written by Ilfak Guilfanov. (ig@datarescue.com)
 *
 *      This program is under GPL (GNU General Public License)
 *      It is based on the VCG tool written by Georg Sander and Iris Lemke
 *
 *      Version 1.01
 *      - printing
 *      - fixed a bug in step4.c
 *      Version 1.02
 *      - 256 color entries are supported
 *
 */

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Printers.hpp>
#include <stdio.h>
#include <io.h>
#include <registry.hpp>
#include "Unit1.h"
#include "wait.h"
#include "about.h"
#include "help.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

extern "C"
{
void vcg_main(int argc, char *argv[]);
bool set_drawing_rectangle(int width, int height);
void draw_graph(void (*_line_cb)(int x1,int y1,int x2,int y2,int color),
                void (*_rectangle_cb)(long x,long y,int w,int h,int color),
                void (*_polygon_cb)(TPoint *hp, int j, int color));
void m_reload(void);
void error(const char *format, ...);
void cleanup_streams(void);
void set_fe_scaling(int stretch, int shrink);
void change_fe_scaling(int stretch, int shrink);
void display_complete_graph(void);
void visualize_part(void);
void relayout(void);
void m_validate_fe(int code);
void incr_fe_focus(int dx, int dy);
int normal_fe_focus(void);
void statistics(void);
void save_input_file_contents(FILE *fp);
extern char *G_title;
extern int G_color;
extern int G_displayel;
extern int G_stretch, G_shrink;
extern int V_xmin, V_ymin;
extern long fe_scaling;
extern long gfocus_x, gfocus_y;
extern int fisheye_view;
}

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}

//---------------------------------------------------------------------------
static void update_origin_status(void)
{
  char buf[80];
  if ( fisheye_view != 0 )
    snprintf(buf, sizeof(buf), "(%d,%d)", gfocus_x, gfocus_y);
  else
    snprintf(buf, sizeof(buf), "(%d,%d)", -V_xmin, -V_ymin);
  Form1->StatusBar1->Panels->Items[1]->Text = buf;
}

//---------------------------------------------------------------------------
static void update_zoom_status(void)
{
  char buf[80];
  double zoom;
  if ( fisheye_view != 0 )
    zoom = fe_scaling;
  else
  {
    zoom = double(G_stretch) * 100;
    if ( G_shrink != 0 ) zoom /= G_shrink;
  }
  snprintf(buf, sizeof(buf), "%5.2f%%", zoom);
  Form1->StatusBar1->Panels->Items[0]->Text = buf;
  update_origin_status();
}

//---------------------------------------------------------------------------
static void move_focus(int dx, int dy)
{
  incr_fe_focus(dx, dy);
  update_origin_status();
}

//---------------------------------------------------------------------------
static TColor vcg2bcc(int color)
{
  extern const unsigned char redmap[];
  extern const unsigned char greenmap[];
  extern const unsigned char bluemap[];
  if ( color < 256 )
  {
    int r = redmap[color];
    int g = greenmap[color];
    int b = bluemap[color];
    return TColor(r + (g<<8) + (b<<16));
  }
  return clRed;
}

static TCanvas *c = NULL;
static int draw_offx;
static int draw_offy;

void draw_line(int x1,int y1,int x2,int y2,int color)
{
  x1 += draw_offx;
  x2 += draw_offx;
  y1 += draw_offy;
  y2 += draw_offy;
  c->Pen->Color = vcg2bcc(color);
  c->MoveTo(x1, y1);
  c->LineTo(x2, y2);
  c->Pixels[x2][y2] = c->Pen->Color;
}

//---------------------------------------------------------------------------
void draw_rect(long x,long y,int w,int h,int color)
{
  x += draw_offx;
  y += draw_offy;
  c->Pen->Color = vcg2bcc(color);
  c->Brush->Color = c->Pen->Color;
  c->Rectangle(x, y, x+w, y+h);
}

//---------------------------------------------------------------------------
void draw_poly(TPoint *hp, int n, int color)
{
  for ( int i=0; i < n; i++ )
  {
    hp[i].x += draw_offx;
    hp[i].y += draw_offy;
  }
  c->Pen->Color = vcg2bcc(color);
  c->Brush->Color = c->Pen->Color;
  c->Polygon(hp, n-1);
}

//---------------------------------------------------------------------------
char *qtmpnam(char *answer)
                            // returns temporary file name
                            // (abs path, includes directory, uses TEMP/TMP vars)
{
  static int n = -1;

  const char *dir = getenv("TEMP");
  {
    if ( dir == NULL )
    {
      dir = getenv("TMP");
      if ( dir == NULL ) dir = "C:\\TEMP";
    }
  }

  if ( n == -1 ) n = time(NULL) * 1000;

  char *ptr = stpcpy(answer, dir);
  if ( ptr[-1] != '\\' && ptr[-1] != '/' ) *ptr++ = '\\';
  strcpy(ptr, "vcg12345.tmp");

  char *change = strchr(answer,0) - 9;
  while ( 1 )
  {
    char buf2[20];
    snprintf(buf2, sizeof(buf2), "%05u", n);
    int len = strlen(buf2);
    char *ptr = buf2 + len - 5;
    change[0] = ptr[0];
    change[1] = ptr[1];
    change[2] = ptr[2];
    change[3] = ptr[3];
    change[4] = ptr[4];
    if ( access(answer,0) ) break;
    n++;
  }
  return answer;
}

//---------------------------------------------------------------------------
static char stderr_file[MAX_PATH]; long stderr_pos;
static char stdout_file[MAX_PATH]; long stdout_pos;

long display_stream(FILE *fp, long pos)
{
  long np = ftell(fp);
  int len = np - pos;
  if ( len > 0 )
  {
    fseek(fp, pos, SEEK_SET);
    char *buf = new char[len+1];
    fread(buf, len, 1, fp);
    buf[len] = '\0';
    Form1->RichEdit1->Text = Form1->RichEdit1->Text + buf;
    Form1->RichEdit1->SelStart = Form1->RichEdit1->Text.Length() - 2;
    SendMessage(Form1->RichEdit1->Handle, EM_SCROLLCARET, 0, 0);
    delete buf;
    if ( Form1->Panel1->Height == 0 ) Form1->Panel1->Height = 100;
  }
  return np;
}

void display_streams(void)
{
  stdout_pos = display_stream(stdout, stdout_pos);
  stderr_pos = display_stream(stderr, stderr_pos);
  Form1->Update();
}

void cleanup_streams(void)
{
  fclose(stderr); unlink(stderr_file);
  fclose(stdout); unlink(stdout_file);
}

//---------------------------------------------------------------------------
static bool inited = false;
static void init(void)
{
  if ( inited ) return;

//  char curdir[1024];
//  GetCurrentDirectory(sizeof(curdir), curdir);
//  MessageBox(NULL, curdir, "Current directory", MB_OK);

  Form1->Panel1->Height = 0;
  Form1->FormResize(NULL);
  inited = true;

  freopen(qtmpnam(stderr_file), "w+", stderr);
  freopen(qtmpnam(stdout_file), "w+", stdout);
  int argc;
  char **argv;
  argc = ParamCount()+1;
  argv = new char* [argc+1];
  for ( int i=0; i < argc; i++ )
    argv[i] = strdup(ParamStr(i).c_str());
  argv[argc] = NULL;
  vcg_main(argc, argv);
  display_complete_graph();
  update_zoom_status();
  char buf[1024];
  snprintf(buf, sizeof(buf), "WinGraph32 - %s", G_title);
  Form1->Caption = buf;
  Application->Title = buf;
  statistics();
  extern int st_nr_vis_nodes, st_nr_vis_edges, nr_crossings;
  snprintf(buf, sizeof(buf), "%d nodes, %d edge segments, %d crossings",
                        st_nr_vis_nodes,
                        st_nr_vis_edges,
                        nr_crossings);
  Form1->StatusBar1->Panels->Items[2]->Text = buf;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
  c = Canvas;
  DoubleBuffered = true;
  cmZoomout->ShortCut = VK_SUBTRACT;
  cmZoomin->ShortCut = VK_ADD;
  restore_registry_options();
  update_origin_status();
  Timer1->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormPaint(TObject *Sender)
{
  if ( inited )
  {
    draw_graph(draw_line, draw_rect, draw_poly);
    display_streams();
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
  int w = ClientWidth - vbar->Width - 4;
  int h = ClientHeight
                - ToolBar1->Height
                - StatusBar1->Height
                - hbar->Height
                - Panel1->Height
                - 4;
  draw_offx = 2;
  draw_offy = Form1->ToolBar1->Height + 2;
  set_drawing_rectangle(w, h);
  if ( inited )
    Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmReloadExecute(TObject *Sender)
{
  m_reload();
  Invalidate();
}
//---------------------------------------------------------------------------
void error(const char *format, ...)
{
  va_list va;
  va_start(va, format);
  vfprintf(stderr, format, va);
  va_end(va);
  display_streams();
  Form1->display_exit_button();
}

//---------------------------------------------------------------------------
void TForm1::display_exit_button(void)
{
  BringToFront();
  TPanel *p = new TPanel(this);
  p->Parent = this;
  p->Align = alClient;
  TButton *b = new TButton(p);
  b->Parent = p;
  b->Left = (Width - b->Width) / 2;
  b->Top  = (Height - b->Height) / 2;
  b->Anchors = TAnchors() << /*akLeft << akTop <<*/ akRight << akBottom;
  b->Action = cmClose;
  for ( int i=0; i < ActionList1->ActionCount; i++ )
    ((TAction *)ActionList1->Actions[i])->Enabled = false;
  cmClose->Enabled = true;
  cmHelp->Enabled = true;
  cmAbout->Enabled = true;
  b->SetFocus();
  while ( 1 )
  {
    Application->HandleMessage();
    if ( Application->Terminated )
    {
      cleanup_streams();
      exit(-1);
    }
  }
}

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  cleanup_streams();
  save_registry_options();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmCloseExecute(TObject *Sender)
{
  cleanup_streams();
  exit(-1);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmZoominExecute(TObject *Sender)
{
  change_fe_scaling(3, 2);
  update_zoom_status();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmZoomoutExecute(TObject *Sender)
{
  change_fe_scaling(2, 3);
  update_zoom_status();
  Invalidate();
}

//---------------------------------------------------------------------------
#define MOVING_CURSOR   crSizeAll
static int ox, oy;
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if ( Button == mbLeft )
  {
         if ( Shift.Contains(ssShift) ) cmZoomin->Execute();
    else if ( Shift.Contains(ssCtrl)  ) cmZoomout->Execute();
    Screen->Cursor = !cmStick->Checked || Screen->Cursor != MOVING_CURSOR
                        ? MOVING_CURSOR
                        : crDefault;
    ox = X;
    oy = Y;
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if ( !cmStick->Checked ) Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
  if ( Screen->Cursor == MOVING_CURSOR )
  {
    if ( X != ox || Y != oy )
    {
      move_focus(ox-X, oy-Y);
      ox = X;
      oy = Y;
      Invalidate();
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmZoomNormalExecute(TObject *Sender)
{
  set_fe_scaling(1, 1);
  update_zoom_status();
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmZoomAllExecute(TObject *Sender)
{
  display_complete_graph();
  update_zoom_status();
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmOriginExecute(TObject *Sender)
{
  normal_fe_focus();
  Invalidate();
  update_origin_status();
}

//---------------------------------------------------------------------------
const int STEP = 20;
const int PAGE = 100;

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  switch( Key )
  {
    case VK_LEFT:
      if ( Shift.Contains(ssCtrl) )
        move_focus(-PAGE, 0);
      else
        move_focus(-STEP, 0);
      break;
    case VK_RIGHT:
      if ( Shift.Contains(ssCtrl) )
        move_focus(PAGE, 0);
      else
        move_focus(STEP, 0);
      break;
    case VK_UP:
      if ( Shift.Contains(ssCtrl) )
        move_focus(0, -PAGE);
      else
        move_focus(0, -STEP);
      break;
    case VK_DOWN:
      if ( Shift.Contains(ssCtrl) )
        move_focus(0, PAGE);
      else
        move_focus(0, STEP);
      break;
    case VK_PRIOR:
      move_focus(0, -ClientHeight / 2);
      break;
    case VK_NEXT:
      move_focus(0, ClientHeight / 2);
      break;
    default:
      return;
  }
  Key = 0;
  Invalidate();
}


//---------------------------------------------------------------------------
static int calc_scroll(TScrollCode ScrollCode, int &ScrollPos)
{
  static int old = 50;
  int code = 0;
  switch ( ScrollCode )
  {
    case scLineUp:	// User clicked the top or left scroll arrow or pressed the Up or Left arrow key.
      code = -STEP;
      break;
    case scLineDown:	// User clicked the bottom or right scroll arrow or pressed the Down or Right arrow key.
      code = STEP;
      break;
    case scPageUp:	// User clicked the area to the left of the thumb tab or pressed the PgUp key.
      code = -PAGE;
      break;
    case scPageDown:	// User clicked the area to the right of the thumb tab or pressed the PgDn key.
      code = PAGE;
      break;
    case scTrack:	// User is moving the thumb tab.  (5)
      code = -20*(old-ScrollPos);
      old = ScrollPos;
      break;
    case scEndScroll:	// User finished moving the thumb tab on the scroll bar. (8)
      old = 50;
      ScrollPos = 50;
      break;
    case scTop:   	// User moved the thumb tab to the top or far left on the scroll bar.
    case scBottom:	// User moved the thumb tab to the bottom or far right on the scroll bar.
    case scPosition:	// User positioned the thumb tab and released it. (4)
      break;
  }
  ScrollPos = 50;
  return code;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::vbarScroll(TObject *Sender, TScrollCode ScrollCode,
      int &ScrollPos)
{
  int code = calc_scroll(ScrollCode, ScrollPos);
  if ( code != 0 )
  {
    move_focus(0, code);
    Invalidate();
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::hbarScroll(TObject *Sender, TScrollCode ScrollCode,
      int &ScrollPos)
{
  int code = calc_scroll(ScrollCode, ScrollPos);
  if ( code != 0 )
  {
    move_focus(code, 0);
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmPSCFExecute(TObject *Sender)
{
  cmPSCF->Checked  = true;
  cmFPSCF->Checked = false;
  cmCSCF->Checked  = false;
  cmFCSCF->Checked = false;
  cmCSNF->Checked  = false;
  m_validate_fe(0);
  update_zoom_status();
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmFPSCFExecute(TObject *Sender)
{
  cmPSCF->Checked  = false;
  cmFPSCF->Checked = true;
  cmCSCF->Checked  = false;
  cmFCSCF->Checked = false;
  cmCSNF->Checked  = false;
  m_validate_fe(1);
  update_zoom_status();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmCSCFExecute(TObject *Sender)
{
  cmPSCF->Checked  = false;
  cmFPSCF->Checked = false;
  cmCSCF->Checked  = true;
  cmFCSCF->Checked = false;
  cmCSNF->Checked  = false;
  m_validate_fe(2);
  update_zoom_status();
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmFCSCFExecute(TObject *Sender)
{
  cmPSCF->Checked  = false;
  cmFPSCF->Checked = false;
  cmCSCF->Checked  = false;
  cmFCSCF->Checked = true;
  cmCSNF->Checked  = false;
  m_validate_fe(3);
  update_zoom_status();
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmCSNFExecute(TObject *Sender)
{
  cmPSCF->Checked  = false;
  cmFPSCF->Checked = false;
  cmCSCF->Checked  = false;
  cmFCSCF->Checked = false;
  cmCSNF->Checked  = true;
  m_validate_fe(4);
  update_zoom_status();
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmStickExecute(TObject *Sender)
{
  cmStick->Checked = !cmStick->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseWheel(TObject *Sender, TShiftState Shift,
      int WheelDelta, TPoint &MousePos, bool &Handled)
{
  if ( Shift.Contains(ssShift) )
    move_focus(-WheelDelta, 0);
  else
    move_focus(0, -WheelDelta);
  Handled = true;
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmAboutExecute(TObject *Sender)
{
  AboutForm->ShowModal();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmHelpExecute(TObject *Sender)
{
  HelpForm->ShowModal();
}

//---------------------------------------------------------------------------
static AnsiString rootkey  = "\\Software\\Datarescue\\WinGraph32\\";
static AnsiString mwidth   = "Width";
static AnsiString mheight  = "Height";
static AnsiString mstick   = "StickMouse";
static AnsiString medgel   = "EdgeLabels";

void TForm1::restore_registry_options(void)
{
  G_displayel = true;
  cmEdgeLabels->Checked = G_displayel;
  TRegistry *r = new TRegistry;
  if ( r->KeyExists(rootkey) )
  {
    try
    {
      r->OpenKey(rootkey, false);
      if ( r->ValueExists(mwidth) )
        Width = r->ReadInteger(mwidth);
      if ( r->ValueExists(mheight) )
        Height = r->ReadInteger(mheight);
      if ( r->ValueExists(mstick) )
        cmStick->Checked = r->ReadInteger(mstick);
      if ( r->ValueExists(medgel) )
      {
        G_displayel = r->ReadInteger(medgel);
        cmEdgeLabels->Checked = G_displayel;
      }
    }
    catch(...)
    {
    }
    // check that the window is entirely on the screen
    if ( Left+Width >= Screen->Width )
    {
      int delta = Left+Width - Screen->Width;
      Left -= delta;
      if ( Left < 0 )
      {
        Width += Left;
        Left = 0;
      }
    }
    if ( Top+Height >= Screen->Height )
    {
      int delta = Top+Height - Screen->Height;
      Top -= delta;
      if ( Top < 0 )
      {
        Height += Top;
        Top = 0;
      }
    }
  }
  r->CloseKey();
  delete r;
}

void TForm1::save_registry_options(void)
{
  TRegistry *r = new TRegistry;
  r->OpenKey(rootkey, true);
  r->WriteInteger(mwidth,  Width);
  r->WriteInteger(mheight, Height);
  r->WriteInteger(mstick,  cmStick->Checked);
  r->WriteInteger(medgel,  cmEdgeLabels->Checked);
  r->CloseKey();
  delete r;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmEdgeLabelsExecute(TObject *Sender)
{
  cmEdgeLabels->Checked = !cmEdgeLabels->Checked;
  G_displayel = cmEdgeLabels->Checked;
  relayout();
  Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  Timer1->Enabled = false;
  init();
  Color = vcg2bcc(G_color);
  Invalidate();
  Cursor = crDefault;
}

//---------------------------------------------------------------------------
void __fastcall TForm1::cmPrintExecute(TObject *Sender)
{
  if ( PrinterSetupDialog1->Execute() == mrOk )
  {
    Graphics::TBitmap *b = NULL;
    try
    {
      Enabled = false;
      WaitForm->Label1->Caption = "Allocating memory...";
      WaitForm->Show();
      TPrinter *P = Printer();
      int w = P->PageWidth;
      int h = P->PageHeight;
      // Create a bitmap for the image
      b = new Graphics::TBitmap;
      b->HandleType = bmDIB;
      b->PixelFormat = pf4bit;    // 16 colors are enough!!! (save memory)
      b->Height = h * ypages;
      b->Width  = w * xpages;
      c = b->Canvas;
      // output the graph title
      AnsiString title = G_title;
      int x = (b->Width - c->TextWidth(title)) / 2;
      c->Font->Size = b->Height / 300;    // so we get about 12 points for one page
      c->TextOut(x, 10, title);
      P->Title = title;
      // output the graph to the bitmap
      WaitForm->Label1->Caption = "Generating graph...";
      WaitForm->Update();
      draw_offx = 60;
      draw_offy = 60;
      set_drawing_rectangle(b->Width-120, b->Height-120);
      display_complete_graph();
      draw_graph(draw_line, draw_rect, draw_poly);
      // copy the bitmap to the printer
      P->BeginDoc();
      TRect src;
      TRect dst(0, 0, w, h);
      for ( int j=0; j < ypages; j++ )
      {
        for ( int i=0; i < xpages; i++ )
        {
          if ( i || j ) P->NewPage();
          src.Left   = i*w;
          src.Top    = j*h;
          src.Right  = src.Left + w;
          src.Bottom = src.Top  + h;
          title.sprintf("Generating page %d...", j*xpages+i+1);
          WaitForm->Label1->Caption = title;
          WaitForm->Update();
          P->Canvas->CopyRect(dst, b->Canvas, src);
        }
      }
      P->EndDoc();
    }
    __finally
    {
      delete b;
      // restore picture on the screen
      WaitForm->Hide();
      Enabled = true;
      c = Canvas;
      FormResize(Sender);
      display_complete_graph();
    }
  }
}

//---------------------------------------------------------------------------
#define CUSTOM_ADDITIONAL_HEIGHT 36
#define CUSTOM_COMBO_X 1234
#define CUSTOM_COMBO_Y 1235

//---------------------------------------------------------------------------
static int AddList(HWND parent, int x, int y, int id)
{
  int width = 40;
  int style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST | CBS_HASSTRINGS;
  HWND combo = CreateWindow("COMBOBOX", "1", style, x, y, width, 23*10, parent,
                    HMENU(id), Application->Handle, NULL);
  SendMessage(combo, WM_SETFONT, WPARAM(GetStockObject(DEFAULT_GUI_FONT)), 0);
  char buf[2];
  for ( int i=1; i < 10; i++ )
  {
    buf[0] = '0' + i;
    buf[1] = '\0';
    SendMessage(combo, CB_ADDSTRING, 0, (LPARAM)buf);
  }
  SendMessage(combo, CB_SETCURSEL, 0, 0);
  return width;
}

static int AddText(HWND parent, int x, int y, const char *text)
{
  int width = 10;
  int style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SS_LEFT;
  HWND txth = CreateWindow("STATIC", text, style, x, y, width, 23, parent,
                    0, Application->Handle, NULL);

  SendMessage(txth, WM_SETFONT, WPARAM(GetStockObject(DEFAULT_GUI_FONT)), 0);

  // calculate the text width
  HDC hdc = GetDC(txth);
  SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
  SIZE s;
  GetTextExtentPoint32(hdc, text, strlen(text), &s);
  ReleaseDC(txth, hdc);
  width = s.cx;

  // change the text control width
  SetWindowPos(txth, 0, 0, 0, width, 23, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
  return width;
}


BOOL CALLBACK enumfunc(HWND h, LPARAM lParam)
{
  RECT *p = (RECT *)lParam;
  RECT rect;
  GetWindowRect(h, &rect);
  if ( rect.top >= p->bottom-CUSTOM_ADDITIONAL_HEIGHT-36 )
  {
    rect.top  += CUSTOM_ADDITIONAL_HEIGHT;
    POINT pnt;
    pnt.x = rect.left;
    pnt.y = rect.top;
    ScreenToClient(GetParent(h), &pnt);
    SetWindowPos(h, 0, pnt.x, pnt.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
  }
  return TRUE;
}

void __fastcall TForm1::PrinterSetupDialog1Show(TObject *Sender)
{
  HWND h = PrinterSetupDialog1->Handle;
  SetWindowText(h, "Print");
  RECT rect;
  GetWindowRect(h, &rect);
  int cx = rect.right - rect.left;
  int cy = rect.bottom - rect.top;
  cy += CUSTOM_ADDITIONAL_HEIGHT;
  /*BOOL ok =*/ SetWindowPos(h, HWND_TOP, 0, 0, cx, cy, SWP_NOMOVE);
  EnumChildWindows(h, WNDENUMPROC(enumfunc), LPARAM(&rect));  // move ok, cancel, network down

  ::GetClientRect(h, &rect);
  int x = 15;
  int y = rect.bottom - CUSTOM_ADDITIONAL_HEIGHT - 36;
    x +=   4 + AddText(h, x, y, "Print graph on");
    x +=   4 + AddList(h, x, y, CUSTOM_COMBO_X);
    x +=   4 + AddText(h, x, y, "by");
    x +=   4 + AddList(h, x, y, CUSTOM_COMBO_Y);
  /*x +=*/ 4 + AddText(h, x, y, "pages");
}

//---------------------------------------------------------------------------
void __fastcall TForm1::PrinterSetupDialog1Close(TObject *Sender)
{
  HWND h = PrinterSetupDialog1->Handle;
  xpages = SendDlgItemMessage(h, CUSTOM_COMBO_X, CB_GETCURSEL, 0, 0) + 1;
  ypages = SendDlgItemMessage(h, CUSTOM_COMBO_Y, CB_GETCURSEL, 0, 0) + 1;
}

//---------------------------------------------------------------------------
static char *contents = NULL;
static long fsize;

void save_input_file_contents(FILE *fp)
{
  fseek(fp, 0, SEEK_END);
  fsize = ftell(fp);
  if ( fsize > 0 )
  {
    contents = (char *)malloc(fsize);
    if ( contents == NULL )
      error("Not enough memory for the input file");
    fseek(fp, 0, SEEK_SET);
    fsize = fread(contents, 1, fsize, fp);
  }
  fseek(fp, 0, SEEK_SET);
}


void __fastcall TForm1::cmSaveAsExecute(TObject *Sender)
{
  if ( SaveDialog1->Execute() )
  {
    FILE *fp = fopen(SaveDialog1->FileName.c_str(), "w");
    if ( fp == NULL )
    {
      MessageBox(NULL, "Can not open output file", "Error", MB_OK);
      return;
    }
    if ( fwrite(contents, 1, fsize, fp) != fsize )
      MessageBox(NULL, "Can not write to the output file", "Error", MB_OK);
    fclose(fp);
  }
}

void __fastcall TForm1::cmSaveAsUpdate(TObject *Sender)
{
  cmSaveAs->Enabled = fsize > 0;        
}

//---------------------------------------------------------------------------

