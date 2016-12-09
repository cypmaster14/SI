#!/bin/bash

gcc -I /usr/local/ssl/include/ -L /usr/local/ssl/lib -o aux.exe aux.cpp -lcrypto -ldl
gcc -I /usr/local/ssl/include/ -L /usr/local/ssl/lib -o generate_cipher.exe generate_cipher.cpp -lcrypto  -ldl

for((i=1;i<=10;i++))
do
   mod="ECB"
   if [ $((i%2)) -eq 0 ];
   then
      mod="ECB"
   else
      mod="CBC"   
   fi


  lines=$(wc -l wordDict.txt | cut -f1 -d' ' | tr -d '[:space:]')
  ./generate_cipher.exe $mod $((1+RANDOM%lines)) "pt.txt" "ct.txt"
  ./aux.exe $mod "pt.txt" "ct.txt"
  #sleep 2s
  echo

done
