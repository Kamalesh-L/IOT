<!DOCTYPE html>
<html>
<head>
    <title>Display Sensor Data</title>
    <meta http-equiv="refresh" content="5"> <!-- Auto-refresh every 1 second -->
    <style>
        table {
            width: 100%;
            border-collapse: collapse;
        }
        table, th, td {
            border: 1px solid black;
            padding: 8px;
            text-align: left;
        }
        th {
            background-color: #f2f2f2;
        }
    </style>
</head>
<body>
    <h2>BMP280 Data for Temperature, Pressure and Altitude.</h2>
    
    <?php
    // MySQL connection details
    $servername = "localhost"; // Change this if your MySQL server is hosted elsewhere
    $username = "root"; // Your MySQL username
    $password = ""; // Your MySQL password
    $dbname = "iot"; // Your database name
    $tablename = "bmp280"; // Your table name
    
    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    
    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }
    
    // Fetch data from the database
    $sql = "SELECT * FROM $tablename";
    $result = $conn->query($sql);
    
    // Display data in a table
    if ($result->num_rows > 0) {
        echo "<table><tr><th>ID</th><th>Temperature</th><th>Pressure</th><th>Altitude</th><th>Timestamp</th></tr>";
        while($row = $result->fetch_assoc()) {
            echo "<tr><td>".$row["id"]."</td><td>".$row["temperature"]."</td><td>".$row["pressure"]."</td><td>".$row["altitude"]."</td><td>".$row["datetime"]."</td></tr>";
        }
        echo "</table>";
    } else {
        echo "0 results";
    }
    
    // Close connection
    $conn->close();
    ?>
    
</body>
</html>
