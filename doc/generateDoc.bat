@echo off
del /Q html\*.*
doxygen Doxy.cfg >doxygen.log 2>doxygen.err
start CRC++.chm
