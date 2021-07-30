# frozen_string_literal: true
#
# cgi.rb - cgi support library
#
# Copyright (C) 2000  Network Applied Communication Laboratory, Inc.
#
# Copyright (C) 2000  Information-technology Promotion Agency, Japan
#
# Author: Wakou Aoyama <wakou@ruby-lang.org>
#
# Documentation: Wakou Aoyama (RDoc'd and embellished by William Webber)
#

# == Overview
#
# The Common Gateway Interface (CGI) is a simple protocol for passing an HTTP
# request from a web server to a standalone program, and returning the output
# to the web browser.  Basically, a CGI program is called with the parameters
# of the request passed in either in the environment (GET) or via $stdin
# (POST), and everything it prints to $stdout is returned to the client.
#
# This file holds the CGI class.  This class provides functionality for
# retrieving HTTP request parameters, managing cookies, and generating HTML
# output.
#
# The file CGI::Session provides session management functionality; see that
# class for more details.
#
# See http://www.w3.org/CGI/ for more information on the CGI protocol.
#
# == Introduction
#
# CGI is a large class, providing several categories of methods, many of which
# are mixed in from other modules.  Some of the documentation is in this class,
# some in the modules CGI::QueryExtension and CGI::HtmlExtension.  See
# CGI::Cookie for specific information on handling cookies, and cgi/session.rb
# (CGI::Session) for information on sessions.
#
# For queries, CGI provides methods to get at environmental variables,
# parameters, cookies, and multipart request data.  For responses, CGI provides
# methods for writing output and generating HTML.
#
# Read on for more details.  Examples are provided at the bottom.
#
# == Queries
#
# The CGI class dynamically mixes in parameter and cookie-parsing
# functionality,  environmental variable access, and support for
# parsing multipart requests (including uploaded files) from the
# CGI::QueryExtension module.
#
# === Environmental Variables
#
# The standard CGI environmental variables are available as read-only
# attribut