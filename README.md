# XX Graphics Library

The XX Library was created for two reasons:  to teach myself low-level graphics 
programming with the X11 system, and to create a truly object-oriented interface
for graphics development.  It will probably never compete with GTK, Qt, or 
OpenGL and I'm fine with that.

It is based on X11 because that was what existed at the time.  I may try to port it to Wayland in the future.

## Dependencies

This library is dependent on X11 and the standard C/C++ (ANSI & POSIX) libraries only.  

Documents are generated with Doxygen.  If you don't want to use it, comment out or delete the appropriate lines in the makefiles.

## Installation

You will need a `make.rules` file in the project's parent directory.  If you want to put it somewhere else, you will need to change its path in each makefile.  
It will need to define target paths for the header files (`$INCDIR`), compiled library (`$LIBDIR`), man pages (`$MANDIR`), and HTML documents (`$DOCDIR`).  

**Example:**
```
ROOTDIR=$(HOME)/.local
INCDIR=$(ROOTDIR)/include
LIBDIR=$(ROOTDIR)/lib
BINDIR=$(ROOTDIR)/bin
MANDIR=$(HOME)/man
DOCDIR=$(HOME)/Documents
```

From there, it's just
```
make all
make install
```
and you're done!

## Usage

A demo program is included.

## Contributing

1. Fork it!
   ```sh
   git clone https://github.com/github_username/repo_name.git
   ```
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## History

- **2012** First draft.
- **2026** Revived.  Updated to the latest C++.

## Credits

Scott Harris is the only person working on this.

## License

![GNU GPL v3](gplv3-88x31.png)

This project is licensed under the GNU General Public License (GPL) version 3 - see the LICENSE.txt file for details.
