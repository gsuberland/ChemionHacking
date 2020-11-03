"""
    Simple library to work with CHEMION glasses in Python
"""
from time import sleep

import bluepy.btle as btle
from bluepy.btle import Scanner, BTLEException

from colorama import Fore

class Glasses:
    """
        Class to interact with the CHEMION glasses
    """
    def __init__(self, glasses_address):
        self.address = glasses_address

        self.uart_service_uuid = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
        self.uart_rx_uuid = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

        self.write_characteristic = None
        self.connected = False
        self.peripheral = None


    def connect(self):
        """
            Connects to the saved address
        """
        self.peripheral = btle.Peripheral(self.address, addrType=btle.ADDR_TYPE_RANDOM)
        print("[" + Fore.CYAN + "*" + Fore.RESET + "] Connected to glasses")
        self.connected = True

        service = self.peripheral.getServiceByUUID(self.uart_service_uuid)

        if service:
            print("[" + Fore.CYAN + "*" + Fore.RESET + "] Service found")
            caracteristics_list = service.getCharacteristics()

            for char in caracteristics_list:
                if char.uuid == self.uart_rx_uuid:
                    self.write_characteristic = char
                    print("[" + Fore.CYAN + "*" + Fore.RESET + "] Characteristic found")


    def display(self, buffer: bytes, delay=10):
        """
        Displays the buffer during 10 seconds by default

        Args:
            buffer (bytes): buffer containing the pattern
            delay (int, optional): time during which the pattern is displayed. Defaults to 10.

        Raises:
            BTLEException: is thrown if no glass are connected 
                when the function is called
            BTLEException: is thrown if the writing characteristic 
                hasn't been found when the function is called
        """
        if self.connected:
            if self.write_characteristic:
                print("[" + Fore.CYAN + "*" + Fore.RESET + "] Sending buffer ...")

                for byte in buffer:
                    self.write_characteristic.write(byte)

                print("[" + Fore.CYAN + "*" + Fore.RESET + "] Buffer sent")
                sleep(delay)
            else:
                raise BTLEException("Characteristic not found")
        else:
            raise BTLEException("No glasses are connected")


    def disconnect(self):
        """
        Disconnects from the glasses

        Raises:
            BTLEException: is thrown if no glasses are connected
        """
        if self.connected:
            self.peripheral.disconnect()
            print("[" + Fore.RED + "-" + Fore.RESET + "] Glasses disconnected")
        else:
            raise BTLEException("No glasses are connected")


    @staticmethod
    def discover():
        """
        Scans for BLE devices and tries to automatically select the glasses

        Returns:
            str: The glasses' MAC address
        """
        scanner = Scanner()
        selected_glasses = None
        devices = scanner.scan(10.0)

        print("[" + Fore.CYAN + "*" + Fore.RESET + "] Looking for glasses ...")
        glasses_list = [
            device.addr for device in devices
            ]

        if len(glasses_list) > 1:
            print(
                "[" +
                Fore.GREEN +
                "+" +
                Fore.RESET +
                "] Multiple glasses have been found, please select one :"
                )

            for index, glasses_address in enumerate(glasses_list):
                print("    {0} - {1}".format(index + 1, glasses_address))

            while not selected_glasses:
                try:
                    selected_glasses = glasses_list[int(input("Selection : ")) - 1]
                except ValueError as parsing_error:
                    print("Error while parsing : please enter a number")
                    print(parsing_error)

        elif len(glasses_list) == 1:
            selected_glasses = glasses_list[0]
            print("[" + Fore.GREEN + "+" + Fore.RESET + "] {0}".format(selected_glasses))

        else:
            print("[" + Fore.RED + "-" + Fore.RESET + "] No glasses have been found")

        return selected_glasses
