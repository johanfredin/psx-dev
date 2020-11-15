psymake
cpe2x /ce main.cpe
..\..\TOOLS\mkpsxiso.exe -o cdimage\game.iso -y .\cuesheet.xml
rm .\main.cpe
rm .\image.cue
rm .\main.sym
rm .\mem.map