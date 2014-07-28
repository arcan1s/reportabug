reportabug
==========

Information
-----------

A simple Qt application/library which allows users to create an issue for GitHub projects. It may work over [GitHub](https://github.com) or [GitReport](https://gitreports.com/). For the developer configuration please use `config.h` header.

Configuration
-------------

Edit `src/config.h` header and set up needed variables.

Main configuration
------------------

Available variables:

* `OWNER` - the owner of the source repository.
* `PROJECT` - the project name.
* `TAG_BODY` - default body of an issue. It may be used for both modules.
* `TAG_TITLE` - default title of an issue. It may be used only for GitHub module.
* `TAG_ASSIGNEE` - assign an issue to this account. It may be used only for GitHub module. This tag will work only if user has push access. If it will be empty, it will be ignored.
* `TAG_LABELS` - set this labels to an issue. Labels should be comma separated. It may be used only for GitHub module. This tag will work only if user has push access. If it will be empty, it will be ignored.
* `TAG_MILESTONE` - set this milestone to an issue. It may be used only for GitHub module. This tag will work only if user has push access. If it will be empty, it will be ignored.

Send issue over GitHub
----------------------

User should type own username and password. [GitHub API](https://developer.github.com/v3/issues/) is used for creating issue. The typical POST request is

        curl -X POST -u user:pass -d '{"title":"A new bug","body":"Some error occurs"}' https://api.github.com/repos/owner/repo/issues

Available variables:

* `ENABLE_GITHUB` - enable this module. Default is `true`.
* `GITHUB_COMBOBOX` - text of this module into comboBox.
* `ISSUES_URL` - issues url, in the most cases do not touch it. Default is `https://api.github.com/repos/$OWNER/$PROJECT/issues`. Available tags here are `$PROJECT`, `$OWNER`.

This module requires `QtNetwork` module. To enable this module set variable `ENABLE_GITHUB` to `true`.

Send issue over GitReport
-------------------------

[GitReport](https://gitreports.com/about) is used for creating issue. Please visit [this page](https://gitreports.com/) and set up it for your repository.

Available variables:

* `CAPTCHA_KEY` - captcha key. It may be found in the source of the `PUBLIC_URL` page. Default is `7f6ef90bce7389088a52c5c9101aad206b21b56d`.
* `CAPTCHA_TEXT` - captcha text. It may be found in the source of the `PUBLIC_URL` page. Default is `QJNZXY`.
* `CAPTCHA_TIME` - captcha time. It may be found in the source of the `PUBLIC_URL` page. Default is `1406531273`.
* `ENABLE_GITREPORT` - enable this module. Default is `true`.
* `GITREPORT_COMBOBOX` - text of this module into comboBox.
* `PUBLIC_URL` - issues url, in the most cases do not touch it. Default is `https://gitreports.com/issue/$OWNER/$PROJECT`. Available tags here are `$PROJECT`, `$OWNER`.

This module requires `QtWebKit` module. To enable this module set variable `ENABLE_GITREPORT` to `true`.

Instruction
===========

Dependencies
------------

* qt5-base (if Qt5 is used) or qt4 (if Qt4 is used)

Optional dependencies
---------------------

* qt5-network (if Qt5 is used) (*requires for GitHub module*)
* qt5-webkit (if Qt5 is used) or qtwebkit (if Qt4 is used) (*requires for GitReport module*)

Make dependencies
-----------------

* automoc4
* cmake

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

* link your application with this library

### Available cmake flags ###

* `-DBUILD_AS_LIBRARY:BOOL=0` - build the application but not a shared library
* `-DUSE_QT5:BOOL=0` - use Qt4 instead of Qt5 for GUI

Additional information
======================

TODO (wish list)
----------------

Links
-----
