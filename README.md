curspace
========

This is a simple command line tool that outputs a number identifying the
current Space for Mac OS X.

The code is from: https://gist.github.com/1129406

I just turned it into an XCode project and added the appropriate frameworks.

I use it to have separate emacs instances on different Mac OS spaces such that
invoking `emacs filename` in a terminal on that space opens the file in the
emacs instance in that space.

This is the script I use to invoke emacs:

    #!/bin/sh

    EMACS=/usr/bin/emacs
    EMACSCLIENT=/usr/bin/emacsclient

    # if we have -nw on the command line, invoke a separate emacs instance
    for ARG in $*; do
        if [ $ARG = "-nw" ]; then
            exec $EMACS $*
        fi
    done

    # otherwise send the file(s) to the emacs instance on this virtual desktop
    if [ $OS = "Darwin" ]; then
        CURDESK=`curspace`
        EMACS=/Applications/Emacs.app/Contents/MacOS/Emacs
        EMACSCLIENT=/Applications/Emacs.app/Contents/MacOS/bin/emacsclient
    else
        CURDESK=`wmctrl -d | grep '\*' | awk '{ print $1 }'`
    fi

    $EMACSCLIENT -s "server$CURDESK" --no-wait $* > /dev/null 2>&1
    if [ $? != 0 ]; then
        # no instance running on this virtual desktop, so start one
        $EMACS $* &
        exit 0
    fi

And my .emacs file contains:

    ;; if we're running on X Windows or the Mac, start up the server with a
    ;; server-name specialized to the virtual desktop we currently occupy; our
    ;; emacs launcher script will attempt to open new files using said server
    (cond ((string= "x" window-system)
           (let ((cmd "wmctrl -d | grep '\*' | awk '{ print $1 }'"))
             (setq server-name (format "server%s" (chomp (shell-command-to-string cmd)))))
           (server-start))
          ((string= "ns" window-system)
           (setq server-name (format "server%s" (chomp (shell-command-to-string "curspace"))))
           (server-start))
          )

Happy hacking!
