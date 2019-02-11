#!/bin/sh

case "$1" in
    start)
        echo "Starting rov-startup.py"
        /usr/local/bin/listen-for-shutdown.py &
	/usr/local/bin/record-video.py &
	/usr/local/bin/get-garbage.py &
        ;;
    stop)
        echo "Stopping rov-startup.py"
        pkill -f /usr/local/bin/listen-for-shutdown.py
	pkill -f /usr/local/bin/get-garbage.py
	pkill -f /usr/local/bin/record-video.py 
        ;;
    *)
        echo "Usage: /etc/init.d/rov-startup.sh {start|stop}"
        exit 1
        ;;
esac

exit 0
