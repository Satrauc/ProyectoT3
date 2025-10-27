##.\terminalArduino.ps1 -Puerto COM4 -Baudios 57600
param(
    [string]$Puerto = "COM4",
    [int]$Baudios = 57600
)

# Crear puerto serial
$port = New-Object System.IO.Ports.SerialPort $Puerto, $Baudios, 'None', 8, 'one'
$port.ReadTimeout = 100
$port.WriteTimeout = 100
$port.Open()

Write-Host "Conectado al puerto $Puerto a $Baudios baudios."
Write-Host "Presiona Ctrl + C o escribe 'exit' para salir.`n"

try {
    while ($true) {
        # Leer datos desde Arduino
        try {
            $data = $port.ReadExisting()
            if ($data) { Write-Host $data -NoNewline }
        } catch {}

        # Ver si el usuario escribió algo
        if ($Host.UI.RawUI.KeyAvailable) {
            $key = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
            if ($key.Character -ne '') {
                if ($key.Character -eq "`n") {
                    # Ignorar Enter directo
                    continue
                }
                # Si escribe 'exit' salir
                if ($key.Character -eq 'x') {
                    Write-Host "`nSaliendo..."
                    break
                }

                # Enviar tecla al Arduino
                $port.Write($key.Character)
            }
        }

        Start-Sleep -Milliseconds 50
    }
}
finally {
    $port.Close()
    Write-Host "`nPuerto cerrado."
}
