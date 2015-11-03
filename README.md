Rin : simulator for Cartelet
=========

使い方  
make  
./rin [options] filename  

オプション  
-h : help  
-s : step exec  
-o [filename] : send8のデータを[filename]にバイナリファイルとして出力  
-r : 結果のみを表示  

step実行時コマンド　（入力時[]はつけない）  
h , help : help  
r , run : run  
s , step : step  
b [addr] : [addr]にbreakpointを設定  
bi [addr] [n] : はじめの[n]回はbreakpointに達しても停止しない  
db [addr] : [addr]に設定したbreakpointを削除  
pg [n] : print GPR [n]  
pf [n] : print FPR [n]  
pm [addr] : print memory [addr]  
ps : print statistics  