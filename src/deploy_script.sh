#!/bin/bash

HOST="192.168.56.101"
USER="glyndaja2"
DEST_DIR="/usr/local/bin"

scp -r cat $USER@$HOST:/home/glyndaja2
scp -r grep/ $USER@$HOST:/home/glyndaja2

if [ $? -eq 0 ]; then
    echo "Артефакты успешно скопированы на glyndaja2"
else
    echo "Ошибка при копировании артефактов на glyndaja2"
    exit 1
fi

ssh $USER@$HOST "
          rm -rf /usr/local/bin/cat; 
          rm -rf /usr/local/bin/grep;
          mv /home/glyndaja2/cat $DEST_DIR; 
          mv /home/glyndaja2/grep $DEST_DIR
        " 

if [ $? -eq 0 ]; then
    echo "Файлы успешно перемещены в /usr/local/bin"
else
    echo "Ошибка при перемещении файлов в /usr/local/bin"
    exit 1
fi