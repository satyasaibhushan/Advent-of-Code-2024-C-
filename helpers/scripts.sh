cpp() {
        folder=$1
        file=${2:-"sol.cpp"}
        if [ -z "$folder" ]; then
                echo "Usage: aoc <folder> [file]"
                return 1
        fi
        if [ ! -d "$folder" ]; then
                echo "Folder does not exist: $folder"
                return 1
        fi
        if [ ! -f "$folder/$file" ]; then
                echo "File does not exist: $folder/$file"
                return 1
        fi
        g++ -std=c++17 -o ./$folder/exec ./$folder/$file
        ./$folder/exec
        rm ./$folder/exec
}

create_folder() {
        folder=$1
        if [ -z "$folder" ]; then
                echo "Usage: aoc <folder>"
                return 1
        fi

        if [ -d "$folder" ]; then
                echo "Folder already exists: $folder"
                return 1
        fi

        pwd
        mkdir $folder
        cp ./boilerplate/sol.cpp $folder/sol.cpp
        touch $folder/question
        touch $folder/sample
        echo "Folder created: $folder"
        echo "File created: $folder/sol.cpp"
        echo "File created: $folder/question"
        echo "File created: $folder/sample"

        return 0
}

if [[ "$1" != "" ]]; then
  "$@"
else
  echo "Usage: $0 <function> [arguments...]"
  echo "Available functions: create_folder, cp"
fi