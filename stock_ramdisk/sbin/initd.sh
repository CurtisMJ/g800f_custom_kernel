#!/system/bin/sh 
export PATH=${PATH}:/system/bin:/system/xbin 
mount -o remount rw /system 
chmod 777 /system/etc/init.d/* 
mount -o remount ro /system 
for FILE in /system/etc/init.d/*; do 
  sh $FILE 
done
