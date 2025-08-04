//
// Created by Benjamin on 04/08/2025.
//

#ifndef _PACKAGES_H_
#define _PACKAGES_H_

std::string stringToUTF8Hex(const std::string& input);

std::string escape_for_js(const std::string& input);

void js_return_drives_letters( webui::window::event* e );

void select_drive_letter_gui( webui::window::event::handler::callback_t callback );

void ds_uninstall_cfw_1( webui::window::event *e );

void ds_uninstall_cfw_2( webui::window::event *e );

void ds_mset9_2( webui::window::event *e );

void ds_mset9_1( webui::window::event *e );

void exe_guiformat( webui::window::event *e );

#endif //PACKAGES_H
