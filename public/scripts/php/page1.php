<?php

if ($_SERVER["REQUEST_METHOD"] == "GET") {
	$fname = isset($_GET['fname']) ? $_GET['fname'] : '';
	setcookie("fname", $fname, time() + (86400 * 30), "/"); // 86400 = 1 day
	echo "<html><body>";
	echo "<h1><center>Form Data Received</h1>";
	echo "<p>First Name: $fname</center></p>";
	echo "<p><a href='page2.php'>page2</a></center></p>";
}
?>