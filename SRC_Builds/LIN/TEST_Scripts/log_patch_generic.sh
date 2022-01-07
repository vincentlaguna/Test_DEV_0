#!/bin/bash
#
# Script for logrotate patch for continuous syslog error output that fills the rootfs memory
set +x 

touch /etc/systemd/system/logrotate.service
touch /etc/systemd/system/logrotate.timer

echo "[-]CREATING=logrotate service..."
echo "[UNIT]" > /etc/systemd/system/logrotate.service
echo "Description=Starts the logrotate service to maintain specified log rotation." >> /etc/systemd/system/logrotate.service
echo " " >> /etc/systemd/system/logrotate.service
echo "[Service]" >> /etc/systemd/system/logrotate.service
echo "ExecStart=/usr/sbin/logrotate -s /run/logrotate.status /etc/logrotate.conf" >> /etc/systemd/system/logrotate.service

echo "[+]CREATED=logrotate service"
echo "[-]CREATING=logrotate timer..."

echo "[UNIT]" > /etc/systemd/system/logrotate.timer
echo "Description=Start logrotate after 1m on boot every 1m" >> /etc/systemd/system/logrotate.timer
echo "Wants=logrotate.service" >> /etc/systemd/system/logrotate.timer
echo " " >> /etc/systemd/system/logrotate.timer
echo "[Timer]" >> /etc/systemd/system/logrotate.timer
echo "OnBootSec=90sec" >> /etc/systemd/system/logrotate.timer
echo "OnUnitActiveSec=1m" >> /etc/systemd/system/logrotate.timer
echo " " >> /etc/systemd/system/logrotate.timer
echo "[Install]" >> /etc/systemd/system/logrotate.timer
echo "WantedBy=timers.target" >> /etc/systemd/system/logrotate.timer

echo "[+]CREATED=logrotate timer"
echo "[-]SETTING PERMISSIONS..."

# set permissions on service files
sudo chmod 664 /etc/systemd/system/logrotate.timer
sudo chmod 664 /etc/systemd/system/logrotate.service
sudo ln -sf "/etc/systemd/system/logrotate.timer" "/etc/systemd/system/timers.target.wants/logrotate.timer"

echo "[-]ENABLING logrotate.timer..."
# enable logrotate.timer
sudo systemctl enable logrotate.timer

echo "[-]CREATING=syslog WD script for monitoring changes in ownership on syslog..."
# generate syslog WD script
touch /usr/bin/test_syslog_wd.sh

echo ...
cat > /usr/bin/test_syslog_wd.sh << 'EOT'
#!bin/bash

# syslog WD script for monitoring changes in ownership on syslog

P=$(echo "password")
SYSLOG=/var/log/syslog
INTERVAL=600

while [ true ];
do
	sleep $INTERVAL

	if [ ! -s $SYSLOG ];
	then
		echo $P | sudo -S -k chown syslog $SYSLOG
		echo $P | sudo -S -k chgrp adm $SYSLOG
		echo "Changed back ownership on /var/log/syslog..." >> /tmp/TestLaunch.log
	fi
done
EOT

echo "[-]SETTING PERMISSIONS on test_syslog_wd.sh..."
# set permissions on syslog WD script
sudo chmod 664 /usr/bin/test_syslog_wd.sh

echo "[+]CREATED=syslog WD script"

echo " "

echo "[-]INSERTING=lines to launch syslog WD script into test_shell_launch.sh"
# insert lines into test_shell_launch script to launch syslog WD script
# test_shell_launch is a script that is ran from a systemd .service on every boot

MATCH='i2cset -y 1 0x5f 0xff 0 0 0x80 0x80 0 0x0f 0 i'
INSERT='#launch syslog WD script in the background'
INSERT1='/usr/bin/test_syslog_wd.sh &'
FILE='/usr/bin/test_shell_launch.sh'

sed -i "/$MATCH/a$INSERT" $FILE
sed -i "/$INSERT/a$INSERT1" $FILE

echo "[+]INSERTED=lines to launch syslog WD script from test_shell_launch.sh"

echo "[-]CREATING=log generator script for testing..."
# generate log test generator script
touch /usr/bin/test_log_gen.sh

echo ...
cat > /usr/bin/test_log_gen.sh << 'EOT'
#!bin/bash

# log output generator

ITERATOR=0

while [ true ];
do
	for i in auth daemon kern syslog local0 local1 local2 local3 local4 local5 local6 local7;
	do
		for j in debug info notice warning err crit alert emerg;
		do
			for k in 1..10
			do
				logger -e -s -p $i.$j "TEST ERROR[$ITERATOR]: OUTPUT #$ITERATOR=ACTIVE, FACILITY[$i], PRIORITY[$j]"
				ITERATOR=$(($ITERATOR+1))
				tail -n 100 /var/log/syslog >> /var/log/syslog
			done
		done
	done
done
EOT

echo "[-]SETTING PERMISSIONS..."
# set permissions on log generator script
sudo chmod 664 /usr/bin/test_log_gen.sh

echo "[+]CREATED=log generator script"
echo " "
echo "***[+]LOG-OUTPUT-PATCH=SUCCESS***"
