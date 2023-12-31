<?php
$cookie_name = "fname";
if(!isset($_COOKIE["fname"])) {
	echo 'There is no cookie';
//  echo "Cookie named '" . $cookie_name . "' is not set!";
} else {
  echo "Value is: " . $_COOKIE[$cookie_name];
}