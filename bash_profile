#
# ~/.bash_profile
#
[[ -f ~/.bashrc ]] && . ~/.bashrc
if [ $(tty) = "/dev/tty1" ]; then
	~/save/tmux_restore
fi
