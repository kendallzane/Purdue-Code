set pagination off
set width 0
set height 0
set verbose off


break 18

run
i r
watch i<=5
continue
echo OVER\n
watch i!=10
continue
set variable i = 0xCAFE
delete 3
delete 2
delete 1
continue