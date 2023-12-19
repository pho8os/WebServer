#!/usr/bin/env python
import sys
import os
import cgi
import cgitb
# sys.path.insert(0, "/usr/bin/python3")

cgitb.enable()
sys.stderr = sys.stdout

# cgi.test()
form = cgi.FieldStorage()
cgi.print_form(form)