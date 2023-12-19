<?php

if ($_SERVER["REQUEST_METHOD"] == "GET") {
	$fname = isset($_GET['fname']) ? $_GET['fname'] : '';
	$lname = isset($_GET['lname']) ? $_GET['lname'] : '';
	$hoobie = isset($_GET['hoobie']) ? $_GET['hoobie'] : '';
	$interests = isset($_GET['interests']) ? $_GET['interests'] : '';
	$fav_language = isset($_GET['fav_language']) ? $_GET['fav_language'] : '';
	echo "<html><body>";
	echo "<h1>Form Data Received:</h1>";
	echo "<p>First Name: $fname</p>";
	echo "<p>Last Name: $lname</p>";
	echo "<p>Hobbies: $hoobie</p>";
	echo "<p>Interests: $interests</p>";
	echo "<p>Favorite Programming Language: $fav_language</p>";
	echo "</body></html>";
}
?>