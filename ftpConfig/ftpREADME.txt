				Ubuntu下vsftpd安装配置
第一步：安装vsftpd
	sudo apt-get install vsftpd
	安装完成后重启VSFSTP服务
	sudo service vsftpd restart

第二步：新建目录/home/uftp
	sudo mkdir /home/uftp

第三步：1)新建用户uftp,指定用户目录和所用shell,并设置密码
	sudo useradd -d /home/uftp -s /bin/bash uftp
	2)将目录/home/uftp的所属着和所属组都改为uftp
	sudo chown uftp:uftp /home/uftp
	3)设置用户的密码
	sudo passwd uftp

第四步：新建文件/etc/vsftpd.user_list,用于存放允许访问ftp的用户
	sudo vim /etc/vsftpd.user_list

第五步：重启vsftpd
	sudo vsftpd restart

