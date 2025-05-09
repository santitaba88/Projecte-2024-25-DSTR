<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "sensores";

// Conectar a la base de datos
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Error de conexión: " . $conn->connect_error);
}

// Obtener el JSON de la solicitud
$json = file_get_contents("php://input");
$data = json_decode($json, true);

// Validar datos recibidos
if (!isset($data['temperatura']) || !isset($data['humedad']) || !isset($data['movimiento'])) {
    die("Error: Datos incompletos.");
}

// Insertar datos en la base de datos
$temperatura = $data['temperatura'];
$humedad = $data['humedad'];
$movimiento = $data['movimiento'];

$sql = "INSERT INTO datos (temperatura, humedad, movimiento) VALUES ('$temperatura', '$humedad', '$movimiento')";
if ($conn->query($sql) === TRUE) {
    echo "Datos guardados correctamente.";
} else {
    echo "Error: " . $conn->error;
}

$conn->close();
?>

