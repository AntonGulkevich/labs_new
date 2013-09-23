#!/bin/bash
start() {
	START_TIME=`date +%s`
	START_TIME_N=`date +%N`
}
end() {
	END_TIME=`date +%s`
	END_TIME_N=`date +%N`
	RESULT=$(( $END_TIME-START_TIME))
	RESULT_N=$(( $END_TIME_N-START_TIME_N ))
	echo "scale=9; $RESULT + $RESULT_N/1000000000" | bc
}

BLOCK_SIZE=$1
COUNT=$2
DEVICE=/dev/cdrom1
BYTES=`fdisk -l $DEVICE  2>/dev/null | grep bytes | awk 'NR==1 {print}' | awk '{print $5}'`

echo Bytes in $DEVICE $BYTES
echo Read $2 blocks of $1 bytes

# read from begin
echo Reading from begin...
start
dd bs=$BLOCK_SIZE count=$COUNT if=$DEVICE of=/dev/null #2>/dev/null
R1=$(end)

# read from end
echo Reading from end...
START=$(( (BYTES-$BLOCK_SIZE*$COUNT)/BLOCK_SIZE ))
start
dd bs=$BLOCK_SIZE count=$COUNT skip=$START if=$DEVICE of=/dev/null #	2>/dev/null
R2=$(end)

# random read
echo Random reading...
start
for i in `seq 0 $(( COUNT / 2 - 1 ))`
	do
		dd bs=$BLOCK_SIZE count=1 skip=$(( (BYTES-$BLOCK_SIZE*($COUNT/2-$i))/BLOCK_SIZE )) if=$DEVICE of=/dev/null 2>/dev/null
		dd bs=$BLOCK_SIZE count=1 skip=$(( $i*$BLOCK_SIZE )) if=$DEVICE of=/dev/null 2>/dev/null	
	done
R3=$(end)
S1=`echo "scale=9;$BLOCK_SIZE*$COUNT/1000/$R1" | bc`
S2=`echo "scale=9;$BLOCK_SIZE*$COUNT/1000/$R2" | bc`
S3=`echo "scale=9;$BLOCK_SIZE*$COUNT/1000/$R3" | bc`
echo Read from begin = $R1 speed = $S1 KB/s
echo Read from end = $R2 speed = $S2 KB/s
echo Random read = $R3 speed = $S3 Kb/s
