package com.liliang.aio;

public class AioServer {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		AsyncServerHandler server = new AsyncServerHandler(8080);
		new Thread(server, "AIO-AyncServerHandler-1").start();
	}

}
