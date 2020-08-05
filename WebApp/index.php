<!DOCTYPE html>
<html>
<head>
<script language="javascript">
  setTimeout(function() {
    window.location.reload();
}, 30000);
</script>


	
<title> Display Smart Poultry data</title>

<style>
	#parent {
		display: flex;
	}
	#div1 {
	  width: 50%;	  
	  /* Just so it's visible */
	  margin-right:10px;
	}
	#div2{
	  flex: 1;
	  width: 40%;
	}
</style>
</head>

<body bgcolor='lightcyan'>
<?php

	$servername = "fdb18.awardspace.net";

	// REPLACE with your Database name
	$dbname = "3348988_temphumidity";
	// REPLACE with Database user
	$username = "3348988_temphumidity";
	// REPLACE with Database user password
	$password = "living2018";

	// Create connection
	$conn = new mysqli($servername, $username, $password, $dbname);
	// Check connection
	if ($conn->connect_error) {
		die("Connection failed: " . $conn->connect_error);
	} 
?>
<br>
<center><img src="kic.jpg" alt="logo KIC"></center>
<br>
<h1 align="center" style="margin-top:10px;"><font color="red"> SMART POULTRY PORTAL </font> </h1>
<h2 align="center"><a href="actuators.php" target="_blank">Display events</a></h2>
<h2 align="center">  Data collected by the Sensor </h2>	
<div id ="parent" width="90%" style="">
	<?php
	$sql = "SELECT id, groupName, location, dateTime, temperature, temperatureC, humidity,flameOdor FROM sensorsData ORDER BY id DESC";
								  
	echo '<table cellspacing="5" cellpadding="5" border="1" align="center">
		  <tr> 
			<td>ID</td> 
			<td>GroupName</td> 
			<td>Location</td> 
			<td>Reading Time</td> 
			<td>Temperature (F)</td>
			<td>Temperature (°C)</td> 
			<td>Humidity(%)</td>
			<td>flameOdor</td>
		  </tr>';

	//data  to collect json
	//$data = array();

	if ($result = $conn->query($sql)) {
		while ($row = $result->fetch_assoc()) {
			$row_id = $row["id"];
			$row_user = $row["groupName"];
			$row_location = $row["location"];
			$row_dateTime = $row["dateTime"];
			$row_temperature = $row["temperature"]; 
			$row_temperatureC = $row["temperatureC"]; 
			$row_humidity = $row["humidity"]; 
			$row_flameOdor= $row["flameOdor"];
			
			if($row_flameOdor==1){
				$row_flameOdor="FLAME SMOKE";				
			}
			else $row_flameOdor="NO FLAME SMOKE";
			
			
			echo '<tr> 
					<td>' . $row_id . '</td> 
					<td>' . $row_user . '</td> 
					<td>' . $row_location . '</td> 
					<td>' . $row_dateTime . '</td> 
					<td>' . $row_temperature . '</td>
					<td>' . $row_temperatureC . '</td> 
					<td>' . $row_humidity . '</td> 
					<td>' . $row_flameOdor . '</td> 
				  </tr>';
		}
	
		
		$result->free();
	}

	echo'</table>';

	//print json_encode($data);
$conn->close();

?>  
</div>
	

</body>
</html>