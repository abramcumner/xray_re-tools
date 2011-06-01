@echo off

set _cs_dir=%ProgramFiles%\GSC World Publishing\S.T.A.L.K.E.R
set _converter=converter.exe
set _ro_flag=

%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db0"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db1"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db2"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db3"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db4"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db5"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db6"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db7"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db8"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.db9"
%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.2947 "%_cs_dir%\gamedata.dba"

%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.3116 "%_cs_dir%\gamedata.dbb"

%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.3191 "%_cs_dir%\gamedata.dbc"

%_converter% %_ro_flag% -unpack -2947ru -dir gamedata.3312 "%_cs_dir%\gamedata.dbd"
