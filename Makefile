# Nombre del microcontrolador
MCU = atmega2560

# Frecuencia del reloj (ajústala según tu cristal/placa)
F_CPU = 16000000UL

# Herramientas (si tienes Arduino IDE instalado, estas rutas funcionan)
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Flags de compilación
CFLAGS = -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

# Archivos
TARGET = main
SRC = main.c adc.c uart.c SignalProcessing.c Motores.c fc03.c

# Programador (ajusta según tu hardware: usbasp, stk500v2, arduino, etc.)
PROGRAMMER = wiring
PORT = COM4
BAUD = 115200

all: $(TARGET).hex

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) -Wl,-u,vfprintf -lprintf_flt -lm -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	$(AVRDUDE) -p m2560 -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$<

clean:
	rm -f *.o *.elf *.hex
