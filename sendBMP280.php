<?php
// Database connection parameters
$servername = "localhost";
$username = "root"; // Replace with your MySQL username
$password = ""; // Replace with your MySQL password
$dbname = "iot";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Retrieve sensor data from HTTP POST request
$temperature = $_POST['temperature'];
$pressure = $_POST['pressure'];
$altitude = $_POST['altitude'];

// Prepare SQL statement to insert sensor data into database table
$sql = "INSERT INTO bmp280 (temperature, pressure, altitude) VALUES ('$temperature', '$pressure', '$altitude')";

if ($conn->query($sql) === TRUE) {
    echo "New record inserted successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
