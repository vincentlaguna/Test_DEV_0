#!/sbin/bash

PASSWORD=$(echo "raspberry")

# Manually set each iface to an assigned MAC

#echo $PASSWORD | sudo -S -k ifconfig eth1 hw ether a0:ce:c8:e1:79:a2

#echo $PASSWORD | sudo -S -k ifconfig eth2 hw ether a0:ce:c8:e1:ee:96

#echo $PASSWORD | sudo -S -k ifconfig eth1 hw ether a0:ce:c8:e1:ec:db

# Set IP rules to reply incoming packets back onto the interface they came in from

echo $PASSWORD | sudo -S -k ip route add table 1 to 192.168.143.201 dev eth1
echo $PASSWORD | sudo -S -k ip route add table 1 to default via 192.168.143.100 dev eth1
echo $PASSWORD | sudo -S -k ip route add table 2 to 192.168.143.202 dev eth2
echo $PASSWORD | sudo -S -k ip route add table 2 to default via 192.168.143.100 dev eth2
echo $PASSWORD | sudo -S -k ip route add table 3 to 192.168.143.203 dev eth3
echo $PASSWORD | sudo -S -k ip route add table 3 to default via 192.168.143.100 dev eth3
echo $PASSWORD | sudo -S -k ip route add table 4 to 192.168.143.204 dev eth4
echo $PASSWORD | sudo -S -k ip route add table 4 to default via 192.168.143.100 dev eth4
echo $PASSWORD | sudo -S -k ip route add table 5 to 192.168.143.205 dev eth5
echo $PASSWORD | sudo -S -k ip route add table 5 to default via 192.168.143.100 dev eth5

echo $PASSWORD | sudo -S -k ip rule add from 192.168.143.201 table 1 priority 1
echo $PASSWORD | sudo -S -k ip rule add from 192.168.143.202 table 2 priority 2
echo $PASSWORD | sudo -S -k ip rule add from 192.168.143.203 table 3 priority 3
echo $PASSWORD | sudo -S -k ip rule add from 192.168.143.204 table 4 priority 4
echo $PASSWORD | sudo -S -k ip rule add from 192.168.143.205 table 5 priority 5

# Manually set each iface to an assigned MAC

#echo $PASSWORD | sudo -S -k ifconfig eth1 hw ether a0:ce:c8:e1:79:a2

#echo $PASSWORD | sudo -S -k ifconfig eth2 hw ether a0:ce:c8:e1:ee:96

#echo $PASSWORD | sudo -S -k ifconfig eth1 hw ether a0:ce:c8:e1:ec:db

# spawn 5 shells

for i in {1..5}
do
	xterm -hold -e "cd /home/pi/Desktop/Test_DEV_0/Inter-Process-Communication_Testing/CS_Build_v0.1/Server_vETH$i/ && make" &
	sleep 1
done

#xterm -hold -e "cd /home/pi/Test_DEV_0/Inter-Process-Communication_Testing/CS_Build_v0/Srv_Build_v0/ && make" &
#xterm -e "cd /home/pi/Test_DEV_0/Inter-Process-Communication_Testing/CS_Build_v0/Srv_Build_v1/ && make" &

#sleep 5

#xterm -hold -e "cd /home/pi/Test_DEV_0/Inter-Process-Communication_Testing/CS_Build_v0/Cl_Build_v0/ && make" &
#xterm -e "cd /home/pi/Test_DEV_0/Inter-Process-Communication_Testing/CS_Build_v0/Cl_Build_v1/ && make" &
