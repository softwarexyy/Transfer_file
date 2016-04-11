# Transfer_file
传输文件，linux

使用方式：
  进入源码文件夹执行指令：sudo make install

  接收文件方：
    键入"yancyrecv 待接收的文件目录"
    （比如要将传来的文件接收到目录并存储到"/home/yancy/demo.txt下时"，键入 yancyrecv /home/yancy/demo.txt)
    
  发送文件方：
    键入"yancysend 对方的IP地址 待发送文件目录"
    (比如要将文件/home/yancy/demo.txt发送给IP是192.168.123的主机时，键入 yancysend 192.168.123 /home/yancy/demo.txt)


卸载软件：
  进入源码文件夹执行指令：sudo make uninstall
