##.\terminalArduino.ps1 -Puerto COM4 -Baudios 57600
param(
    [string]$Puerto = "COM3",
    [int]$Baudios = 9600
)

$port = New-Object System.IO.Ports.SerialPort $Puerto, $Baudios, 'None', 8, 'one'
$port.Open()

Write-Host "Conectado al puerto $Puerto a $Baudios baudios."
Write-Host "Presiona Ctrl + C para salir.`n"

try {
    while ($port.IsOpen) {
        $data = $port.ReadExisting()
        if ($data) { Write-Host $data -NoNewline }
        Start-Sleep -Milliseconds 100
    }
}
finally {
    $port.Close()
    Write-Host "`nPuerto cerrado."
}
