# FreeFlameDevShell
<img width="268" height="268" alt="Chip with fire icon on it." src="https://github.com/user-attachments/assets/5c6fa0a7-fbbf-46b5-9456-97080e57b09e" />

# FreeFDS
  Small toolbox written on Python, almost ready to run on Everything.
# Instructions
## Main 
  To use toolbox just write the command you want to use, so simple.
  It can perform basic functions such as : 
* date, pwd, cd, clear and ls.
  Just like in BSDs.
## PKG tool 
In this you can use pkg to play or download programms. 

`pkg play/add/list [pkgname] [local filename]`
 
 Renamed to pkg in update 2.0,
 when using function `add` downloads pkgs from programs repo (downloads [pkgname] as [local filename].py ,uses ftp tool)
 when using function `list` views https://raw.githubusercontent.com/mintdev128/FDSPKG/refs/heads/main/list.txt file contents.

`constr play [programm name]`

Available from release 1.0, 
when using function `play` executes programs.
## FTP tool
To download files to current working directory you can use FTP.
Supports ftp, http and https.
`ftp (get or req)/list/read [url] [local filename]`

Available from release 2.0, 
when using function `get` or `req` downloads files from [url] and saves as [local filename],
when using function `read` views file contents from [url] (saves it to ./tmp/ as .tmp and deletes after),
when using function `list` views links in html or xhtml files (shows <a> tags).
## More
To see more commands use

`toolbox`
Available from release 1.1 fix .
