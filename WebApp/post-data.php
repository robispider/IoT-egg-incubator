<?php

$servername = "fdb18.awardspace.net";

// REPLACE with your Database name
$dbname = "3348988_temphumidity";
// REPLACE with Database user
$username = "3348988_temphumidity";
// REPLACE with Database user password
$password = "living2018";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "tPmAT5Ab3j7F9";

$api_key= $User = $location = $dateTime = $temperature = $temperatureC = $humidity = $fanState= $eggRotatorState= $windowsState= $humidifierState= $flameOdor= "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $User = test_input($_POST["User"]);
        $location = test_input($_POST["location"]);
        $dateTime = test_input($_POST["dateTime"]);
        $temperature = test_input($_POST["temperature"]);
        $temperatureC = test_input($_POST["temperatureC"]);
        $humidity = test_input($_POST["humidity"]);
        $fanState = test_input($_POST["fanState"]);
        $eggRotatorState = test_input($_POST["eggRotatorState"]);
        $windowsState = test_input($_POST["windowsState"]);
        $humidifierState = test_input($_POST["humidifierState"]);
        $flameOdor = test_input($_POST["flameOdor"]);
        
		
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
		//storing sensors current data into the online database
        $sql = "INSERT INTO sensorsData (groupName, location, dateTime, temperature, temperatureC, humidity,flameOdor)
        VALUES ('" . $User . "', '" . $location . "', '" . $dateTime . "', '" . $temperature . "', '" . $temperatureC . "', '" . $humidity . "', '" . $flameOdor . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
        
		// Storing actuators current state in the online database
		$sqlactuators = "INSERT INTO actuatorsData (dateTime, fanState, eggRotatorState, windowsState,humidifierState,flameOdor)
        VALUES ('" . $dateTime . "', '" . $fanState . "', '" . $eggRotatorState . "', '" . $windowsState . "', '" . $humidifierState . "','" . $flameOdor . "')";
        
        if ($conn->query($sqlactuators) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sqlactuators . "<br>" . $conn->error;
        }
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}