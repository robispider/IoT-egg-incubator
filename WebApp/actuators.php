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
<h1 align="center" style="margin-top:10px;"><font color="red"> SMART POULTRY PORTAL </font> </h1>
<br><h2 align="center">  Actuators States </h2>
<div id ="parent" width="90%" style="">
	   
	<br>
	<?php
	$sql1 = "SELECT id, dateTime, fanState, eggRotatorState, windowsState,humidifierState FROM  actuatorsData ORDER BY id DESC";
	 									  
	echo '<table cellspacing="5" cellpadding="5" border="1" align="center">
		  <tr> 
			<td>ID</td>  
			<td>Reading Time</td> 
			<td>Fan State</td>
			<td>Egg Rotator</td> 
			<td>Windows State</td>
			<td>HumidifierState</td>
		  </tr>';

	//data  to collect json
	//$data = array();

	if ($result = $conn->query($sql1)) {
		while ($row = $result->fetch_assoc()) {
			$row_id = $row["id"];
			$row_dateTime = $row["dateTime"];
			$row_fanState = $row["fanState"]; 
			$row_eggRotatorState = $row["eggRotatorState"]; 
			$row_windowsState = $row["windowsState"]; 
			$row_humidifierState= $row["humidifierState"]; 
			
			if($row_fanState==1){
				$row_fanState="ON";				
			}
			else $row_fanState="OFF";
			
			if($row_eggRotatorState==1){
				$row_eggRotatorState="ON";				
			}
			else $row_eggRotatorState="OFF";
			
			if($row_windowsState==1){
				$row_windowsState="OPEN";				
			}
			else $row_windowsState="CLOSE";
			
			if($row_humidifierState==1){
				$row_humidifierState="ON";				
			}
			else $row_humidifierState="OFF";
			
			echo '<tr> 
					<td>' . $row_id . '</td> 
					<td>' . $row_dateTime . '</td>
					
					<td>' . $row_fanState . '</td>
					<td>' . $row_eggRotatorState . '</td> 
					<td>' . $row_windowsState . '</td> 
					<td>' . $row_humidifierState . '</td> 
				  </tr>';
		}
	
		
		$result->free();
	}

	echo'</table>';
?>
	</div>
<?php
	//print json_encode($data);
$conn->close();

?>  
</div>
	

</body>
</html>