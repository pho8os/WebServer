<?php

	session_start();
	$fname = isset($_GET['fname']) ? $_GET['fname'] : '';
	$nname = isset($_GET['nname']) ? $_GET['nname'] : '';

	$_SESSION["fname"] = $fname;
	$_SESSION["nname"] = $nname;
	echo "Session variables are set.";
	



	echo "<html><body>";
	echo "<h1><center>Form Data Received</h1>";
	echo "<p>First Name: $fname</center></p>";
	echo "<p>Nick Name: $nname</center></p>";
	echo "<p><a href='session2.php'>page2</a></center></p>";
?>