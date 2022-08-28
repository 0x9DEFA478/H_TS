
echo off
 
set CurrPath=%~dp0 
 
cd Listings
DEL /F/S/Q *.map
DEL /F/S/Q *.lst
cd %CurrPath%
 
cd Objects
DEL /F/S/Q *.o
DEL /F/S/Q *.d
DEL /F/S/Q *.crf
DEL /F/S/Q *.dep
DEL /F/S/Q *.htm
DEL /F/S/Q *.lnp
DEL /F/S/Q *.Bak
cd %CurrPath%
 
 
pause
