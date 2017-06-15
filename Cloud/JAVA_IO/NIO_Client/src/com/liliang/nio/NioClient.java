package com.liliang.nio;



public class NioClient {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		new Thread(new NioClientHandler("127.0.0.1", 8080), "Nio-Client-1").start();
	}

}
