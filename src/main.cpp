#include <iostream>
#include <filesystem>
#include "../include/webui.hpp"
#include "../include/storage.h"
#include "../include/bridge.h"

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#define VERSION "0.0.1"

namespace fs = std::filesystem;

int main() {

    log_info( "Begin Stelare Downloader: " + std::string(VERSION) );

    webui::window win;
    win.set_root_folder("gui/");
    set_bridge_events(&win);

    log_info( "Boot main window." );

    win.show("index.html");
    webui::wait();
    return 0;

}