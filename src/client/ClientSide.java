/*
	////////////////////////////////////////////////////////////////
	// Author name: Francisco Martinez		//
	// Class: 		  CSE 3320					//
	// Description:  Client Side      	    //
	// Date: 	 December 08, 2014	 	    //   
	////////////////////////////////////////////////////////////////
*/
package client;

import java.io.BufferedInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

public class ClientSide {
    public static void main(String[] args) throws IOException 
    {
	
        try (Socket sock = new Socket("localhost", 5991)) {
            System.out.println("Connecting.........");
            
            File myFile = new File("/Users/Paco/Desktop/SourceFolder/");
            File[] Files = myFile.listFiles();
            
            OutputStream os = sock.getOutputStream();
            try (DataOutputStream dos = new DataOutputStream(os)) {
                dos.writeInt(Files.length);
                
                for (File File : Files) {
                    dos.writeUTF(File.getName());
                }
                for (File File : Files) {
                    int filesize = (int) File.length();
                    dos.writeInt(filesize);
                }
                
                for (File File : Files) {
                    int filesize = (int) File.length();
                    byte [] buffer = new byte [filesize];
                    FileInputStream fis = new FileInputStream(File.toString());
                    BufferedInputStream bis = new BufferedInputStream(fis);
                    bis.read(buffer, 0, buffer.length);
                    dos.write(buffer, 0, buffer.length);
                    dos.flush();
                }
            }
        }

	}

}

