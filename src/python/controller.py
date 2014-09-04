#!/usr/bin/python

import BaseHTTPServer
import SimpleHTTPServer
import serial

HTTP_PORT = 56789
#SERIAL_PORT = '/dev/ttyACM0'
SERIAL_PORT = '/dev/ttyUSB0'

SERIAL_REF = None

def execute_command(commandData):
    global SERIAL_REF
    if 'o' == commandData:
        SERIAL_REF = serial.Serial(SERIAL_PORT, baudrate=9600, timeout=None)
        return 'ok - opened'
    elif 'c' == commandData:
        SERIAL_REF.close()
        return 'ok - closed'
    else:
        SERIAL_REF.write(commandData)
        response = SERIAL_REF.readline()
        return response

class SimpleHTTPHandlerImpl(SimpleHTTPServer.SimpleHTTPRequestHandler):
    def do_GET(self):
        try:
            #serve files, and directory listings by following self.path from current working directory
            SimpleHTTPServer.SimpleHTTPRequestHandler.do_GET(self)
        except:
            result = '<html><body>error...</body></html>'
            self.wfile.write(result)
    def do_POST(self):
        """Handle a post request"""
        length = int(self.headers.getheader('content-length'))        
        data_string = self.rfile.read(length)
        result = execute_command(data_string)
        self.wfile.write(result)
            
            
def main():
    server = BaseHTTPServer.HTTPServer(("localhost", HTTP_PORT), SimpleHTTPHandlerImpl)
    try:
        print "serving at port", HTTP_PORT
        server.serve_forever()
    except KeyboardInterrupt:
        print 'shutting down...'
        server.socket.close()
        
if __name__ == '__main__':
    main()     
