package com.liliang.BIO;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class TimeServer {

	public static void main(String[] args) throws IOException {
		// TODO Auto-generated method stub
		int port = 8080;
		if(args != null && args.length > 0){
			try{
				port = Integer.valueOf(args[0]);
			}catch(NumberFormatException e){
				System.out.println(e.toString());
			}
		}
		ServerSocket server = null;
		try{
			server = new ServerSocket(port);
			Socket socket = null;
			ServerHandlerExecutePool singleExecutor = new ServerHandlerExecutePool(50, 10000);
			while(true){
				socket = server.accept();
				singleExecutor.execute(new MyServerHandler(socket));
			}			
			
		} finally {
			if(server != null){
				System.out.println("Time server will close");
				server.close();
				server = null;
			}
		}
	}
}
