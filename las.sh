#!/bin/bash

print_help() {
  echo -e "Las (Log, Analyze and Share) app
The Las tool is useful for removing temporary code and substitute it with final version when developing new features, debugging issues etc.
If you want to commit changes, you don't need to check and clear every file to remove unwanted code.
You can share with others clean code version using diff files without actually changing your current code.

Las tags can be used with multiple programming languages or with plain text files. Available tags should be prefixed with given language comment or # for plain text:
Removes code between ^^las begin^^ and ^^las end^^ or single line tagged with ^^las^^.
Optionally substitutes removed code with changes enclosed by ^^las sub begin^^ and ^^las sub end^^ or for single line by the code following ^^las^^ in that line. 
Usage for CPP language, where // is comment(for other langs substitute \"//\" for corresponding comment token):

//^^las begin^^
code
//^^las end^^
//^^las sub begin^^
code - often commented
//^^las sub end^^

or
line//^^las^^ substitution code

or
line//^^las^^
//^^las sub begin^^
code - often commented
//^^las sub end^^

Las commands:\n
  - las cut [filenames]
    e.g. las cut ; las cut file1 file2
    Remove code/add substitution code marked with LAS tags.
    Command changes files content. \n
  - las diff [filenames]
    e.g. las diff ; las diff file1 file2
    Create diff output from removed code/added substitution code marked with LAS tags
    in files from working area.
    Command keeps files content untouched.\n
  - las diff head [filenames]
    e.g. las diff head ; las diff head file1 file2
    Create diff output from removed code/added substitution code marked with LAS tags
    in files from working and staging area.
    Command keeps files content untouched.\n
  - las diff staged [filenames]
    e.g. las diff staged ; las diff staged file1 file2
    Create diff output from removed code/added substitution code marked with LAS tags
    in files from staging area.
    Command keeps files content untouched.\n
  - las restore -n<number>
    e.g. las restore -n0 \n
    Command allows to restore state of repository as before executing given las command.
    <number> is related to given snapshot and can be obtained using \"las restore --list\".
    Command changes files content and must be run on repository without uncommited changes.\n
  - las restore --list
    Command shows list of already taken repository's snapshots.
    Command keeps files content untouched.\n
  - las restore --show -n<number>
    e.g. las restore --show -n2
    Command show changes related to given snapshot, where <number> can be obtained using \"las restore --list\".
    Command keeps files content untouched.\n "
}

if ! command -v podman &> /dev/null; then
  echo -e "Podman is not installed on the system"
  exit 1
fi

GIT_REPO_ROOT=$(git rev-parse --show-toplevel)
if [ $? -ne 0 ]; then
  echo -e "You are not in GIT repository"
  exit 1
fi

GIT_REPO_NOT_EMPTY=$(git rev-parse HEAD &> /dev/null)
if [ $? -ne 0 ]; then
  echo -e "GIT repository is empty. Please create first commit before use"
  exit 1
fi

if [[ "$1" == "--help" || "$1" == "-- help" || "$1" == "-h" ]]; then
  print_help
  exit 0
fi

#parse input files if exist
REPO_PREFIX_ROOT=$(git rev-parse --show-prefix)
LAS_ARGS=()
for arg in "$@"; do
  if [ -e "$arg" ]; then
    file_with_prefix="${REPO_PREFIX_ROOT}${arg}"
    LAS_ARGS+=("$file_with_prefix")
  else
    LAS_ARGS+=("$arg")
  fi
done

LAS_IMAGE="las_app_image"
mkdir -p ".las_directory"
mkdir -p ".las_directory/areas" ".las_directory/backup"

if podman inspect ${LAS_IMAGE} > /dev/null 2>&1; then
  podman run --rm --interactive --tty -v ${GIT_REPO_ROOT}:/run_las ${LAS_IMAGE} ${LAS_ARGS[@]}
else
  echo -e "Container for las app cannot be created.\nRun \"las_setup.sh\" from the downloaded las app directory"
  exit 1
fi

