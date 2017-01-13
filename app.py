from Tkinter import *
import tkMessageBox

class App:
    def __init__(self, master):
        #frame = Frame(master)
        #frame.pack()

        self.menubar = Menu(master)

        self.filemenu = Menu(self.menubar, tearoff=0)
        self.filemenu.add_command(label="Open CHIP-8 ROM", command=self.donothing)
        self.filemenu.add_command(label="Open CHIP-8 ROM", command=self.donothing)
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

    def donothing(self):
        print "Do nothing!"

    def about(self):
        tkMessageBox.showinfo("About", "CHIP-8 Emulator\nCreated by: irbp\nVersion: 1.0.0")

root = Tk()
root.title("CHIP-8 Emulator");

app = App(root)

root.config(menu=app.menubar)
root.mainloop()