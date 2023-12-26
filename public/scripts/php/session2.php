<?php
session_start();

$fname = isset($_SESSION["fname"]) ? htmlspecialchars($_SESSION["fname"]) : 'Not set !!';
$nname = isset($_SESSION["nname"]) ? htmlspecialchars($_SESSION["nname"]) : 'Not set !!';


echo "<html><body>";
echo "<h1><center>Form Data Received</h1>";
echo "<p>First Name: $fname</center></p>";
echo "<p>Nick Name: $nname</center></p>";

print_r($_SESSION);

?>