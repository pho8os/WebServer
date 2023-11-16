<?php
	if ($_SERVER['REQUEST_METHOD'] === 'GET')
		echo "<h2>Merhbaaa bl GET asi ", $_GET["f_name"], " ", $_GET["f_name"], "</h2>";
	else if ($_SERVER['REQUEST_METHOD'] === 'POST')
		echo "<h2>Merhbaaa bl POST asi ", $_POSt["f_name"], " ", $_POST["f_name"], "</h2>";
