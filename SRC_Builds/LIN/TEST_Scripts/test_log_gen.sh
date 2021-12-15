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
