package com.liliang.BIO;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class TimeClient {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int port = 8080;
		/*
		if(args != null && args.length > 0){
			try{
				port = Integer.valueOf(args[0]);
			}catch(NumberFormatException e){
				System.out.println(e.toString());
			}
		}*/
		Socket socket = null;
		BufferedReader in = null;
		PrintWriter out = null;
		try{
			System.out.println("AAAAAAAAAAA");
			socket = new Socket("127.0.0.1", port);
			in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
			out = new PrintWriter(socket.getOutputStream(), true);
			out.println("QUERY TIME ORDER");
			String resp = in.readLine();
			System.out.println("Now is : "+resp);			
		}catch(Exception e){
			if(out != null){
				out.close();
				out = null;
			}
			if(in != null){
				try{
					in.close();					
				}catch(IOException el){
					el.printStackTrace();
				}
			}
			if(socket != null){
				try{
					socket.close();
				}catch(IOException ex){
					ex.printStackTrace();
				}
				socket = null;
			}
		}
		
	}

}
