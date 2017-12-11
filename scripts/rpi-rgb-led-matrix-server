#! /bin/sh
### BEGIN INIT INFO
# Provides:          rpi-rgb-led-matrix-server
# Required-Start:    $all
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Led Matrix Server
### END INIT INFO

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/bin

. /lib/init/vars.sh
. /lib/lsb/init-functions

case "$1" in
  start)
    log_begin_msg "Starting rpi-rgb-led-matrix-server"
    cd /usr/local/rpi-rgb-led-matrix-server/bin
    ./DisplayServerLed --led-gpio-mapping=adafruit-hat-pwm --led-chain=2 --led-slowdown-gpio=2 --led-pwm-lsb-nanoseconds=200 &
    log_end_msg $?
    exit 0
    ;;
  stop)
    log_begin_msg "Stopping rpi-rgb-led-matrix-server"
    killall DisplayServerLed
    log_end_msg $?
    exit 0
    ;;
  *)
    echo "Usage: /etc/init.d/<your script> {start|stop}"
    exit 1
    ;;
esac