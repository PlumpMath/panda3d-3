#                                        [Emacs: -*- python -*-]
# --- This is the Pmw.definition file ---
#
# It is invoked by the Pmw.dynamic loader in Pmw.__init__.
#
#    widgets       : tuple with the names of those widget classes that are
#                    stacked in a module of the same name.
#    widgetclasses : dictionary from names of widget classes to module names.
#    functions     : dictionary from function names to modules names.
#    modules       : tuple of module names that don't contain widget classes
#                    of the same name.
#

# Widgets whose name is the same as its module.
_widgets = (
    'AboutDialog', 'Balloon', 'ButtonBox', 'ComboBox',
    'ComboBoxDialog', 'Counter', 'CounterDialog', 'Dialog',
    'EntryField', 'Group', 'HistoryText', 'LabeledWidget',
    'MainMenuBar', 'MenuBar', 'MessageBar',
    'MessageDialog', 'NoteBook', 'OptionMenu', 'PanedWidget',
    'PromptDialog', 'RadioSelect', 'ScrolledCanvas', 'ScrolledField',
    'ScrolledFrame', 'ScrolledListBox', 'ScrolledText', 'SelectionDialog',
    'TextDialog', 'TimeCounter',
)

# Widgets whose name is not the same as its module.
_extraWidgets = {
}

_functions = {
    'logicalfont'           : 'LogicalFont',
    'logicalfontnames'      : 'LogicalFont',
    'aboutversion'          : 'AboutDialog',
    'aboutcopyright'        : 'AboutDialog',
    'aboutcontact'          : 'AboutDialog',
    'datestringtojdn'       : 'TimeFuncs',
    'timestringtoseconds'   : 'TimeFuncs',
    'setyearpivot'          : 'TimeFuncs',
    'ymdtojdn'              : 'TimeFuncs',
    'jdntoymd'              : 'TimeFuncs',
    'stringtoreal'          : 'TimeFuncs',
    'aligngrouptags'        : 'Group',
    'OK'                    : 'EntryField',
    'ERROR'                 : 'EntryField',
    'PARTIAL'               : 'EntryField',
    'numericvalidator'      : 'EntryField',
    'integervalidator'      : 'EntryField',
    'hexadecimalvalidator'  : 'EntryField',
    'realvalidator'         : 'EntryField',
    'alphabeticvalidator'   : 'EntryField',
    'alphanumericvalidator' : 'EntryField',
    'timevalidator'         : 'EntryField',
    'datevalidator'         : 'EntryField',
}

_modules = (
    'Color', 'Blt',
)
