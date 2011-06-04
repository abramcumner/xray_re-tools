###########################################################
S.T.A.L.K.E.R. gamemtl.xr compiler/decompiler

=========Использование============

Декомпиляция:	gamemtl_cdc.pl [-d input_file] [-o outdir]

-d input_file - входной файл (gamemtl.xr)
-o outdir - папка, куда сохранять конфиги материалов

Компиляция:	gamemtl_cdc.pl [-c input_dir] [-o outfile]

-c input_dir - папка, где лежат конфиги анимаций
-o outfile - выходной файл

Работает для всех билдов.

###########################################################
S.T.A.L.K.E.R. lanims.xr compiler/decompiler v.0.4

=========Использование============

Декомпиляция:	lanims_cdc.pl [-d input_file] [-o outdir] [-rgb]

-d input_file - входной файл (lanims.xr)
-o outdir - папка, куда сохранять конфиги анимаций
-rgb - представление цвета кадра в формате rgb

Компиляция:	lanims_cdc.pl [-c input_dir] [-o outfile] [-rgb]

-c input_dir - папка, где лежат конфиги анимаций
-o outfile - выходной файл
-rgb - представление цвета кадра в формате rgb

Работает для ТЧ и ЗП, остальное не пробовал.


=========История версий==========

v.0.4 - поддержка всех билдов

v.0.3 - добавлен ключ -rgb для представления цвета кадра в формате rgb

v.0.2 - добавлен разбор параметров анимаций

v.0.1 - распаковка/упаковка lanims.xr

##############################################################
S.T.A.L.K.E.R. senvironment.xr compiler/decompiler v.0.1

=========Использование============

Декомпиляция:	seinvironment_cdc.pl [-d input_file] [-o outdir]

-d input_file - входной файл (lanims.xr)
-o outdir - папка, куда сохранять конфиги анимаций

Компиляция:	seinvironment_cdc.pl [-c input_dir] [-o outfile]

-c input_dir - папка, где лежат конфиги анимаций
-o outfile - выходной файл

Работает для ТЧ и ЗП, остальное не пробовал.


=========История версий==========

v.0.1 - распаковка/упаковка senvironment.xr 

###############################################################
S.T.A.L.K.E.R. shaders_xrlc.xr compiler/decompiler v.0.1

=========Использование============

Декомпиляция:	shaders_xrlc_cdc.pl [-d input_file] [-o outdir]

-d input_file - входной файл (shaders_xrlc.xr)
-o outdir - папка, куда сохранять конфиги шейдеров

Компиляция:	shaders_xrlc_cdc.pl [-c input_dir] [-o outfile]

-c input_dir - папка, где лежат конфиги шейдеров
-o outfile - выходной файл


=========История версий==========

v.0.1 - распаковка/упаковка
