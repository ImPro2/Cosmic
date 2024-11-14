#!/bin/bash

function ensure_makefile_exists() {
    if [ ! -f ./Makefile ]; then
        ./Scripts/LinuxGenProjects.sh
    fi
}

function print_help() {
    echo "Help"
}

function incorrect_usage() {
    echo "Incorrect usage.\n"
    print_help
}

function build_projects() {
    bear -- make config=$1 $2
}

function clean_projects() {
    bear -- make clean
}

ensure_makefile_exists

case $1 in
    help)
        print_help
        ;;
    config)
        if [ "$#" = 3 ]; then
            build_projects $2 $3
        else 
            incorrect_usage
        fi
        ;;
    clean)
        clean_projects
        ;;
    *)
        incorrect_usage
        ;;
esac

