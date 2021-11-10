#!/bin/bash

sleep 1

PASSWORD=$(echo "root")

# Set IP rules to reply incoming packets back unto the interface they came from

echo $PASSWORD | sudo -S -k ip route add table 1 to 192.168.128.250 dev enxa0cec8e179a2 
echo $PASSWORD | sudo -S -k ip route add table 1 to default via 192.168.136.100 dev enxa0cec8e179a2

echo $PASSWORD | sudo -S -k ip route add table 2 to 192.168.128.251 dev enxa0cec8e1b5a3 
echo $PASSWORD | sudo -S -k ip route add table 2 to default via 192.168.136.100 dev enxa0cec8e1b5a3

echo $PASSWORD | sudo -S -k ip route add table 3 to 192.168.128.252 dev enxa0cec8e1ecdb 
echo $PASSWORD | sudo -S -k ip route add table 3 to default via 192.168.136.100 dev enxa0cec8e1ecdb

echo $PASSWORD | sudo -S -k ip route add table 4 to 192.168.128.253 dev enxa0cec8e1ece0
echo $PASSWORD | sudo -S -k ip route add table 4 to default via 192.168.136.100 dev enxa0cec8e1ece0

echo $PASSWORD | sudo -S -k ip route add table 5 to 192.168.128.254 dev enxa0cec8e1ee96
echo $PASSWORD | sudo -S -k ip route add table 5 to default via 192.168.136.100 dev enxa0cec8e1ee96


echo $PASSWORD | sudo -S -k ip rule add from 192.168.128.250 table 1 priority 1
echo $PASSWORD | sudo -S -k ip rule add from 192.168.128.251 table 2 priority 2
echo $PASSWORD | sudo -S -k ip rule add from 192.168.128.252 table 3 priority 3
echo $PASSWORD | sudo -S -k ip rule add from 192.168.128.253 table 4 priority 4
echo $PASSWORD | sudo -S -k ip rule add from 192.168.128.254 table 5 priority 5

# spawn 5 shells

for i in {1..5}
do
	xterm -hold -e "cd /home/pi/Desktop/Test_DEV_0/Inter-Process-Communication_Testing/CS_Build_v0.1/Server_vETH$i/ && echo $PASSWORD | sudo -S -k make all" &
	sleep 1
done

#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e179a2 down
#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e179a2 up

#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e1b5a3 down
#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e1b5a3 up

#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e1ecdb down
#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e1ecdb up

#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e1ece0 down
#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e1ece0 up

#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e1ee96 down
#echo $PASSWORD | sudo -S -k ip link set dev enxa0cec8e1ee96 up
