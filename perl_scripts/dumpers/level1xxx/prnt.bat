@dump_level11xx.pl level |gawk -F"'" '/shader/ {print $2}' |gawk -F, '{print $1}' |sort|uniq
