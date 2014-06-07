# README #

ffos-cli is a simple program made in C to help programmers in creating Firefox OS applications easier. 

### Getting and compiling ffos-cli ###
To get the ffos-cli and compile it in your machine, firstly get the jansson c library, which you can get by executing
```
sudo apt-get install libjansson-dev
```

After that, clone this repository to your machine by executing
```
git clone https://github.com/aziflaj/ffos-cli.git
cd ffos-cli
make
```

This will compile ffos-cli. After that, add ffos-cli to your path. You've got ffos completely installed now.

### Creating a new project ###
In order to create a new project folder with ffos-cli, you should execute
```
ffos create FooProject    # assuming that the project will be called FooProject
```

Then, position yourself to the created directory by executing
```
cd FooProject
```

Let's change some things in the manifest file. Firstly, we change the developer's name by executing
```
ffos set dev.name Aldo Ziflaj
ffos set dev.url https://www.aldoziflaj.me
```

(I'm using my own name and non-existing url here)

After that, we change the description of the project, which by default is "App description here". We do this by executing
```
ffos set descr "This application does magnificient things etc..."
```

By default, the project version is set to 1.0, but you may change this by executing
```
ffos set version 0.3
```

Now you start writing the code for your application. ffos-cli creates by default a file named index.html which is set as the launch path. But, you may change this. Firstly you create a html file by executing
```
ffos add html main
After main.html is created, you make it launch path by executing
ffos set launch_path main.html
```

You can also add other files by executing
```
ffos add css main 	# this creates main.css
ffos add js app		# this creates app.js
```

### License ###
ffos-cli is licensed under [GNU General Public License](http://www.gnu.org/copyleft/gpl.html)
