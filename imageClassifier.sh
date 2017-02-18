#  Put this code in the folder where there are a lot of images (png/jpg)
#  Open Terminal : 
#   1. First go to the destination directory using cd
#   2. Then write the following commands :
#         chmod 777 imageClassifier.sh
#         ./imageClassifier.sh

for i in *
do
    if [[ $i == *".sh"* ]]
    then
        continue
    fi
    s=`identify -verbose code.jpg | sed -ne '/date:create: /p' | awk '{print $2}'`
    year=`echo $s | cut -c -4`
    month=`echo $s | cut -c 6- | cut -c -2`
    if [ ! -d $year ];then
        mkdir $year
    fi
    if [ ! -d ./$year/$month ];then
        mkdir ./$year/$month
    fi
    cp -u $i ./$year/$month/
done

for j in *
do
    if [[ $j == *".sh"* ]]
    then
        continue
    fi
    if [ -d $j ];then
        continue
    fi
    #echo $j
    rm $j
done
