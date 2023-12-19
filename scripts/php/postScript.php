<?php

if ($_SERVER["REQUEST_METHOD"] == "POST") {
	$fname = isset($_POST['fname']) ? $_POST['fname'] : '';
	$lname = isset($_POST['lname']) ? $_POST['lname'] : '';
	$hoobie = isset($_POST['hoobie']) ? $_POST['hoobie'] : '';
	$interests = isset($_POST['interests']) ? $_POST['interests'] : '';
	$fav_language = isset($_POST['fav_language']) ? $_POST['fav_language'] : '';
	echo "<html><body>";
	echo "<h1>Form Data Received:</h1>";
	echo "<p>First Name: $fname</p>";
	echo "<p>Last Name: $lname</p>";
	echo "<p>Hobbies: $hoobie</p>";
	echo "<p>Interests: $interests</p>";
	echo "<p>Favorite Programming Language: $fav_language</p>";
	echo "</body></html>";
}
else
	echo ' <center><h1>Methode Not Supported !!</h1></center>'
?>