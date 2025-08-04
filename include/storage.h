#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <iostream>
#include <vector>
#define STORAGE_TEMP_FOLDER "temp"
#define STORAGE_BACKUP_FOLDER "backup"
#define STORAGE_EXE_FOLDER "executables"
#define STORAGE__OK 0
#define STORAGE__ERR_CANTOPEN 55
#define STORAGE__ERR_CANTDL 57
#define STORAGE__ERR_WTFWHORU 69

void log_info( std::string info );

std::vector<std::string> return_available_removable_drives();

std::vector<std::string> get_file_list_recursively( std::string folder );

int download_in_temp_folder( std::string url, std::string filename );

int extract_all_in_temp_folder( std::string zip_path_temp, std::string output_folder_in_temp );

int extract_file_in_temp_folder( std::string zip_path_temp, std::string file_name_in_zip, std::string output_folder_in_temp_w_filename );

int extract_folder_in_temp_folder(std::string zip_path_temp, std::string file_name_in_zip, std::string output_folder_in_temp_w_filename);

int do_a_backup( std::string drive_path, std::vector<std::string> files_to_backup );

int do_a_backup_and_copy_file_to_drive( std::string drive_path, std::string drive_folder, std::string temp_file );

int do_a_backup_and_copy_folder_to_drive( std::string drive_path, std::string drive_folder, std::string temp_folder );

int move_to_executables( std::string temp_file );

#endif
