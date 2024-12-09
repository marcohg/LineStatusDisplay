#!/usr/bin/python3
"""Line Display

Arrange in 2 columns 
- Subclass tk for the application
- Provide a MainFrame to place widgets
  - Title
  - Rows with variable-units
  - Status line 
- Update data to display from file
"""
import tkinter as tk
from tkinter import ttk
from tkinter import font
from pathlib import Path
import os
from manage_cfg import GetConfiguration, CreateEClientConfiguration
import re

class MainFrame(tk.Frame):
  """Main Frame holds application widgets"""
  # def __init__(self, parent, title_arg, vars, *args, **kwargs):
  def __init__(self, parent, vars, *args, **kwargs):
    super().__init__(parent, *args, **kwargs)
    
    # Root geometry
    parent.title("Display Application")
    parent.geometry('800x600+50+10')  # 800x600 ->1280x720 (16:9) -> 1920x1080
    parent.resizable(False, False) # comment out for testing.

    # Expand the 2 columns
    self.columnconfigure(0, weight=4)
    self.columnconfigure(1, weight=1)

    i = 0
    for key,var in vars.items() :
      if key == 'title':
        var['txt_var'].set(var['text'])
        tk.Label(self, textvariable=var['txt_var'], font= var['font']
        ).grid(columnspan=2,sticky=tk.EW)
      else :
        if key != 'status' : # Variables value and their units
          tk.Label(self, textvariable = var['txt_var'],font= var['font'][0]
          ).grid(row=i+1, column=0,sticky=tk.EW)
          tk.Label(self, text=var['units'], font=var['font'][1]
          ).grid(row=i+1, column=1, sticky=tk.W)
          i += 1

class StatusFrame(tk.Frame):
  """Status Frame holds status and notifications """
  def __init__(self, parent, var, event, font_arg, *args, **kwargs):
    kwargs['relief'] = 'groove'
    kwargs['borderwidth'] = 5
    super().__init__(parent, *args, **kwargs)
    pad = {'padx': 10, 'pady': 5}
    ttk.Label( self, text="1",font=font_arg).grid(row=0, column=0, **pad)
    ttk.Label( self, textvariable=var, font=font_arg).grid(sticky=tk.W, row=0, column=1, **pad)
    ttk.Label( self, text="3",font=font_arg).grid(row=0, column=2, **pad)
    b1 = tk.Button( self, text='ACK',repeatdelay=100, repeatinterval= 250, font=font_arg) # ttk no font
    b1.grid(row=0, column=3, sticky=tk.E ) # padx='10', pady=5)
    self.columnconfigure(1, weight=1)
    b1.configure(command=event)

class Application(tk.Tk):
  """Line Display root window"""
  def __init__(self, *args, **kwargs):
    if os.environ.get('DISPLAY','') == '':
      print('no display found. Using :0.0')
      os.environ.__setitem__('DISPLAY', ':0.0')
    
    super().__init__(*args, **kwargs) # self = tk.Tk
    c = os.environ['HOME'] + "/" + "configuration.json" # configuration.json at home folder
    # c = "configuration.json" - HOME+?
    self.cfg = GetConfiguration(c)
    CreateEClientConfiguration(self.cfg)
    # print (self.cfg)


    fonts = {
      'title': font.Font( family='Anta', size=30, weight='bold', slant='roman', underline=False, overstrike=False ),
      'value': font.Font( family='Anta', size=80, weight='bold', slant='roman', underline=False, overstrike=False ),
      'unit' : font.Font( family='Anta', size=30, weight='normal', slant='italic', underline=False, overstrike=False ),
      'status': font.Font( family='Arial', size=18, weight='bold', slant='roman', underline=False, overstrike=False )
    }
    
    self.ack = False
    self.vars = {
      'title': { 'text': 'Line 1 Production Status 2', 'txt_var': tk.StringVar(), 'font': fonts['title'] },
      'speed': { 'txt_var': tk.StringVar(), 'units': 'mt/min', 'font': (fonts['value'], fonts['unit']) },
      'total': { 'txt_var': tk.StringVar(), 'units': 'mt', 'font': (fonts['value'], fonts['unit']) },
      'status': tk.StringVar() 
    }

    # Main Frame is at row 0 and expand it in row and column 
    self.columnconfigure(0, weight=1)
    self.rowconfigure(0,weight=1)
    main_frame_args = { 'relief': 'groove' ,'borderwidth': 5 } 
    MainFrame(self, self.vars, **main_frame_args ).grid(padx=10, pady=10, sticky=tk.NSEW)
    StatusFrame(self, self.vars['status'], self.on_ack, fonts['status'] ).grid(padx=10, pady=10, sticky=tk.NSEW)
    
    # Init some display data
    self.ticks = 0
    self.vars['status'].set('Start Application')

    # Trigger first periodic task
    self.after(2500, self.periodic_task)
  
  def on_ack(self) :
    """User ACK status"""
    self.ack = True
    
  def periodic_task(self) :

    total = self.vars['total']  # reference alias
    status = "---"

    # Open File and read it
    path = Path(self.cfg["status_file"])
    if path.is_file() :
      content = path.read_text()
      lines = content.splitlines()

      # epoch, status, secs10, counts, delta_counts_tb, counts_per_tb
      if lines :  # file is regenerated avoid pop empty list
        # last3 = lines [-3:] #  \todo Average speed from last 3 samples
        # print(last3)
        last_line = lines.pop()
        n, args = GetArgs(last_line,6) # from last one
        if n == 6 : # CSV reading 6 fields
          node_cfg = self.cfg['nodes'][0]  # for Now only first node
          speed = float(args[5]) * node_cfg['speed_factor']
          length = float(args[3]) * node_cfg['length_factor']
          self.vars['speed']['txt_var'].set( f'{speed:.2f}' )
          self.vars['total']['txt_var'].set( f'{length:.1f}')
          # print(f"{args[2]}, { args[3] }, { args[4] },  { args[5] }")
        else :
          status = f"Received: { last_line }"
      # else :
      #   for line in lines :
      #     print(line)
    else :
      status = "waiting for encoder client"

    if self.ack :
      self.vars['status'].set('Acknowledged!')
      self.ack = False
    else :
      self.vars['status'].set(status)

    self.after( int(self.cfg['gui_sleep_ms']), self.periodic_task)

def GetArgs(str, argc) :
  """Get argc in str arg1,arg2,..,argc"""
  list = []
  nbr = 0
  for n in range(argc) :
    argr = re.match(r'[+-]*[0-9]+\.*[0-9]*,*',str)
    if argr :
      arg = argr.group().rstrip(',')
      str = str[argr.end():]
      list.append(arg)
      nbr += 1
  return nbr, list


if __name__ == "__main__":

  app = Application()
  app.mainloop()
