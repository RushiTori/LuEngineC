INC_DIR="include"
for inc_file in "$INC_DIR"/*
do
	echo -n "#include <LuEngineC/" >> LuEngine.h
	echo -n "${inc_file##*/}" >> LuEngine.h
	echo ">" >> LuEngine.h
done
