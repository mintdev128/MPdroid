#Importing needed libraries
import math,os,time,ubinascii,utime,sys,errno,utime,machine
#Setting up definitions and 
bs = 0.10
usr = 'root'
host = sys.implementation.name
ps1 = f'{host}@{usr}:~ # '
seldir = os.getcwd()
nocom = 'Command not fully ported yet'
name = 'MPdroid 1.0'
author = 'MintDev'
def base64(arg,dtr):
    if arg == '-d' :
        print(ubinascii.a2b_base64(dtr))
    elif arg != '-d' :
        print(ubinascii.b2a_base64(dtr))
def pwd() :
    seldir = os.getcwd()
    print(seldir)
author == 'MintDev'
def cd(loc) :
    os.chdir(loc)
def clear() :
    print('\n' * 8)
def whoami() :
    print(usr)
def reset() :
    clear()
    ps1 = f'{host}@{usr}:{seldir} # '
    bs = 0.10
author = 'MintDev'
def ls() :
    print('       '.join(os.listdir()))
def cp(fileone, filetwo):
    print(nocom)
  #  seldir = os.getcwd()
   # if os.path.exists({seldir},{fileone}):
    with open(f'{seldir}{fileone}','r') as file0:
        cfp = filr0.read()
        close()
    with open(f'{seldir}{filetwo}','w') as file0:
        file0.write(cpf)
author = 'MintDev'
def editdir(fileone,action) :
    if action == 'make':
        try :
            os.mkdir(f'{seldir}/{fileone}')
        except OSError as e :
            if e.errno == errno.EEXIST :
                print(f'editdir: \'{fileone}\': Directory exists. ')
            else: print(e)
    if action == 'remove':
        try :
            os.rmdir(f'{seldir}/{fileone}')
        except OSError as e:
            if e.errno == errno.ENOENT :
                print(f'diredit: \'{fileone}\': No such directory. ')
            else: print(e)
def editfile(fileone, action):
    if action == 'remove':
        try :os.remove(f'{seldir}/{fileone}')
        except OSError as e :
            if e.errno == errno.ENOENT :
                print(f'fileedit: \'{fileone}\': No such file. ')
def powerc(arg, time):
    if arg == 'light':
        mashine.lightsleep(float(time * 1000))
    if arg == 'deep':
        machine.deepsleep(float(time * 1000))
def date(form):
    d = utime.localtime()
    year, month, mday, hour, minute, second, _, _, _ = d
    if form == 'basic':
        print("{} {} {}:{}:{} {}".format(
    d[2], 
    d[1], 
    d[3], 
    d[4],  
    d[5],
    d[0])) 
    else:
        print(nocom)
def constr(mode, node) :
    print(f'Before using Constr make sure you using it in the dir where programms is placed. \n')
    if mode == 'play':
        seldir = os.getcwd()
        print(f'Playing {node} on {name}. \n')
        exec(open(f'{seldir}/{node}.py').read())
        print('\n')
author = 'MintDev'
while True:
    usr = input('Press enter to login. : ')
    if usr == 'mintdev':
        print()
    else :
        usr = 'root'
    ps1 = f'{host}@{usr}:{seldir}# '
    cset = []
    print(f'{name} on {host} created by {author}. ')
    while True:
        ttyc = list(map(str,input(ps1).split( )))
        if len(ttyc) >= 1 :
            if len(ttyc) == 2:
                if ttyc[0] == 'mkdir':
                    editdir(ttyc[1], 'make')
                elif ttyc[0] == 'rmdir':
                    editdir(ttyc[1], 'remove')
                elif ttyc[0] == 'cd':
                    cd(ttyc[1])
                elif ttyc[0] == 'sleep' :
                    powerc('light', ttyc[1])
                elif ttyc == 'rm' and len(ttyc) == 2 :
                    editfile(ttyc[1], 'remove')
                elif ttyc[0] == 'base64' :
                    base64('', ttyc[1])
                elif ttyc[0] == 'echo' or ttyc[0] == 'printf':
                    print(ttyc[1])
            elif len(ttyc) == 3 :
                if ttyc[0] == 'constr':
                    if ttyc[1] == 'play' :
                        constr('play', ttyc[2])
            elif ttyc[0] == 'toybox' :
                print(cset)
            elif ttyc[0] == 'pwd' :
                pwd()
            elif ttyc[0] == 'clear' :
                clear()
            elif ttyc[0] == 'whoami' :
                whoami()
            elif ttyc[0] == 'exit' :
                break
            elif ttyc[0] == 'reset' :
                reset()
            elif ttyc[0] == 'yes' or ttyc[0] == 'no' :
                print('You better not to use that without pipe. ')
            elif ttyc[0] == 'ls' :
                ls()
            elif ttyc[0] == 'export' and len(ttyc) == 4 :
                if ttyc[1] == 'ps1' :
                    ps1 = str,ttyc[3]
            elif ttyc[0] == 'cp' and len(ttyc) == 3 :
                cp(ttyc[1], ttyc[2])
            elif ttyc[0] == 'date' :
                if len(ttyc) == 1 :date('basic')
                else:date(ttyc[1])
            else :
                print(f'MPysh: {ttyc[0]}:not found or can\'t be used without attributes')
        else:
            print()
author = 'MintDev'