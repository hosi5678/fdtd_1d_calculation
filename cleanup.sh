#!/bin/bash

# ./に含まれる*.csv , *.mp4を全て削除する

find ./ -type f -name "*.csv" -delete
find ./ -type f -name "*.mp4" -delete
