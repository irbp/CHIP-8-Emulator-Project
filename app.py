from Tkinter import *
import tkMessageBox
import tkFileDialog as tkfd
import os
import thread
import time

class App:
    def __init__(self, master):
        #frame = Frame(master)
        #frame.pack()

        self.menubar = Menu(master)

        self.filemenu = Menu(self.menubar, tearoff=0)
        self.filemenu.add_command(label="Open CHIP-8 ROM", command=self.openFile)
        self.filemenu.add_command(label="Close CHIP-8 ROM", command=self.closeFile)
        self.filemenu.add_command(label="Recent ROM's", command=self.donothing)
        self.filemenu.add_separator()
        self.filemenu.add_command(label="Quit", command=master.quit)
        self.menubar.add_cascade(label="File", menu=self.filemenu);

        self.settingsmenu = Menu(self.menubar, tearoff=0)
        self.settingsmenu.add_command(label="Configure Keypad", command=self.donothing)
        self.menubar.add_cascade(label="Settings", menu=self.settingsmenu)

        self.helpmenu = Menu(self.menubar, tearoff=0)
        self.helpmenu.add_command(label="About", command=self.about)
        self.menubar.add_cascade(label="Help", menu=self.helpmenu)

    def gameThread(self, file):
        os.system('./main "' + file + '"')
    
    def openFile(self):
        filename = tkfd.askopenfile(initialdir=os.system("pwd"), title="Select the CHIP-8 ROM", filetypes=(("CHIP-8 ROM", "*.ch8"), ("All Files", "*.*")))
        print filename.name
        thread.start_new_thread(self.gameThread, (filename.name, ))

    def closeFile(self):
        os.system("killall main")

    def about(self):
        tkMessageBox.showinfo("About", "CHIP-8 Emulator\nCreated by: irbp\nVersion: 1.0.0")

    def donothing(self):
        print "Do nothing!"

root = Tk()
root.title("CHIP-8 Emulator");

app = App(root)

root.config(menu=app.menubar)
root.mainloop()