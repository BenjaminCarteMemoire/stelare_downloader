#include <iostream>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <filesystem>
#include <codecvt>
#include <locale>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <string>
#include "../include/webui.hpp"
#include "../include/storage.h"
#include "../include/json.hpp"

using json = nlohmann::json;

// Each used windows.
webui::window process;
webui::window drive_letter_window;

std::string stringToUTF8Hex(const std::string& input) {
    std::ostringstream oss;
    for (unsigned char c : input) {
        oss << "%" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }
    return oss.str();
}

std::string escape_for_js(const std::string& input) {
    std::string output;
    for (char c : input) {
        switch (c) {
            case '\\': output += "\\\\"; break;
            case '\'': output += "\\\'"; break;
            case '\"': output += "\\\""; break;
            case '\n': output += "\\n"; break;
            case '\r': output += "\\r"; break;
            case '\t': output += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20 || static_cast<unsigned char>(c) > 0x7E) {
                    char buffer[7];
                    snprintf(buffer, sizeof(buffer), "\\u%04x", (unsigned char)c);
                    output += buffer;
                } else {
                    output += c;
                }
        }
    }
    return output;
}

void js_return_drives_letters( webui::window::event* e ) {

    std::vector<std::string> removable_drives = return_available_removable_drives();

    json j = removable_drives;
    std::cout << j.dump() << std::endl;

    std::string json_string = j.dump();
    e->return_string(json_string );

}

void js_change_status( std::string new_message ) {

    process.run("document.getElementById('waiting_text').innerHTML = '" + new_message + "'");

}

void js_job_done() {

    process.run( "document.getElementById('loader_bar').style.display='none'" );
    process.run( "document.getElementById('success_mark').style.display='block'" );
    process.run( "document.getElementById('warning_text').innerHTML=''" );
    process.run("document.getElementById('title_text').innerHTML='Processus terminé !'");
    js_change_status( "Le transfert a été effectué sur le lecteur. Vous pouvez fermer la fenêtre et quitter le gestionnaire." );

}

void js_summary( std::string summary ) {

    std::string hex = stringToUTF8Hex(summary);
    process.run( "document.getElementById('sep1').style.display='block'" );
    process.run( "document.getElementById('summary_text').style.display='block'" );
    process.run( "document.getElementById('sep2').style.display='block'" );
    process.run( "document.getElementById('summary').style.display='block'" );
    process.run( "document.getElementById(\"summary\").textContent = decodeURIComponent(\"" + hex + "\");" );

}

void select_drive_letter_gui( webui::window::event::handler::callback_t callback ) {

    drive_letter_window.set_root_folder("gui/");
    drive_letter_window.set_size(640, 480);
    drive_letter_window.bind("spc_drive_letters", js_return_drives_letters );
    drive_letter_window.bind("continue_package", callback);
    log_info( "Boot Drive window." );
    drive_letter_window.show("select_drive.html");
    webui::wait();

}

void processing_gui() {

    process.set_root_folder("gui/");
    process.set_icon( "assets/images/favicon.ico", "image/ico" );
    process.set_size(640, 480);
    log_info( "Boot Processing window." );
    process.show("process.html");
    // webui::wait();

}

void ds_uninstall_cfw_2( webui::window::event *e ) {

    std::string selected_drive_letter = e->get_string();
    drive_letter_window.close();
    processing_gui();
    usleep(2000000);
    log_info( "[Pack] Continue Uninstall CFW" );

    // Download process
    std::vector<std::string> prettyname = { "Luma3DS", "GodMode9", "DSi-Ware-Uninstaller", "safety-test", "uninstall_cfw" };
    std::vector<std::string> files = { "https://github.com/LumaTeam/Luma3DS/releases/download/v13.3.3/Luma3DSv13.3.3.zip", "https://github.com/d0k3/GodMode9/releases/download/v2.1.1/GodMode9-v2.1.1-20220322194259.zip", "https://github.com/MechanicalDragon0687/DSiWare-Uninstaller/releases/download/1.0.1/DSiWareUninstaller.3dsx", "https://stelare.org/assets/stelare/tutorials/uninstall_cfw/safety_test.gm9", "https://stelare.org/assets/stelare/tutorials/uninstall_cfw/uninstall_cfw.gm9" };

    if ( prettyname.size() == files.size() ) {
        for ( int i = 0; i < prettyname.size(); i++ ) {
            js_change_status( "Téléchargement de : " + prettyname[i] );
            download_in_temp_folder( files[i], std::filesystem::path(files[i]).filename().string() );
        }
    }

    // Extract process, different foreach.
    js_change_status( "Décompression de : Luma3DSv13.3.3.zip" );
    extract_all_in_temp_folder( "Luma3DSv13.3.3.zip", "luma3ds" );

    js_change_status( "Décompression de : GodMode9-v2.1.1-20220322194259.zip" );
    extract_folder_in_temp_folder( "GodMode9-v2.1.1-20220322194259.zip", "gm9", "godmode9/gm9" );
    extract_file_in_temp_folder( "GodMode9-v2.1.1-20220322194259.zip", "GodMode9.firm", "GodMode9.firm");

    // Copy folder.
    js_change_status( "Copie de Luma3DS sur le lecteur." );
    do_a_backup_and_copy_folder_to_drive( selected_drive_letter, "", "luma3ds" );

    js_change_status( "Copie de GodMode9 sur le lecteur." );
    do_a_backup_and_copy_file_to_drive( selected_drive_letter, "luma/payloads/", "GodMode9.firm" );
    do_a_backup_and_copy_folder_to_drive( selected_drive_letter, "", "godmode9" );

    js_change_status( "Copie de DSiWareUninstaller sur le lecteur.");
    do_a_backup_and_copy_file_to_drive( selected_drive_letter, "3ds/", "DSiWareUninstaller.3dsx" );

    js_change_status( "Copie de safety_test sur le lecteur.");
    do_a_backup_and_copy_file_to_drive( selected_drive_letter, "gm9/scripts/", "safety_test.gm9" );

    js_change_status( "Copie de uninstall_cfw sur le lecteur.");
    do_a_backup_and_copy_file_to_drive( selected_drive_letter, "gm9/scripts/", "uninstall_cfw.gm9" );

    // Job done.
    js_job_done();
    js_summary("-Téléchargement de Luma3DS depuis leur Github dans /temp. -Téléchargement de GodMode9 depuis leur Github dans /temp. -Téléchargement de DSiWareUninstaller depuis leur Github dans /temp. -Téléchargement de safety_test dans /temp. -Téléchargement de uninstall_cfw dans /temp. -Décompression de l'archive Luma3DS dans /temp/luma3ds. -Décompression du dossier gm9 dans /temp/godmode9. -Décompression du fichier GodMode9.firm dans /temp.-Copie de Luma3DS dans le lecteur.-Copie de GodMode9 dans le lecteur. -Copie de DSiWareUninstaller dans /3ds du lecteur. -Copie du safety_test.gm9 dans /gm9/scripts du lecteur. -Copie de uninstall_cfw.gm9 dans /gm9/scripts.");

}

void ds_uninstall_cfw_1( webui::window::event *e ){

    log_info( "[Pack] Begin Uninstall CFW" );
    select_drive_letter_gui( ds_uninstall_cfw_2 );

}

void ds_mset9_2( webui::window::event *e ) {

    std::string selected_drive_letter = e->get_string();
    drive_letter_window.close();
    processing_gui();
    usleep(2000000);
    log_info( "[Pack] Continue MSET9" );

    // Download process
    std::vector<std::string> prettyname = { "MSET9", "Python 3.13.5" };
    std::vector<std::string> files = { "https://github.com/hacks-guide/MSET9/releases/download/v2.1/MSET9-v2.1.zip", "https://www.python.org/ftp/python/3.13.5/python-3.13.5-amd64.exe" };

    if ( prettyname.size() == files.size() ) {
        for ( int i = 0; i < prettyname.size(); i++ ) {
            js_change_status( "Téléchargement de : " + prettyname[i] );
            download_in_temp_folder( files[i], std::filesystem::path(files[i]).filename().string() );
        }
    }

    // Extract process, different foreach.
    js_change_status( "Décompression de : MSET9-v2.1.zip" );
    extract_all_in_temp_folder( "MSET9-v2.1.zip", "mset9" );

    // Copy folder.
    js_change_status( "Copie de MSET9 sur le lecteur." );
    do_a_backup_and_copy_folder_to_drive( selected_drive_letter, "", "mset9" );

    js_change_status( "Transfert de Python dans les exécutables." );
    move_to_executables( "python-3.13.5-amd64.exe" );

    // Job done.
    js_job_done();
    js_summary("- MSET9 téléchargé depuis GitHub dans /temp - Installeur de Python téléchargé depuis le site officiel dans /temp - Décompression du fichier MSET9 dans /temp/mset9 - Copie des fichiers de MSET9 sur le lecteur. - Copie de l'installeur de Python dans les exécutables." );

}

void ds_mset9_1( webui::window::event *e ) {

    log_info( "[Pack] Begin MSET9" );
    select_drive_letter_gui( ds_mset9_2 );

}

void exe_guiformat( webui::window::event *e ) {

    processing_gui();
    usleep(2000000);
    log_info( "[Exe] Continue GUIFormat" );

    js_change_status( "Téléchargement de : guiformat.exe" );
    download_in_temp_folder( "https://stelare.org/assets/stelare/tutorials/sd/guiformat.exe", "guiformat.exe" );

    js_change_status( "Transfert de GUIFormat dans les exécutables." );
    move_to_executables( "guiformat.exe" );

    js_job_done();
    js_summary("- GUIFormat téléchargé dans /temp - guiformat.exe transféré dans /executables" );

}
