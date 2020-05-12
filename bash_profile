#
# ~/.bash_profile
#
[[ -f ~/.bashrc ]] && . ~/.bashrc
if [ $(tty) = "/dev/ttyS0" ] || [ $(tty) = "/dev/tty1" ]; then
	/home/patatash/inception/tmux_restore
fi
