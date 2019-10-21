# How to build morda in Linux #

## Step by step instructions ##
- Add the repo to your sources (needs to be done once):
	
	```
	echo "deb http://dl.bintray.com/igagis/deb unstable main" | sudo tee /etc/apt/sources.list.d/igagis.list > /dev/null
	```

- Update apt packages
	
	```
	sudo apt-get update
	```

- Install necessary libraries
	
	```
	sudo apt-get install libpng-dev libglew-dev libjpeg-dev libfreetype6-dev pkg-config doxygen prorab lib-dev libnitki-dev libkolme-dev libpapki-dev libsvgren-dev
	```

- Check out **morda** sources, build it and run test app:
	```
	git clone https://github.com/igagis/morda.git
	cd morda
	make debug=true
	make test
	```
