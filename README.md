reportabug
==========

Information
-----------

Qt application/library which allows users to create an issue for GitHub projects. It may work over [GitHub](https://github.com) or [GitReport](https://gitreports.com/). For the developer configuration please use `config.h.in` header.

Configuration
-------------

Edit `src/config.h.in` header and set up needed variables.

### Main configuration ###

Available variables:

* `OWNER` - the owner of the source repository.
* `PROJECT` - the project name.
* `TAG_BODY` - default body of an issue. It may be used for both modules.
* `TAG_TITLE` - default title of an issue. It may be used only for GitHub module.
* `TAG_ASSIGNEE` - assign an issue to this account. It may be used only for GitHub module. This tag will work only if user has push access. If it will be empty, it will be ignored.
* `TAG_LABELS` - set these labels to an issue. Labels should be comma separated. It may be used only for GitHub module. This tag will work only if user has push access. If it will be empty, it will be ignored.
* `TAG_MILESTONE` - set this milestone to an issue. It may be used only for GitHub module. This tag will work only if user has push access. If it will be empty, it will be ignored.

### Send issue over GitHub ###

User should type own username and password. [GitHub API](https://developer.github.com/v3/issues/) is used for creating issue. The typical POST request is

    curl -X POST -u user:pass -d '{"title":"A new bug","body":"Some error occurs"}' https://api.github.com/repos/owner/repo/issues

Available variables:

* `GITHUB_COMBOBOX` - text of this module into comboBox.
* `ISSUES_URL` - issues url, in the most cases do not touch it. Default is `https://api.github.com/repos/$OWNER/$PROJECT/issues`. Available tags here are `$PROJECT`, `$OWNER`.

This module requires `QtNetwork` module. To disable this module use `-DENABLE_GITHUB=0` cmake flag.

### Send issue over GitHub using own token ###

This module requires your access token. Please visit [this page](https://github.com/settings/applications) and generate a new one. Needed scopes are `public_repo` (or `repo` if you will use it for a private repository). Please keep in mind that passing the token in the clear, you may discredit your account. The typical POST request is

    curl -X POST -H "Authorization: token token" -d '{"title":"A new bug","body":"Some error occurs"}' https://api.github.com/repos/owner/repo/issues

This module requires `QtNetwork` module. To enable this module set up your token using `-DOWN_GITHUB_TOKEN=` cmake flag.

### Send issue over GitReport ###

[GitReport](https://gitreports.com/about) is used for creating issue. Please visit [this page](https://gitreports.com/) and set up it for your repository.

Available variables:

* `CAPTCHA_URL` - captcha url, in the most cases do not touch it. Default is `https://gitreports.com/simple_captcha?code=`.
* `GITREPORT_COMBOBOX` - text of this module into comboBox.
* `PUBLIC_URL` - issues url, in the most cases do not touch it. Default is `https://gitreports.com/issue/$OWNER/$PROJECT`. Available tags here are `$PROJECT`, `$OWNER`.

This module requires `QtWebKit` and `QtNetwork` modules. To disable this module use `-DENABLE_GITREPORT=0` cmake flag.

Instruction
===========

Dependencies
------------

* qt5-base (if Qt5 is used) or qt4 (if Qt4 is used)
* qt5-network (if Qt5 is used)

### Optional dependencies ###

* qt5-webkit (if Qt5 is used) or qtwebkit (if Qt4 is used) *(requires for GitReport module)*

### Make dependencies ###

* automoc4
* cmake
* doxygen *(optional, developers documentation)*

Installation
------------

### Build as a single application ###

* download sources
* extract it and set up your configuration
* install the application:

        cd /path/to/extracted/archive
        mkdir build && cd build
        cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBUILD_AS_LIBRARY:BOOL=0 ../
        make
        sudo make install

### Add to your project ###

* download sources
* extract it and set up your configuration
* include it into your project. For example if you use `cmake` just add the following line into `CMakeLists.txt`:

        add_subdirectory (reportabug)

* declare class in you sources. For example:

        Reportabug *reportWindow = new Reportabug(this, false);
        reportWindow->showWindow();

* link your application with this library

### Available cmake flags ###

* `-DBUILD_AS_LIBRARY:BOOL=0` - build the application but not a shared library
* `-DBUILD_DOCS:BOOL=1` - build developer documentation
* `-DENABLE_GITHUB=0` - disable GitHub module
* `-DENABLE_GITREPORT=0` - disable GitReport module
* `-DOWN_GITHUB_TOKEN=STRING` - use STRING as own GitHub token
* `-DUSE_QT5:BOOL=0` - use Qt4 instead of Qt5 for GUI

Additional information
======================

TODO (wish list)
----------------

Links
-----
