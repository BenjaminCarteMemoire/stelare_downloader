#include "../include/webui.hpp"
#include "../include/packages.h"
#include "../include/storage.h"
#include <iostream>
#include <vector>
void set_bridge_events( webui::window* win ) {

    std::vector<std::string> bridge_events = {
        "dl_uninstall_cfw_pack",
        "dl_mset9_pack",
           "dl_gui_format_exe"
    };

    std::vector<webui::window::event::handler::callback_t> bridge_handlers = {
        ds_uninstall_cfw_1,
        ds_mset9_1,
        exe_guiformat
    };

    if ( bridge_events.size() == bridge_handlers.size() ) {
        for ( int i = 0; i < bridge_events.size(); i++ ) {
            win->bind( bridge_events[i], bridge_handlers[i] );
        }
        log_info( "Main window bridge set." );
    }


}