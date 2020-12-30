psymake
cpe2x /ce main.cpe
..\..\..\TOOLS\mkpsxiso.exe -o ..\cdimage\game.iso -y .\cuesheet.xml
del .\main.cpe
del .\main.sym
del .\mem.map
del .\MAIN.EXE