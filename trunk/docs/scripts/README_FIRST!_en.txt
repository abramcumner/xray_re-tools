=== readme for perl scripts (trunk/scripts) ====

There are three types of scripts in this folder: dumpers, unpackers and service scripts. Dumpers are to
print content of stalker files in system console, unpackers unpacks these files to ltx files and service
scripts let your mod work easier. You should install Activeperl (http://www.activestate.com/activeperl/downloads)
and put stkutils folder into the folder with desired script to achieve correct work of script.

Dumpers folder consist of:

	-level1xxx/: dump of level files (builds 1098-1154).
	-level/: dump of level files (other builds and final versions).
	-SDK/: dump of level source files.
	-misc/: dump of some game resouce (anm, dm, ogf, ppe).

Unpackers folder consist of:
	-game_graph/: scripts for work with global game graph.
		gg_cdc_zp.pl - packer/unpacker of global CS and COP graph.
		ggrc.pl - script for unpack and merge global graphs.
	-spawn/: packers/unpackers of all.spawn.
	-level/: packers/unpackers of some level files.
		-level
		-level.game
		-level.env_mod
		-level.ps_statis
		-level.snd_statis
	-xr_formats/: packers/unpackers of some *xr files.
		-gamemtl.xr
		-lanims.xr
		-seinvironment.xr
		-shaders_xrlc.xr

Service folder consist of:
	-resource_copier/ - copier of working level from SDK with resources linked with level.
	-chunk_parser/ - decompilation of RIFF-files to separate chunks.

See other readmes for detailed information.
Note: if using xiani scripts use stkutils.xiani (rename it to stkutils).