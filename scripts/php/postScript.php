<?php
$fname = isset($_POST["fname"]) ? $_POST["fname"] : '-';
$lname = isset($_POST["lname"]) ? $_POST["lname"] : '-';



echo "<html><body>";
echo "<h1>Form Data Received:</h1>";
echo "<p>First Name: $fname</p>";
echo "<p>Last Name: $lname</p>";
echo "</body></html>";
?>
