<?php
$cookie_fname = "default";
$cookie_lname = "default";
setcookie($cookie_fname, $cookie_lname, time() + (86400 * 30), "/"); // 86400 = 1 day
?>
<html>
<body>

<?php
if(!isset($_COOKIE[$cookie_fname])) {
  echo "Cookie named '" . $cookie_fname . "' is not set!";
} else {
  echo "Cookie '" . $cookie_fname . "' is set!<br>";
  echo "Value is: " . $_COOKIE[$cookie_fname];
}
?>

</body>
</html>