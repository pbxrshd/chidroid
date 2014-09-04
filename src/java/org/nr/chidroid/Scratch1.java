package org.nr.chidroid;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortList;

public class Scratch1 {

    public static void main(String[] args) {
        //simplehttpserver();
        getJsscPortNames();
        System.out.println("done");
    }

    public static void getJsscPortNames() {
        String[] portNames = SerialPortList.getPortNames();
        for (int i = 0; i < portNames.length; i++) {
            System.out.println(portNames[i]);
        }
    }
    public static SerialPort serialPort;
    
    public void jssctest() {
    	serialPort = new SerialPort("COM1");
        try {
            //Открываем порт
            serialPort.openPort();
            //Выставляем параметры
            serialPort.setParams(SerialPort.BAUDRATE_9600,
                                 SerialPort.DATABITS_8,
                                 SerialPort.STOPBITS_1,
                                 SerialPort.PARITY_NONE);
            //Включаем аппаратное управление потоком
            serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | 
                                          SerialPort.FLOWCONTROL_RTSCTS_OUT);
            //Устанавливаем ивент лисенер и маску
            serialPort.addEventListener(new PortReader(), SerialPort.MASK_RXCHAR);
            //Отправляем запрос устройству
            serialPort.writeString("Get data");
        }
        catch (Exception ex) {
            System.out.println(ex);
        }
    }
    
    public static class PortReader implements SerialPortEventListener {

        public void serialEvent(SerialPortEvent event) {
            if(event.isRXCHAR() &&event.getEventValue() > 0){
                try {
                    //Получаем ответ от устройства, обрабатываем данные и т.д.
                    String data = serialPort.readString(event.getEventValue());
                    //И снова отправляем запрос
                    serialPort.writeString("Get data");
                }
                catch (Exception ex) {
                    System.out.println(ex);
                }
            }
        }
    }
    
    
    public static void simplehttpserver() {
        int port = 58080;
        ServerSocket s = null;
        while (true) {
            try {
                s = new ServerSocket(port);
                Socket serverSocket = s.accept();
                handleRequest(serverSocket);
            } catch (IOException e) {
                System.out.println("Failed to start server: " + e.getMessage());
                System.exit(0);
                return;
            } finally {
                if (null != s) {
                    try {
                        s.close();
                    } catch (IOException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    public static void handleRequest(Socket s) {
        BufferedReader is;     // inputStream from web browser
        PrintWriter os;        // outputStream to web browser
        String request;        // Request from web browser

        try {
            String webServerAddress = s.getInetAddress().toString();
            System.out.println("Accepted connection from " + webServerAddress);
            is = new BufferedReader(new InputStreamReader(s.getInputStream()));

            request = is.readLine();
            System.out.println("Server recieved request from client: " + request);

            os = new PrintWriter(s.getOutputStream(), true);
            os.println("HTTP/1.0 200");
            os.println("Content-type: text/html");
            os.println("Server-name: myserver");
            String response = "<html><head>" +
                    "<title>Simpl Web Page</title></head>\n" +
                    "<h1>Congratulations!!!</h1>\n" +
                    "<h3>This page was returned by " + webServerAddress + "</h3>\n" +
                    "<p>This is the first page hosted by your web server.\n</p>" +
                    "Visit <A HREF=\"http://www.techwiki.ordak.org\"> http://www.techwiki.ordak.org</A> for more sample codes.\n" +
                    "</html>\n";
            os.println("Content-length: " + response.length());
            os.println("");
            os.println(response);
            os.flush();
            os.close();
            s.close();
        } catch (IOException e) {
            System.out.println("Failed to send response to client: " + e.getMessage());
        } finally {
            if (s != null) {
                try {
                    s.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return;
    }

}
