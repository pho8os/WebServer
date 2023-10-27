#!/usr/bin/php-cgi
<?php
echo "<h2>the cgi signature : <\h2>";
if ($argc > 1)
	include $argv[1];
echo "<h2>*****the end *****<\h2>";
?>