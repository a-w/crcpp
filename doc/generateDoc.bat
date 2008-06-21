@echo off
del /Q html\*.*
c:\programme\tortoisesvn\bin\subwcrev.exe .. foot.tmpl foot.html
doxygen Doxy.cfg >doxygen.log 2>doxygen.err
start CRC++.chm
