@echo off

set _cs_dir=%ProgramFiles%\GSC World Publishing\Clear Sky
set _converter=converter.exe
set _ro_flag=

%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_agroprom.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_atp.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_autostation.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_bath.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_darkvalley.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_factory.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_firestation.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_garbage.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_limansk.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_lost_village.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_military_1.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_military_2.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_pool.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_railroad.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_rembasa.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_rostok.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_sport_center.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_training_camp.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\mp\mp_workshop.db"

%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\levels\levels.db0"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\levels\levels.db1"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\levels\levels.db2"

%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\resources\resources.db0"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\resources\resources.db1"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\resources\resources.db2"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\resources\configs.db

%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3456 "%_cs_dir%\localization\xrussian.db"

@rem %_converter% %_ro_flag% -unpack -xdb -dir gamedata.3487 "%_cs_dir%\patches\xpatch_03.db"
%_converter% %_ro_flag% -unpack -xdb -dir gamedata.3502 "%_cs_dir%\patches\xpatch_04.db"
