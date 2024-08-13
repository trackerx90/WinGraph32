object HelpForm: THelpForm
  Left = 380
  Top = 289
  BorderStyle = bsDialog
  Caption = 'WinGraph32 Help'
  ClientHeight = 498
  ClientWidth = 380
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
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 380
    Height = 425
    Align = alTop
    Lines.Strings = (
      
        'The purpose of WinGraph32 is to visualize graphs which consist o' +
        'f nodes '
      'and edges.'
      ''
      
        'WinGraph32 tool reads a textual and readable specification of a ' +
        'graph and '
      
        'visualizes  the graph. It layouts the graph using several heuris' +
        'tics as reducing  '
      
        'the number of crossings,  minimizing the  size of  edges,  cente' +
        'ring of nodes. '
      
        'The specification language of WinGraph32 is GDL, Graph Descripti' +
        'on '
      'Language.'
      ''
      'The user interface:'
      ''
      '  N - normal view'
      '  P - polar fish view with fixed radius'
      '  Ctrl-P - polar fish view'
      '  C - cartesian fish view with fixed radius'
      '  Ctrl-C - cartesian fish view'
      ''
      '  Numpad + - zoom in 150%'
      '  Numpad - - zoom out 66%'
      '  M - fit the whole graph'
      '  0 - normal (100%) zoom'
      ''
      '  O - position on origin'
      '  MouseWheel - scroll vertically'
      '  Shift-MouseWheel - scroll horizontally'
      '  Arrows - scroll, Ctrl-arrows - scroll faster'
      '  Shift-LeftClick - zoomin, Ctrl-LeftCLick - zoomout'
      ''
      
        'You can pan the graph by dragging it in any direction. If the "e' +
        'asy graph '
      
        'panning" is checked then the drag operation is started after any' +
        ' mouse click. '
      
        'In this case you may release the mouse button and the panning wi' +
        'll continue.')
    ReadOnly = True
    TabOrder = 0
  end
  object Button1: TButton
    Left = 152
    Top = 448
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
end
