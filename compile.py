import os

if os.path.isfile('_ls'):
	os.system("make clean")
else:
	os.system("make qemu");
