<?php
require './privat/config.php';
$pdo = new PDO("mysql:host=".DBhost.";dbname=".DBname.";charset=utf8", DBuser, DBpassword);
$sensorID = $_GET['sensorID'];
$speed = $_GET['speed'];
$direction = $_GET['direction'];
//echo "Sensor $sensorID: $speed km/h, Richtung: $direction ";
$statement = $pdo->prepare("INSERT INTO ".DBtable." (sensorID, speed, direction) VALUES (?, ?, ?)");
$statement->execute(array(intval($_GET['sensorID']), intval($_GET['speed']), intval($_GET['direction'])));   
?>