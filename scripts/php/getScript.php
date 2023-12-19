<?php

if ($_SERVER["REQUEST_METHOD"] == "POST") {
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
	// echo "<p>Favorite Programming Language: $fav_language</p>";

	echo "</body></html>";
}
else
echo '
	<form action="" method="POST">
	<label for="fname">First name:</label><br>
	<input type="text" name="fname"><br>
	<label for="lname">Last name:</label><br>
	<input type="text" name="lname"><br>
	<label for="hoobies">Hoobies:</label><br>
	<input type="text" name="hoobie"><br>
	<label for="lname">interests:</label><br>
	<input type="text" name="interests"><br>
	<label for="lname">Programming languages:</label><br>
	  <input type="radio" id="html" name="fav_language" value="C"><br>
	  <label for="html">C</label><br>
	  <input type="radio" id="css" name="fav_language" value="C++"><br>
	  <label for="css">C++</label><br>
	  <input type="radio" id="javascript" name="fav_language" value="Python"><br>
	  <label for="javascript">Python</label><br>
	<input type="submit" value="Submit"><br>
	</form>
';
?>
