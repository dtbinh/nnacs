package provide getoptbyrs 1.0

# Copyright Richard Suchenwirth 2006-12-08
# See http://wiki.tcl.tk/17342

proc getopt {_argv name {_var ""} {default ""}} {
    upvar 1 $_argv argv $_var var
    set pos [lsearch -regexp $argv ^$name]
    if {$pos>=0} {
	set to $pos
	if {$_var ne ""} {
	    set var [lindex $argv [incr to]]
	}
	set argv [lreplace $argv $pos $to]
	return 1
    } else {
	if {[llength [info level 0]] == 5} {set var $default}
	return 0
    }
}
