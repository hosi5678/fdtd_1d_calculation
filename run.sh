#!/bin/bash

# ./に含まれる*.csv , *.mp4を全て削除する
find ./ -type f -name "*.csv" -delete
find ./ -type f -name "*.mp4" -delete

directory="./csv_files/"

if [ -d "$directory" ]; then
   echo "direcory:" "$directory found"
else
   mkdir "$directory"
fi

directory="./ey_timestep_csvs/"

if [ -d "$directory" ]; then
   echo "directory:" "$directory found"
else
   mkdir  "$directory"
fi

directory="./hz_timestep_csvs/"

if [ -d "$directory" ]; then
   echo "directory:" "$directory found"
else
   mkdir  "$directory"
fi

directory="./mp4/"

if [ -d "$directory" ]; then
   echo "directory:" "$directory found"
else
   mkdir "$directory"
fi

tree > tree.txt

directory="./build/"

if [ -d "$directory" ]; then
   echo "directory:" "$directory found"
   rm -rf "$directory"
   echo "directory:" "$directory" " was deleted."

   mkdir "$directory"

   echo "directory:" "$directory" " was created."

else
   mkdir  "$directory"
fi

cmake -B ./build -G Ninja
cd ./build
ninja -t clean
ninja
cd ..
