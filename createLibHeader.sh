INC_DIR="include"
for inc_file in "$INC_DIR"/*
do
	echo -n "#include <LuEngine/" >> LuEngine.h
	echo -n "${inc_file##*/}" >> LuEngine.h
	echo ">" >> LuEngine.h
done
