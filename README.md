# tktk - triko toolkit
tktk is a couple of libraries designed to help in solving common game development tasks; it is written in c++.

Status
------
The project is in early stages of development.
So currently tktk provides: the asset store/access system ( tktk-asset library ), the entity-component system implementation (tktk-ecs library) and couple of related types of tktk-util library.

License
-------
tktk is released under the terms of zlib/png license; see full license text in LICENSE.md file or at https://opensource.org/licenses/Zlib

Build
-----
To build libraries:

    cd <path/to/tktk/dir>
    mkdir build
    cd build
    cmake ..
    make

Use Vagrant, Luke
-----------------
Install [virtualbox](https://www.virtualbox.org/).
For ubuntu hosts:
```bash
	sudo apt-get install virtualbox virtualbox-dkms
```

Install [vagrant](https://www.vagrantup.com/).
For ubuntu hosts (Pleasem check last avaliable release on [downloads page](https://www.vagrantup.com/downloads.html)):
```bash
	wget https://releases.hashicorp.com/vagrant/1.8.1/vagrant_1.8.1_x86_64.deb
	sudo dpkg -i vagrant_1.8.1_x86_64.deb
```

Change dir to PROJECT_DIR.
```bash
cd <path/to/tktk/dir>
```

Download Ubuntu vagrant box
```bash
vagrant box add ubuntu/trusty64
```

Run environment
```bash
vagrant up
```
To login to the VM, use `vagrant ssh`. You project folder is `/vagrant` folder on guest machine.




Documentation
-------------
To build documentation:

    cd <path/to/tktk/dir>
    doxygen
