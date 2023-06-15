package main

import (
	"os"
	"fmt"
	"go.bug.st/serial"
)

func ShowPortList() {
	ports, err := serial.GetPortsList()
	if err != nil {
		panic(err)
	}

	if len(ports) == 0 {
		fmt.Println("No serial ports found!")
		os.Exit(1)
	}

	fmt.Println("==================================")
	fmt.Println("           SERIAL PORTS")
	fmt.Println("==================================")
	for _, port := range ports {
		fmt.Printf("Found port: %v\n", port)
	}
	fmt.Println("==================================")
}

func SerialHTTP(serialPort string, baudRate int) {
	mode := &serial.Mode{
		BaudRate: baudRate,
	}

	port, err := serial.Open(serialPort, mode)
	if err != nil {
		panic(err)
	}

	n, err := port.Write([]byte("Hello, Golang!!\n"));
	fmt.Printf("Write: %d\n", n)

	buf := make([]byte, 1024)
	for {
		n, err := port.Read(buf)
		if err != nil {
			panic(err)
		}
		if n == 0 {
			fmt.Println("\nEOF")
			break
		}
		fmt.Println(string(buf[:n]))
	}
}

func main() {
	fmt.Println("Serial HTTP Requests")

	ShowPortList()

	// var serialPort string
	// fmt.Print("\nInput port: ")
	// fmt.Scanln(&serialPort)
// 
	// var baudRate int
	// fmt.Print("Baud Rate: ")
	// fmt.Scanln(&baudRate)

	// SerialHTTP(serialPort, baudRate)
	SerialHTTP("/dev/serial0", 9600)
}
