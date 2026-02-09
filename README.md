# FreeFlameDevShell
<img width="268" height="268" alt="Untitled271_20260117175637" src="https://github.com/user-attachments/assets/5c6fa0a7-fbbf-46b5-9456-97080e57b09e" />

# FreeFDS
  Small toolbox written on Python, almost ready to run on Everything.
# Instructions
## Main 
  To use toolbox just write the command you want to use, so simple.
  It can perform basic functions such as : 
* date, pwd, cd, clear and ls.
  Just like in BSD.
## PKG tool 
  In this you can use pkg to play or download programms. 

`pkg play/add [pkgname] [local filename]`
 
 Renamed to pkg in update 2.0, added function `add` to download files from programs repo (uses FTP tool), if using add name as [pkgname] and local name as [local filename].

`constr play [programm name]`

Available from release 1.0, use function play `play` to execute programs .
## FTP tool
  To download files to current working directory you can use FTP.

`ftp (get/req) [url] [local filename]`

Available from release 2.0, it downloads files from [url] and saves as [local filename].
## More
  To see more commands use

`toolbox`
  Available from release 1.1 fix .
