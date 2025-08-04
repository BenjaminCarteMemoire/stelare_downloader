# stelare_downloader

## Build

g++ src/main.cpp src/storage.cpp src/bridge.cpp src/packages.cpp -Iinclude -L. -lwebui-2 -o app.exe my.res -static-libgcc -static-libstdc++ -lurlmon -lzip -lws2_32 -lole32 -loleaut32 -luuid
