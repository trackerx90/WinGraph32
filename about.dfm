object AboutForm: TAboutForm
  Left = 285
  Top = 255
  BorderStyle = bsDialog
  Caption = 'About WinGraph32'
  ClientHeight = 237
  ClientWidth = 317
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 160
    Width = 313
    Height = 13
    Cursor = crHandPoint
    Alignment = taCenter
    AutoSize = False
    Caption = 'http://www.datarescue.com'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentColor = False
    ParentFont = False
    OnClick = Label1Click
  end
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 317
    Height = 145
    Align = alTop
    Alignment = taCenter
    BorderStyle = bsNone
    Color = clBtnFace
    Lines.Strings = (
      ''
      'WinGraph32'
      ''
      'Copyright (c) 2001 Datarescue'
      'Version 1.02'
      ''
      'This program is based on VCG library written by'
      'Georg Sander and Iris Lemke'
      ''
      'WinGraph32 is released under GPL'
      '(GNU General Public License)')
    ReadOnly = True
    TabOrder = 0
  end
  object Button1: TButton
    Left = 121
    Top = 192
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
end
