#! /bin/bash
# This is v.0 of the MIB network test

SDELAY=0.5
MDELAY=2
LDELAY=30
COUNT=0

HOST=192.168.143.10
SERVER=192.168.135.100
TARGETIP=192.168.135.20
MIBIP=192.168.135.20
TYPE=1

echo "<<< This is an MIB Network Test V.0 >>>"
#echo ""
#echo "Please Enter Host IP Address: "
#read HOST

#echo ""
#echo "Please Enter Server IP: "
#read SERVER

#echo ""
#echo "Enter Target IP Address: "
#read TARGETIP

#echo ""
#echo "Enter Target MIB IP Address: "
#read MIBIP

#echo ""
#echo "Would you like to perform TCP or UDP Network Tests? (1=TCP, 2=UDP): "
#read TYPE

#echo ""
#echo "Checking Connection to Target MicroLink...>>>"
#echo ""
#sleep $MDELAY
#echo ""
#echo "Checking Connection to Target MicroLink MIB IP...>>>"
#echo ""
#sleep $MDELAY
echo "----------------------------------------------------------------------------------------------------"
ping -w 5 -c 3 $MIBIP
echo "----------------------------------------------------------------------------------------------------"

ping -w 5 -c 3 $TARGETIP


if [ $? -eq 0 ];
then
	sleep $SDELAY
	echo ""
	echo "<<< Connection to Target MicroLink Target IP Successful >>>"
	echo ""
else
	echo ""
	echo "<<< Failed to connect to Target MicroLink Target IP! >>>"
	echo ""
fi

echo ""
echo "Checking Connection to Server...>>>"
echo ""
sleep $MDELAY
echo "----------------------------------------------------------------------------------------------------"
ping -w 5 -c 3 $SERVER
echo "----------------------------------------------------------------------------------------------------"
if [ $? -eq 0 ];
then
	sleep $SDELAY
	echo ""
	echo "<<< Connection to Server Successful >>>"
	echo ""
else
	echo ""
	echo "<<< Failed to connect to Server! >>>"
	echo ""
fi

echo ""
echo "<<< All Connections Validated >>>"
echo ""
sleep $SDELAY

CONNECTED=$?

while [ $CONNECTED -eq 0 ]
do

	echo ""
	sleep $SDELAY
        echo "<<<--- Periodically Checking Connection to Target MIB IP --->>>"
	echo ""
       	ping -w 5 -c 3 $MIBIP
	if [ $? -eq 0 ];
	then
	#while [ $? -eq 0 ];
	#do
		echo ""
		echo "Connection to Target MIB (through the 1213 MMB) Successful! Initializing Iperf3 Tests"
		echo ""
		echo "----------------------------------------------------------------------------------------------------"
		echo ""
		if [ $TYPE -eq 1 ];
		then
			#/usr/bin/iperf-3.7/src/iperf3 -c $SERVER -B $HOST --bidir
		        iperf3 -c $SERVER -B $HOST -i 0.75 -t 5
		else
			iperf3 -c $SERVER -B $HOST -u -i 2 -b 64M
		fi
		echo ""
		echo "---------------------------------------------------------------------------------------------------"
		echo ""
		echo "<<< Iperf3 Tests Complete >>>"
		echo ""
	else
		ping -w 5 -c 5 $MIBIP
		while [ $? -eq 1 ]
		do
			echo ""
			echo "Connection to Target MIB (through the 1213 MMB) Failed! Initializing Log Dump..."
			echo $(date) ": No MIB COMMS - Failure Number: $COUNT" >> LogDump.txt
	        	sleep $LDELAY
			$((COUNT++)) #Try this instead...
		done
		echo ""
		echo "----------------------------------------------------------------------------------------------------"
		echo ""
		if [ $TYPE -eq 1 ];
		then
			iperf3 -c $SERVER -B $HOST -i 0.75 -t 5
		else
			iperf3 -c $SERVER -B $HOST -u -i 2 -b 64M
		fi
		echo ""
		echo "---------------------------------------------------------------------------------------------------"
		echo ""
		echo "<<< Iperf3 Tests Complete >>>"
		echo ""
	#done
	fi
done

exit 0

