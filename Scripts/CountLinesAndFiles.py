from os import walk
import argparse

SRC_DIR = "../Engine/Core/Engine/Source"

def count_lines_in_file(filename: str) -> int:
    line_count = 0
    with open(filename, 'r') as file:
        for line in file:
            line_count += 1
    return line_count

def count_semicolons_in_file(filename: str) -> int:
    semi_count = 0
    with open(filename, 'r') as file:
        for line in file:
            if line.find(';') != -1:
                semi_count += 1
    return semi_count

def get_all_dirs_and_files():
    files = []
    for (dirpath, _, filenames) in walk(SRC_DIR):
        if dirpath.endswith('/'):
            for file in filenames:
                files.extend([ dirpath + file ])
        else:
            dirpath += '/'
            for file in filenames:
                files.extend([ dirpath + file ])
    return files

def __main__():
    total_line_count = 0
    total_semicolon_count = 0

    files = get_all_dirs_and_files()
    for file in files:
        total_line_count += count_lines_in_file(file)
        total_semicolon_count += count_semicolons_in_file(file)
        
    print("Total Files: ", len(files))
    print("Total Lines: ", total_line_count)
    print("Total Semicolons: ", total_semicolon_count)

if __name__ == '__main__':
    __main__()