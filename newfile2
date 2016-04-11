all:yancysend	yancyrecv
	
yancysend:transfer_cli.c
	gcc transfer_cli.c -o yancysend

yancyrecv:transfer_serv.c
	gcc transfer_serv.c -o yancyrecv
	
clean:yancysend	yancyrecv
	rm -rf yancysend yancyrecv

install:yancysend	yancyrecv
	sudo cp ./yancysend /usr/bin
	sudo cp ./yancyrecv /usr/bin

uninstall:/usr/bin/yancysend	/usr/bin/yancyrecv
	sudo rm -rf /usr/bin/yancysend
	sudo rm -rf /usr/bin/yancyrecv
