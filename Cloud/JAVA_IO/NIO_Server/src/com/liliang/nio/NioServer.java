package com.liliang.nio;

public class NioServer {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		MultiplexerServer server = new MultiplexerServer(8080);
		new Thread(server, "NIO-MultiplexerServer-1" ).start();
	}

}
