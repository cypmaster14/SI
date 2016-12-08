gcc -I /usr/local/ssl/include/ -L /usr/local/ssl/lib -o aux.exe aux.cpp -lcrypto -ldl
gcc -I /usr/local/ssl/include/ -L /usr/local/ssl/lib -o generate_cipher.exe generate_cipher.cpp -lcrypto  -ldl

for((i=1;i<=10;i++))
do
   mod="ECB"
   if [ $((i%2)) -eq 0 ];
   then
      mod="ECB"
      echo "If ECB"
   else
      echo "IF CBC"
      mod="CBC"   
   fi
	
  ./generate_cipher.exe $mod $((1+RANDOM%100))
  ./aux.exe $mod
  echo $((1+RANDOM%100))
  #sleep 2s

done
