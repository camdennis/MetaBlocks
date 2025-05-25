virtualenv -p /usr/bin/python3.12 myenv
source myenv/bin/activate
cd ~/Documents/Code/emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd ~/Documents/Code/websites/camdennis/static/MetaBlocks/MetaBlocks
emcc MetaBlocksFE.cpp -o MetaBlocks.js -sEXPORTED_FUNCTIONS=_check_click,_get_score,_get_box_x,_get_box_y,_get_box_size,_reset_box -sEXPORTED_RUNTIME_METHODS=ccall,cwrap
